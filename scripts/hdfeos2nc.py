#!/usr/bin/python3

helptxt = """
hdfeos2nc.py - Globs an input directory for MODIS HDF-EOS files and writes one output netCDF4 per product and tile.

$ python3 hdfeos2nc.py <input> <output>
  <input>     a directory with MODIS HDF-EOS tiles
  <output>    a directory to write output netCDFs """


import os,sys,glob
from osgeo import gdal
import datetime as dt
import numpy as np
import pandas as pd
import netCDF4 as nc4


# ------------------------------------------------
# input validation

def _check_arguments(a):

    if len(a) < 3:
        raise(" ~ error: less than three arguments. usage: python3 hdfeos2nc.py <input> <output> ")

    elif not all([ os.path.exists(a[1]),os.path.exists(a[2]) ]):
        raise(" ~ error: one or both inputs does not exist ")

    elif all([ os.path.isdir(a[1]),os.path.isdir(a[2]) ]):
        if len( glob.glob(a[1]) ) == 0:
            raise(" ~ error: no HDFs found at input dir ")


def _get_dirdict(_dir):

    _hdf = glob.glob(_dir+"*.hdf")                                              # list of HDFs
    _products = list(set([os.path.basename(h).split(".")[0] for h in _hdf]))    # list of products
    _tiles = list(set([os.path.basename(h).split(".")[2] for h in _hdf]))       # list of tiles
    print("Found "+str(len(_products))+" products and "+str(len(_tiles))+" tiles.\nProducts:\n"+str(_products)+"\nTiles:\n"+str(_tiles))

    # iterate over products; iterate over tiles; check for HDFs; add to dict of outputs
    d = {}                                                       # key = output filename; value = list of files
    for p in _products:
        _phdf = [f for f in _hdf if p in f]                      # list of HDFs for product p

        for t in _tiles:
            _thdf = sorted([f for f in _phdf if t in f])         # list of HDFs for product p and tile t

            if len(_thdf) > 0:                                   # if files exist for p,t pair, add to dict
                ffile = os.path.basename(_thdf[0]).split(".")    # first file in series
                lfile = os.path.basename(_thdf[-1]).split(".")   # last file in series

                d[str(ffile[0]+"."+ffile[2]+"."+ffile[1]+"-"+lfile[1])] = _thdf

    return(d)


# ------------------------------------------------
# dataset processing

spatts = [
    'EASTBOUNDINGCOORDINATE',
    'NORTHBOUNDINGCOORDINATE',
    'SOUTHBOUNDINGCOORDINATE',
    'WESTBOUNDINGCOORDINATE'
    'CHARACTERISTICBINSIZE',
    'DATACOLUMNS',
    'DATAROWS' ]
dsatts = [
    'RANGEBEGINNINGDATE',
    'RANGEENDINGDATE',
    'INPUTPOINTER',
    'QAPERCENTGOODQUALITY',
    'QAPERCENTNOTPRODUCEDCLOUD',
    'QAPERCENTNOTPRODUCEDOTHER',
    'QAPERCENTOTHERQUALITY',
    'LOCALGRANULEID',
    'DAYNIGHTFLAG',
    'AVERAGENUMBEROBS',
    'PERCENTLANDINTILE',
    'PRODUCTIONDATETIME']
sdsatts = [
    'scale_factor',
    'scale_factor_err',
    'add_offset',
    'add_offset_err',
    'Description',
    'long_name',
    'units',
    'valid_range',
    'calibrated_nt' ]
hdfatts = [
    'SHORTNAME',
    'LONGNAME',
    'identifier_product_doi',
    'VERSIONID',
    'INPUTPOINTER',
    'LOCALGRANULEID',
    'DAYNIGHTFLAG',
    'VERTICALTILENUMBER',
    'HORIZONTALTILENUMBER',
    'AVERAGENUMBEROBS',
    'PERCENTLANDINTILE',
    'PRODUCTIONDATETIME',
    'QAPERCENTGOODQUALITY',
    'QAPERCENTNOTPRODUCEDCLOUD',
    'QAPERCENTNOTPRODUCEDOTHER',
    'QAPERCENTOTHERQUALITY',
    'RANGEBEGINNINGDATE',
    'RANGEENDINGDATE',
    'SCIENCEQUALITYFLAGEXPLANATION' ]

def _try_converttype(v):

    try:
        vc = int(v)

    except ValueError:
        try:
            vc = float(v)
        except ValueError:
            vc = v

    return(vc)


_get_meta = lambda m,t: { a: _try_converttype(m[a]) for a in t if a in m.keys() }
_get_mdate = lambda f: os.path.basename(f).split(".")[1]
_get_mtile = lambda f: os.path.basename(f).split(".")[2]
_get_subdataset = lambda h,i: gdal.Open(h.GetSubDatasets()[i][0], gdal.GA_ReadOnly)

def _proc_tile(srcs,dst):

    # get list of tiles to check for multiple
    tiles = list(set([os.path.basename(h).split(".")[2] for h in srcs]))

    if len(list(set(tiles))) > 1:
        raise(" ~ error: Multiple MODIS tiles passed to _proc_tile() !")
    else:

        # open first file to get cols, rows, geotransform
        hdf = gdal.Open(srcs[0])

        cols = _get_subdataset(hdf,0).RasterXSize
        rows = _get_subdataset(hdf,0).RasterYSize
        geo = _get_subdataset(hdf,0).GetGeoTransform()

        # list of subdatasets
        subdatasets = hdf.GetSubDatasets()

        # iterate over subdatasets and append to dictionary
        subdatasetsdict = {}                        # key = variable name, value = attributes and fill
        for i,s in enumerate(subdatasets):

            sds = _get_subdataset(hdf,i)                            # subdataset
            sdsattributes = _get_meta(sds.GetMetadata(),sdsatts)    # attributes        # fill if exists
            fill = _try_converttype(sds.GetMetadata()['_FillValue']) if '_FillValue' in sds.GetMetadata().keys() else None

            layerid = sdsattributes['long_name']    # output variable name
            layerid = layerid.replace("/"," ")      # replace / if in variable long_name (e.g. GPP)

            subdatasetsdict[layerid] = { 'attributes': sdsattributes, 'fill': fill }    # add to dict

        mdate = os.path.basename(srcs[0]).split(".")[1]                                 # modis date string
        datedt = dt.datetime(int(mdate[1:5]), 1, 1) + dt.timedelta(int(mdate[5:]) - 1)  # datetime
        datestr = datedt.strftime("%Y-%m-%d")                                           # yyyy-mm-dd string

        # sinu xy coordinate arrays
        xarr = np.array([float(geo[0])]+[ i*geo[1]+float(geo[0]) for i in range(1,int(cols))])
        yarr = np.array([float(geo[3])]+[ -i*geo[5]+float(geo[3]) for i in range(1,int(rows))])


       # attach output netCDF for writing
        with nc4.Dataset(dst, "w") as nc:

            # add dims
            time = nc.createDimension("time", None)
            ydim = nc.createDimension("y", int(rows))
            xdim = nc.createDimension("x", int(cols))

            # time variable
            time = nc.createVariable("time","f8",("time",),zlib=True)
            time.setncatts({ "calendar": "standard", "standard_name": "time", "units": "days since "+datestr })
            time[:] = list(range(1,len(srcs)))

            # xy variables
            xvar = nc.createVariable("x","f4",("x",),zlib=True)
            xvar.setncatts(dict(standard_name="projection_x_coordinate",long_name="x coordinate of projection",units="m"))
            xvar[:] = xarr
            yvar = nc.createVariable("y","f4",("y",),zlib=True)
            yvar.setncatts(dict(standard_name="projection_y_coordinate",long_name="y coordinate of projection",units="m"))
            yvar[:] = yarr

            # crs variable
            crs = nc.createVariable("crs","f8",zlib=True)
            crs.setncatts(dict(
                grid_mapping_name="sinusoidal", long_name="crs definition",
                false_easting=0., false_northing=0., longitude_of_central_meridian=0., longitude_of_prime_meridian=0., semi_major_axis=6371007.181, inverse_flattening=0.,
                spatial_ref="PROJCS[\"unnamed\",GEOGCS[\"unnamed ellipse\",DATUM[\"unknown\",SPHEROID[\"unnamed\",6371007.181,0],EXTENSION[\"PROJ4_GRIDS\",\"@null\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Sinusoidal\"],PARAMETER[\"longitude_of_center\",0],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],EXTENSION[\"PROJ4\",\"+proj=sinu +R=6371007.181 +nadgrids=@null +wktext\"]]",
                GeoTransform="-360.3515625 0.7028340789935529 0 89.81365616296401 0 -0.7028340789935529 "
            ))

            # iterate over subdatasetsdict and initialize netCDF variable for each
            for name,attributes in subdatasetsdict.items():
                tmp = nc.createVariable(name,"f8",("time","y","x"),fill_value=attributes['fill'],zlib=True)
                tmp.setncatts(attributes['attributes'])

            # iterate over hdf list and write subdatasets to output netCDF variables
            for i,h in enumerate(srcs):
                print(".  "+ str(i+1)+" of "+str(len(srcs))+" ... ")

                fn = os.path.basename(h)
                hdf = gdal.Open(h)
                hdfmeta = _get_meta(hdf.GetMetadata(), hdfatts)

                mdate = os.path.basename(srcs[0]).split(".")[1]                                 # modis date string
                datedt = dt.datetime(int(mdate[1:5]), 1, 1) + dt.timedelta(int(mdate[5:]) - 1)  # datetime
                datestr = datedt.strftime("%Y-%m-%d")                                           # yyyy-mm-dd string

                # iterate over subdatasets, read, write arrays to output netCDF
                for j,s in enumerate(subdatasets):
                    sds = _get_subdataset(hdf,j)
                    layerid = sdsattributes['long_name']
                    layerid = layerid.replace("/"," ")
                    print(sds.ReadAsArray())
                    var = nc.variables[layerid]
                    print(var)

                    nc.variables[layerid][i] = np.flipud(sds.ReadAsArray())
                    nc.sync()

# ------------------------------------------------
# main

def _iter_print(i,t,dst):
    dstr = dst.split(".")
    istr = """[ {i} / {n} ]: {p} | {t} | {o} """.format(i=str(i),n=str(t),p=dstr[0],t=dstr[1],o=dst+".nc")
    print("-"*len(istr)+"\n"+istr)

if __name__ == '__main__':

    # check arguments
    if len(sys.argv) < 3:
        print("Input error. See help:\n" + helptxt)
    else:
        _check_arguments(sys.argv)

    # input,output directories
    inpath = sys.argv[1]
    outpath = sys.argv[2]

    # get dictionary of datasets; keys = output filename, values = list of hdfs
    dirdict = _get_dirdict(inpath)

    # iterate over key, value pairs and write netCDFs
    for i,key in enumerate(dirdict.keys()):
        _iter_print( i+1, len(dirdict.keys()), key )
        _proc_tile( dirdict[key], str(outpath+"/"+key+".nc") )






#def _get_meta(obj,atttype):
#    meta = obj.GetMetadata()
#    return({ a: _try_converttype(meta[a]) for a in atttype if a in meta.keys() })
#def _get_mdate(f):
#    return(os.path.basename(f).split(".")[1])
#def _get_mtile(f):
#    return(os.path.basename(f).split(".")[2])
#def _get_subdataset(h,i):
#    return(gdal.Open(h.GetSubDatasets()[i][0], gdal.GA_ReadOnly))
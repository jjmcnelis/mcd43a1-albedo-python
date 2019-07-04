"""
MODIS MCD43A1 Albedo
"""

from ipywidgets import FloatSlider, interact
from pyproj import Proj, transform
from math import radians, cos
from io import StringIO
import xarray as xr   
import pandas as pd
import numpy as np
import datetime
import sys
import os

import matplotlib.pyplot as plt
import matplotlib
matplotlib.rc('font', **{
    'family': 'normal', 
    'weight': 'bold', 
    'size': 8})

# ---------------------------------------------------------------------- 
# coordinate + sza functions
# ----------------------------------------------------------------------


def get_proj(crs):
    """Get srs parameters as proj4 string."""
    
    getpar = lambda a: str(crs.attrs[a])
    return(Proj(" ".join([
        "+proj=sinu",
        "+lon_0="+getpar("longitude_of_central_meridian"),
        "+x_0="+getpar("false_easting"),
        "+y_0="+getpar("false_northing"),
        "+a="+getpar("semi_major_axis"),
        "+b="+getpar("semi_minor_axis"),
        "+units="+"meter +no_defs"])))


# get lat and lon 2d arrays
def get_latlon(ds, inproj, outproj):
    """ """
    
    xx, yy = np.meshgrid(ds.x.data, ds.y.data)
    lon1d, lat1d = transform(
        inproj, 
        outproj, 
        xx.flatten(), 
        yy.flatten())
    lon2d, lat2d = lon1d.reshape(xx.shape), lat1d.reshape(yy.shape)
    
    return(xx, yy, lon1d, lat1d, lon2d, lat2d)


def get_solar_zenith(doy, latitude, ndoy=365):
    """Calculate solar zenith angle (1) at local 
    noon for input day of the year and latitude."""
    declination = cos(radians((doy+10)*(360/ndoy)))*-23.45
    altitude = 90 - latitude + declination
    zenith = 90 - altitude
    return(zenith)


def sza_eval(doy, lat):
    """Convert CF to Python datetime."""
    func = lambda l: get_solar_zenith(doy, l)
    return(xr.apply_ufunc(func, lat))


def get_sza(ds):
    """Calculate solar zenith angles vectorized at local 
    noon for input day of the year and latitude."""
    sza = xr.DataArray(
        data=np.dstack([sza_eval(t.dt.dayofyear, ds.lat) for t in ds.time]), 
        coords=[ds.y, ds.x, ds.time],       # note that we reorder coords in
        dims=["y", "x", "time"],            # dims argument to match others
        attrs=dict(
            units="degree",
            standard_name="solar zenith angle",
            long_name="solar zenith angle"))
    sza.name = "solar_zenith_angle"
    sza = sza.transpose("time", "y", "x")
    
    return(sza)


# ----------------------------------------------------------------------------
# ALBEDO
# ----------------------------------------------------------------------------


albedo_attributes = dict(
    _FillValue=32767,
    grid_mapping="crs",
    valid_min=0,
    valid_max=32766,
    units="reflectance, no units",
    scale_factor_err=0.0,
    add_offset_err=0.0,
    calibrated_nt=5,
    scale_factor=0.001,
    add_offset=0.0)


# ---------------------------------------------------------------------- 
# 2d lat and lon arrays
# ----------------------------------------------------------------------      


def get_coordinates(ds):
    """ """

    inproj = get_proj(ds.crs)
    outproj = Proj(init="epsg:4326")

    xx, yy, lon1d, lat1d, lon2d, lat2d = get_latlon(ds, inproj, outproj)

    latatts = dict(
        standard_name="latitude",
        long_name="latitude coordinate",
        units="degrees_north")

    lat = xr.DataArray(
        data=lat2d, 
        coords=[ds.y, ds.x], 
        dims=["y", "x"], 
        attrs=latatts)

    lonatts = dict(
        standard_name="longitude",
        long_name="longitude coordinate",
        units="degrees_east")

    lon = xr.DataArray(
        data=lon2d, 
        coords=[ds.y, ds.x], 
        dims=["y", "x"], 
        attrs=lonatts)
    
    return(lat, lon)


# ---------------------------------------------------------------------------
# black
# ---------------------------------------------------------------------------


def black(par1, par2, par3, sza):
    """ """
    
    iso = ( 1.000000,  0.000000, 0.000000)    # iso: Isotropic
    vol = (-0.007574, -0.070987, 0.307588)    # vol: RossThick
    geo = (-1.284909, -0.166314, 0.041840)    # geo: LiSparseR

    sza2, sza3 = sza**2, sza**3               # get exponentiated sza
    
    func = lambda p1, p2, p3: (               # def apply function
        p1*(iso[0]+iso[1]*sza2+iso[2]*sza3)+  #  iso
        p2*(vol[0]+vol[1]*sza2+vol[2]*sza3)+  #  vol
        p3*(geo[0]+geo[1]*sza2+geo[2]*sza3)   #  geo
    )*0.0001
    
    return(xr.apply_ufunc(func, par1, par2, par3))


# ---------------------------------------------------------------------------
# white
# ---------------------------------------------------------------------------


def white(par1, par2, par3):
    """ """
    iso, vol, geo = (1.000000, 0.189184, -1.377622)
    func = lambda p1, p2, p3: (
        p1*iso +                              # Isotropic
        p2*vol +                              # RossThick
        p3*geo )                              # LiSparseR 
    return(xr.apply_ufunc(func, par1, par2, par3))


# ---------------------------------------------------------------------------
# blue
# ---------------------------------------------------------------------------


def lookup(band, sza, sod=0.02):
    """ """    
    luc = band.lookup["0.02"]
    lfunc = lambda s: luc.iloc[s].values
    return(xr.apply_ufunc(lfunc, abs(sza).round(),))
        

def blue(wsa, bsa, lookup):
    """Vectorize albedo polynomials over two 3d arrays."""
    func = lambda white,black,lookup: (white*lookup) + (black*(1 - lookup))
    return(xr.apply_ufunc(func, wsa, bsa, lookup))


#############################################################################
# introduction only
#############################################################################


# ---------------------------------------------------------------------------
# prep
# ---------------------------------------------------------------------------

get_band_name = lambda ds:[v for v in ds.variables if all([
    "param" in ds[v].dims, v!="param"])][0]


with open("data/skyl_lut.dat", "r") as f:
    tab = f.read().replace("  ", " ")
con, mar = [t.split("Band") for t in tab.split("Aerosol_type: ")[1:]]
get_lut = lambda s: pd.read_csv(
    StringIO(s),
    index_col="S&O",
    skiprows=1,
    sep=" ")


# ---------------------------------------------------------------------------
# plot
# ---------------------------------------------------------------------------

# MODIS Albedos for Florida USGS Report

## MCD43

The MODIS BRDF/Albedo/NBAR Product MCD43 is led by [Professor Crystal Schaaf at UMass Boston](https://www.umb.edu/spectralmass/terra_aqua_modis/modis_brdf_albedo_product_mcd43). [Binaries](code.zip) for computing black, white, and blue sky albedos are maintained by her lab at this link: 
[ftp://rsftp.eeos.umb.edu/data01/Website/actual_albedo_tool.tar.gz](ftp://rsftp.eeos.umb.edu/data01/Website/actual_albedo_tool.tar.gz)

## Paths

### Processing
* [proc.ipynb](proc.ipynb) -- comprehensive documentation of workflow
* [data/MCD43A1/MCD43A1-request.json](data/MCD43A1/MCD43A1-request.json) -- request JSON to request an identical subset of MCD43A1 from APPEEARS: [https://lpdaacsvc.cr.usgs.gov/appeears/](https://lpdaacsvc.cr.usgs.gov/appeears/)
* [data/MCD43A3/MCD43-FloridaUSGS-2018-request.json](data/MCD43A3/MCD43-FloridaUSGS-2018-request.json) -- and for MCD43A3 [https://lpdaacsvc.cr.usgs.gov/appeears/](https://lpdaacsvc.cr.usgs.gov/appeears/)
* 

### Albedo source code
* (proc/actual_albedo_tool/albedo)[.../albedo/] -- albedo compile directory
* (proc/actual_albedo_tool/szn/)[.../szn/] -- solar zenith number compile directory
* (proc/actual_albedo_tool/albedo/readme)[.../albedo/readme] -- readme gives a succinct description of albedo calculation.
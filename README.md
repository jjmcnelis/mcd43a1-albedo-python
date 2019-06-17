# MODIS Albedos for Florida USGS Report

## MCD43

The MODIS BRDF/Albedo/NBAR Product MCD43 is led by [Professor Crystal Schaaf at UMass Boston](https://www.umb.edu/spectralmass/terra_aqua_modis/modis_brdf_albedo_product_mcd43). [Binaries](code.zip) for computing black, white, and blue sky albedos are maintained by her lab at this link: 
[ftp://rsftp.eeos.umb.edu/data01/Website/actual_albedo_tool.tar.gz](ftp://rsftp.eeos.umb.edu/data01/Website/actual_albedo_tool.tar.gz)

## Paths

### Processing
* [floridaproc.ipynb](floridaproc.ipynb) -- comprehensive documentation of workflow
* [data/MCD43-FloridaUSGS-2018-request.json](data/MCD43-FloridaUSGS-2018-request.json) -- request JSON with which you can download an identical slice of MCD43 from APPEEARS: [https://lpdaacsvc.cr.usgs.gov/appeears/](https://lpdaacsvc.cr.usgs.gov/appeears/)
* 

### Albedo source code
* [code/albedo/](code/albedo/) -- albedo binaries compile directory
* [code/szn/](code/szn/) -- solar zenith number binary compile directory
* [code/albedo/readme](code/albedo/readme) -- readme gives a succinct description of the albedo formulas. I print the information from that file throughout the workflow description in the ipython notebook.

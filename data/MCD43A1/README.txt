TABLE OF CONTENTS
-----------------
                                               Section
Request Parameters ................................. 1
Request File Information ........................... 2
AppEEARS Area Sample Extraction Process ............ 3
AppEEARS Area Naming Convention .................... 4
Data Quality ....................................... 5
  Moderate Resolution Imaging
    Spectroradiometer (MODIS) ................... 5.01
  NASA MEaSUREs Web Enabled
    Landsat Data (WELD) ......................... 5.02
  NASA MEaSUREs Shuttle Radar Topography
    Mission (SRTM) Version 3 (v3) ............... 5.03
  Gridded Population of the World (GPW)
    Version 4 (v4) .............................. 5.04
  Suomi National Polar-orbiting Partnership (S-NPP)
    NASA Visible Infrared Imaging Radiometer Suite
    (VIIRS) ..................................... 5.05
  Soil Moisture Active Passive (SMAP) ........... 5.06
  MODIS Simplified Surface Energy Balance (SSEBop)
    Actual Evapotranspiration (ETa) ............. 5.07
  eMODIS Smoothed Normalized Difference Vegetation
    Index (NDVI) ................................ 5.08
  Daymet ........................................ 5.09
  U.S. Landsat Analysis Ready Data (ARD) ........ 5.10
Data Caveats ....................................... 6
Documentation ...................................... 7
Sample Request Retention ........................... 8
Data Product Citations ............................. 9
Software Citation .................................. 10
Feedback ........................................... 11


1. Request Parameters
   ------------------
Request Name:   MCD43A1
Date Completed: 2019-06-15T18:32:08.794389
Id:             9993d076-0ffc-4cef-96d2-5877e4d063b9

Details:
    Input Vector Name:         florida
    Number of Vector Features: 1
    Start Date:                01-01-2018
    End Date:                  12-31-2018
    Layers:
                               BRDF_Albedo_Parameters_nir (MCD43A1.006)
                               BRDF_Albedo_Parameters_shortwave (MCD43A1.006)
                               BRDF_Albedo_Parameters_vis (MCD43A1.006)
                               BRDF_Albedo_Band_Mandatory_Quality_nir (MCD43A1.006)
                               BRDF_Albedo_Band_Mandatory_Quality_shortwave (MCD43A1.006)
                               BRDF_Albedo_Band_Mandatory_Quality_vis (MCD43A1.006)
    Output Projection:         MODIS Sinusoidal
                               PROJ.4: "+proj=sinu +lon_0=0 +x_0=0 +y_0=0 +a=6371007.181 +b=6371007.181 +units=m +no_defs"
    Output Format:             netcdf4

Version:
    This request was processed by AppEEARS version 2.23


2. Request File Information
   ------------------------
Supporting Files:
    MCD43A1-MCD43A1-006-metadata.xml
    MCD43A1-granule-list.txt
    MCD43A1-request.json
    MCD43A1-006-BRDF-Albedo-Band-Mandatory-Quality-nir-lookup.csv
    MCD43A1-006-BRDF-Albedo-Band-Mandatory-Quality-shortwave-lookup.csv
    MCD43A1-006-BRDF-Albedo-Band-Mandatory-Quality-vis-lookup.csv
    MCD43A1-006-BRDF-Albedo-Band-Mandatory-Quality-nir-Statistics-QA.csv
    MCD43A1-006-BRDF-Albedo-Band-Mandatory-Quality-shortwave-Statistics-QA.csv
    MCD43A1-006-BRDF-Albedo-Band-Mandatory-Quality-vis-Statistics-QA.csv
    MCD43A1-006-Statistics.csv

Data Files:
    Number of Extracted Data Files:     1
    Total Size of Extracted Data Files: 2,086.52 MB


3. AppEEARS Area Sample Extraction Process
   ---------------------------------------
When an area sample extraction request is successfully submitted, AppEEARS
implements a series of tools and services to extract the exact data the user is
interested in; or rather, extracts data from specific data layers that
intersect with the input vector file's features. AppEEARS first uploads the
input vector file and reprojects it to the source projection of the data layer
of interest. The PROJ.4 definitions for each data collection available through
AppEEARS are listed below.

MODIS (TERRA, AQUA, & Combined):
"+proj=sinu +lon_0=0 +x_0=0 +y_0=0 +a=6371007.181 +b=6371007.181 +units=m +no_defs"

WELD (CONUS):
"+proj=aea +lat_1=29.5 +lat_2=45.5 +lat_0=23 +lon_0=-96 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"

WELD (Alaska):
"+proj=aea +lat_1=55 +lat_2=65 +lat_0=50 +lon_0=-154 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"

SRTM v3 (30m & 90m):
"+proj=longlat +datum=WGS84 +no_defs"

MODIS Snow Products (TERRA & AQUA):
"+proj=sinu +lon_0=0 +x_0=0 +y_0=0 +a=6371007.181 +b=6371007.181 +units=m +no_defs"

GPW v4:
"+proj=longlat +datum=WGS84 +no_defs"

S-NPP NASA VIIRS:
"+proj=sinu +lon_0=0 +x_0=0 +y_0=0 +a=6371007.181 +b=6371007.181 +units=m +no_defs"

SMAP - Global:
"+proj=cea +lon_0=0 +lat_ts=30 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"

SMAP - Northern Hemisphere:
"+proj=laea +lat_0=90 +lon_0=0 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"

SSEBop Actual Evapotranspiration (ETa):
"+proj=longlat +datum=WGS84 +no_defs"

eMODIS Smoothed Normalized Difference Vegetation Index (NDVI):
"+proj=laea +lat_0=45 +lon_0=-100 +x_0=0 +y_0=0 +a=6370997 +b=6370997 +units=m +no_defs"

Daymet
"+proj=lcc +lat_0=42.5 +lat_1=25 +lat_2=60 +lon_0=-100 +x_0=0 +y_0=0 +ellps=WGS84 +units=km +no_defs"

U.S. Landsat ARD (CONUS)
"+proj=aea +lat_1=29.5 +lat_2=45.5 +lat_0=23 +lon_0=-96 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"

U.S. Landsat ARD (Alaska)
"+proj=aea +lat_1=55 +lat_2=65 +lat_0=50 +lon_0=-154 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"

U.S. Landsat ARD (Hawaii)
"+proj=aea +lat_1=8 +lat_2=18 +lat_0=3 +lon_0=-157 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs"


Following the reprojection of the vector file, a bounding box for each feature
in the vector file is determined. Each corner point is first determined using
the minimum and maximum latitude and longitude values. An additional 1 pixel
buffer is applied to each corner point to create the feature bounding box. See
below for details on how the bounding box is determined.

Feature Bounding Box Calculation
    UpperLeft   =   (maxLat + cellSize), (minLon - cellSize)
    LowerLeft   =   (minLat - cellSize), (minLon - cellSize)
    UpperRight  =   (maxLat + cellSize), (maxLon + cellSize)
    LowerRight  =   (minLat - cellSize), (maxLon + cellSize)

For each feature, a series of tools and services are used to determine which
spatial tiles intersect with the coordinates of the feature bounding box for
the data layer of interest. These tiles are extracted (from OPeNDAP) to a
server-side work environment where they are mosaicked into an image. Tile
extraction and processing is implemented for each requested composite period
(e.g., daily, weekly, 8-day, 16-day, monthly, seasonal, or annual) to create a
time series image stack. If the user chooses to have the output projection for
each extracted data file match, then the image stack is reprojected into the
user-requested projection using the PROJ.4 definition described above. The
image stacks are finally clipped to the input feature shape to only maintain
the data intersecting the feature shape. Data outside of the feature shape are
converted to a product-specific NODATA value. Each clipped image in the time
series image stack is saved as a CF-compliant NetCDF file or in a series of
Geospatial Tagged Image File Format (GeoTIFF) files with a unique name
following the naming conventions described in Section 4 of this README.

AppEEARS implements a strict procedure for reprojecting data outputs. Pixel size
and resampling methods are non-customizable in AppEEARS. Reprojected data are
produced using the Geospatial Data Abstraction Library (GDAL) gdalwarp function
in combination with the PROJ.4 string definition for the user-defined output
projection type. Reprojection is performed using nearest neighbor resampling.
If the projection units are the same between the native and output projections,
the native pixel size is used to reproject the image. If the projection units
between the native and output projections are different (e.g. sinusoidal (m) to
geographic (degrees), pixel size is calculated by reprojecting the center pixel
of the original image, calculating its width and height, and then squaring all
pixels. The latitude and longitude of the region of interest are maintained in
the conversion.

NOTE:
  - Requested date ranges may not match the reference date for multi-day
  products. AppEEARS takes an inclusive approach when extracting data for
  sample requests, often returning data that extends beyond the requested
  date range. This approach ensures that the returned data includes records for
  the entire requested date range.
  - If selected, the SRTM v3 product will be extracted regardless of the time
  period specified in AppEEARS because it is a static dataset. The date field
  in the data tables reflect the nominal SRTM date of February 11, 2000.
  - If the visualizations indicate that there are no data to display, proceed
  to checking the .csv output file. Data products that have both categorical
  and continuous data values (e.g. MOD15A2H) are not able to be displayed in
  the visualizations within AppEEARS.


4. AppEEARS Area Sample Naming Convention
   --------------------------------------
Output data files returned by AppEEARS have the following naming convention:

<ProductShortName>.<Version>_<Resolution>_<AppEEARSFeatureID>.<FileFormat>

Example:
    MOD13Q1.006_250m_aid0002.nc

    <ProductShortName> .......... MOD13Q1
    <Version> ................... 006
    <Resolution> ................ 250m
    <AppEEARSFeatureID> ......... aid0002
    <FileFormat> ................ nc

The AppEEARS Feature ID is assigned automatically by the system.


5. Data Quality
   ------------
When available, AppEEARS extracts and returns quality assurance (QA) data for
each data file returned regardless of whether the user requests it. This is
done to ensure that the user possesses the information needed to determine the
usability and usefulness of the data they get from AppEEARS. Most data products
available through AppEEARS have an associated QA data layer. Some products have
more than one QA data layer to consult. See below for more information
regarding data collections/products and their associated QA data layers.

 5.01 MODIS (Terra, Aqua, & Combined)
      -------------------------------
  All MODIS land products, as well as the MODIS Snow Cover Daily product,
  include quality assurance (QA) information designed to help users understand
  and make best use of the data that comprise each product.
  - See the MODIS Land
  Products QA Tutorials (https://lpdaac.usgs.gov/user_resources/
  e_learning) for more QA information regarding each MODIS land product
  suite.
  - See the MODIS Snow Cover Daily product user guide for information
  regarding QA utilization and interpretation.

 5.02 NASA MEaSUREs WELD (CONUS & Alaska)
      -----------------------------------
  Each WELD product contains a pixel saturation band and two cloud
  detection bands generated using the Landsat automatic cloud cover
  assessment (ACCA) algorithm and a decision tree based cloud detection
  approach.
  - See the WELD product documentation
  (https://lpdaac.usgs.gov/dataset_discovery/measures/
  measures_products_table) or Roy et al. 2010 (http://dx.doi.org/10.1016/
  j.rse.2009.08.011) for details regarding these bands.

 5.03 NASA MEaSUREs SRTM v3 (30m & 90m)
      ---------------------------------
  SRTM v3 products are accompanied by an ancillary "NUM" file in place of the
  QA/QC files. The "NUM" files indicate the source of each SRTM pixel, as well
  as the number of input data scenes used to generate the SRTM v3 data for that
  pixel.
  - See the user guide (https://lpdaac.usgs.gov/sites/default/files/
  public/measures/docs/NASA_SRTM_V3.pdf) for additional information regarding
  the SRTM "NUM" file.

 5.04 GPW v4
      ------
  The GPW Population Count and Population Density data layers are accompanied
  by two Data Quality Indicators datasets. The Data Quality Indicators were
  created to provide context for the population count and density grids, and to
  provide explicit information on the spatial precision of the input boundary
  data. The data context grid (data-context1) explains pixels with "0"
  population estimate in the population count and density grids, based on
  information included in the census documents. The mean administrative unit
  area grid (mean-admin-area2) measures the mean input unit size in square
  kilometers. It provides a quantitative surface that indicates the size of the
  input unit(s) from which the population count and density grids were created.

 5.05 S-NPP NASA VIIRS
      ----------------
  All S-NPP NASA VIIRS land products include quality information
  designed to help users understand and make best use of the data that comprise
  each product. For product-specific information, see the link to the S-NPP
  VIIRS products table provided in section 6.

  NOTE:
    - The S-NPP NASA VIIRS Surface Reflectance data products VNP09A1 and VNP09H1
    contain two quality layers: 'SurfReflect_State' and 'SurfReflect_QC'. Both
    quality layers are provided to the user with the request results. Due to changes
    implemented on August 21, 2017 for forward processed data, there are differences
    in values for the 'SurfReflect_QC' layer in VNP09A1 and 'SurfReflect_QC_500m'
    in VNP09H1.
    - Refer to the S-NPP NASA VIIRS Surface Reflectance User's Guide Version 1.1
    (https://lpdaac.usgs.gov/sites/default/files/public/product_documentation/vnp09_user_guide.pdf)
    for information on how to decode the 'SurfReflect_QC' quality layer for data
    processed before August 21, 2017. For data processed on or after August 21, 2017,
    refer to the S-NPP NASA VIIRS Surface Reflectance User's guide Version 1.6
  (https://lpdaac.usgs.gov/sites/default/files/public/product_documentation/vnp09_user_guide_v1.6.pdf).

 5.06 SMAP
      ----
  SMAP products provide multiple means to assess quality. Each data product
  contains bit flags, uncertainty measures, and file-level metadata that provide
  quality information. Results downloaded from AppEEARS and/or data directly
  requested via middleware services contain not only the requested pixel/data
  values, but also the decoded bit flag information associated with each pixel/data
  value extracted. For additional information regarding the specific bit flags,
  uncertainty measures, and file-level metadata contained in this product, refer
  to the Quality Assessment section of the user guide for the specific SMAP data
  product in your request (https://nsidc.org/data/smap/smap-data.html).

 5.07 SSEBop Actual Evapotranspiration (ETa)
      --------------------------------------
  The SSEBop evapotranspiration monthly product does not have associated quality
  indicators or data layers. The data are considered to satisfy the quality
  standards relative to the purpose for which the data were collected.

 5.08 eMODIS Smoothed Normalized Difference Vegetation Index (NDVI)
      -------------------------------------------------------------
  The smoothed eMODIS NDVI product does not have associated quality
  indicators or data layers. The data are considered to satisfy the quality
  standards relative to the purpose for which the data were collected.

 5.09 Daymet
      ------
  Daymet station-level daily weather observation data and the corresponding Daymet 
  model predicted data for three Daymet model parameters: minimum temperature (tmin), 
  maximum temperature (tmax), and daily total precipitation (prcp) are available.
  These data provide information into the regional accuracy of the Daymet model
  for the three station-level input parameters. Corresponding comma separated value
  (.csv) files that contain metadata for every surface weather station for the 
  variable-year combinations are also available.
  https://daac.ornl.gov/cgi-bin/dsviewer.pl?ds_id=1391

 5.10 U.S. Landsat ARD
      ----------------
  Quality assessment bands for the U.S. Landsat ARD data products are produced 
  from Level 1 inputs with additional calculations derived from higher-level 
  processing. A pixel quality assessment band describing the general state of 
  each pixel is supplied with each AppEEARS request. In addition to the pixel 
  quality assessment band, Landsat ARD data products also have additional bands 
  that can be used to evaluate the usability and usefulness of the data. These 
  include bands that characterize radiometric saturation, as well as parameters 
  specific to atmospheric correction. Refer to the U.S. Landsat ARD Data Format 
  Control Book (DFCB) 
  (https://www.usgs.gov/media/files/landsat-analysis-ready-data-ard-data-format-control-book-dfcb) 
  for a full description of the quality assessment bands for each product (L4-L8)
  as well as guidance on interpreting each band’s bit-packed data values. 

6. Data Caveats
   ------------
 SSEBop Actual Evapotranspiration (ETa)
 --------------------------------------
  - A list of granule files is not provided for the SSEBop ETa data product.
  The source data for this product can be obtained by using the download
  interface at:
  https://earlywarning.usgs.gov/fews/datadownloads/Continental%20Africa/Monthly%20ET%20Anomaly

 eMODIS Smoothed Normalized Difference Vegetation Index (NDVI)
 -------------------------------------------------------------
  - The raw data values within the smoothed eMODIS NDVI product represent scaled
  byte data with values between 0 and 200. To convert the scaled raw data to
  smoothed NDVI (smNDVI) data values, the user must apply the following conversion
  equation:

       smNDVI = (0.01 * Raw_Data_Value) - 1

  - A list of granule files is not provided for the SSEBop ETa data product.
  The source data for this product can be obtained by using the download
  interface at: https://phenology.cr.usgs.gov/get_data_smNDVI.php


7. Documentation
   -------------
The documentation for AppEEARS can be found at https://lpdaacsvc.cr.usgs.gov/appeears/help.

Documentation for data products available through AppEEARS are listed below.

 MODIS Land Products(Terra, Aqua, & Combined)
 --------------------------------------------
  - https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table

 MODIS Snow Products (Terra and Aqua)
 ------------------------------------
  - https://nsidc.org/data/modis/data_summaries

 NASA MEaSUREs Web Enabled Landsat Data (WELD) (CONUS & Alaska)
 --------------------------------------------------------------
  - https://lpdaac.usgs.gov/dataset_discovery/measures/measures_products_table

 NASA MEaSUREs SRTM v3
 ---------------------
  - https://lpdaac.usgs.gov/dataset_discovery/measures/measures_products_table

 GPW v4
 ------
  - http://sedac.ciesin.columbia.edu/binaries/web/sedac/collections/gpw-v4/gpw-v4-documentation.pdf

 S-NPP NASA VIIRS Land Products
 ------------------------------
  - https://lpdaac.usgs.gov/dataset_discovery/viirs/viirs_products_table

 SMAP Products
 -------------
  - http://nsidc.org/data/smap/smap-data.html

 SSEBop Actual Evapotranspiration (ETa)
 --------------------------------------
  - https://earlywarning.usgs.gov/fews/product/66#documentation

 eMODIS Smoothed Normalized Difference Vegetation Index (NDVI)
 -------------------------------------------------------------
  - https://phenology.cr.usgs.gov/get_data_smNDVI.php

 Daymet
 ------
  - https://daac.ornl.gov/cgi-bin/dsviewer.pl?ds_id=1328
  - https://daymet.ornl.gov/

 U.S. Landsat ARD
 ----------------
  - https://www.usgs.gov/land-resources/nli/landsat/us-landsat-analysis-ready-data?qt-science_support_page_related_con=0#qt-science_support_page_related_con


8. Sample Request Retention
   ------------------------
AppEEARS sample request outputs are available to download for a limited amount of time after completion.
Please visit https://lpdaacsvc.cr.usgs.gov/appeears/help?section=sample-retention for details.


9. Data Product Citations
   ----------------------
 Schaaf, C., Wang, Z. (2015). MCD43A1 MODIS/Terra+Aqua BRDF/Albedo Model Parameters Daily L3 Global - 500m V006. NASA EOSDIS Land Processes DAAC. doi: 10.5067/MODIS/MCD43A1.006. Accessed June 15, 2019.
 

10. Software Citation
   -----------------
 AppEEARS Team. (2019). Application for Extracting and Exploring Analysis Ready Samples (AppEEARS). Ver. 2.23. NASA EOSDIS Land Processes Distributed Active Archive Center (LP DAAC), USGS/Earth Resources Observation and Science (EROS) Center, Sioux Falls, South Dakota, USA. Accessed June 15, 2019. https://lpdaacsvc.cr.usgs.gov/appeears


11. Feedback
   --------
We value your opinion. Please help us identify what works, what doesn't, and
anything we can do to make AppEEARS better by submitting your feedback at
https://lpdaacsvc.cr.usgs.gov/appeears/feedback or to LP DAAC User Services at
https://lpdaac.usgs.gov/user_services.

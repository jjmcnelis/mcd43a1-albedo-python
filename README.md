# Daily albedos from MODIS BRDF Parameters (MCD43A1)

Comprehensive Python workflow for calculating black-, white-, and blue-sky albedos from MODIS MCD43A1 BRDF/Albedo model parameters product using **custom solar zenith angles and optical depth** inputs. Input MCD43A1 netCDF from AppEEARS; flexible outputs.

![bsa](proc/img/bsa.png)
*Black sky albedo time series from [0_Introduction.ipynb](0_Introduction.ipynb).*

## Data, source code
* Data: [Go](data/)         
* Docs: [Go](proc/)           

## Notebooks
<table>
    <tr>
        <td style="text-align:left"><a href="0_Introduction.ipynb">0_Introduction.ipynb</a></td>
        <td style="text-align:left">Fundamental concepts explained.</b></td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=0_Introduction.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="1_Workflow_Demo.ipynb">1_Workflow_Demo.ipynb</a></td>
        <td style="text-align:left">Process albedo model parameters to black, white, blue albedos</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=1_Workflow_Demo.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="2_Batch_Process.ipynb">2_Batch_Process.ipynb</a></td>
        <td style="text-align:left">Bundle workflow into a few functions and loop</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=2_Batch_process.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="3_Plot_Results.ipynb">3_Plot_Results.ipynb</a></td>
        <td style="text-align:left">Calculate statistics and plot</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=3_Plot_Results.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="4_Validate.ipynb">4_Validate.ipynb</a></td>
        <td style="text-align:left">Check results against MCD43A3 albedos</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=4_Validate.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    <tr>
        <td style="text-align:left"><a href="5_HDF_Workflow.ipynb">5_HDF_Workflow.ipynb</a></td>
        <td style="text-align:left">AppEEARS alternative. Examples get you started processing HDFs</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=5_HDF_Workflow.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
</table>


## Why?

While some users are content to use the MODIS black-sky albedo at local solar noon and the white-sky albedo measures as provided in [**MCD43A3**](https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table/mcd43a3_v006), most researchers want to make use of the BRDF model parameters ([**MCD43A1**](https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table/mcd43a1_v006)) to obtain black-sky albedos at other solar illumination angles or to combine the black-sky and white-sky albedos as a function of optical depth to calculate "actual" or blue-sky albedos for validation studies.

[**MCD43A1**](https://modis.ornl.gov/documentation.html#MCD43) is the unofficial MODIS product code for **black sky**, **whitesky**, and **blue sky albedo** calculated using the BRDF model parameters and user-specified **optical depth** and **solar zenith angle**. The MCD43A Calculated Albedo Product is available through the [MODIS Global Subsetting and Visualization Tool](https://modis.ornl.gov/cgi-bin/MODIS/global/subset.pl) maintained by the [ORNL DAAC](https://daac.ornl.gov). Alternatively, it can be calculated from MODIS HDF files using the binaries available at the [product page](https://www.umb.edu/spectralmass/terra_aqua_modis) maintained by the PI of MCD43, Dr. Crystal Schaaf.

In MODIS Collection 5, MCD43 was generated in 16-day composite time intervals. In Collection 6 (C6), MCD43 is a daily product; thus, the data volume is prohibitively large. The large data volume for MCD43 in C6 was the motivation for writing these scripts. 

## How?

### Input data from AppEEARS

The albedo formulas are implemented in Python. They take as input a subset of the MCD43A1 product in NetCDF format generated through [AppEEARS](https://lpdaac.usgs.gov/tools/data_access/appeears). AppEEARS is a data delivery tool maintained by the LP DAAC offering spatial and temporal subsets of MODIS data and other data products. You will need to sign up for a NASA Earthdata account to use AppEEARS.

To request a compatible subset, choose **Area Sample** from the ***Extract*** button in AppEEARS. Click **Start a new request**. 
1. Provide a name for your sample.
2. Choose a spatial extent by either uploading an ESRI Shapefile or GeoJSON, or by drawing a box/polygon on the map.
3. Choose a temporal extent for the subset.
4. Search for MCD43A1 under ***Select the layers to include in the sample*** and choose the C6 product: **MCD43A1.006**
5. Select the following layers:
    *   **BRDF_Albedo_Parameters_Band\[`1-10`\]
    *   **BRDF_Albedo_Band_Mandatory_Quality_Band\[`1-10`\]
    *   **BRDF_Albedo_Parameters_vis**
    *   **BRDF_Albedo_Band_Mandatory_Quality_vis**
    *   **BRDF_Albedo_Parameters_nir**
    *   **BRDF_Albedo_Band_Mandatory_Quality_nir**
    *   **BRDF_Albedo_Parameters_shortwave**
    *   **BRDF_Albedo_Band_Mandatory_Quality_shortwave**
6. For ***File Format***, choose **NetCDF**.
7. For ***Projection***, choose **Geographic**.

You will receive an email upon order completion.

# Daily albedos from MODIS BRDF Parameters (MCD43A1)

Comprehensive Python workflow for calculating black-, white-, and blue-sky albedos from the MODIS BRDF model parameters product ([MCD43A1](https://www.umb.edu/spectralmass/terra_aqua_modis/modis_brdf_albedo_product_mcd43)) using **custom solar zenith angles and optical depth** inputs. 

The plot below depicts the *differenced* daily black sky albedo time series (2x) for a single pixel in central Florida as computed by: **(1)** *you* in the notebooks [1_Workflow.ipynb](1_Workflow.ipynb) and [2_Batch.ipynb](2_Batch.ipynb), and **(2)** the [MODIS/VIIRS Global Subset Tool](https://modis.ornl.gov/cgi-bin/MODIS/global/subset.pl) hosted by [ORNL DAAC](https://daac.ornl.gov/):

![val](docs/img/browse0_validation.png)
*Continued: Our time series is identical to the ORNL DAAC's excluding rounding factors.* 

The difference should be related to the zenith angle. The concave time series resembles the zenith angle curve in the northern hemisphere. 

I can tell you with a lot of confidence that the tiny disparity (see y-axis precision) between the two time series comes from the ORNL DAAC's solar zenith angle calculator because it returns five decimal places. We're using the max precision allowed by `numpy` for our number.

## Notebooks
Some of these I've barely started on. `WIP == WORK IN PROGRESS`
<table>
    <tr>
        <td style="text-align:left"><a href="0_Introduction.ipynb">0_Introduction.ipynb</a></td>
        <td style="text-align:left"> <b>WIP</b> Fundamental concepts explained.</b></td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=0_Introduction.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="1_Workflow.ipynb">1_Workflow.ipynb</a></td>
        <td style="text-align:left">Process albedo model parameters to black, white, blue albedos</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=1_Workflow.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="2_Batch.ipynb">2_Batch.ipynb</a></td>
        <td style="text-align:left">Bundle workflow into a few functions and loop</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=2_Batch.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="3_Results.ipynb">3_Results.ipynb</a></td>
        <td style="text-align:left"> <b>WIP</b> Calculate statistics and plot</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=3_Results.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="4_Validate.ipynb">4_Validate.ipynb</a></td>
        <td style="text-align:left">Check results against MCD43A3 albedos</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=4_Validate.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    <tr>
        <td style="text-align:left"><a href="5_HDFs.ipynb">5_HDFs.ipynb</a></td>
        <td style="text-align:left"> <b>WIP</b> AppEEARS alternative, processing HDFs</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=5_HDFs.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
</table>

## Inputs

### MCD43A1    

Two options:
1. Input MCD43A1 time series in netCDF format via [AppEEARS](https://lpdaac.usgs.gov/tools/data_access/appeears). Or, if you don't have convenient Python environment 
2. you can batch download the HDFs from LP DAAC data pool by following the steps in [5_HDFs.ipynb](5_HDFs.ipynb).

AppEEARS is web interface for requesting subsets from LP DAAC's data pool. The notebooks assume you used AppEEARS to get the subset. You need to sign up for a NASA Earthdata account. It was just convenient to have the subset come out in the shape of Florida like I wanted.

To request a subset, choose **Area Sample** from the **Extract** button in AppEEARS. Click **Start a new request**. 
1. Provide a name for your sample.
2. Choose a spatial extent by either uploading an ESRI Shapefile or GeoJSON, or by drawing a box/polygon on the map.
3. Choose a temporal extent for the subset.
4. Search for MCD43A1 under **Select the layers to include in the sample** and choose the C6 product: **MCD43A1.006**
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


## Why?

While some users are content to use the MODIS black sky albedo at local solar noon and the white sky albedo measures as provided in [**MCD43A3**](https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table/mcd43a3_v006), most researchers want to make use of the BRDF model parameters ([**MCD43A1**](https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table/mcd43a1_v006)) to get  black sky albedo at different illumination angles or to combine the black sky and white sky albedo as a function of optical depth to get the blue sky albedo.

## How?

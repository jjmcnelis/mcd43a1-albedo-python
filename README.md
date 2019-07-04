# Daily albedos from MODIS BRDF Parameters (MCD43A1)

Comprehensive Python workflow for calculating black-, white-, and blue-sky albedos from the MODIS BRDF model parameters product ([MCD43A1](https://www.umb.edu/spectralmass/terra_aqua_modis/modis_brdf_albedo_product_mcd43)) using **custom solar zenith angles and optical depth** inputs. 

The plot below depicts the *differenced* daily black sky albedo time series (2x) for a single pixel in central Florida as computed by: **(1)** *you* in the notebooks [1_Workflow.ipynb](1_Workflow.ipynb) and [2_Batch.ipynb](2_Batch.ipynb), and **(2)** the [MODIS/VIIRS Global Subset Tool](https://modis.ornl.gov/cgi-bin/MODIS/global/subset.pl) hosted by [ORNL DAAC](https://daac.ornl.gov/):

![val](docs/img/browse0_validation.png)

## Notebooks
<table>
    <tr>
        <td style="text-align:left"><a href="0_Introduction.ipynb">0_Introduction.ipynb</a></td>
        <td style="text-align:left">Fundamental concepts explained.</b></td>
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
        <td style="text-align:left">Calculate statistics and plot</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=3_Results.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="4_Validate.ipynb">4_Validate.ipynb</a></td>
        <td style="text-align:left">Check results against MCD43A3 albedos</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=4_Validate.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    <tr>
        <td style="text-align:left"><a href="5_HDFs.ipynb">5_HDFs.ipynb</a></td>
        <td style="text-align:left">AppEEARS alternative, processing HDFs</td>
        <td style="text-align:center"><a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=5_HDFs.ipynb"><img src="https://mybinder.org/badge_logo.svg"></a></td>
    </tr>
</table>

## Inputs

## MCD43A1
An input MCD43A1 time series in netCDF format via AppEEARS. Or, if you don't have convenient Python environment you can batch download the HDFs from LPDAAC data pool. Go to the [inventory service](https://lpdaacsvc.cr.usgs.gov/services/inventory) page and figure out the query you need to get a list of HDFs. 

I know there to be two MODIS tiles that fully contain the state of Florida (h10v05, h10v06), so I queried random points spread apart a bit to make sure I got both tiles. I covered this briefly in [0_Introduction]()


`FIND this.. i did this convert really nicely once with shell/GDAL 2.x piping. old hdds in ut115.`

## Why?

While some users are content to use the MODIS black sky albedo at local solar noon and the white sky albedo measures as provided in [**MCD43A3**](https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table/mcd43a3_v006), most researchers want to make use of the BRDF model parameters ([**MCD43A1**](https://lpdaac.usgs.gov/dataset_discovery/modis/modis_products_table/mcd43a1_v006)) to get  black sky albedo at different illumination angles or to combine the black sky and white sky albedo as a function of optical depth to get the blue sky albedo.

## How?

[AppEEARS](https://lpdaac.usgs.gov/tools/data_access/appeears) is a data access at the LP DAAC that makes spatial and temporal subsets of a variety of remote sensing and model data. You need a NASA Earthdata account to use AppEEARS.

To request a compatible subset, choose **Area Sample** from the **Extract** button in AppEEARS. Click **Start a new request**. 
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

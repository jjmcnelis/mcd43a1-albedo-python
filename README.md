<!--
*** Daily albedos from MODIS BRDF Parameters (MCD43A1)
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for build-url, contributors-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<p align="center">

  <a href="https://github.com/jjmcnelis/mcd43a1-albedo-python">
    <img src="https://avatars1.githubusercontent.com/u/27017734?s=400&u=eefe777334306e1bcfcc26351b39d28e0631cc54&v=4" alt="Logo" width="80" height="80">
  </a>

  <!-- HEADER -->
  <h2 align="center">Daily Albedo from MODIS BRDF/Albedo Parameters</h2>

  <!-- DESCRIPTION -->
  <p style="display: block; margin-left: auto; margin-right: auto; width: 70%; text-align:center">
    A comprehensive workflow for calculating black, white, and blue sky albedos from the MODIS BRDF model parameters product (MCD43A1) using custom solar zenith angles and optical depth inputs.
  </p>

  <!-- LINKS -->
  <p align="center">
    <a href="https://github.com/jjmcnelis/mcd43a1-albedo-python"><strong>Explore the docs »</strong></a> · 
    <a href="https://mybinder.org/v2/gh/jjmcnelis/mcd43a1-albedo-python/master?filepath=0_Introduction.ipynb">Interactive Tutorial</a> ·
    <a href="https://github.com/jjmcnelis/mcd43a1-albedo-python/issues">Report a Bug</a>
  </p>
  
  <!-- BROWSE1 -->
  <figure class="image">
    <img src="docs/img/fig1.png" alt="{{ include.description }}" style="display: block; margin-left: auto; margin-right: auto; width: 75%;">
    <figcaption style="display: block; margin-left: auto; margin-right: auto; width: 75%; text-align:center">
      <i>Figure 1. Time series blue, black, and white albedo for 2018 for one pixel in central Florida.</i>
    </figcaption>
  </figure>



</p>

<!-- ABOUT THE PROJECT -->
## Overview

Use BRDF/albedo model parameters to get black sky albedo at different illumination angles or to combine the black sky and white sky albedo as a function of optical depth to get the blue sky albedo. This series of notebooks covers that process.

<!-- BROWSE2 -->
<figure class="image">
  <img src="docs/img/fig2.png" alt="{{ include.description }}" style="display: block; margin-left: auto; margin-right: auto; width: 75%;">
  <figcaption style="display: block; margin-left: auto; margin-right: auto; width: 85%; text-align:center"><i>Figure 2. Time series difference between black sky albedo computed by the ORNL DAAC MODIS Global Tool and this workflow as a way to validate my implementation of the albedo algorithm. Solar zenith angles are shown by the shaded region.</i></figcaption>
</figure>

## Notebooks
The entire process to implement the albedo algorithm in Python is documented in these notebooks:
<table style="display: block; margin-left: auto; margin-right: auto; width: 85%; text-align:center">
    <th>Notebook</th>
    <th>Description</th>
    <tr>
        <td style="text-align:left"><a href="https://nbviewer.jupyter.org/github/jjmcnelis/mcd43a1-albedo-python/blob/master/0_Introduction.ipynb">0_Introduction.ipynb</a></td>
        <td style="text-align:left">Interactive tutorial covers core concepts. Launch it in MyBinder: <a href="https://mybinder.org/v2/gh/jjmcnelis/florida_usgs_albedo_evapotranspiration/master?filepath=0_Introduction.ipynb"><img src="https://mybinder.org/badge_logo.svg" style="vertical-align:sub; padding-left: 15px;"></a></td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="https://nbviewer.jupyter.org/github/jjmcnelis/mcd43a1-albedo-python/blob/master/1_Workflow.ipynb">1_Workflow.ipynb</a></td>
        <td style="text-align:left">Process albedo model parameters to black, white, blue albedos.</td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="https://nbviewer.jupyter.org/github/jjmcnelis/mcd43a1-albedo-python/blob/master/2_Batch.ipynb">2_Batch.ipynb</a></td>
        <td style="text-align:left">Bundle workflow into a few functions and loop.</td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="https://nbviewer.jupyter.org/github/jjmcnelis/mcd43a1-albedo-python/blob/master/3_Validate.ipynb">3_Validate.ipynb</a></td>
        <td style="text-align:left">Check results against MCD43A3 albedos.</td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="https://nbviewer.jupyter.org/github/jjmcnelis/mcd43a1-albedo-python/blob/master/4_Results.ipynb">4_Results.ipynb</a></td>
        <td style="text-align:left">Calculate stats and plot. Also write outputs for Florida USGS evapotranspiration model. (<code>WIP</code>)</td>
    </tr>
    <tr>
        <td style="text-align:left"><a href="https://nbviewer.jupyter.org/github/jjmcnelis/mcd43a1-albedo-python/blob/master/5_HDFs.ipynb">5_HDFs.ipynb</a></td>
        <td style="text-align:left">AppEEARS alternative, processing HDFs. (<code>WIP</code>)</td>
    </tr>
    <tr><td colspan=2><i>Some of these I've barely started </i>(<code>WIP</code>)<i>. Stay tuned.</i></td></tr>
</table>

## Inputs  

You have two options:
1. Input MCD43A1 time series in netCDF format via [AppEEARS](https://lpdaac.usgs.gov/tools/data_access/appeears). Or, if you don't have convenient Python environment 
2. you can batch download the HDFs from LP DAAC data pool by following the steps in [5_HDFs.ipynb](5_HDFs.ipynb).       

Go to the [first notebook (1_Workflow.ipynb)](1_Workflow.ipynb) for more details about data access.

## Outputs
The figure and text below explain a little bit about how I validated the workflow against MCD43A1 albedos computed by the ORNL DAAC's MODIS Global Tool (Figure 2) and the standard black and white sky albedos from the MCD43A3 product (Figure 4).

The difference between our time series and the one computed by the ORNL DAAC (*see Figure 2 above*) is negligible, purely rounding error; and, if you check the plot above, it's obviously related to the zenith angle. The concave shape of the curve resembles the zenith angle change through the year. The disparity (see y-axis precision in scientific notation at the top of y1-axis) comes from the ORNL DAAC's zenith angle calculator. It returns five decimal places. I didn't do any rounding so we're using the max precision allowed by `numpy`, some huge number of decimal places.        

<!-- BROWSE4 -->
<figure class="image">
  <img src="docs/img/fig4.png" alt="{{ include.description }}" style="display: block; margin-left: auto; margin-right: auto;">
  <figcaption style="display: block; margin-left: auto; margin-right: auto; width: 60%; text-align:center;"><i>Figure 4. Difference of black sky albedo rasters (2) for January 1, 2018: a. released in MCD43A3 standard product, and b. computed in this workflow.</i></figcaption>
</figure>        

<br>
Figure 4 depicts the differenced black sky albedo rasters for January 1, 2018 computed by (1) the SIPS for the standard black and white sky albedo product (MCD43A3) and (2) this workflow.

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<!-- CONTACT -->
## Contact

Jack McNelis - jjmcnelis@outlook.com

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/badge/contributors-1-orange.svg?style=flat-square
[contributors-url]: https://github.com/jjmcnelis/mcd43a1-albedo-python/graphs/contributors

[license-shield]: https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
[license-url]: https://choosealicense.com/licenses/mit/

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/jack-mcnelis

#####################################################################################
# NOTES
#####################################################################################
# Probably want to loop through pixels to do the entirety of FL
# NOTE! THIS IS NOT THE SAME SZA AS CALCULATED BY ORNL DAAC!!! HOW DO WE CALC SZA??
# I probably want to precalculate SZA!!!!!!!!!!!
#
#
#attributes(nc$var)$names[2]
#[1] "BRDF_Albedo_Band_Mandatory_Quality_nir"
#attributes(nc$var)$names[3]
#[1] "BRDF_Albedo_Band_Mandatory_Quality_shortwave"
#attributes(nc$var)$names[4]
#[1] "BRDF_Albedo_Band_Mandatory_Quality_vis"
#attributes(nc$var)$names[5]
#[1] "BRDF_Albedo_Parameters_nir"
#attributes(nc$var)$names[6]
#[1] "BRDF_Albedo_Parameters_shortwave"
#attributes(nc$var)$names[7]
#[1] "BRDF_Albedo_Parameters_vis"
#
#
#### Loop by time?? Too slow
#
# # Initialize start and count to read one timestep of the variable.
# start <- rep(1,nir_ndims)
# start[nir_ndims] <- 1
# count <- nir_varsize
# count[nir_ndims] <- 365
# # Get data for tstep n
# data_nir <- ncvar_get(nc, nir_par, start=start, count=count)
# 
# 
# print(data_nir[1,33,33, 1])
# print(data_nir[2,33,33, 1])
# print(data_nir[3,33,33, 1])
# print(data_nir[1,33,33, 2])
# print(data_nir[2,33,33, 2])
# print(data_nir[3,33,33, 2])
# 
# remove(data_nir, start, count)
# 
# stop()
#
#####################################################################################
# ::METHODS::
# 
#   https://modis.ornl.gov/documentation.html
#   https://www.umb.edu/spectralmass/terra_aqua_modis/modis_brdf_albedo_product_mcd43
#
# Black-sky Albedo = 
#   Parameters_01 + 
#   Parameters_02 * (-0.007574 + (-0.070987 * szn^2) + (0.307588 * szn^3)) + 
#   Parameters_03 * (-1.284909 + (-0.166314 * szn^2) + (0.041840 * szn^3))
#
# White-sky Albedo = 
#   Parameters_01 + 
#   Parameters_02 * (0.189184) + 
#   Parameters_03 * (-1.377622) 
#
# Blue-sky (Actual) Albedo = 
#   White-sky Albedo * f(optical depth, solar zenith angle, aerosol type, band) + 
#   Black-sky Albedo * (1 - f(optical depth, solar zenith angle, aerosol type, band)) 
#


library(ncdf4)
library(data.table)

## SET UP DIRECTORIES

#data_dir <- 'D:\\Projects\\FloridaMCD43A\\data\\' #HOME
data_dir <- 'E:\\Dropbox\\MCD43A\\data\\' # WORK
out_dir <- paste(data_dir, 'output\\', sep = '')
suppressWarnings(dir.create(out_dir))

nc_in = paste(data_dir, 'MCD43A1.006_aid0001.nc', sep = '')
nc <- nc_open(nc_in)




# Get data.frame of lat/lon combinations
pxcoords <- expand.grid(nc$dim$lat$vals, nc$dim$lon$vals)
# Get time dimension values
nc_time <- nc$dim$time$vals


#####################################################################################
# Albedo calculation functions
#####################################################################################

Deg2Rad = 3.1415926535/180
SF = 1 # Scale factor (0.001) already applied to values by ncdf4. Set to 1.

BlackSA <- function(p1arr, p2arr, p3arr, szn){
  sznrad = szn*Deg2Rad
  result = (p1arr*SF)+(p2arr*SF)*(-0.007574+(-0.070987*sznrad^2)+(0.307588*sznrad^3))+(p3arr*SF)*(-1.284909+(-0.166314*sznrad^2)+(0.041840*sznrad^3))
  return(result)
}

WhiteSA <- function(p1arr, p2arr, p3arr){
  result = (p1arr*SF)+(p2arr*SF)*0.189184+(p3arr*SF)*-1.377622
  return(result)
}

BlueSA <- function(WSA, BSA, LUTVal){
  result = (WSA*LUTVal)+(BSA*(1-LUTVal))
  return(result)
}

#####################################################################################
# NetCDF operations
#####################################################################################

library(ncdf4)
library(data.table)
library(RAtmosphere)

# nc_in = "D:\\Projects\\FloridaMCD43A\\data\\MCD43A1.006_aid0001.nc" # HOME
nc_in = "E:\\Dropbox\\MCD43A\\data\\MCD43A1.006_aid0001.nc" #WORK
nc <- nc_open(nc_in)

# Get data.frame of lat/lon combinations
pxcoords <- expand.grid(nc$dim$lat$vals, nc$dim$lon$vals)
# Get time dimension values
nc_time <- nc$dim$time$vals

#####################################################################################
# Processing routine
#####################################################################################

#####################################################################################
# TEST (NIR from bsawsa_check.xls) YYYYDOY = 2000055
#####################################################################################

# Print all NetCDF metadata
print(nc)
# Print NetCDF file attributes
attributes(nc)$names
# Print NetCDF variable names
attributes(nc$var)$names
# Print NIR attributes
attributes(nc$var$BRDF_Albedo_Parameters_nir)$names
# Print scale factor for NIR (0.001)
print(nc$var$BRDF_Albedo_Parameters_nir$scaleFact)

NIRpar1 = 203
NIRpar2 = 128
NIRpar3 = 25
#SolarZenithAngle = 46.198238
# NOTE! THIS IS NOT THE SAME SZA AS CALCULATED BY ORNL DAAC!!!
SolarZenithAngle = SZA(timein = as.Date(55, origin = as.Date("2000-01-01")), Lat = 35.958767, Lon = -84.287433) # Day 55 
LUT2000055 = 0.113

bsaresult <- BlackSA(NIRpar1, NIRpar2, NIRpar3, SolarZenithAngle)
bsaresult
wsaresult <- WhiteSA(NIRpar1, NIRpar2, NIRpar3)
wsaresult
BlueSA(wsaresult, bsaresult, LUT2000055)


#####################################################################################
# TEST2 NetCDF Ops
# https://cran.r-project.org/web/packages/ncdf4/ncdf4.pdf
#####################################################################################

# Init data.table with date column
albedo_nir <- data.table(Date = as.Date(nc_time, origin = as.Date("2000-01-01")))

# data_nir = ncvar_get(nc, nir_par) # Too large (11.6 gb)
nir_varsize = nir_par$varsize
nir_ndims = nir_par$ndims
nt = nir_varsize[nir_ndims]


# Now read in the value of the timelike dimension


print(data_nir) # Too large to print
# Print NIR Parameter 1, lat33, lon33, timestep 1
data_nir[1,33,33]
# Print NIR Parameter 2, lat33, lon33, timestep 1
data_nir[2,33,33]
# Print NIR Parameter 3, lat33, lon33, timestep 1
data_nir[3,33,33]

#####################################################################################
# TEST3 Looping
#####################################################################################

# Read all data from variable BRDF_Albedo_Parameters_nir
nir_par = nc$var[[5]]

# Get variable metadata
nir_varsize = nir_par$varsize # Variable size (3[parameters] 1826[lon] 1562[lat] 365[timesteps])
nir_ndims = nir_par$ndims # Number of dimensions (4)
nt = 365 # Number of timesteps
# Get time value array
timeval <- ncvar_get(nc, nir_par$dim[[nir_ndims]]$name, start=1, count=365)

# Index NA locations
naIndices <- which(ncvar_get(nc, nir_par, start = c(1,1,1,1), count = c(3, 1826, 1562, 1)) == NA)

# Loop through pxcoords
for(yx in 1:nrow(pxcoords)){
  
  for(tstep in 1:nt){
    # Initialize start and count to read one timestep of the variable.
    start <- rep(1,nir_ndims)
    start[nir_ndims] <- tstep
    count <- nir_varsize
    count[nir_ndims] <- 1
    # Get data for tstep n
    data_nir <- ncvar_get(nc, nir_par, start=start, count=count)
    
    print(pxcoords[yx,1])
    print(pxcoords[yx,2])
    
    print(as.Date(timeval[tstep], origin = as.Date("2000-01-01")))
    
    result <- BlackSA(
      data_nir[1, yx, yx], 
      data_nir[2, yx, yx], 
      data_nir[3, yx, yx], 
      SZA(timein = as.Date(timeval[tstep], origin = as.Date("2000-01-01")), Lat = pxcoords[yx,1], Lon = pxcoords[yx,2])
      )
    
    print(result)

  }
  
}



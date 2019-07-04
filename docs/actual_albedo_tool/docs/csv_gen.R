#####################################################################################
# Functions to read NetCDF data and export CSVs
# https://lpdaac.usgs.gov/sites/default/files/public/elearning/AppEEARS_NC_QualityFiltering_R.html#13_export_netcdf-4_files
#####################################################################################

library(ncdf4)
library(data.table)

######################################
#### Generate CSV for nc variable ####
######################################

gen_csv <- function(ncdf, var, ntsteps, outfile){
  
  # Get nc variable identifiers
  if (var == 'BRDF_Albedo_Parameters_nir'){
    param <- 5
    paramqc <- 2
  }
  if (var == 'BRDF_Albedo_Parameters_shortwave'){
    param <- 6
    paramqc <- 3
  }
  if (var == 'BRDF_Albedo_Parameters_vis'){
    param <- 7
    paramqc <- 4
  }
  
  # Open output csv for writing
  out_csv <- file(outfile)
  
  # Loop through timesteps
  for(tstep in 1:ntsteps){

    # Initialize start and count to read one timestep of the variable.
    start <- rep(1,ndims)
    start[ndims] <- tstep
    count <- varsize
    count[ndims] <- 1
    pardata <- ncvar_get(ncdf, var, start=start, count=count)

    print(pardata)

    stop()

    }
  
  }









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



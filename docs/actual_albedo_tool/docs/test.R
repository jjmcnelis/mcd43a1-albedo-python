library(ncdf4)
library(data.table)

## SET UP DIRECTORIES

#data_dir <- 'D:\\Projects\\FloridaMCD43A\\data\\' #HOME
data_dir <- 'E:\\Dropbox\\MCD43A\\data\\' # WORK
out_dir <- paste(data_dir, 'output\\', sep = '')
suppressWarnings(dir.create(out_dir))

## OPEN NC

nc_in = paste(data_dir, 'MCD43A1.006_aid0001', 'jan', '.nc', sep = '')
nc <- nc_open(nc_in)

## GET LAT,LON, TIME DIMENSIONS

# Get data.frame of lat/lon combinations
pxcoords <- expand.grid(nc$dim$lat$vals, nc$dim$lon$vals)
# Get time dimension values
nc_time <- nc$dim$time$vals

## GET NIR VARIABLE

var <- nc$var[[5]]
varsize <- var$varsize # Variable size (3[parameters] 1826[lon] 1562[lat] #[timesteps])
ndims <- var$ndims # Number of dimensions (4)
nt <- varsize[ndims]

# Initialize start and count to read entire variable.
vardata <- ncvar_get(nc, var)

for(tstep in 1:nt){
  result <- BlackSA(
    vardata[1, yx, yx], 
    data_nir[2, yx, yx], 
    data_nir[3, yx, yx], 
    SZA(timein = as.Date(nc_time, origin = as.Date("2000-01-01")), Lat = pxcoords[yx,1], Lon = pxcoords[yx,2])
  )
}


#############################################################################
# !make
#
# makefile name: actual_albedo_hdf.mk
#
#
#
# !Variables
#
#       VARIABLES       DESCRIPTION
#       ~~~~~~~~~       ~~~~~~~~~~~
#       TARGET          Program executable name
#       ADD_CFLAGS      Additional compiler options
#       LIB             Libraries
#       INC             Include files
#       OBJ             Object files
#       INC_FILES       All include files
#
# !Env_variables
#
#       ENV VARIABLES   DESCRIPTION
#       ~~~~~~~~~~~~~   ~~~~~~~~~~~
#       CFLAGS          C compiler flags which are set by the script
#                       (e.g. v2.1_n32_f77)
#       CC              C compile command set by the script
#       PGSINC          Include directory of PGS toolkit set by the script
#     	API_INC         Include directory of M-API set by the script
#       HDFINC          Include directory of HDF set by the script
#       HDFEOS_INC      Include directory of HDF-EOS set by the script
#       PGSLIB          Library of PGS toolkit set by the script
#     	API_LIB		Library of M-API set by the script 
#       HDFLIB          Library of HDF set by the script
#       HDFEOS_LIB      Library of HDF-EOS set by the script
#
#
#!END
#############################################################################
# Redefine stuff that you're not supposed to but else it won't run locally
CC = cc 

# Define executable name
TARGET = actual_albedo_hdf.exe

# Combine the includes using pre-defined includes and your inclueds
INC   = -I$(HDFINC) -I$(HDFEOS_INC) 

# Additional flags for compile
ADD_CFLAGS = -O3 -DLYNX  -ffloat-store


# Combine the includes using pre-defined library and your library
#at BU
LIB =   -L/usr/lib/x86_64-linux-gnu/ -lhdfeos -lgctp \
        -L/usr/lib/ -lmfhdf -ldf -lz  \
         -L/usr/lib/x86_64-linux-gnu/ -ljpeg -lm -L/usr/lib/x86_64-linux-gnu/ -lsz -static -Wall


# additional flags for conditional compilation
# -DAVHRR     compile specifically for use with AVHRR data.
#             when compiled without this flag, it generally assumes modis data as the input.
# COMP_FLAGS = -DAVHRR

# Define object files
OBJ = actual_albedo_hdf.o 


all: $(TARGET)

$(OBJ) : $(INC_FILES)

# Make the process
$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(COMP_FLAGS) $(ADD_CFLAGS) $(OBJ) $(LIB)  -o $(TARGET)

#
# Rules
#
.c.o: $(INC_FILES)
	$(CC) $(CFLAGS) $(COMP_FLAGS) $(ADD_CFLAGS) $(INC) -c $< -o $@

#******************* End of make file *******************************





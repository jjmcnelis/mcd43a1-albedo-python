/*
this one changed to print black, white, actual
this one gets on value only, spits out only one value

albedo.ascii.1x1.1.exe   79 55 17 0. 1. 1 
gets
bsa 0.056740 wsa 0.065986 actual 0.061159




!C***********************************************************************

  !DESCRIPTION:
  This program reads MODIS BRDF/albedo product MOD43B (out1), computes 
  the black and white sky albedos (with Wolfgang's simple polynomial   
  equation) and then assuming optical depth is an input, computes the  
  actual albedo (linear combination of white and black-sky albedo) and 
  saves in binary format.                                         
                                                                       
  !INPUTS:                                                               
        MOD43B1: semiempirical BRDF parameters in binary
                 (use MRT to convert data from HDF file to binary file)
        Optical depth                                                  
        Solar zenith angle                                                     
 
  !OUTPUTS:                                                             
        Actual albedo                                                  
                                                                       
  !DEVELOPERS:
        Feng Gao, Crystal Schaaf (Boston University)
	fgao@crsa.bu.edu, schaaf@crsa.bu.edu

  !REVISIONS:
        1/2000, 5/2000, 1/2002

!END *******************************************************************
*/
    

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h> /*must be added to get atof to work smm */

#define SUCCESS 0	
#define FAILURE -1
#define Max_Str_Len 100

#ifndef PI
#define PI 3.1415926535
#endif
#define D2R PI/180.0

#define NROWS 1	 /* smm */
#define NCOLS 1	 /* smm */
#define FILLV 32767
#define SCALE_FACTOR 0.001

typedef struct 
{
  float iso;
  float vol;
  float geo;
} PARAMETERS;


typedef struct
{
  int aerosol_type;
  int bandno;
  float solar_zenith;
  float optical_depth;
  float skyl;
} SKYL;


void  usage(void);
void  read_skyl_table (float skyl_lut[2][10][90][50]);
float get_skyl (SKYL s,float skyl_lut[2][10][90][50]);
float cal_bsa(PARAMETERS p, SKYL s);
float cal_wsa(PARAMETERS p);
float cal_actual_albedo(PARAMETERS p, SKYL s);

int main(int argc, char **argv)
{
  char outname[Max_Str_Len];
  char iso_name[Max_Str_Len];
  char vol_name[Max_Str_Len];
  char geo_name[Max_Str_Len];

  int   i,j,k;
  short int par_data[3];
  float albedo;
  float skyl_lut[2][10][90][50];

  PARAMETERS p;
  SKYL s;
  FILE *iso, *vol, *geo, *out, *fpin;

int ascii_iso,ascii_vol,ascii_geo; /* smm when read .txt, last varies fastest*/
float  in_szn,in_od ;
int in_band;
char infile[512];

/*	
	ascii_iso[0][0]=atoi(argv[1]);
	ascii_vol[0][0]=atoi(argv[2]);
	ascii_geo[0][0]=atoi(argv[3]);
	s.solar_zenith= atof(argv[4]);
	s.optical_depth=atof(argv[5]);
	s.bandno =      atoi(argv[6])-1;
*/	
 
 	
 /* printf("\nRead SKYL table \n");*/
  read_skyl_table(skyl_lut);

 
 
/*called $0 full_input_file szn od  band */
/*called $0  /home/smm/c/test.in2  65.568601  .2 8  */

strcpy(infile,argv[1]);
s.solar_zenith= atof(argv[2]);
s.optical_depth=atof(argv[3]);
s.bandno = atoi(argv[4])-1;   
	 
	
fpin=fopen(infile,"r");
/* 32 24 7  65.568601  .2 8 gets bsa 0.030571 wsa 0.026897 actual 0.029061*/
	
 while  (fscanf(fpin,"%i %i %i",&ascii_iso,&ascii_vol,&ascii_geo) != -1)
	{
	/*printf("linesmm ascii_iso %d, ascii_vol %d, ascii_geo %d, in_szn %f, in_od %f, in_band %d  \n",ascii_iso,ascii_vol,ascii_geo,in_szn,in_od,in_band );*/
	/*
	printf("linesmm %d %d %d %f %f %d ",ascii_iso,ascii_vol,ascii_geo,s.solar_zenith,s.optical_depth,s.bandno);    
	*/
	par_data[0]		=ascii_iso;
	par_data[1]		=ascii_vol;
	par_data[2]		=ascii_geo;
	 
	
		      /* check parameters and see if they are valid values */
      if((par_data[0]!=FILLV) & (par_data[1]!=FILLV) & (par_data[2]!=FILLV)) 
      	{

			/* calculate real parameter values */ 
	p.iso=par_data[0]*SCALE_FACTOR;
	p.vol=par_data[1]*SCALE_FACTOR;
	p.geo=par_data[2]*SCALE_FACTOR;
	

			/* NOTE: need to make sure you get the right LUT band order: 
			   # bandwidth:    0: 0.620-0.670  (red)
			   #               1: 0.841-0.876  (nir)
			   #               2: 0.459-0.479
			   #               3: 0.545-0.565
			   #               4: 1.230-1.250
			   #               5: 1.628-1.652
			   #               6: 2.105-2.155
			   #          BB   7: 0.400-0.700  (vis)
			   #          BB   8: 0.700-4.000  (nir)
			   #          BB   9: 0.250-4.000  (sw)
			   you may need index your band order to LUT band order, 
			   such as: s.bandno=index[band]; */

			/* assume continental type, but finally need to get it from BRDF_type
			   0: continental 1: maritime */

	s.aerosol_type=0;
	
			/* get SKYL from SKYL lookup table (it depends on optical depth,
			   solar zenith angle, aerosol type, and bands) */
	
	s.skyl=get_skyl(s,skyl_lut);
	
	albedo=cal_actual_albedo(p,s);
	printf("actual %f\n",albedo);
      }
      else
	 
	 
 	printf("bsa 32767.00 wsa 32767.00 actual 32767.00\n");
	
	}/* end read*/
		   
}


void usage(void)
{

  printf("\nUsage: actual_albedo_bin.exe [-iso][-vol][-geo][-band][-od][-szn][-out] \n\n");
  printf("   -iso  <f_iso_file>    input MOD43B1 isotropic binary file\n");
  printf("   -vol  <f_vol_file>    input MOD43B1 volumetric binary file\n");
  printf("   -geo  <f_geo_file>    input MOD43B1 geometric binary file\n");
  printf("   -band <band_no>       input band no in the MOD43B1 (int, 1-10)\n"); 
  printf("   -od   <optical_depth> input optical depth (float, range: 0.0-1.0)\n");
  printf("   -szn  <solar_zenith>  input solar zenith angle you want to compute\n");
  printf("                         (float, range: 0.0-89.0 degrees)\n");
  printf("   -out  <albedo_file>   output file to save actual albedo in binary\n\n");

}



/*******************************************************
   read SKYL LUT from pre-generated LUT file (skyl_lut.dat)
   2 aerosol types
   10 bands (7 MODIS bands + 3 broad bands (VIS, NIR, SW)
   90 degrees (0-89 degrees with 1 degree step
   50 optical depth ( 0-1 with 0.02 step)
********************************************************/

void read_skyl_table (float skyl_lut[2][10][90][50])
{
  
  char str[Max_Str_Len];
  FILE *in;
  int aerosol,band,szn,od,NBANDS;

  NBANDS=10;  /* MODIS defaults */

/*  if((in=fopen("/var/www/cgi-bin/MODIS/GR_1/skyl_lut.dat","r"))==NULL) {  The SKY Look up table was changed so that
, we can call the LUT from a local dir. THis was changed by Suresh. When we moved to the GLobal tool*/ 
  if((in=fopen("./plug_1/skyl_lut_mlv.dat","r"))==NULL) {
    printf("Can't open SKYL LUT file (./plug_1/skyl_lut_mlv.dat) \n");
    exit(1);
  }

  for(aerosol=0;aerosol<2;aerosol++) {
    fscanf(in,"%s %s\n",str,str);
      for(band=0;band<NBANDS;band++) {
	fscanf(in,"%s %s\n",str,str);
	for(od=0;od<51;od++)
	  fscanf(in,"%s ",str);
	for(szn=0;szn<90;szn++){ 
	  fscanf(in,"%s ",str);
	  for(od=0;od<50;od++) {
	    fscanf(in,"%f ",&(skyl_lut[aerosol][band][szn][od]));
	    /*printf("%5.3f ",(skyl_lut[aerosol][band][szn][od]));*/
	  }
	}
	/*printf("\n"); getchar();*/
      }
  }  
 
  fclose(in);
}




/* get SKYL value from LUT */

float get_skyl (SKYL s,float skyl_lut[2][10][90][50])
{
  int szn,od;

  szn=(int)(s.solar_zenith+0.5); /*get solar zenith index */
  if(szn==90) szn=89;
  od=(int)(s.optical_depth/0.02+0.5); /*get optical depth index */
  if(od>=50) od=49;

  return skyl_lut[s.aerosol_type][s.bandno][szn][od];
} 



/********************************************
 calculate actual albedo with the linear 
 combination of white-sky and black-sky albedo  
**********************************************/

float cal_actual_albedo(PARAMETERS p, SKYL s)
{

  float actual_albedo;
  float bsa,wsa;

  bsa=cal_bsa(p,s);
  wsa=cal_wsa(p);

  actual_albedo=wsa*s.skyl + bsa*(1.0-s.skyl);

  if(actual_albedo<0) actual_albedo=0;
  if(actual_albedo>1) actual_albedo=1; 
	printf("bsa %f wsa %f ",bsa,wsa);
  return actual_albedo;

}



/*******************************************
 calculate black-sky albedo according to 
 Wolfgang's polynomial albedo representation 
********************************************/

float cal_bsa(PARAMETERS p, SKYL s)
{

  int i;
  float bsa,bsa_weight[3],szn,sq_szn,cub_szn;
  float poly_coef[3][3]={1.0,-0.007574,-1.284909,
			 0.0,-0.070987,-0.166314,
			 0.0, 0.307588, 0.041840};

  szn=s.solar_zenith*D2R;
  sq_szn=szn*szn;
  cub_szn=sq_szn*szn;

  for(i=0;i<3;i++)
    bsa_weight[i]=poly_coef[0][i]+poly_coef[1][i]*sq_szn+poly_coef[2][i]*cub_szn;

  bsa=bsa_weight[0]*p.iso+bsa_weight[1]*p.vol+bsa_weight[2]*p.geo;
  
  return bsa;

}


/* calculate white-sky albedo with fixed weight */
float cal_wsa(PARAMETERS p)
{

  float wsa,wsa_weight[3]={1.0,0.189184,-1.377622};
  wsa=wsa_weight[0]*p.iso+wsa_weight[1]*p.vol+wsa_weight[2]*p.geo;
  return wsa;

}
 


/*
!C***********************************************************************

  !DESCRIPTION:
  This program reads MODIS BRDF/albedo parameters saved in binary files, computes 
  the black and white sky albedos (with Wolfgang's simple polynomial   
  equation) and then assuming optical depth is an input, computes the  
  actual albedo (linear combination of white and black-sky albedo) and 
  saves in binary format.                                         
                                                                       
  !INPUTS:                                                               
        semiempirical BRDF parameters in binary
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
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SUCCESS 0	
#define FAILURE -1
#define Max_Str_Len 100

#ifndef PI
#define PI 3.1415926535
#endif
#define D2R PI/180.0

#define NROWS 1200
#define NCOLS 1200
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

void main(int argc, char **argv)
{
  char outname[Max_Str_Len];
  char iso_name[Max_Str_Len];
  char vol_name[Max_Str_Len];
  char geo_name[Max_Str_Len];

  int   i,j;
  short int par_data[3];
  float albedo;
  float skyl_lut[2][10][90][50];

  PARAMETERS p;
  SKYL s;
  FILE *iso, *vol, *geo, *out;

  /* see if commmand line are correct or not */
  if(argc!=15) {
    usage();
    exit(1);
  }

  /* parse command line */
  for(i=1;i<argc;i++){
    if(strcmp(argv[i],"-iso")==0)
      strcpy(iso_name,argv[++i]);
    else
      if(strcmp(argv[i],"-vol")==0)
	strcpy(vol_name,argv[++i]);
      else
	if(strcmp(argv[i],"-geo")==0)
	  strcpy(geo_name,argv[++i]);
	else
	  if(strcmp(argv[i],"-band")==0)
	    /* use 0-based index */
	    s.bandno = atoi(argv[++i])-1;  
	  else
	    if(strcmp(argv[i],"-out")==0)
	      strcpy(outname,argv[++i]);
	    else 
	      if(strcmp(argv[i],"-od")==0)
		s.optical_depth=atof(argv[++i]);
	      else
		if(strcmp(argv[i],"-szn")==0)
		  s.solar_zenith=atof(argv[++i]);
		else{
		  printf("\nWrong option:%s\n",argv[i]);
		  usage();
		  exit(1);
		}
  }

  printf("\nRead SKYL table \n");
  read_skyl_table(skyl_lut);

  /* open BRDF parameters binary files */
  if((iso=fopen(iso_name,"rb"))==NULL){
    printf("Parameter %s open file error!\n",iso_name);
    exit(1);
  }

  /* open BRDF parameters binary files */
  if((vol=fopen(vol_name,"rb"))==NULL){
    printf("Parameter %s open file error!\n",vol_name);
    exit(1);
  }

  /* open BRDF parameters binary files */
  if((geo=fopen(geo_name,"rb"))==NULL){
    printf("Parameter %s open file error!\n",geo_name);
    exit(1);
  }

  /* open output file for write */
  if((out=fopen(outname,"wb"))==NULL){
    printf("Output %s openfile error!\n",outname);
    exit(1);
  }

  printf("Computing actual albedo\n");

  /* to process each line */
  for(i=0; i<NROWS; i++){

    /* process each pixel */
    for(j=0; j<NCOLS; j++){

      /* read BRDF parameters */
      fread(&par_data[0],1,2,iso);
      fread(&par_data[1],1,2,vol);
      fread(&par_data[2],1,2,geo);

      /* for Linux system, you need to comment out the following lines 
	 - from Stephen M. Margle on May 3, 2004 */
      //par_data[0]= ((par_data[0]>> 8)  | (par_data[0]<< 8));   
      //par_data[1]= ((par_data[1]>> 8)  | (par_data[1]<< 8));  
      //par_data[2]= ((par_data[2]>> 8)  | (par_data[2]<< 8));  

      /* check parameters and see if they are valid values */
      if(par_data[0]!=FILLV ) {

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

      }
      else
	albedo=FILLV;

      fwrite(&albedo,1,sizeof(float),out);
    }
  }  

  printf("Finished\n\n");
}


void usage(void)
{

  printf("\nUsage: actual_albedo_bin.exe [-iso][-vol][-geo][-band][-od][-szn][-out] \n\n");
  printf("   -iso  <f_iso_file>    input isotropic binary file\n");
  printf("   -vol  <f_vol_file>    input volumetric binary file\n");
  printf("   -geo  <f_geo_file>    input geometric binary file\n");
  printf("   -band <band_no>       input band no (int, 1-10)\n"); 
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

  if((in=fopen("skyl_lut.dat","r"))==NULL) {
    printf("Can't open SKYL LUT file (skyl_lut.dat) \n");
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
 


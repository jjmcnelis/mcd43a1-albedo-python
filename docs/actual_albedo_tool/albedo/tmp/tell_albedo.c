/**
 * calculate black-sky, white-sky and actual albedo for a single pixel
 * by accepting the following command line parameters:
 * [-par]    parameters for three kernels
 * They should be entered in the sequence of f_iso, f_vol and f_geo 
 * and should be the actual float number. 
 * Please note the file values in MODIS products are the scaled interger 
 * numbers. They should be re-scaled to the actual values by dividing by
 * 1000. Normally, these parameters are in the range of [0..1]
 * [-fd]    fraction of the diffuse light
 * You can use your own estimation or by running program "tell_skyl.exe" 
 * to get the estimation from a pre-defined lookup table. The data range
 * should be [0..1]
 * [-szn]   solar zenith angle in degree
 * data range should be between 0 and 89 degrees  
 *
 * by Feng Gao at Boston University on July, 2002
 * contact fgao@crsa.bu.edu
 *
 **/

#include <stdio.h>
#include <math.h>

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

#ifndef PI
#define PI 3.1415926535
#endif
#define D2R PI/180.0

float cal_bsa(PARAMETERS p, SKYL s);
float cal_wsa(PARAMETERS p);
float cal_actual_albedo(PARAMETERS p, SKYL s);

void main(int argc, char **argv)
{

  int i;
  PARAMETERS p;
  SKYL s;

  if(argc!=9) {
    printf("Usage: %s [-par][-fd][-szn]\n",argv[0]);
    printf("Example: %s -par 0.2 0.10 0.03 -fd 0.2 -szn 45\n", argv[0]);
    exit(1);
  }

  /* parse command line */
  for(i=1;i<argc;i++){
    if(strcmp(argv[i],"-par")==0) {
      p.iso = atof(argv[++i]);
      p.vol = atof(argv[++i]);
      p.geo = atof(argv[++i]);
    }
    else
      if(strcmp(argv[i],"-fd")==0)
	s.skyl=atof(argv[++i]);
      else
	if(strcmp(argv[i],"-szn")==0)
	  s.solar_zenith=atof(argv[++i]);
	else{
	  printf("\nWrong option:%s\n",argv[i]);
	  printf("Usage: %s [-par][-fd][-szn]\n",argv[0]);
	  printf("Example: %s -par 0.2 0.10 0.03 -fd 0.2 -szn 45\n", argv[0]);
	  exit(1);
	}
  }

  printf("Black-sky albedo: %5.3f\n",cal_bsa(p,s));
  printf("White-sky albedo: %5.3f\n",cal_wsa(p));  
  printf("Actual surface albedo: %5.3f\n", cal_actual_albedo(p,s));

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
 

/**
 * A simple sample program to calculate black-sky albedo at given solar
 * zenith angle with polynomial equation (see Wolfgang Lucht et. al., IEEE 
 * TGRAS, Vol. 38, No.2, PP977-998).
 *
 * Input:  BRDF parameters for RossThick-LiSparse-R model
 *         solar zenith angle
 *
 * Output: black-sky albedo
 *
 * compile program: 
 * cc polynomial_bsa.c -o polynomial_bsa.exe
 *
 * example:
 * polynomial.exe <in_par0> <in_par1> <in_par2> <out_bsa> <szn>
 *
 * by Feng Gao (fgao@crsa.bu.edu)
 *
 **/

#include <stdio.h>
#include <math.h>

#define RES_1KM      /*use 1-km res. product as input */
/*#define RES_5KM*/  /*use 5-km res. product as input */

#ifdef RES_1KM
   #define NROWS 21600
   #define NCOLS 43200
   #define RES 926.625427
#endif

#ifdef RES_5KM
   #define NROWS 4320
   #define NCOLS 8640
   #define RES 4633.126953
#endif

#define SCALE 0.001
#define FILLV 32767
#define PI 3.14159
#define D2R PI/180.0

/* define three parameters for RTLSP model*/
typedef struct 
{
  float iso;
  float vol;
  float geo;
} PARAMETERS;

/* function to calculate black-sky albedo */
float cal_bsa(PARAMETERS p, float szn);

void main(int argv, char **argc)
{
  int i, j, k;
  unsigned short int temp_int[3];
  unsigned short int bsa_int;
  float szn, bsa;
  PARAMETERS p;
  FILE *in[3], *out;
  
  if(argv!=6) {
    printf("Usage: %s <in_par0> <in_par1> <in_par2> <out_bsa> <szn>\n",argc[0]);    
    exit(1);
  }
  
  for(i=0; i<3; i++)
    if((in[i]=fopen(argc[i+1],"rb"))==NULL) {
      printf("can't open file %s\n", argc[i+1]);
      exit(1);
    }

  if((out=fopen(argc[4],"wb"))==NULL) {
    printf("can't open file %s for write\n",argc[4]);
    exit(1);
  }

  szn = atof(argc[5]);

  /* do image */  
  for(i=0; i<NROWS; i++)
    for(j=0; j<NCOLS; j++) {
      for(k=0; k<3; k++)
	fread(&(temp_int[k]), sizeof(short),1,in[k]);
      if(temp_int[0] != FILLV) {
	/* get three parameters in float */
	p.iso = temp_int[0]*SCALE;
	p.vol = temp_int[1]*SCALE;
	p.geo = temp_int[2]*SCALE;
	bsa = cal_bsa(p, szn);
	/* convert to integer with scale factor = SCALE */
	bsa_int = (int) (bsa/SCALE + 0.5);
      } 
      else
	bsa_int = FILLV;
      fwrite(&bsa_int, sizeof(short), 1, out);
    }
  
  for(i=0; i<3 ; i++)
    fclose(in[i]);
  fclose(out);

}


/**
 * calculate black-sky albedo according to 
 * Wolfgang's polynomial albedo representation 
 **/

float cal_bsa(PARAMETERS p, float szn)
{

  int i;
  float bsa,bsa_weight[3],sq_szn,cub_szn;
  float poly_coef[3][3]={1.0,-0.007574,-1.284909,
                         0.0,-0.070987,-0.166314,
                         0.0, 0.307588, 0.041840};

  szn*=D2R;
  sq_szn=szn*szn;
  cub_szn=sq_szn*szn;

  for(i=0;i<3;i++)
    bsa_weight[i]=poly_coef[0][i]+poly_coef[1][i]*sq_szn+poly_coef[2][i]*cub_szn;

  bsa=bsa_weight[0]*p.iso+bsa_weight[1]*p.vol+bsa_weight[2]*p.geo;
  
  return bsa;

}


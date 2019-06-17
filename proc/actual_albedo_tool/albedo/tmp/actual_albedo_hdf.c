/*
!C***********************************************************************

  !DESCRIPTION:
  This program reads MODIS BRDF/albedo product MOD43A1, computes 
  the black and white sky albedos (with Wolfgang's simple polynomial   
  equation) and then assuming optical depth is an input, computes the  
  actual albedo (linear combination of white and black-sky albedo) and 
  prints it out in HDF format.                                         
                                                                       
  !INPUTS:                                                               
        MOD43A1 
        Optical depth                                                  
        Solar zenith angle                                                     
 
  !OUTPUTS:                                                             
        Actual albedo (blue sky albedo)
                                                                       
  !DEVELOPERS:
        Feng Gao, Crystal Schaaf (Boston University)
	fgao@crsa.bu.edu, schaaf@crsa.bu.edu

  !REVISIONS:
        1/2000, 5/2000, 6/2015

!END *******************************************************************
*/
    

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "mfhdf.h"

#define SUCCESS 0	
#define FAILURE -1
#define Max_Str_Len 100

#ifndef PI
#define PI 3.1415926535
#endif
#define D2R PI/180.0

#define ALBEDO_fillv 32767
#define ALBEDO_factor 0.001
#define ALBEDO_offset 0

char m_band_names[10][20] = {"Band1","Band2","Band3","Band4",
				"Band5","Band6","Band7","vis","nir","shortwave"};

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

typedef struct
{
  int nrows;
  int ncols;
  int nbands;
  int16   ***par_data;
  float64 scale;
  float64 offset;
  int16   fillv;  
	int32 hdf_id;
	int32 sds_id[10];
} BRDF_SDS;


typedef struct
{
  int nrows;
  int ncols;
  int nbands;
  int16   ***albedo_data;
  float64 scale;
  float64 offset;
  int16   fillv;  
	int32 hdf_id;
	int32 sds_id[10];
} ALBEDO_SDS;


void  usage(void);
void  read_skyl_table (float skyl_lut[2][10][90][50]);
float get_skyl (SKYL s,float skyl_lut[2][10][90][50]);
float cal_bsa(PARAMETERS p, SKYL s);
float cal_wsa(PARAMETERS p);
float cal_actual_albedo(PARAMETERS p, SKYL s);
int  readSDSattribute(BRDF_SDS *b);
void writeSDSattribute(ALBEDO_SDS *a); 
void alloc_1dim_contig (void **ptr,int d1,int elsize);
void alloc_3dim_contig (void ****ptr,int d1,int d2,int d3,int elsize);
void CloseAlbedoFile(ALBEDO_SDS *a);
void CloseInput(BRDF_SDS *b);
char parname[Max_Str_Len];
char outname[Max_Str_Len];

void main(int argc, char **argv)
{
  int  i,j,band;
  float albedo;
  float skyl_lut[2][10][90][50];

  intn status;
  int32 sd_id,sds_id,sds_index,sdout_id,sdsout_index,sdsout_id; 
  int32 n_datasets,n_file_attrs,index;
  int32 rank,data_type,n_attrs;
  char name[MAX_NC_NAME];
	 
  ALBEDO_SDS a;
  BRDF_SDS b;
  PARAMETERS p;
  SKYL s;
//  MODFILE *par,*out;

  /* see if commmand line are correct or not */
  if(argc!=9) {
    usage();
    exit(1);
  }

  /* parse command line */
  for(i=1;i<argc;i++){
    if(strcmp(argv[i],"-par")==0)
      strcpy(parname,argv[++i]);
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

  /* open BRDF parameters HDF-EOS file and get factor scale and offset*/
 

  printf("Read BRDF SDS attribute\n");
  /* read neccessary BRDF parameters SDS attributes */

  if(0 != readSDSattribute(&b)){
		printf("Read hdf info failed: %s\n", parname);
		exit (1);
	}

  /* initialize albedo SDS attribute */
  a.fillv=ALBEDO_fillv; a.scale=ALBEDO_factor; a.offset=ALBEDO_offset;
  a.nrows=b.nrows; a.ncols=b.ncols; a.nbands=b.nbands;

  printf("Write ALbedo SDS attribute\n");

 writeSDSattribute(&a);

  printf("Allocate memory\n");
  /* allocate all memory */
  //alloc_3dim_contig((void ****)&a.albedo_data,1,a.ncols,a.nbands,sizeof(int16));  
  //alloc_3dim_contig((void ****)&b.par_data,b.ncols,b.nbands,3,sizeof(int16));
	int16 (*buf_par)[3] = (int16 (*)[3])malloc(sizeof(int16)*b.ncols*3);
	int16 *buf_alb = (int16 *)malloc(sizeof(int16)*b.ncols);
 
  printf("Computing actual albedo\n");
  /* to process each line */

	int32 start[3] = {0, 0, 0};
	int32 edge[3] = {1, b.ncols, 3};

	for(band=0; band<b.nbands; band++){
  	for(i=0; i<b.nrows; i++){
    	start[0]=i;
    	
			status =SDreaddata(b.sds_id[band], start, NULL ,edge,buf_par);
			assert(status == SUCCEED);
    	/* process each pixel */
    	for(j=0; j<b.ncols; j++){
				/* check parameters and see if they are valid values */
				if(buf_par[j][0]==b.fillv ) {
					buf_alb[j] = a.fillv;
					continue;
				}

	  		/* calculate real parameter values */ 
	  		p.iso=(float)(buf_par[j][0]-b.offset)*b.scale;
	  		p.vol=(float)(buf_par[j][1]-b.offset)*b.scale;
	  		p.geo=(float)(buf_par[j][2]-b.offset)*b.scale;

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
	  		s.bandno=band;

	  		/* assume continental type, but finally need to get it from BRDF_type
	   			0: continental 1: maritime */

	  		s.aerosol_type=0;

	  		/* get SKYL from SKYL lookup table (it depends on optical depth,
	     		solar zenith angle, aerosol type, and bands) */

	  		s.skyl=get_skyl(s,skyl_lut);

	  		albedo=cal_actual_albedo(p,s);
	  		buf_alb[j]=albedo/a.scale+a.offset;
    	}	// for col
    
    	/* write albedo data */
  		status=SDwritedata(a.sds_id[band], start, NULL, edge, buf_alb);
			HEprint(stdout, 0);
			assert(status == SUCCEED);
  	}  // for row
	}	// for band
        
	CloseInput(&b);
	CloseAlbedoFile(&a);

  printf("Finished\n\n");
}


void usage(void)
{

  printf("\nUsage: actual_albedo.exe [-par][-od][-szn][-out] \n\n");
  printf("   -par <par_file>      input MCD43A1 (V005 or V006, in HDF format)\n");
  printf("   -od  <optical_depth> input optical depth (float, range: 0.0-1.0)\n");
  printf("   -szn <solar_zenith>  input solar zenith angle you want to compute\n");
  printf("                        (float, range: 0.0-89.0 degrees)\n");
  printf("   -out <albedo_file>   output file to save actual albedo (in HDF format)\n\n");

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
 

/* read SDS attribute */
//void readSDSattribute(BRDF_SDS *b, MODFILE *par) 
int readSDSattribute(BRDF_SDS *b)
{
       intn status;
	     int32 n_datasets,n_file_attrs,index;
	     int32 dim_sizes[10];
	     int32 rank,data_type,n_attrs;
	     char name[MAX_NC_NAME];
	     int i;
	     int32 sd_id,sds_id,sds_index,att_id; 
	     float64 float_att;
	     int16 in16_att;
	     

     /* find image size and number of bands*/
	     sd_id=SDstart(parname, DFACC_READ);

	      sds_index=SDnametoindex(sd_id,"BRDF_Albedo_Parameters_Band1");
				if(sds_index < 0){
					printf("Didn't find BRDF_Albedo_Parameters_Band1 from %s.\n", parname);
					SDend(sd_id);
					return -1;
				}

	      sds_id=SDselect(sd_id,sds_index);
				if(sds_id <= 0){
					SDend(sd_id);
					return -1;
				}

	  	  status=SDgetinfo(sds_id,name,&rank,dim_sizes,&data_type,&n_attrs);
				if(status != SUCCEED){
					printf("Get dataset info failed.\n");
					SDend(sd_id);
					return -1;
				}

	      b->nrows = dim_sizes[0];
        b->ncols = dim_sizes[1];
        //b->nbands = dim_sizes[2];
				if(dim_sizes[0] == 2400){
					b->nbands = 10;
				}
				else if(dim_sizes[0] == 4800){
					 b->nbands = 2;
				}
				else{
					 b->nbands = 0;
				}

   	 	  /* Get scale factor */
   		 att_id=SDfindattr(sds_id,"scale_factor");
   		 if (att_id==FAIL){
   		 	printf ("Cannot find attribute field  matched with 'Scale Factor' \n");                 
   		 	exit(1);
   		 	}
   		 else{
   		   status=SDreadattr(sds_id,att_id,&float_att);
   		     if(status==FAIL){
   		       	printf ("Cannot read attribute field 'Scale Factor' \n");
   		      exit(1);
   		      }
   		    else{
   		       	b->scale = float_att;
   		     }
   		 	}
   	//	 	printf("bscale is %f\n",b->scale);
   		 	
   		 	
   		 	 att_id=SDfindattr(sds_id,"add_offset");
   		 if (att_id==FAIL){
   		 	printf ("Cannot find attribute field  matched with 'add_offset'\n");                 
   		 	exit(1);
   		 	}
   		 else{
   		   status=SDreadattr(sds_id,att_id,&float_att);
   		     if(status==FAIL){
   		       	printf ("Cannot read attribute field 'add_offset'\n");
   		      exit(1);
   		      }
   		    else{
   		       	b->offset = float_att;
   		     }
   		 	}
  // 		 	printf("boffset is %f\n",b->offset);
   		 	
   		 	att_id=SDfindattr(sds_id,"_FillValue");
   		 if (att_id==FAIL){
   		 	printf ("Cannot find attribute field  matched with '_FillValue'\n");                 
   		 	exit(1);
   		 	}
   		 else{
   		   status=SDreadattr(sds_id,att_id,&in16_att);
   		     if(status==FAIL){
   		       	printf ("Cannot read attribute field '_FillValue'\n");
   		      exit(1);
   		      }
   		    else{
   		       	b->fillv = in16_att;
   		     }
   		 	}
   		 	

  /*printf("nrows:%d ncols:%d nbands:%d offset:%f scale:%f fillv:%d\n",b->nrows,
    b->ncols,b->nbands,b->offset,b->scale,b->fillv);*/
     	  status=SDendaccess (sds_id);

				int band;
				char sds_name[100];
				for(band=0; band<b->nbands; band++){
					sprintf(sds_name, "BRDF_Albedo_Parameters_%s", m_band_names[band]);
					index = SDnametoindex(sd_id, sds_name);
					if(index < 0){
						printf("Didn't find %s from %s\n", sds_name, parname);
						return -1;
					}
					b->sds_id[band] = SDselect(sd_id, index);
				}

				b->hdf_id = sd_id;

	return 0;
}




/* write albedo SDS attribute */
void writeSDSattribute(ALBEDO_SDS *a) 
{
  int ret;
  long int tmp1L = 1L;
  long int tmp2L = 2L;
  int32 rank = 2;
  int32 dimsizes[3];
  int16 valid_range[2]={0,1000};

  dimsizes[0]=a->nrows;
  dimsizes[1]=a->ncols;
  
  int32 sd_id, sds_id,sds_index,dim_id,dim_index; /* SD interface and data set identifiers */
  intn status; /* status returned by some routines; has value SUCCEED or FAIL */
  int i, j;


  sd_id = SDstart (outname, DFACC_CREATE);
	a->hdf_id = sd_id;

	int band;
	char sds_name[100];
	for(band=0; band<a->nbands; band++){

		sprintf(sds_name, "Actual_Albedo_%s", m_band_names[band]);
	  sds_id = SDcreate (sd_id, sds_name, DFNT_INT16, rank, dimsizes);
		a->sds_id[band] = sds_id;
		/*comp_info c_info;
		c_info.deflate.level = 8;
		SDsetcompress(sds_id, COMP_CODE_DEFLATE , &c_info);*/
		HDF_CHUNK_DEF cdef;
		cdef.chunk_lengths[0] = 100;
		cdef.chunk_lengths[1] = a->ncols;
		cdef.comp.chunk_lengths[0] = 100;
		cdef.comp.chunk_lengths[1] = a->ncols;
		cdef.comp.comp_type = COMP_CODE_DEFLATE;
		cdef.comp.cinfo.deflate.level = 8;
		SDsetchunk(sds_id, cdef, HDF_CHUNK | HDF_COMP);
  
	  status = SDsetattr (sds_id, "long_name", DFNT_CHAR8, 30, sds_name);
  	status = SDsetattr (sds_id, "units", DFNT_CHAR8, 30, (VOIDP)"albedo, no units");
	  status = SDsetattr (sds_id, "valid_range", DFNT_INT16, tmp2L, (VOIDP)valid_range);
  	status = SDsetattr (sds_id, "_FillValue", DFNT_INT16, tmp1L, (VOIDP)&(a->fillv));
	  status = SDsetattr (sds_id, "scale_factor", DFNT_FLOAT64, tmp1L, (VOIDP)&(a->scale));
  	status = SDsetattr (sds_id, "add_offset", DFNT_FLOAT64, tmp1L, (VOIDP)&(a->offset));
	}
}

void CloseInput(BRDF_SDS *b)
{
	int band;
	for(band=0; band<b->nbands; band++){
		SDendaccess(b->sds_id[band]);
	}
	SDend(b->hdf_id);
}

void CloseAlbedoFile(ALBEDO_SDS *a)
{
	int b;
	for(b=0; b<a->nbands; b++){
		SDendaccess(a->sds_id[b]);
	}
	SDend(a->hdf_id);
}


/* memory allocation function are taken from MOD43B */

void
alloc_3dim_contig (
		     void ****ptr,
		     int d1,
		     int d2,
		     int d3,
		     int elsize)
/*
!C****************************************************************************

!Description:
This function allocates a 3-dimensional array of type integer using calloc.
The memory is allocated completely contiguously, not just contiguous
in each dimension.

!Input Parameters:
d1  	Length of the 1st dimension to be allocated contiguously
d2  	Length of the 2nd dimension to be allocated contiguously
d3  	Length of the 3rd dimension to be allocated contiguously
elsize	Size of the type to be allocated in bytes

!Output Parameters:
ptr     Adress of the pointer to which memory was allocated

!Returns:
void

!Revision History::
Original version - Ambrals V4.0
Wolfgang Lucht (Boston University)

!Team-unique Header:
This software was developed for NASA under contract NAS5-31369.

!References and Credits
Principal Investigators: Alan H. Strahler, Boston University
	                 Jan-Peter Muller, University College London
		         alan@crsa.bu.edu, jpmuller@ps.ucl.ac.uk
Developers: Wolfgang Lucht, Crystal Schaaf (Boston University)
	    and numerous other contributors (thanks to all)
	    wlucht@crsa.bu.edu, schaaf@crsa.bu.edu

!Design Notes
The pointer-adress returned referes to a pointer pointing to
the type void. So in calling this function you need to cast
the pointer to the type you're using to a pointer to void.

!END**************************************************************************
*/
{
   void *p = NULL;
   void **pp = NULL;
   void ***ppp = NULL;
   int i = 0;

   /* allocate the data array */
   alloc_1dim_contig ((void **) (&p), d1 * d2 * d3, elsize);

   /* alloc the double pointers */
   alloc_1dim_contig ((void **) (&pp), d1 * d2, sizeof (void *));

   /* and again for the first dimensional pointers */
   alloc_1dim_contig ((void **) (&ppp), d1, sizeof (void **));

   /* first set the d1 pointers */
   for (i = 0; i < d1; i++) {
      ppp[i] = pp + (i * d2);
   }

   /* next set all of the d2 pointers */
   for (i = 0; i < d1 * d2; i++) {
      pp[i] = (char *) p + (i * d3 * elsize);
   }

   *ptr = ppp;

   return;
}




void
alloc_1dim_contig (
		     void **ptr,
		     int d1,
		     int elsize)
/*
!C****************************************************************************

!Description:
This function allocates a 1-dimensional array of type integer using calloc.
The memory is allocated contiguously.

!Input Parameters:
d1  	Length of the 1st dimension to be allocated contiguously
elsize	Size of the type to be allocated in bytes

!Output Parameters:
ptr     Adress of the pointer to which memory was allocated

!Returns:
void

!Revision History::
Original version - Ambrals V4.0
Wolfgang Lucht (Boston University)

!Team-unique Header:
This software was developed for NASA under contract NAS5-31369.

!References and Credits
Principal Investigators: Alan H. Strahler, Boston University
	                 Jan-Peter Muller, University College London
		         alan@crsa.bu.edu, jpmuller@ps.ucl.ac.uk
Developers: Wolfgang Lucht, Crystal Schaaf (Boston University)
	    and numerous other contributors (thanks to all)
	    wlucht@crsa.bu.edu, schaaf@crsa.bu.edu

!Design Notes
The pointer-adress returned referes to a pointer pointing to
the type void. So in calling this function you need to cast
the pointer to the type you're using to a pointer to void.

!END**************************************************************************
*/
{
   void *p = NULL;

   p = calloc (d1, elsize);
   if (!p) {
      printf ("Memory allocation error in alloc_1dim_contig\n");
      exit(1);
   }
   *ptr = p;

   return;
}


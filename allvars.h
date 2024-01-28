#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>


#define X 0
#define Y 1
#define Z 2

typedef struct
{
  int Npart[6];
  double mass[6];
  double time;
  double   redshift;
  int      flag_sfr;
  int      flag_feedback;
  unsigned int npartTotal[6];
  int      flag_cooling;
  int      num_files;
  double   BoxSize;
  double   Omega0;
  double   OmegaLambda;
  double   HubbleParam; 
  int flag_stellarage;
  int flag_metals;
  unsigned int npartTotalHighWord[6];
  int  flag_entropy_instead_u;
  char     fill[256 - 6*4 - 6*8 - 2*8 - 2*4 - 6*4 - 2*4 - 4*8 - 2*4 - 6*4 - 1*4];  /* fills to 256 Bytes */
} io_encabezado;

io_encabezado encabezado;

typedef struct 
{
  unsigned int id;
  float pos[3];
  float vel[3];
  float masa;
  float accel[3];
  float accelMag;
}Particulas;

Particulas *part;

double G = 43007.1;
double etha = 1e-2; //parámetro que controla la precisión de dt (lo que variamos en el segundo punto del parcial)
double eps = 2.0; //longitud de suavizado[kpc] (lo que variamos en el primer punto del parcial)
int nTotal;

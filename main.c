#include "allvars.h"


int main(int argc, char *argv[])
{

  int i, j, dummy;
  int nPartConMasa, nMin, nMax;
  double totalTime, dt;
  char *infile;
  FILE * fDatos;

  infile = argv[1];

  printf("Leyendo el archivo %s\n",infile);

  fDatos = fopen(infile,"r");

  // leyendo encabezado
  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);
  fread(&encabezado, sizeof(io_encabezado), 1, fDatos);
  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);  

  nTotal = 0;
  for( i=0; i<6; i++ )
    nTotal += encabezado.Npart[i];

  printf("%d particulas en el snapshot\n",nTotal);
   
  part = (Particulas *)malloc((size_t)nTotal*sizeof(Particulas));

  // leyendo posiciones
  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);

  for( i=0; i<nTotal; i++ )
    fread(&part[i].pos[0], sizeof(float), 3, fDatos);

  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);  

  // leyendo velocidades
  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);

  for( i=0; i<nTotal; i++ )
    fread(&part[i].vel[0], sizeof(float), 3, fDatos);

  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);  

  // leyendo ids
  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);

  for( i=0; i<nTotal; i++ )
    fread(&part[i].id, sizeof(unsigned int), 1, fDatos);

  fread(&dummy, sizeof(int), 1, fDatos);
  printf("dummy = %d\n",dummy);  

  // leyendo masas
  nPartConMasa = 0;
  for(i=0; i<6; i++)
    {
      if( encabezado.Npart[i]>0 )
	{
	  if( encabezado.mass[i]>0.0 )
	    nPartConMasa = nPartConMasa + 0;
	  else
	    nPartConMasa = nPartConMasa + encabezado.npartTotal[i];
	}
    }
  
  printf("particulas con masa explicita = %d\n",nPartConMasa);
  
  if( nPartConMasa>0  ) 
    { 
      fread(&dummy,sizeof(dummy),1,fDatos);
      printf("dummy masa                        = %.9d",dummy);
    }
  
  nMin = nMax = 0;  
  for( j=0; j<6; j++)
    {
      nMax=nMax+encabezado.npartTotal[j];

      printf("tipo %d = %d particulas [ %d , %d)\n",j,encabezado.npartTotal[j],nMin,nMax);

      if( encabezado.npartTotal[j]>0 )
	{
	  if( encabezado.mass[j]>0.0 )
	    {
	      for( i=nMin; i<nMax; i++)
		part[i].masa = encabezado.mass[j];
	    }
	  else
	    {
	      for( i=nMin; i<nMax; i++)
		{
		  fread(&part[i].masa,sizeof(float),1,fDatos);
		} 
	    }
	  nMin = nMax;
	}
    }
 
  if( nPartConMasa>0  )  
    fread(&dummy,sizeof(dummy),1,fDatos);

  fclose(fDatos);

  //  exit(0);
  
  totalTime = 1.0;//tiempo total de simulación[Gyr]
  dt = 1.0/1000.0;

  evolve(totalTime, dt);//evoluciona la simulación

  free(part);//libero la estructura de partículas
  
  return 0;
}

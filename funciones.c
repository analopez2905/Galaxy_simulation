#include "allvars.h" 

int evolve(double totalTime, double dt)
{

  int i, counter=0, counter2=0;
  double t =0.0;
  char nombreArchivo[1000];

  sprintf(nombreArchivo,"./output/sim_eps_galaxia_%.4d",counter); //genero nombre para los archivos
  imprimirSnapshot(nombreArchivo, t);//invoco la función para imprimir nuevamente las condiciones iniciales
  
  // calcula aceleracion sobre todas las particulas 
  acceleration();//llamamos la función aceleración

  while( t<totalTime )
    {
      //(esta simulación utiliza un tiempo adaptativo)
      tiempoAdactativo(&dt);//calcular el dt 

      printf("tiempo = %.10lf  dt = %.10lf\n",t,dt);

      // drift
      for( i=0; i<nTotal; i++ )
	{
	  part[i].pos[X] = part[i].pos[X] + 0.5*dt*part[i].vel[X];
	  part[i].pos[Y] = part[i].pos[Y] + 0.5*dt*part[i].vel[Y];
	  part[i].pos[Z] = part[i].pos[Z] + 0.5*dt*part[i].vel[Z];
	}
      
      // calcula aceleracion sobre todas las particulas
      acceleration();
      
      // kick - evoluciono la velocidad
      for( i=0; i<nTotal; i++ )
	{
	  part[i].vel[X] = part[i].vel[X] + dt*part[i].accel[X];
	  part[i].vel[Y] = part[i].vel[Y] + dt*part[i].accel[Y];
	  part[i].vel[Z] = part[i].vel[Z] + dt*part[i].accel[Z];
	}
      
      // drift - evoluciono la posición
      for( i=0; i<nTotal; i++ )
	{
	  part[i].pos[X] = part[i].pos[X] + 0.5*dt*part[i].vel[X];
	  part[i].pos[Y] = part[i].pos[Y] + 0.5*dt*part[i].vel[Y];
	  part[i].pos[Z] = part[i].pos[Z] + 0.5*dt*part[i].vel[Z];
	}
      
      t += dt;
      counter++;
      
      //if( counter%100 == 0 )
      //{
      //  counter2++;
      // sprintf(nombreArchivo,"sim_eps_galaxia_%.3d",counter2);
      //imprimo el archivo con las nuevas pocisiones y velocidades
	  sprintf(nombreArchivo,"./output/sim_eps_galaxia_%.4d",counter);
	  imprimirSnapshot(nombreArchivo, t);
	  //}

    }

  return 0;
}

int acceleration(void)
{

  int i, j;
  double dr[3]; //diferencial de distancia para las tres dimensiones 
  double r, r3; //radio y radio al cubo
  double eps2; //longitud de suavizado para evitar problema de relajación de dos cuerpos para distancias muy pequeñas

  eps2 = eps*eps;

  for( i=0; i<nTotal; i++ )
    {
      //aceleración 1-1 por efectos de la ley de ravitación universal
      part[i].accel[X] = 0.0;
      part[i].accel[Y] = 0.0;
      part[i].accel[Z] = 0.0;

      for( j=0; j<nTotal; j++ )//recorrido sobre el número total de partículas
      {
	if( j != i )//asegura que no haya divergencia en el denominador
	  {

	    dr[X] = part[i].pos[X] - part[j].pos[X];
	    dr[Y] = part[i].pos[Y] - part[j].pos[Y];
	    dr[Z] = part[i].pos[Z] - part[j].pos[Z];

	    r = sqrt( dr[X]*dr[X] + dr[Y]*dr[Y] + dr[Z]*dr[Z] + eps2 );//distancia entre dos partículas
	    r3 = r*r*r;

	    part[i].accel[X] = part[i].accel[X] - G*part[j].masa*( dr[X]/r3 );
	    part[i].accel[Y] = part[i].accel[Y] - G*part[j].masa*( dr[Y]/r3 );
	    part[i].accel[Z] = part[i].accel[Z] - G*part[j].masa*( dr[Z]/r3 );

	  }
      }
      //como el cálculo se hizo por componentes aquí calculo la magnitud del vector aceleracion de cada partícula
      part[i].accelMag = sqrt( part[i].accel[X]*part[i].accel[X] +
			       part[i].accel[Y]*part[i].accel[Y]+
			       part[i].accel[Z]*part[i].accel[Z] );

    }

  return 0;
}

int imprimirSnapshot(char *nombreArchivo, double t)//nombre del archivo a imprimir y el tiempo asociado
{
  int i, dummy;
  FILE *fSnaps=fopen(nombreArchivo,"w");//abre el archivo de escritura (en binario)

  encabezado.time = t;//lo único que debemos actualizar en el encabezado es la variable time

  dummy = 256;//lo que vale la estructura de datos 
  
  // imprimo encabezado
  fwrite(&dummy, sizeof(int), 1, fSnaps);
  fwrite(&encabezado, sizeof(io_encabezado), 1, fSnaps);
  fwrite(&dummy, sizeof(int), 1, fSnaps);

  // imprimo posiciones
  dummy = nTotal*3*sizeof(float);
  fwrite(&dummy, sizeof(int), 1, fSnaps);
  for( i=0; i<nTotal; i++ )
    fwrite(&part[i].pos[0], sizeof(float), 3, fSnaps);
  fwrite(&dummy, sizeof(int), 1, fSnaps);//escribimos la posición de todas las partículas

  // imprimo velocidades
  dummy = nTotal*3*sizeof(float);
  fwrite(&dummy, sizeof(int), 1, fSnaps);
  for( i=0; i<nTotal; i++ )
    fwrite(&part[i].vel[0], sizeof(float), 3, fSnaps);
  fwrite(&dummy, sizeof(int), 1, fSnaps);
  
  // imprimo ids
  dummy = nTotal*sizeof(int);
  fwrite(&dummy, sizeof(int), 1, fSnaps);
  for( i=0; i<nTotal; i++ )
    fwrite(&part[i].id, sizeof(int), 1, fSnaps);
  fwrite(&dummy, sizeof(int), 1, fSnaps);

  // imprimo masas (en este caso de todas las partículas)
  dummy = nTotal*sizeof(float);
  fwrite(&dummy, sizeof(int), 1, fSnaps);
  for( i=0; i<nTotal; i++ )
    fwrite(&part[i].masa, sizeof(float), 1, fSnaps);
  fwrite(&dummy, sizeof(int), 1, fSnaps);
  
  fclose(fSnaps);
  
  return 0;
}

int tiempoAdactativo(double *dt)//recibe dirección de memoria de dt
{

  int i;
  double dtAux, accel;

  *dt = etha/sqrt( part[0].accelMag ); //calcula el tiempo adaptativo
  //lo calculo para cada partícula
  for( i=1; i<nTotal; i++ )
    {

      dtAux =  etha/sqrt( part[i].accelMag );
      //selecciono el más pequeño; con ese voy a correr
      if( dtAux<*dt )
	{
	  *dt = dtAux;
	  accel = part[i].accelMag;
	}

    } 

  printf("accel = %.10lf  dt = %.10lf\n",accel,*dt);

  return 0;
}

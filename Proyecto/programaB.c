#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/types.h>
#include<pthread.h>

#include"gestionarSemaforos.h"
#include"parametrosFourier.h"


void HiloSumarTerminos(ParametrosSerie* parametros)
{
int k,j,i=parametros->k;

float*** terminos=parametrosHilo.memoria.terminos;
float** resultados=parametrosHilo.memoria.resultados;
int sem=parametrosHilo.memoria.sem;

printf("\nDentro del hilo 2-%d con ID: %ld cuyo proceso es el %d\n\n",i+1,pthread_self(),getpid());
printf("Matriz de terminos de la serie %d\n",i+1);   
 for(j=0;j<7;j++)
 {
    resultados[i][j]=ao_arr[i];
    for(k=0;k<10;k++)
    {
       down(sem);
         printf("%f ",terminos[i][j][k]);
         resultados[i][j]+=terminos[i][j][k];
       up(sem);
    }
    printf("\n");
 }
 printf("\n\n");        
pthread_exit(0);  //Terminar hilo
}

void CalcularResultados()
{
  pthread_attr_t atributos;   
  pthread_t identificadores[3];    //Arreglo de 3 hilos
  int i;
  
  pthread_attr_init(&atributos);   
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);   
   for(i=0;i<3;i++)
   {
      pthread_create(&identificadores[i],&atributos,(void*)HiloSumarTerminos,(void*)&parametrosHilo.serie[i]);  //Creacion del hilo
      sleep(1);
   }

}


void liberarMemoria(float*** terminos, float** resultados)
{
int i;
for(i=0;i<3;i++)
     {
        free(terminos[i]);
     }
   free(resultados);
}

int main() {
 //Declaracion de variables      
   int shmid,i,j,sem;
   float *datos_matriz;
   float **m1,**m2,**m3;
   key_t llave, llave_sem;      
   
   llave=ftok("Terminos",'k');   
   //Semaforo
   llave_sem=ftok("S1",'k'); 
   parametrosHilo.memoria.sem=Crea_semaforo(llave_sem,1);
   
   shmid=shmget(llave,3*7*10*sizeof(datos_matriz[0]),IPC_CREAT|0777); //Region de los terminos
   datos_matriz=(float*)shmat(shmid,0,0);   
   
    //Reservar memoria para las matrices de terminos
   m1=(float**)malloc(7*sizeof(m1[0]));
   m2=(float**)malloc(7*sizeof(m2[0]));
   m3=(float**)malloc(7*sizeof(m3[0]));
   
    for(i=0;i<7;i++)
     {
        m1[i]=datos_matriz+i*10;
        m2[i]=datos_matriz+70+i*10;
        m3[i]=datos_matriz+140+i*10;
     }
   
   //Reservar memoria para el arreglo que agrupa las 3 matrices
   parametrosHilo.memoria.terminos=(float***)malloc(3*sizeof(float**));
   parametrosHilo.memoria.terminos[0]=m1;
   parametrosHilo.memoria.terminos[1]=m2;
   parametrosHilo.memoria.terminos[2]=m3;
  
  
   //Reservar memoria para la matriz de resultados
  parametrosHilo.memoria.resultados=(float**)malloc(3*sizeof(float*));
   for(i=0;i<3;i++)
     {
         parametrosHilo.memoria.resultados[i]=(float*)malloc(7*sizeof(float));
     }
  
   
  printf("\nSoy el proceso B con PID %d\n",getpid());
  //Se invoca a la funcion para calcular la suma de terminos
  
  CalcularResultados();       //Invocacion a la funcion para crear hilos  
  sleep(1);
  printf("\nResultados de la suma de terminos:\n");
  for(i=0;i<3;i++)
   { 
    for(j=0;j<7;j++)
     { 
      printf("%f ",parametrosHilo.memoria.resultados[i][j]);
     }
      printf("\n");
   }
  printf("\nProceso B finalizado\n");

   liberarMemoria(parametrosHilo.memoria.terminos, parametrosHilo.memoria.resultados);  
   shmdt(&datos_matriz);
   shmctl(shmid,IPC_RMID,0);
   return 0;
}

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/wait.h>
#include "gestionarSemaforos.h"

void Hilo1(Parametros *parametros)
{
  int *resultados=(int*)parametros->resultados;
  int semaforo=(int)parametros->semaforo;
  
  while(resultados[0]==0); 
  
  
  down(semaforo);
  printf("\nDentro del hilo 2-1 con ID: %ld cuyo proceso es el %d\n",pthread_self(),getpid());
  printf("Resultado del hilo 1-1: %d\n",resultados[0]);
  up(semaforo);
  
  pthread_exit(0);
}


void Hilo2(Parametros *parametros)
{
 int *resultados=(int*)parametros->resultados;
  int semaforo=(int)parametros->semaforo;
     while(resultados[1]==0);
     
   down(semaforo);     
   printf("\nDentro del hilo 2-2 con ID: %ld cuyo proceso es el %d\n",pthread_self(),getpid());
   printf("Resultado del hilo 1-2: %d\n",resultados[1]);
   up(semaforo);

   pthread_exit(0);
}
 

void Hilo3(Parametros *parametros)
{
  int *resultados=(int*)parametros->resultados;
  int i,semaforo=(int)parametros->semaforo;
   while(resultados[2]==0);
   
 
   down(semaforo);      
   printf("\nDentro del hilo 2-3 con ID: %ld cuyo proceso es el %d\n",pthread_self(),getpid());
   printf("Resultado del hilo 1-3:\n");
     for(i=2;i<22;i++)
     {
        printf("%d ",resultados[i]);   
     }
     up(semaforo);      
     printf("\n");   
     
   pthread_exit(0);
}


int main()
{

   pthread_t identificadores[3];    //Arreglo de 3 hilos
   
   int shmid1,semaforo,j;
   key_t llave1, llave_sem;
   Parametros parametros;
   
   llave1=ftok("Archivo1",'k');   
   shmid1=shmget(llave1,22*sizeof(int),IPC_CREAT|0777);  //Region de los resultados
   parametros.resultados=(int *)shmat(shmid1,0,0);
   
   
   printf("\nSoy el proceso padre 2 con ID: %d\n",getpid());
   
  //Crear semaforos
   llave_sem=ftok("S1",'k'); 
   parametros.semaforo=Crea_semaforo(llave_sem,1);
         
   pthread_create(&identificadores[0],NULL,(void*)Hilo1,(void*)&parametros);
   pthread_create(&identificadores[1],NULL,(void*)Hilo2,(void*)&parametros);
   pthread_create(&identificadores[2],NULL,(void*)Hilo3,(void*)&parametros);
 
  
  for(j=0;j<3;j++)
  {
    pthread_join(identificadores[j],NULL);
  }
    
   shmdt(&parametros.resultados);
   shmctl(shmid1,IPC_RMID,0);
   
   return 0;
}

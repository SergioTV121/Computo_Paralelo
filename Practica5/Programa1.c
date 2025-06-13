#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/wait.h>
#include "gestionarSemaforos.h"

int sumaGauss(int n) 
{
  return (n*(n+1))/2;
}

int factorial(int n)
{
  if(n<=1)
  {
    return 1;
  }  
  
  return n*factorial(n-1);
}

int fibonacci(int n)
{
  if(n<=1)
  {
    return n;
  }
  
  return fibonacci(n-1) + fibonacci(n-2);
}

void Hilo1(Parametros *parametros)
{
  int *resultados=(int*)parametros->resultados;
  int semaforo=(int)parametros->semaforo;
  
   printf("\nDentro del hilo 1-1 con ID: %ld cuyo proceso es el %d\n",pthread_self(),getpid());
    down(semaforo);
    resultados[0]=sumaGauss(100);
    up(semaforo); 
  
    pthread_exit(0);
}


void Hilo2(Parametros *parametros)
{
  int *resultados=(int*)parametros->resultados;
  int semaforo=(int)parametros->semaforo;
  
   printf("\nDentro del hilo 1-2 con ID: %ld cuyo proceso es el %d\n",pthread_self(),getpid());   
    down(semaforo);
    resultados[1]=factorial(10);
    up(semaforo); 
   
   pthread_exit(0);
}
 

void Hilo3(Parametros *parametros)
{
  int *resultados=(int*)parametros->resultados;
  int i,semaforo=(int)parametros->semaforo;
   
   sleep(1);
   down(semaforo);
   printf("\nDentro del hilo 1-3 con ID: %ld cuyo proceso es el %d\n",pthread_self(),getpid());
   for(i=1;i<=20;i++)
   {
     resultados[i+1]=fibonacci(i);   
   }
   up(semaforo);               
   pthread_exit(0);
}

int main()
{
      
   pthread_attr_t atributos;   
   pthread_t identificadores[3];    //Arreglo de 3 hilos
   
   int shmid1,semaforo;
   int *resultados;
   key_t llave1, llave_sem;
   Parametros parametros;
   
   //Se define el segmento de memoria compartida
   llave1=ftok("Archivo1",'k');   
   shmid1=shmget(llave1,22*sizeof(int),IPC_CREAT|0777);  //Region de los resultados
   parametros.resultados=(int *)shmat(shmid1,0,0);
   
   
   pthread_attr_init(&atributos);   
   pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);   
   printf("\nSoy el proceso padre 1 con ID: %d\n",getpid());
   
   //Crear semaforos
   llave_sem=ftok("S1",'k'); 
   parametros.semaforo=Crea_semaforo(llave_sem,1);
    
   sleep(2);      
   pthread_create(&identificadores[0],&atributos,(void*)Hilo1,(void*)&parametros);
   pthread_create(&identificadores[1],&atributos,(void*)Hilo2,(void*)&parametros);
   pthread_create(&identificadores[2],&atributos,(void*)Hilo3,(void*)&parametros);
   
 
   sleep(3);   
   shmdt(&parametros.resultados);
   shmctl(shmid1,IPC_RMID,0);   
   return 0;
}

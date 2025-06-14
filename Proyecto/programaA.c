#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<pthread.h>
#include<unistd.h>
#include<wait.h>
#include<math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/types.h>

#include"gestionarSemaforos.h"
#include"parametrosFourier.h"



void HiloCalcularTerminos(ParametrosSerie *parametros)
{
    int i,j,k=parametros->k;
    float (*a)(int n)=parametros->a_Funct;
    float (*b)(int n)=parametros->b_Funct;
    float *x=parametros->x;
    
    float*** terminos=parametrosHilo.memoria.terminos;
    int sem=parametrosHilo.memoria.sem;
    printf("\nDentro del hilo 1-%d con ID: %ld cuyo proceso es el %d\n",k,pthread_self(),getpid());
    
  //Creacion de procesos hijos para calcular los 10 terminos de la serie
   for(i=0;i<10;i++)
     {
          int n=i+1;
          //Calculo de los terminos de la serie
          for(j=0;j<7;j++)
          {
            down(sem);  //Semaforo en bajo
              terminos[k][j][n-1]=(*a)(n)*cos(n*x[j])+(*b)(n)*sin(n*x[j]);    //Aplicar la definicion de la serie de Fourier
            up(sem);    //Semaforo en alto
          }       
     }
     
    pthread_exit(0);  //Terminar hilo
}

int calcularSeries()
{
  pthread_attr_t atributos;   
  pthread_t identificadores[3];    //Arreglo de 3 hilos
  int i;
  
  pthread_attr_init(&atributos);   
  pthread_attr_setdetachstate(&atributos,PTHREAD_CREATE_DETACHED);   

  for(i=0;i<3;i++)
   {
      //Creacion del hilo y paso de parametros
      pthread_create(&identificadores[i],&atributos,(void*)HiloCalcularTerminos,(void*)&parametrosHilo.serie[i]);
   }
}


void liberarMemoria(float*** terminos)
{
    int i;
    for(i=0;i<3;i++)
     {
        free(terminos[i]);
     }
}



int main() 
{
 //Declaracion de variables      
   int shmid, i,j,k, sem;
   float *datos_matriz;
   float **m1,**m2,**m3;
   float ***terminos;
   key_t llave,llave_sem;
   pid_t pid;
      
   
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
  

  printf("\nSoy el proceso A con PID %d\n",getpid());
  calcularSeries();       //Se invoca a la funcion para calcular las series de Fourier   
  sleep(3);
  liberarMemoria(parametrosHilo.memoria.terminos);  
  printf("Proceso A finalizado\n");
  
   shmdt(&datos_matriz);
   shmctl(shmid,IPC_RMID,0);
   return 0;
}

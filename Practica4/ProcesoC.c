#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gestionarSemaforos.h"

#define PERMISOS 0644


int main()
{     
   int *resultados=NULL, *datos_matriz=NULL;
   int semaforo_matriz,semaforo_resultados,suma=0,j=0;
   
   int shmid0, shmid1, i;
   int **app_mat;
   key_t llave0, llave1,llave_sem1,llave_sem2;
   pid_t pid;
   
   //Generar las llaves para la memoria compartida
   llave0=ftok("Archivo1",'m');
   llave1=ftok("Archivo2",'m');
   
   shmid0=shmget(llave0,3*9*sizeof(datos_matriz[0]),IPC_CREAT|PERMISOS); //Region de la matriz
   shmid1=shmget(llave1,3*sizeof(int),IPC_CREAT|PERMISOS);  //Region de los resultados
   
   datos_matriz=(int*)shmat(shmid0,0,0);
   resultados=(int *)shmat(shmid1,0,0);
   
   //Reservar memoria
   app_mat=(int**)malloc(3*sizeof(app_mat[0]));
   
   for(i=0;i<3;i++)
     {
        app_mat[i]=datos_matriz+i*9;
     }

  //Generar las llaves para los semaforos
   llave_sem1=ftok("S1",'k'); 
   llave_sem2=ftok("S2",'k'); 
   
   //Crear semaforos
   semaforo_matriz=Crea_semaforo(llave_sem1,1);
   semaforo_resultados=Crea_semaforo(llave_sem2,1);
   
   //Sumar fila 3 usando semaforos
   for(int j=0;j<9;j++)
   {
      down(semaforo_matriz);
      sleep(1);
      suma+=app_mat[2][j];
      up(semaforo_matriz); 
   }

   printf("\n\nEl proceso %d, termino de sumar la fila 3\n\n",getpid());
   printf("Suma de la tercer fila: %d\n",suma);
   
   //Registrar resultado usando semaforos
      down(semaforo_resultados);
      resultados[2]=suma;
      up(semaforo_resultados); 

   //Disociar apuntadores de los segmentos de memoria
   shmdt(&app_mat);
   shmdt(&resultados);
   shmctl(shmid0,IPC_RMID,0);
   shmctl(shmid1,IPC_RMID,0);
   return 0;
}

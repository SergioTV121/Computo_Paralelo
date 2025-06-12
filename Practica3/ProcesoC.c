#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   int shmid0, shmid1, i,j,suma=0;
   int *resultados, *datos_matriz;
   int **app_mat;
   key_t llave0, llave1;
   pid_t pid;
   
   llave0=ftok("Archivo1",'k');
   llave1=ftok("Archivo2",'k');
   
   shmid0=shmget(llave0,3*9*sizeof(datos_matriz[0]),IPC_CREAT|0777);        //Region de la matriz
   shmid1=shmget(llave1,3*sizeof(int),IPC_CREAT|0777);                    //Region de los resultados
   
   datos_matriz=(int*)shmat(shmid0,0,0);
   resultados=(int *)shmat(shmid1,0,0);
   
   //Reservar memoria
   app_mat=(int**)malloc(3*sizeof(app_mat[0]));
   
   for(i=0;i<3;i++)
     {
        app_mat[i]=datos_matriz+i*9;
     }
   
   
   //Sumar fila 3
  
     for(j=0;j<9;j++)
     {
        suma+=app_mat[2][j]; 
        sleep(1);
     }
  

     
   printf("\n\nLa suma de la tercer fila ha terminado\n\n");
   printf("Resultado de la suma: %d\n",suma);
   resultados[2]=suma;
   
   shmdt(&app_mat);
   shmdt(&resultados);
   shmctl(shmid0,IPC_RMID,0);
   shmctl(shmid1,IPC_RMID,0);
   return 0;
}

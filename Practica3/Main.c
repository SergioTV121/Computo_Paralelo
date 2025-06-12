#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   int shmid0, shmid1, i,j;
   int *resultados, *datos_matriz;
   int **app_mat;
   int matriz[3][9]={ {1, 2, 3, 4, 5, 6, 7, 8, 9}, 
                      {1, 3, 5, 7, 9, 11, 13, 15, 17}, 
                      {2, 4, 6, 8, 10, 12, 14, 16, 18 }};
   key_t llave0, llave1;
   pid_t pid;
   
   llave0=ftok("Archivo1",'k');
   llave1=ftok("Archivo2",'k');
   
   shmid0=shmget(llave0,3*9*sizeof(datos_matriz[0]),IPC_CREAT|0777);  //Region de la matriz
   shmid1=shmget(llave1,3*sizeof(int),IPC_CREAT|0777);                //Region de los resultados
   
   datos_matriz=(int*)shmat(shmid0,0,0);
   resultados=(int *)shmat(shmid1,0,0);
   
   
   //Reservar memoria para la matriz
   app_mat=(int**)malloc(3*sizeof(app_mat[0]));
   
   for(i=0;i<3;i++)
     {
        app_mat[i]=datos_matriz+i*9;
     }
   
   
  //Guardar matriz en la memoria compartida
   for(i=0;i<3;i++)
     {
        for(j=0;j<9;j++)
       {
          app_mat[i][j]=matriz[i][j];
       }
     }
   
   
  
   printf("\n\nEl escritor de la matriz ha terminado\n\n");
   //Espera hasta que todos los resultados se registren
   
   while(resultados[0]==1 || resultados[1]==2 || resultados[2]==3); 
   
   //Evalua si ya se registraron todos los resultados de las sumas
   if(resultados[0]!=1 && resultados[1]!=2 && resultados[2]!=3) 
   {
    for(i=0;i<3;i++)
     {
        printf("Suma de la fila %d: %d\n",i+1,resultados[i]); //Imprime los resultados de cada fila
     }
   }
   
   sleep(5);
   //Disociar los segmentos de memoria del apuntador
   shmdt(&app_mat); 
   shmdt(&resultados);
   //Eliminiar el segmento de memoria
   shmctl(shmid0,IPC_RMID,0);
   shmctl(shmid1,IPC_RMID,0);
   return 0;
}

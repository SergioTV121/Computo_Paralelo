  #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<wait.h>
#include<sys/types.h>


void guardarMatriz(int matriz[3][9] );
void borrarResultados(FILE *archivo);
void sumarFilas(FILE* archivo, pid_t pid, int fila);
void guardarResultado(int resultado);
void mostrarResultados(FILE *archivo);

void guardarMatriz(int matriz[3][9] )
{

  FILE *archivo=NULL;
   int i,j;
	
  //GUARDAR MATRIZ EN ARCHIVO DE TEXTO

  archivo = fopen("matriz.txt", "w"); // Abrir archivo en modo escritura

   
   if (archivo == NULL) {
      printf("No se pudo abrir el archivo\n");
   }

    // Escribir los elementos de la matriz en el archivo
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 9; j++)
        {
            fprintf(archivo, "%d ", matriz[i][j]);
        }
        fprintf(archivo, "\n");
    }
     fclose(archivo); // Cerrar archivo
}


void borrarResultados(FILE *archivo)
{

 archivo = fopen("resultados.txt", "w"); // Abrir archivo en modo escritura

    // Escribir el resultado
   fprintf(archivo, "");
    
   fclose(archivo); // Cerrar archivo
  
}





void guardarResultado(int resultado)
{

  FILE *archivo;
  // Abrir archivo en modo escritura para agregar elementos al final
 archivo = fopen("resultados.txt", "a"); 

   
         if (archivo == NULL) {
            printf("No se pudo abrir el archivo 'resultados'\n");
         }

          // Escribir el resultado
            fprintf(archivo, "%d\n", resultado);
                   
              
  fclose(archivo); // Cerrar archivo

}

void mostrarResultados(FILE *archivo)
{
  printf("Resultados\n");

 archivo = fopen("resultados.txt", "r"); // Abrir archivo en modo escritura

   
         if (archivo == NULL) {
            printf("No se pudo abrir el archivo 'resultados'\n");
         }
         
        char linea[5]; 
        int contador=0;
        
        // Leer caracteres mientras no se llegue al final del archivo 
        while ((fgets(linea, 5, archivo)) != NULL) 
        {
          contador++;
          printf("Suma de la fila %d:%s\n",contador,linea);                 
        }
                   
              
  fclose(archivo); // Cerrar archivo

}


void sumarFilas(FILE* archivo, pid_t pid, int fila)
{

  int contador=0,suma=0;
  char linea[25];
   pid = fork();
    char* tok;
   
   if(pid==-1)
   {
      perror("\nError al crear el proceso\n");
      exit(-1);
   }
   if(pid==0)
   {
      printf("\nHola soy el proceso hijo %d\n",fila);
      printf("\nMi identificador es: %d\n",getpid());
      printf("\nMi proceso padre es: %d\n",getppid());
      
    archivo = fopen("matriz.txt", "r"); // Abrir archivo en modo lectura
   
      while ((fgets(linea, 25, archivo)) != NULL) // Leer caracteres mientras no se llegue al final del archivo 
        {
        contador++;
          if(contador==fila)
          {
            break;
          }
        }
        
      fclose(archivo); // Cerrar archivo
      
      //Separar linea por espacios y almacenarlo en arreglo
      printf("Mi fila es: %s",linea);
      tok=strtok(linea," ");
    
      while(tok!=NULL)
      {
      
        suma+=atoi(tok);
        
      
        tok=strtok(NULL," ");
      }
      guardarResultado(suma);
      
      if(fila==1)
      {
        sleep(4);
      }
      else if(fila==2)
      {
        sleep(5);
      }
      else
      {
       sleep(5);
      }
      
     
      
    exit(0);   
   }
  
   
}


int main() {
   FILE *resultados,*archivo1,*archivo2,*archivo3;
   int matriz[3][9]={{1,2,3,4,5,6,7,8,9},{1,3,5,7,9,11,13,15,17},{2,4,6,8,10,12,14,16,18}};

   system("touch matriz.txt");	//Se crea el archivo para almacenar la matriz
   system("touch resultados.txt");	//Se crea el archivo para almacenar los resultados 
  	
   guardarMatriz(matriz);	//Se invoca la funcion para guardar la matriz
  
  
  //CREACION DE SUBPROCESOS   
   
   pid_t pid1,pid2,pid3;
   
   //Se limpian los resultados almacenados en otra ejecucion
   borrarResultados(resultados);   
  
   
   //Se invoca la funcion para sumar las filas indicadas
   sumarFilas(archivo1, pid1, 1);
   sumarFilas(archivo2, pid2, 2);
   sumarFilas(archivo3, pid3, 3);
   
  wait(NULL);      
  printf("\n\nHola soy el proceso padre\n");
  printf("\nMi identificador es: %d\n",getpid());
  printf("\nMi proceso padre es: %d\n",getppid());
      
  mostrarResultados(resultados);
  
   return 0;
}

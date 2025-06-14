
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#define PERMISOS 0644
#define M (M_PI/2.0)

#define ao_Sergio (((4.0*M_PI*M_PI)+12.0)/3.0)
#define ao_Emilio 0.0
#define ao_Flavio 13.0/3.0


//Defino un tipo de dato abstracto por medio de un struct para los parametros

typedef struct 
{
  float (*a_Funct)(int n);
  float (*b_Funct)(int n);
  float x[7];
  int k;
}ParametrosSerie;


typedef struct 
{
  float*** terminos;
  float** resultados;
  int sem;
}ParametrosMemoria;


typedef struct 
{
  ParametrosSerie serie[3];
  ParametrosMemoria memoria;
}ParametrosHilo;

//SE DEFINEN LAS FUNCIONES PARA CALCULAR CADA TERMINO DE LA SERIE POR MEDIO DE LOS COEFICIENTES DE FOURIER
float a_Sergio(int n){
  return (16.0/(n*n))*(pow(-1.0,n));
}

float b_Sergio(int n){
  return pow(-1.0,n)*((4.0*M_PI*M_PI*n*n-(24.0+(2.0*n*n)))/(pow(n,3.0)));
}

float a_Emilio(int n){
  return 0.0;
}

float b_Emilio(int n){
  return (1.0/n);
}

float a_Flavio(int n){
  return ((16.0*pow(-1.0,n))/((n*n)*M_PI*M_PI));
}

float b_Flavio(int n){
  return ((((pow(-1.0,n))*4.0)/(n*M_PI)));
}

float ao_arr[3]={ao_Sergio,ao_Flavio,ao_Emilio};

//Inicializar TDA
ParametrosHilo parametrosHilo={
    .serie={
              {a_Sergio, b_Sergio,{-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0},0},  //Serie de Sergio
              {a_Flavio, b_Flavio,{-2.0*M,-1.5*M,-1.0*M,0.0,1.0*M,1.5*M,2.0*M},1},   //Serie de Flavio
              {a_Emilio, b_Emilio,{0.0,0.5,1.0,1.5,2.0,2.5,3.0},2,}   //Serie de Emilio
             },  //Parametros Series
    .memoria={NULL,NULL,0}    //Parametros Memoria Compartida
};  





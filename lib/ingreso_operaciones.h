#ifndef INGRESO_OPERACIONES_H_INCLUDED
#define INGRESO_OPERACIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estructuras.h"

#define TAM_BUFFER_STDIN_OPP 100
#define TAM_BUFFER_OPP 10

enum INGRESO{

    INGRESO_OK                  =  1,
    INGRESO_OPP_INVALIDA        = -5,
    INGRESO_PAR_INVALID0        = -10,
    INGRESO_MUELLES_INVALIDO    = -15,
    INGRESO_ZONAS_INVALIDO      = -20,
    INGRESO_CAD_INVALIDA        = -25,
    INGRESO_CAD_EXIT            = -30
};

//Estructura construida luego de parsear el codigo de operacion
typedef struct{
    char     cod[TAM_BUFFER_OPP];
    unsigned codParametro_1;
    unsigned codParametro_2;
}tOperacion;


void aMayusculas(char *s);
void asignarParametro_tOperacion(tOperacion *opp, unsigned nroParametro, unsigned valor);
int procesarComandoSTDIN(const char *buffer, tOperacion *oppDest, tConfig *config);
int procesarComandoSTDIN_validarOPP(const char *buffer, char *ordenParametros);
int procesarComandoSTDIN_validarPAR(const char *ordenParametros, char const **dirParametros, unsigned cantParametros, tOperacion *oppDest, tConfig *config);
    int procesarComandoSTDIN_validarPAR_MUE(const char *buffer, unsigned cantMuelles);
    int procesarComandoSTDIN_validarPAR_ZON(const char *buffer, unsigned cantZonas);

//FUNCION PRINCIPAL:
    // Debe estar config inicializado, debe haber una variable tOperacion que almacene el valor parseado de la cadena. Orden de operandos estricto. Opcion exit (se puede quitar)
    // Lee la cadena y la convierte a mayuscula.
    int solicitarOperacionSTDIN(tOperacion *oppDest, tConfig *config);


#endif // INGRESO_OPERACIONES_H_INCLUDED

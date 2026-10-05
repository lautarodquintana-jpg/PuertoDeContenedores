#ifndef INGRESO_OPERACIONES_H_INCLUDED
#define INGRESO_OPERACIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estructuras.h"

enum INGRESO{
    INGRESO_OK                  =  1,
    INGRESO_OPP_INVALIDA        = -5,
    INGRESO_PAR_INVALID0        = -10,
    INGRESO_MUELLES_INVALIDO    = -15,
    INGRESO_ZONAS_INVALIDO      = -20,
    INGRESO_CAD_INVALIDA        = -25,
    INGRESO_CAD_EXIT            = -30
};

//Codigos de operacion asignados a cada una
enum OPERACION_NUMERO{
    OPERACION_DES = 1,
    OPERACION_REU = 2,
    OPERACION_ENT = 3,
    OPERACION_VER = 4,
    OPERACION_ESP = 5
};

void aMayusculas(char *s);
void asignarParametro_tOperacion(tOperacion *opp, unsigned nroParametro, unsigned valor);
int procesarComandoSTDIN(const char *buffer, tOperacion *oppDest, tConfig *config, tJornada *jornada);
int procesarComandoSTDIN_validarOPP(const char *buffer, char *ordenParametros);
int procesarComandoSTDIN_validarPAR(const char *ordenParametros, char const **dirParametros, unsigned cantParametros, tOperacion *oppDest, tConfig *config);
    int procesarComandoSTDIN_validarPAR_MUE(const char *buffer, unsigned cantMuelles);
    int procesarComandoSTDIN_validarPAR_ZON(const char *buffer, unsigned cantZonas);
int consultarTiempoRequeridoOperacion(const char *oppCod, tConfig *config);
int construirMensajeOperacion(tOperacion *opp, char *buffer); //Para despues imprimir todos los mensajes en un .txt
int obtenerNROOperacion(tOperacion *opp);

//FUNCION PRINCIPAL:
    // Debe estar config inicializado, debe haber una variable tOperacion que almacene el valor parseado de la cadena. Orden de operandos estricto. Opcion exit (se puede quitar)
    // Lee la cadena y la convierte a mayuscula.
    int solicitarOperacionSTDIN(tOperacion *oppDest, tConfig *config, tJornada *jornada);

#endif // INGRESO_OPERACIONES_H_INCLUDED

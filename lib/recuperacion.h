#ifndef RECUPERACION_H_INCLUDED
#define RECUPERACION_H_INCLUDED

/**
    ARCHIVO DE LOCACION DE LAS FUNCIONES DE GUARDADO TEMPORAL Y CARGA DE PARTIDAS EN EJECUCIÓN PARA EL CASO DE SALIDAS ANTICIPADAS.
    PARA CADA PARTIDA, SE GUARDA UNA TERNA:
        (archivoMovimiento.log ; archivoPuerto.txt ; archivoConfig.txt)
        Debe existir una sola terna por jugador, es decir, se deberan sobreescribir en caso de que el jugador decida no recuperar una partida (de corresponder).
        Estos registros se guardarán en la carpeta "./log"
    SOBRE LOS ARCHIVOS:
        -archivoMovimiento.log: Es un archivo de forma "log/USER.log"    -> Con registros tOperacion efectivos realizados en la jornada.
        -archivoPuerto.txt    : Es un archivo de forma "log/USER_puerto.txt" -> Es una copia del puerto.txt de la jornada para la cual se guardaron registros tOperacion.
        -archivoConfig.txt    : Es un archivo de forma "log/USER_config.txt" -> Es una copia del config.txt de la jornada para la cual se guardaron registros tOperacion.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "estructuras.h"
#include "configuracion.h"
#include "colores.h"
#define MAX_FILENAME            100
#define MAX_LINEA_RECU          200
#define PATH_TEMPORALES         "./log/"
#define SUBFIJO_LOG             ".log"
#define SUBFIJO_PUERTO          "_puerto.txt"
#define SUBFIJO_CONFIG          "_config.txt"
#define PREFIJO_RECUPERANDO     "recu_"
#define RECU_NOMBARCH_CONFIG    "config.txt"

typedef enum{
    ER_TODO_OK          =  1,
    ER_EX_ARCHIVO       =  2,
    ER_NE_ARCHIVO       = -2,
    ER_ERROR_ARCHIVO    = -3,
    ER_ERROR_FIRMA      = -4,
    ER_NO_RECUPERA      = -5,
    ER_RECUPERA         =  3,
    ER_EX_REGISTROS_REC =  5,
    ER_NE_REGISTROS_REC = -5
}ESTADOS_RECUPERACION;

int duplicarArchivoTexto(const char *origen, const char *destino);
int duplicarPuertoACarpetaPrincipal(const char *user);
int duplicarConfigACarpetaPrincipal(const char *user);

void eliminarArchivoPuertoTemporal(const char *user);
void eliminarArchivoConfigTemporal(const char *user);
void eliminarArchivoMovimientosTemporales(const char *user, unsigned iniciado);

void eliminarArchivosTemporales(const char *user);

FILE* abrirArchivoMovimientosTemporales(const char *user, const char *modo, unsigned iniciado);
FILE* abrirArchivoPuertoTemporal(const char *user, const char *modo, unsigned iniciado);
FILE* abrirArchivoConfigTemporal(const char *user, const char *modo, unsigned iniciado);

int renombrarArchivoTemporalAIniciado(const char *user, const char *tipoArchivoTemporal);

void guardarOperacion( FILE* pf_log, tOperacion operacion );
int guardarCopiaConfigYPuerto(const char *user);

int existeArchivoDeRecuperacion (const char *user);
int consultarArchivosDeRecuperacionYCarga(tJornada *jornada);
int leerRegistroDelArchivoTemporal(tOperacion *operacion, tJornada *jornada, tRecuperacion *recuperacion);
int prepararEstructuraRecuperacion(tJornada *jornada, tConfig *config, tRecuperacion *log);

#endif // RECUPERACION_H_INCLUDED

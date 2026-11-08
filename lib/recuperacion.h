#ifndef RECUPERACION_H_INCLUDED
#define RECUPERACION_H_INCLUDED

/**
    ARCHIVO DE LOCACION DE LAS FUNCIONES DE GUARDADO TEMPORAL Y CARGA DE PARTIDAS EN EJECUCIÓN PARA EL CASO DE SALIDAS ANTICIPADAS.
    PARA CADA PARTIDA, SE GUARDA UN PAR:
        (archivoMovimiento.log ; archivoPuerto.txt)
        Debe existir un solo par por jugador, es decir, se deberan sobreescribir en caso de que el jugador decida no recuperar una partida.
        Estos registros se guardarán en la carpeta "./log"
    SOBRE LOS ARCHIVOS:
        -archivoMovimiento.log: Es un archivo de forma "log/USER.log"    -> Con registros tOperacion efectivos realizados en la jornada.
        -archivoPuerto.txt    : Es un archivo de forma "log/USER_puerto.txt" -> Es una copia del puerto.txt de la jornada para la cual se guardaron registros tOperacion.
*/

#include <stdio.h>
#include "estructuras.h"
#define MAX_FILENAME            100
#define MAX_LINEA_RECU          200
#define PATH_TEMPORALES         "./log/"
#define SUBFIJO_LOG             ".log"
#define SUBFIJO_PUERTO          "_puerto.txt"
#define SUBFIJO_CONFIG          "_config.txt"
#define PREFIJO_RECUPERANDO     "recu_"

typedef enum{
    ER_TODO_OK          =  1,
    ER_EX_ARCHIVO       =  2,
    ER_NE_ARCHIVO       = -2,
    ER_ERROR_ARCHIVO    = -3,
    ER_ERROR_FIRMA      = -4
}ESTADOS_RECUPERACION;

int duplicarArchivoTexto(const char *origen, const char *destino);
int duplicarPuertoACarpetaPrincipal(const char *user);

void eliminarArchivoPuertoTemporal(const char *user);
FILE* abrirArchivoMovimientosTemporales(const char *user, const char *modo, unsigned iniciado);
FILE* abrirArchivoPuertoTemporal(const char *user, const char *modo, unsigned iniciado);

int renombrarArchivoTemporalAIniciado(const char *user, const char *tipoArchivoTemporal);

int existeArchivoDeRecuperacion (const char *user);
int prepararEstructuraRecuperacion(const char *user, tRecuperacion *log);

void guardarOperacion( FILE* pf_log, tOperacion operacion );

FILE* abrirArchivoConfigTemporal(const char *user, const char *modo, unsigned iniciado);
int duplicarConfigACarpetaPrincipal(const char *user);
void eliminarArchivoConfigTemporal(const char *user);

#endif // RECUPERACION_H_INCLUDED

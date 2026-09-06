#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

#include "estructuras.h"
#include <string.h>
#include <stdio.h>
#define MAX_LINEA_CONFIG 100
#define ERROR_LECTURA_ARCHIVO -19




int leerConfigTxt(tConfig *config, const char *nomArch);
void mostrarConfig(tConfig *config);


#endif // CONFIGURACION_H_INCLUDED

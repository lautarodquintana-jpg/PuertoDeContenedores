#ifndef VALIDACIONES_H_INCLUDED
#define VALIDACIONES_H_INCLUDED

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "estructuras.h"
#include "ingreso_operaciones.h"
#include "muelle.h"

enum VALIDACION{
    INGRESO_SIN_TIEMPO            = -35,
    INGRESO_MUELLE_SIN_CARGA      = -40,
    INGRESO_ZONA_LLENA            = -45,
    INGRESO_ZONA_VACIA            = -50,
    INGRESO_ZONAS_IGUALES         = -55,
    INGRESO_SIN_CAMION            = -60,
    INGRESO_CONTENEDOR_NO_EN_TOPE = -65,
    INGRESO_ERROR_INTERNO         = -70
};

char leerYValidarCharMenu();

int  consultarTiempoRequeridoOperacion(const char *oppCod, tConfig *config);

int  validarTiempoOperacion(const tOperacion *opp, tConfig *config, const tJornada *jornada);

int  validarOperacion(const tOperacion *opp, tConfig *config, tJornada *jornada);

void mostrarMotivoRechazo(int codigo);


#endif // VALIDACIONES_H_INCLUDED

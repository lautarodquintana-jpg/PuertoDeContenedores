#ifndef CAMION_H_INCLUDED
#define CAMION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "../TDA/ListaSimplementeEnlazada.h"
#include "estructuras.h"
#include "zona.h"

enum COD_CAMION{
    COD_CAMION_OK          = 1,
    COD_CAMION_INCORRECTO   = -1,
    ENCOLAR_CAMION_MAL = -2,
    ERROR_RETIRAR_CONTE = -3,
    NO_HAY_CAMION=-4
};

void mostrarCamion( const void* elem );
int mostrarPrimeros3Camiones( tCola* camionesEnCola );
int realizarEntregaCamion( tCola* camionesEnCola , tLista* plZonasAlmacenamiento , tOperacion *ope);

#endif // CAMION_H_INCLUDED

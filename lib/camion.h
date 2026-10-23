#ifndef CAMION_H_INCLUDED
#define CAMION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "../TDA/ListaSimplementeEnlazada.h"
#include "estructuras.h"
#include "zona.h"

enum COD_CAMION{
    COD_CAMION_OK          = 1,
    COD_CAMION_MAL         = -2,
    COD_CAMION_INCORRECTO   = -1,
    ENCOLAR_CAMION_MAL = -3,
    ERROR_RETIRAR_CONTE = -4
};

void mostrarCamion( const void* elem );
void mostrarPrimerCamion( tCola* camionesEnCola );
int ponerCamionesArribadosEnCola( tCola* camionesEnCola , tLista* camionesEnCamino , unsigned tiempoActual );
int realizarEntregaCamion( tCola* camionesEnCola , tLista* plZonasAlmacenamiento );

#endif // CAMION_H_INCLUDED

#ifndef ZONA_H_INCLUDED
#define ZONA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "../TDA/ListaSimplementeEnlazada.h"

#include "estructuras.h"
#include "../TDA/pila.h"

enum COD_ZONAS{
    COD_ZONAS_OK          = 1,
    COD_ZONAS_MAL         = -2,
    COD_ZONA_INCORRECTO   = -1,
    ZONA_NO_DISPO         = 0,
    ERROR_OBTENER_CANT_OCUPADA = -3,
    ZONA_APILAR_CONTENEDOR_MAL = -4
};

void crearListaZonas( tLista* zonas );
int generarZonas( tLista* zonas , unsigned cantZonas );
int actualizarZonaN( tLista *plZonas , unsigned codZona , const tContenedor* contenedor );
int tieneZonaEspacio( tLista *plZonas , unsigned codZona , unsigned capacidadPila );
void consultarCantidadOcupada(void *zonaLista, const void *actualizador);
void apilarContenedorEnZona( void* pZona , const void* elem );

void mostrarTopeContenedorPorZona( void* elem , const void *actualizador );
void mostrarZonas( tLista *plZonas );

#endif // ZONA_H_INCLUDED

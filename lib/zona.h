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
    ZONA_APILAR_CONTENEDOR_MAL = -4,
    ZONA_LLENA      = -5,
    ZONA_VACIA      = -6,
    ZONA_DISPONIBLE = 2
};

void crearListaZonas( tLista* zonas );
int generarZonas( tLista* zonas , unsigned cantZonas , int largoMaxCodContenedor , int contenedoresApiladosMax);
int actualizarZonaN( tLista *plZonas , unsigned codZona , const tContenedor* contenedor );
int tieneZonaEspacio( tLista *plZonas , unsigned codZona , unsigned capacidadPila );
void consultarCantidadOcupada(void *zonaLista, const void *actualizador);
void apilarContenedorEnZona( void* pZona , const void* elem );
void liberarZonasDeAlmacenamiento (tLista *zonas);

void mostrarContenedoresdeZonaAlmacenamiento( void* elem , const void *actualizador );
void mostrarZonas( tLista *plZonas );
int mostrarCadContenedoresEnFormaDePilaRecursiva(const char *cad);

///////
void desapilarContenedorCB(void *zonaLista, const void *ctx);
int retirarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor *contenedorDest);
int compararZonaTopeContenedor( const void* elem_z, const void* elem_c );
int buscarCodZona( tLista *plZonasAlmacenamiento, const char* codContenedor );

///////
int consultarDisponibilidadZona(tLista *plZonasAlmacenamiento, unsigned codZona, unsigned capacidadPila);
void obtenerDisponibilidadCB(void *zonaLista, const void *cantidadOcupado);

void apilarContenedorCB(void *zonaLista, const void *ctx);
int agregarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor contenedor, unsigned capacidadPila);

#endif // ZONA_H_INCLUDED

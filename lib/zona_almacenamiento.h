#ifndef ZONA_ALMACENAMIENTO_H_INCLUDED
#define ZONA_ALMACENAMIENTO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estructuras.h"
#include "../TDA/ListaSimplementeEnlazada.h"
#include "../TDA/pila.h"

enum COD_ZONAS{
    COD_ZONAS_OK      =  1,
    COD_ZONAS_MAL     = -2,
    ZONA_DISPONIBLE   =  5,
    ZONA_LLENA        = 10,
    ZONA_VACIA        = 15
};

//Funciones callback que se usan como parte de otras primitivas
void obtenerDisponibilidadCB(void *zonaLista, const void *cantidadOcupado); //Fx CB usada en consultarDisponibilidadZona();
void apilarContenedorCB(void *zonaLista, const void *ctx);                  //Fx CB usada en agregarContenedorZonaAlmacenamiento();
void desapilarContenedorCB(void *zonaLista, const void *ctx);               //Fx CB usada en retirarContenedorZonaAlmacenamiento();
void mostrarZonaAlmacenamientoCB(void *ctx, const void *elem);              //Fx CB usada en mostrarZonasAlmacenamiento()

//Funciones que se pueden usar directamente:
int generarZonasAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned cantZonas);
void eliminarZonasAlmacenamiento(tLista *plZonasAlmacenamiento);
int consultarDisponibilidadZona(tLista *plZonasAlmacenamiento, unsigned codZona, unsigned capacidadPila);
int agregarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor contenedor, unsigned capacidadPila);
int retirarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor *contenedorDest);
void mostrarZonasAlmacenamiento(tLista lZonasAlmacenamiento, unsigned capPila, unsigned lineas);

#endif // ZONA_ALMACENAMIENTO_H_INCLUDED

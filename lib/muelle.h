#ifndef MUELLE_H_INCLUDED
#define MUELLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "estructuras.h"
#include "../TDA/ListaSimplementeEnlazada.h"
#include "barco.h"

enum COD_MUELLES{
    COD_MUELLES_OK      = 1,
    COD_MUELLES_MAL     = -2,
    MUELLE_DISPONIBLE   = 1,
    MUELLE_OCUPADO      = 0
};

//FUNCIONES CALLBACK: NO SE DEBEN LLAMAR A MENOS QUE SE HAGA UNA PRIMITIVA QUE LA REQUIERA (MOSTRARMUELLECB SI PUEDE LLAMARSE O ADAPTARSE)
void encontrarMuelleCB(void *ctx, const void *elem);                    //Fx callback usada en encontrarMuelleDisponible
void mostrarMuelleCB(void *ctx, const void *elem);                      //Fx callback de uso general para pruebas -> se usa en recorrerLista()
void actualizarMuelleCB(void *muelleLista, const void *actualizador);   //Fx callback usada en ocuparMuelle() -> para copiar el barco al campo barco del muelle
void consultarEstadoMuelleCB(void *muelleLista, const void *actualizador);  //Fx callback usada en estadoMuelle() -> para encontrar el estado de un muelle y devolverlo
void liberarMuelleCB(void *muelleLista, const void *actualizador);      //Fx callback usada en liberarMuelle(), no es la que se debe usar: se pone mismo nombre a la que se debe usar por falta de ingenio...
void mostrarMuelleYBarco(void *ctx, const void* elem );

//FUNCIONES QUE SE PUEDEN USAR: Se concidera codigos de muelles desde 1 hasta tConfig.cantidadMuelles (incluidos)
void crearListaMuelles( tLista *plMuelles );
int  generarMuelles(tLista *plMuelles, unsigned cantMuelles);            //Genera n cantidad de muelles con codigos inicializados desde 1 hasta cantMuelles (incluido) -> No hace crearLista()
void eliminarMuelles(tLista *plMuelles);                                //Elimina todos los muelles de la lista de muelles
int  encontrarMuelleDisponible(tLista lMuelles);                         //Devuelve el cod del primer muelle disponible -> Importante, no requiere puntero a lista solo la lista (puntero a nodo)
int  ocuparMuelle(tLista *plMuelles, unsigned codMuelle, tBarco barco, unsigned minutoArribo);  //Ocupa un muelle con un barco que se le debe pasar por parametro.
int  estadoMuelle(tLista *plMuelles, unsigned codMuelle);                //Retorna el campo estado de un muelle.
int  liberarMuelle(tLista *plMuelles, unsigned codMuelle);               //Para un codigo de muelle dado, cambia el estado de ocupacion -> deberia liberar la lista de ocupacion
int tieneBarcoContenedores( tLista *plMuelles , unsigned codMuelle );
int  obtenerContenedorMuelle(tLista *plMuelles, unsigned codMuelle, tContenedor* contenedor);
void mostrarMuelles( tLista *plMuelles );
int  quitarContenedorMuelle(tLista *plMuelles, unsigned codMuelle, tContenedor* contenedor);      //Requiere las primitivas de tBarco, se deja para mas adelante ->
                                                                                                  //deberia retornar el tContenedor quitado o su codigo, y eliminarlo de barco (lista de contenedores)
                                                                                                  //Es la que se ejecutar acuando se haga DES
#endif // MUELLE_H_INCLUDED

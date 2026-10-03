#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include <stdio.h>
#include "estructuras.h"
#include "../TDA/ListaSimplementeEnlazada.h"
#include "../TDA/cola.h"
#include "../TDA/nodo.h"
#include "../TDA/pila.h"

int inicializarValoresYGenerarPuertoTXT(tConfig *config, tJornada *jornada, const char *nomUser, tLista *listaBarcosEnCamino, tLista *listaCamionesEnCamino);
int compararTiempos(const void *elem1, const void *elem2);
int compararContenedores(const void *elem1, const void *elem2);
int compararTiemposDeCamiones(const void *elem1, const void *elem2);
void actualizarBarco(void *actualizado, const void *actualizador);
void avanzarTiempoHastaEventoFuturo( tJornada* jornada , const tLista* camionesEnCamino , const tLista* barcosEnCamino );
int procesarTiempo( tJornada* jornada, tLista* camionesEnCamino, tLista* barcosEnCamino );

#endif // JORNADA_H_INCLUDED

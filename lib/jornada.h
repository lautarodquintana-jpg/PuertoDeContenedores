#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include <stdio.h>
#include "estructuras.h"
#include "muelle.h"
//#include "zona_almacenamiento.h"
#include "../TDA/ListaSimplementeEnlazada.h"
#include "../TDA/cola.h"
#include "../TDA/nodo.h"
#include "../TDA/pila.h"

#include "barco.h"
#include "camion.h"
#include "muelle.h"
#include "zona.h"

enum COD_JORNADA{
    TIEMPO_AVANZADO    = -300,
    TIEMPO_NO_AVANZADO = -305
};

int prepararJornada( tJornada* jornada , tLista* camionesEnCamino , tLista* barcosEnCamino , const tConfig* config , int largoMaxCodContenedor);
int generarPuertoTXT(tConfig *config, tJornada *jornada, const char *nomUser, int *largoMaxCodContenedor);
int cargarPuertoTxt(tJornada *jornada, tConfig *config, tLista *listaBarcosEnCamino, tLista *listaCamionesEnCamino);
int compararTiempos(const void *elem1, const void *elem2);
int compararContenedores(const void *elem1, const void *elem2);
int compararTiemposDeCamiones(const void *elem1, const void *elem2);
void actualizarBarco(void *actualizado, const void *actualizador);
int avanzarTiempoHastaEventoFuturo( tJornada* jornada , const tLista* camionesEnCamino , const tLista* barcosEnCamino );
int procesarTiempo( tCola* camionesEnCola, tCola* barcosEnCola , tLista* camionesEnCamino, tLista* barcosEnCamino , unsigned tiempoActual );
void procesarLlegadas( tJornada* jornada , tLista* camionesEnCamino, tLista* barcosEnCamino );
void liberarJornada( tJornada* jornada, tLista* camionesEnCamino, tLista* barcosEnCamino, tCola* colaOperaciones );
void mostrarResumenJornada( unsigned tiempoJornada, unsigned tiempoDispo, unsigned cantDesc, unsigned cantReubi, unsigned cantEntre, unsigned puntuacionFinal, const char* nomOpera , unsigned camionesPendientes);
void mostrarHistorialOperaciones(tCola *colaOperaciones, tConfig *config);
unsigned calcularCantCamionesPendientes(tCola *colaCamiones);
#endif // JORNADA_H_INCLUDED

#ifndef BARCO_H_INCLUDED
#define BARCO_H_INCLUDED

#include "../TDA/ListaSimplementeEnlazada.h"
#include "muelle.h"

#define ERROR_SACAR_CONTENEDOR_BARCO -150

void mostrarBarco(const void* elem);
void ponerBarcosArribadosEnMuelles( tLista *plMuelles , tCola* barcosEnCola , unsigned tiempoActual );
void mostrarTopeContenedorBarcoEnMuelle( tLista* plMuelle, unsigned codMuelle );
void quitarPrimerContenedorBarco( void *ctx, const void* elem  );
void obtenerPrimerContenedorDeBarco( void *ctx, const void* elem  );
void obtenerCantidadDeContenedoresRestantesBarco( void *ctx, const void* elem  );

#endif // BARCO_H_INCLUDED

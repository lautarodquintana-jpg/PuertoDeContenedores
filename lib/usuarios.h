#ifndef USUARIOS_H_INCLUDED
#define USUARIOS_H_INCLUDED

#include "../TDA/ListaSimplementeEnlazada.h"
#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>

#define EXISTE_USER -75
#define NO_EXISTE_USER -76
#define ERROR_AGREGAR_USER -80

typedef struct
{
    char nomUser[TAM_USER];
    unsigned puntajeTotal;
}tUsuario;

void iniciarSesion( tJornada* jornada );

// ACTUALIZACIONES
void actualizarPuntaje( void* reg_1, const void* reg_2 );

// MODIFICACIONES
int modificarMejorPuntaje( const tJornada* jornada );

// INSERCION
int agregarUserRankingOrdenado( const tJornada* jornada );

// COMPARACIONES
int compararPuntaje( const void* reg_1, const void* reg_2 );
int compararNombres( const void* reg_1, const void* reg_2 );

// SALIDA
void mostrarInfoUsuario( const void* reg_1 );
int mostrarRanking();

#endif // USUARIOS_H_INCLUDED

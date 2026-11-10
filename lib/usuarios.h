#ifndef USUARIOS_H_INCLUDED
#define USUARIOS_H_INCLUDED

#include "../TDA/arbol.h"
#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>

#define EXISTE_USER -75
#define NO_EXISTE_USER -76
#define ERROR_AGREGAR_USER -80

#define ARCH_USUARIOS "operadores.dat"
#define ARCH_INDICE   "operadores.idx"
#define ARCH_JORNADAS "jornadas.dat"

typedef struct
{
    char nomUser[TAM_USER];
    unsigned puntajeTotal;
    unsigned jornadasJugadas;
}tUsuario;

typedef struct
{
    char clave[TAM_USER];
    long nroRegistro;
}tIndiceUsuario;

typedef struct
{
    char nomUser[TAM_USER];
    int puntaje;
    unsigned tiempoUsado;
    unsigned cantDescargas;
    unsigned cantReubicaciones;
    unsigned cantEntregas;
    unsigned camionesPendientes;
}tRegistroJornada;

int iniciarSesion( tJornada* jornada , tArbol* indice);

// ACTUALIZACIONES
void actualizarPuntaje( void* reg_1, const void* reg_2 );

// MODIFICACIONES
int modificarMejorPuntaje( const tJornada* jornada );

// INSERCION
int agregarUserRankingOrdenado( const tJornada* jornada );
int cargarIndiceUsuarios( tArbol* indice );
int guardarIndiceUsuarios( tArbol* indice );
int registrarJornada( tArbol* indice, const tRegistroJornada* jornada );


// COMPARACIONES
int compararPuntaje( const void* reg_1, const void* reg_2 );
int compararNombres( const void* reg_1, const void* reg_2 );
int compararIndiceUsuarios( const void* reg_1, const void* reg_2 );

// SALIDA
void mostrarInfoUsuario( const void* reg_1 );
int mostrarRanking();

#endif // USUARIOS_H_INCLUDED

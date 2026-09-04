#ifndef INCLUDE_COLA_H

#include <string.h>
#include <stdlib.h>

#define OK               1
#define COLA_VACIA      10
#define COLA_LLENA      11
#define COLA_DISPONIBLE 12

typedef struct sNodo
{
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
}tNodo;

typedef struct
{
    tNodo *pri;
    tNodo *ult;
}tCola;

void crearCola( tCola *cola );
int colaVacia( const tCola *cola );
int colaLlena( unsigned tamUsuario );
int ponerEnCola( tCola *cola , const void *dato , unsigned tamUsuario );
int sacarDeCola( tCola *cola , void *dato , unsigned tamUsuario );
int verPrimeroCola( const tCola *cola , void *dato , unsigned tamUsuario );
void vaciarCola( tCola *cola );

#endif // INCLUDE_COLA_H

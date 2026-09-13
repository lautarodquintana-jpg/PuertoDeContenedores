#ifndef INCLUDE_PILA_H

#include <string.h>
#include <stdlib.h>
#include "nodo.h"

#define OK               1
#define PILA_VACIA      10
#define PILA_LLENA      11
#define PILA_DISPONIBLE 12

typedef tNodo* tPila;

void crearPila( tPila *pila );
int pilaVacia( const tPila *pila );
int pilaLlena( const tPila *pila , unsigned tamUsuario );
int ponerEnPila( tPila *pila , const void *dato , unsigned tamUsuario );
int sacarDePila( tPila *pila , void *dato , unsigned tamUsuario );
int verTope( const tPila *pila , void *dato , unsigned tamUsuario );
void vaciarPila( tPila *pila );

#endif // INCLUDE_PILA_H

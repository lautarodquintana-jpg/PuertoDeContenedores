#include "pila.h"

#define MIN(x,y) x < y ? x : y

void crearPila( tPila *pila )
{
    *pila = NULL;
}

int pilaVacia( const tPila *pila )
{
    return *pila ? PILA_DISPONIBLE : PILA_VACIA;
}

int pilaLlena( const tPila *pila , unsigned tamUsuario )
{
    tNodo* aux;
    void* info;

    if( (aux = (tNodo*)malloc(sizeof(tNodo))) == NULL )
        return PILA_LLENA;

    if( (info = malloc(tamUsuario)) == NULL )
    {
        free(aux);
        return PILA_LLENA;
    }

    free(info);
    free(aux);
    return OK;
}

int ponerEnPila( tPila *pila , const void *dato , unsigned tamUsuario )
{
    tNodo* nuevo;

    if( (nuevo = (tNodo*)malloc(sizeof(tNodo))) == NULL )
        return PILA_LLENA;

    if( (nuevo->info = malloc(tamUsuario)) == NULL )
    {
        free(nuevo);
        return PILA_LLENA;
    }

    nuevo->tamInfo = tamUsuario;
    memcpy( nuevo->info , dato , tamUsuario );

    nuevo->sig = *pila; // El nuevo nodo apunta al anterior tope de la pila
    *pila = nuevo; // El nuevo nodo ahora es el tope

    return OK;
}

int sacarDePila( tPila *pila , void *dato , unsigned tamUsuario )
{
    tNodo *aux = *pila;

    if( !aux )
        return PILA_VACIA;

    memcpy( dato , aux->info , MIN(aux->tamInfo,tamUsuario) );
    *pila = aux->sig;

    free(aux->info);
    free(aux);

    return OK;
}

int verTope( const tPila *pila , void *dato , unsigned tamUsuario )
{
    tNodo *aux = *pila;

    if(!aux)
        return PILA_VACIA;

    memcpy( dato , aux->info , MIN(tamUsuario,aux->tamInfo) );

    return OK;
}

void vaciarPila( tPila *pila )
{
    tNodo *aux;
    while( *pila )
    {
        aux = *pila;
        *pila = aux->sig;

        free(aux->info);
        free(aux);
    }
}

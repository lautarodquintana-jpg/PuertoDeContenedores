#include "cola.h"

#define MIN(x,y) x < y ? x : y

void crearCola( tCola *cola )
{
    cola->pri = NULL;
    cola->ult = NULL;
}

int colaVacia( const tCola *cola )
{
    return cola->pri ? COLA_DISPONIBLE : COLA_VACIA;
}

int colaLlena( unsigned tamUsuario )
{
    tNodo *aux;
    void *dato;

    if( (aux = (tNodo*)malloc(sizeof(tNodo))) == NULL )
        return COLA_LLENA;

    if( (dato = malloc(tamUsuario)) == NULL )
    {
        free(aux);
        return COLA_LLENA;
    }

    free(dato);
    free(aux);

    return COLA_DISPONIBLE;
}

int ponerEnCola( tCola *cola , const void *dato , unsigned tamUsuario )
{
    tNodo *nuevo;

    if( (nuevo = (tNodo*)malloc(sizeof(tNodo))) == NULL )
        return COLA_LLENA;

    if( (nuevo->info = malloc(tamUsuario)) == NULL )
    {
        free(nuevo);
        return COLA_LLENA;
    }

    memcpy( nuevo->info , dato , tamUsuario );
    nuevo->tamInfo = tamUsuario;
    nuevo->sig = NULL;

    if( cola->ult )
        cola->ult->sig = nuevo;
    else
        cola->pri = nuevo;

    cola->ult = nuevo;

    return OK;
}

int sacarDeCola( tCola *cola , void *dato , unsigned tamUsuario )
{
    tNodo *aux = cola->pri;

    if( !aux )
        return COLA_VACIA;

    memcpy( dato , aux->info , MIN(tamUsuario,aux->tamInfo) );
    cola->pri = aux->sig;

    free(aux->info);
    free(aux);

    if( !cola->pri )
        cola->ult = NULL;

    return OK;
}

int verPrimeroCola( const tCola *cola , void *dato , unsigned tamUsuario )
{
    tNodo *aux = cola->pri;

    if( !aux )
        return COLA_VACIA;

    memcpy( dato , aux->info , MIN(tamUsuario,aux->tamInfo) );

    return OK;
}

void vaciarCola( tCola *cola )
{
    tNodo *aux;

    while( cola->pri )
    {
        aux = cola->pri;
        cola->pri = aux->sig;
        free(aux->info);
        free(aux);
    }
}

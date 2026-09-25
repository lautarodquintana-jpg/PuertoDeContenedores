#ifndef LISTASIMPLEMENTEENLAZADA_H_INCLUDED
#define LISTASIMPLEMENTEENLAZADA_H_INCLUDED
#include <string.h>
#include <stdlib.h>

#define SINMEM -2
#define NO_ENCONTRADO -1
#define TODO_OK 1
#define DUPLICADO 2
#define INSERTADO_EN_OTRA_POS 5
#define minimo(a,b)(a<=b?a:b)
typedef struct sNodoLista
{
    void *info;
    unsigned tamElem;
    struct sNodoLista *sig;
}tNodoLista;
typedef tNodoLista* tLista;
typedef int(*cmp)(const void *elem1, const void *elem2);
typedef void(*accion)(void *param, const void *elem);

void crearLista(tLista *pl);
void vaciarLista(tLista *pl);
void mostrarLista(const tLista *pl, void MostrarElemento(const void *elem));
void mostrarListaInversa(const tLista *pl, void MostrarElemento(const void *elem));
void mostrarListaInversaRecursivamente(const tLista *pl, void MostrarElemento(const void *elem));
int listaVacia(const tLista *pl);
int listaLlena(const tLista *pl);
int insertarAlFinalDeLista(tLista *pl, const void *elem, unsigned tam);

int insertarOrdenadoenLista(tLista *pl, const void *elem, unsigned tam, cmp comparacion, int conDuplicado ,accion accionar);
int eliminarDeListaOrdenada(tLista *pl, void *elem, unsigned tam, cmp comparacion);
int eliminarDeListaDesordenada(tLista *pl, void *elem, unsigned tam, cmp comparacion);
void eliminarElementosDuplicados(tLista *pl, cmp comparacion);
int insertarEnPosicionEnLista(tLista *pl, const void *elem, unsigned tam, unsigned pos);
void ordenarLista(tLista *pl, cmp comparacion);
int verNElem(tLista *pl, unsigned pos, void *elem, unsigned tam);
int actualizarNElemDeLista(tLista *pl, const void *elem, unsigned pos, void(actualizar)(void *actualizado, const void *actualizador));
#endif // LISTASIMPLEMENTEENLAZADA_H_INCLUDED

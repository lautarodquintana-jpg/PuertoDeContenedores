#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK 1
#define ERROR_MEMORIA 0
#define DUPLICADO 2
#define ARBOL_VACIO 3
#define NO_ENCONTRADO 4
#define ERROR_ARCHIVO -2
#define MAYOR(a,b)(a>=b?a:b)
#define MINIMO(a,b)(a<=b?a:b)

typedef struct sNodo
{
    void *info;
    unsigned tamElem;
    struct sNodo *izq;
    struct sNodo *der;
}tNodo;

typedef tNodo* tArbol;


void crearArbol(tArbol *pa);
void recorrerPreOrdenArbol(tArbol *pa, void(mostrar)(const void *elem));
void recorrerInOrdenArbol(tArbol *pa, void(mostrar)(const void *elem));
void recorrerPosOrdenArbol(tArbol *pa, void(mostrar)(const void *elem));
int insertarOrdenadoConRecursividad(tArbol *pa, const void *elem, unsigned tam, int(comparacion)(const void *elem1, const void *elem2));
int insertarOrdenadoSinRecursividad(tArbol *pa, const void *elem, unsigned tam, int(comparacion)(const void *elem1, const void *elem2));

int contarNodosDeArbol(tArbol *pa);
int _contarNodosDeArbol(tArbol *pa, int nodos);

int contarNodosConHijosporIzqDeArbol(tArbol *pa);
int _contarNodosConHijosporIzqDeArbol(tArbol *pa, int nodos);

int contarNodosConHijosSOLOporIzqDeArbol(tArbol *pa);
int _contarNodosConHijosSOLOporIzqDeArbol(tArbol *pa, int nodos);

int contarNodosConHijosporIzqyDerDeArbol(tArbol *pa);
int _contarNodosConHijosporIzqyDerDeArbol(tArbol *pa, int nodos);

int contarHojasDeArbol(tArbol *pa);

int calcularAlturadelArbol(tArbol *pa);
int _calcularAlturadelArbol(tArbol *pa, int altura);

int contarNodosEnUnaAltura(tArbol *pa, int altura);
int _contarNodosEnUnaAltura(tArbol *pa, int altura, int altActual, int nodos);

int contarNodosHastaUnaAltura(tArbol *pa, int altura);
int _contarNodosHastaUnaAltura(tArbol *pa, int altura, int altActual, int nodos);

int contarNodosAPartirdeUnaAltura(tArbol *pa, int altura);
int _contarNodosAPartirdeUnaAltura(tArbol *pa, int altura, int altActual, int nodos);

void cortarHojas(tArbol *pa);
int cortarNodo(tArbol *pa, const void *elem, int(cmp)(const void *elem1, const void *elem2));

int generarArbolAPartirDeArchivoBinario(tArbol *pa, const char *nomArch, unsigned tamElem,int(cmp)(const void *elem1, const void *elem2));

int generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado(tArbol *pa, const char *nomArch, unsigned tamElem);
int _generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado(tArbol *pa, unsigned tamElem, FILE *pf, int ini, int fin);

int crearArchivoBinarioAPartirDeArbol(tArbol *pa, const char *nomarch);
void _crearArchivoBinarioAPartirDeArbol(tArbol *pa, FILE *pf);

int eliminarElemDeArbol(tArbol *pa, void *elem, unsigned tamElem,int(cmp)(const void *elem1, const void *elem2));
tArbol* buscarElemEnArbol(tArbol *pa, void *elem, int(cmp)(const void *elem1, const void *elem2));
int eliminarRaizDeArbol(tArbol *pa, void *elem, unsigned tamElem,int(cmp)(const void *elem1, const void *elem2));
int eliminarNodo(tArbol *pa, void *elem, tArbol *aEliminar);

tArbol* BuscarMayorClave(tArbol *pa);
tArbol* BuscarMenorClave(tArbol *pa);

void destruirArbol(tArbol *pa);

#endif // ARBOL_H_INCLUDED

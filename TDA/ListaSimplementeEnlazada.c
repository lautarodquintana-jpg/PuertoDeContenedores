#include "ListaSimplementeEnlazada.h"

void crearLista(tLista *pl)
{
    *pl=NULL;
}
void vaciarLista(tLista *pl)
{
    tNodoLista *elim;

    while(*pl)
    {
        elim=*pl;
        *pl=(*pl)->sig;
        free(elim->info);
        free(elim);
    }
}
void mostrarLista(const tLista *pl, void MostrarElemento(const void *elem))
{
    while(*pl)
    {
        MostrarElemento((*pl)->info);
        pl=&(*pl)->sig;
    }
}
void mostrarListaInversa(const tLista *pl, void MostrarElemento(const void *elem))
{
    tNodoLista *act;
    int cantElems=0, i;

    if(*pl==NULL)
        return;

    act=*pl;
    while(act)
    {
        cantElems++;
        act=act->sig;
    }
    while(cantElems>0)
    {
        act=*pl;
        i=1;
        while(i<cantElems)
        {
            act=act->sig;
            i++;
        }
        MostrarElemento(act->info);
        cantElems--;
    }
}
void mostrarListaInversaRecursivamente(const tLista *pl, void MostrarElemento(const void *elem))
{
    tNodoLista *act=*pl;

    if(act==NULL)
        return;

    mostrarListaInversaRecursivamente(&act->sig, MostrarElemento);
    MostrarElemento(act->info);
}
int listaVacia(const tLista *pl)
{
    return (*pl)==NULL? 1: 0;
}
int listaLlena(const tLista *pl)
{
    tNodoLista *elemPrueba;

    elemPrueba=malloc(sizeof(tNodoLista));
    if(!elemPrueba)
        return 1;
    free(elemPrueba);
    return 0;
}
int insertarAlFinalDeLista(tLista *pl, const void *elem, unsigned tam)
{
    tNodoLista *nue;

    nue=malloc(sizeof(tNodoLista));
    if(!nue)
        return SINMEM;

    nue->info=malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return SINMEM;
    }

    memcpy(nue->info, elem, tam);
    nue->tamElem=tam;
    nue->sig=NULL;

    while(*pl)
    {
        pl=&(*pl)->sig;
    }
    *pl=nue;

    return 1;
}

int insertarOrdenadoenLista(tLista *pl, const void *elem, unsigned tam, cmp comparacion, int conDuplicado ,accion accionar)
{
    tNodoLista *nue;


    while(*pl && comparacion(elem, (*pl)->info)>0)
    {
        pl=&(*pl)->sig;
    }

    if(conDuplicado==0 && (*pl)!=NULL && comparacion(elem, (*pl)->info)==0)//No quiero duplicados
    {
        if(accionar)
        {
            accionar((*pl)->info, elem);
        }
        return DUPLICADO;
    }
    else
    {
        nue=malloc(sizeof(tNodoLista));
        if(!nue)
            return SINMEM;

        nue->info=malloc(tam);
        if(!nue->info)
        {
            free(nue);
            return SINMEM;
        }

        memcpy(nue->info, elem, tam);
        nue->tamElem=tam;
        nue->sig=*pl;
        *pl=nue;
    }

    return TODO_OK;
}
int eliminarDeListaOrdenada(tLista *pl, void *elem, unsigned tam, cmp comparacion)
{
    tNodoLista *elim;
    while(*pl && comparacion(elem, (*pl)->info)>0)
    {
        pl=&(*pl)->sig;
    }
    if(*pl && comparacion(elem, (*pl)->info)==0)
    {// Lo encontre y debo eliminarlo: pl está mirando al nodo a eliminar desde el anterior, debo asignar a *pl=(*pl)->sig
        memcpy(elem, (*pl)->info, minimo(tam, (*pl)->tamElem));
        elim=*pl;
        *pl=(*pl)->sig;
        free(elim->info);
        free(elim);
        return TODO_OK;
    }
    else
        return NO_ENCONTRADO;
}
int eliminarDeListaDesordenada(tLista *pl, void *elem, unsigned tam, cmp comparacion)
{//Retorna si lo elimino o no
    tNodoLista *elim;
    while(*pl && comparacion(elem, (*pl)->info)!=0)
    {
        pl=&(*pl)->sig;
    }
    if(*pl && comparacion(elem, (*pl)->info)==0)
    {// Lo encontre y debo eliminarlo: pl está mirando al nodo a eliminar desde el anterior, debo asignar a *pl=(*pl)->sig
        memcpy(elem, (*pl)->info, minimo(tam, (*pl)->tamElem));
        elim=*pl;
        *pl=(*pl)->sig;
        free(elim->info);
        free(elim);
        return TODO_OK;
    }
    else
        return NO_ENCONTRADO;
}
//Eliminar creativamente
int insertarEnPosicionEnLista(tLista *pl, const void *elem, unsigned tam, unsigned pos)
{
    unsigned cont=0;
    tNodoLista *nue=malloc(sizeof(tNodoLista));
    if(!nue)
        return SINMEM;
    nue->info=malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return SINMEM;
    }
    memcpy(nue->info, elem, tam);
    nue->tamElem=tam;
    while(*pl && cont<pos)
    {
        cont++;
        pl=&(*pl)->sig;
    }
    if(cont<pos)//El usuario me paso un numero de posicion que excede la cantidad de elementos que poseo, por lo que inserto a lo ultimo
    {
        *pl=nue;
        nue->sig=NULL;
        return INSERTADO_EN_OTRA_POS;
    }

    nue->sig=*pl;
    *pl=nue;
    return TODO_OK;
}
void ordenarLista(tLista *pl, cmp comparacion)
{//Debo enganchar y desenganchar los nodos
    tNodoLista *nodoAMover;
    tLista *PMin, *aux; //Esto es tNodoLista **

    while(*pl)
    {
        aux=PMin=pl;
        nodoAMover=*aux;

        while(*aux)
        {
            if(comparacion(nodoAMover->info, (*aux)->info)>0)
            {
                PMin=aux;
                nodoAMover=(*PMin);
            }
            aux=&(*aux)->sig;
        }

        *PMin=nodoAMover->sig;
        nodoAMover->sig=(*pl);
        *pl=nodoAMover;

        pl=&(*pl)->sig;
    }
}
void eliminarElementosDuplicados(tLista *pl, cmp comparacion)
{
    int band;
    tLista *aux;
    tNodoLista *elem, *elim;
    while(*pl)
    {
        aux=pl;
        elem=*aux;
        band=0;
        while(*aux)
        {
            if(*aux!=elem && comparacion(elem->info, (*aux)->info)==0)
            {
                elim=*aux;
                *aux=elim->sig;
                free(elim->info);
                free(elim);
                band=1;
            }
            else
                aux=&(*aux)->sig;
        }
        if(band==1)//Debo borrar ese elemento tambien
        {
            elim=*pl;
            *pl=elim->sig;
            free(elim->info);
            free(elim);
        }
        else
        {
            pl=&(*pl)->sig;
        }
    }
}
int verNElem(tLista *pl, unsigned pos, void *elem, unsigned tam)
{
    unsigned i=0;

    while(*pl && i<pos)
    {
        pl=&(*pl)->sig;
        i++;
    }

    if(*pl)
    {
        memcpy(elem, (*pl)->info, minimo(tam, (*pl)->tamElem));
        return TODO_OK;
    }

    return NO_ENCONTRADO;
}
int actualizarNElemDeLista(tLista *pl, const void *elem, unsigned pos, void(actualizar)(void *actualizado, const void *actualizador))
{
    int i=0;

    while(*pl && i<pos)
    {
        pl=&(*pl)->sig;
        i++;
    }

    if(i==pos)
    {
        actualizar((*pl)->info, elem);
        return TODO_OK;
    }
    else
        return NO_ENCONTRADO;
}

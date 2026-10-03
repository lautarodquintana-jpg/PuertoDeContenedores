#include "muelle.h"

//Genera los muelles y los asigna a la lista de muelles
int generarMuelles(tLista *plMuelles, unsigned cantMuelles){
//    crearLista(plMuelles);
    tMuelle auxMuelle;
        auxMuelle.barco.arriboProgramado = 0;
        auxMuelle.barco.cantidadContenedoresRestante = 0;
        auxMuelle.barco.cantidadContenedoresTotal = 0;
    auxMuelle.estado = MUELLE_DISPONIBLE; //Estado disponible
    for(int i=0 ; i<cantMuelles ; i++){
        auxMuelle.cod = i+1;
        if( 0 == insertarAlFinalDeLista(plMuelles,&auxMuelle,sizeof(tMuelle)))
            return COD_MUELLES_MAL;
    }
    return COD_MUELLES_OK;
}

//Elimina los muelles (para terminar el programa)
void eliminarMuelles(tLista *plMuelles){
    vaciarLista(plMuelles);
}

//Funcion callback para mostrar un muelle
void mostrarMuelleCB(void *ctx, const void *elem){
    tMuelle *muelle = (tMuelle*)elem;
    printf("\n ||-> COD: %u - EST: %u - MA: %u",muelle->cod, muelle->estado, muelle->minutoArribo);
}

//Funcion callback encontrar muelle disponible
void encontrarMuelleCB(void *ctx, const void *elem){
    tMuelle *mue = (tMuelle*)elem;
    if(!(*(unsigned*)ctx) && MUELLE_DISPONIBLE == mue->estado)
        memcpy(ctx, &mue->cod, sizeof(unsigned));
}

// Recorre la lista de muelles y devuelve el primero que este disponible, si no hay ninguno disponible, retorna cero
int encontrarMuelleDisponible(tLista lMuelles){
    unsigned codDisponible = 0;
    if(TODO_OK != recorrerLista(lMuelles, &codDisponible, encontrarMuelleCB))
        return COD_MUELLES_MAL;
    return codDisponible;
}

//Funcion callback para actualizar muelle
void actualizarMuelleCB(void *muelleLista, const void *actualizador){
    tBarco  *barco         =  (tBarco*)((const void **)actualizador)[0];
    unsigned minutoArribo  = *(unsigned*)((const void**)actualizador)[1];

    tMuelle *muelle        = (tMuelle*)muelleLista;
    muelle->estado         = MUELLE_OCUPADO;
    muelle->minutoArribo   = minutoArribo;

    memcpy(&muelle->barco, barco, sizeof(tBarco));
}

//Funcion callback para encontrar el muelle por su codigo y asignar su estado de ocupacion en la variable ctx que se manda (unsigned)
void consultarEstadoMuelleCB(void *muelleLista, const void *actualizador){
    tMuelle *muelle = (tMuelle*)muelleLista;
    unsigned *estado = (unsigned*)actualizador;
    *estado = muelle->estado;
}

// Se le pasa un tBarco inicializado y lo copia en el campo tBarco de un muelle, concidera codigos de 1 a N, siendo 1 el primero y N el maxcant muelles
    // Importante, no valida que exista disponibilidad en el muelle.
int ocuparMuelle(tLista *plMuelles, unsigned codMuelle, tBarco barco, unsigned minutoArribo){
    void *ptxCtx[2] = {&barco, &minutoArribo};
    if(--codMuelle < 0)
        return COD_MUELLES_MAL;
    if(TODO_OK != actualizarNElemDeLista(plMuelles, ptxCtx, codMuelle, actualizarMuelleCB))
        return COD_MUELLES_MAL;
    return COD_MUELLES_OK;
}

int estadoMuelle(tLista *plMuelles, unsigned codMuelle){
    unsigned estadoMuelle;
    if(--codMuelle < 0)
        return COD_MUELLES_MAL;
    if(TODO_OK != actualizarNElemDeLista(plMuelles, &estadoMuelle, codMuelle, consultarEstadoMuelleCB))
        return COD_MUELLES_MAL;
    return estadoMuelle;
}

//Funcion callback para liberar un barco
void liberarMuelleCB(void *muelleLista, const void *actualizador){
    tMuelle *muelle = (tMuelle*)muelleLista;
    muelle->estado = MUELLE_DISPONIBLE;
    muelle->minutoArribo = 0;
    vaciarCola(&muelle->barco.contenedores);
}

int liberarMuelle(tLista *plMuelles, unsigned codMuelle){
    if(--codMuelle < 0)
        return COD_MUELLES_MAL;
    if(TODO_OK != actualizarNElemDeLista(plMuelles, NULL, codMuelle, liberarMuelleCB))
        return COD_MUELLES_MAL;
    return COD_MUELLES_OK;
}

int quitarContenedorMuelle(tLista *plMuelles, unsigned codMuelle); // Requiere primitivas de barco

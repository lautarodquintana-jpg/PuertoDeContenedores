#include "zona_almacenamiento.h"

//Genera las zonas de almacenamiento y las asigna a la lista de zonas de almacenamiento de jornada
int generarZonasAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned cantZonas){
    crearLista(plZonasAlmacenamiento);
    tZona auxZona;
        auxZona.cantidadOcupada = 0;
        crearPila(&auxZona.contenedores);

    for(int i=0 ; i<cantZonas ; i++){
        auxZona.cod = i+1;
        if( 0 == insertarAlFinalDeLista(plZonasAlmacenamiento,&auxZona,sizeof(tZona))){
            vaciarLista(plZonasAlmacenamiento);
            return COD_ZONAS_MAL;
        }
    }
    return COD_ZONAS_OK;
}

//Función callback para mostrar una zona de almacenamiento
void mostrarZonaAlmacenamientoCB(void *ctx, const void *elem){
    tContenedor auxContenedor;
    tZona *zona = (tZona*)elem;
    unsigned *capPila = (unsigned*)ctx;
    if(0 == zona->cantidadOcupada){
        printf("\n o Z%-2u [%-2u/%-2u]", zona->cod,zona->cantidadOcupada, *capPila);
        return;
    }
    if(PILA_VACIA == verTope(&zona->contenedores, &auxContenedor, sizeof(tContenedor))){
        printf("\n ● Z%-2u [%-2u/%-2u] TOPE>[%s]", zona->cod,zona->cantidadOcupada, *capPila, "Error");
        return;
    }
    if(*capPila == zona->cantidadOcupada)
        printf("\n ● Z%-2u [%-2u/%-2u] TOPE>[%s]", zona->cod,zona->cantidadOcupada, *capPila, auxContenedor.cod);
    else
        printf("\n o Z%-2u [%-2u/%-2u] TOPE>[%s]", zona->cod,zona->cantidadOcupada, *capPila, auxContenedor.cod);
}

//Funcion para mostrar todas las zonas de almacenamiento
void mostrarZonasAlmacenamiento(tLista lZonasAlmacenamiento, unsigned capPila, unsigned lineas){
    if(lineas)
        printf("\n [  ZONAS DE ALMACENAMIENTO  ]\n");
    if(lineas>=100)
        while(lineas--)
            printf("-");
    recorrerLista(lZonasAlmacenamiento, &capPila, mostrarZonaAlmacenamientoCB);
}

//Función callback que devuelve la disponibilidad de una zona de almacenamiento
void obtenerDisponibilidadCB(void *zonaLista, const void *cantidadOcupado){
    tZona *zona = (tZona*)zonaLista;
    *(unsigned*)cantidadOcupado = zona->cantidadOcupada;
}

//Funcion que devuelve el estado de ocupación de una zona: ZONA_LLENA (0) - ZONA_DISPONIBLE(1)
int consultarDisponibilidadZona(tLista *plZonasAlmacenamiento, unsigned codZona, unsigned capacidadPila){
    unsigned cantidadOcupada;
    if(0 == codZona--)
        return COD_ZONAS_MAL;
    if(TODO_OK != actualizarNElemDeLista(plZonasAlmacenamiento, &cantidadOcupada, codZona, obtenerDisponibilidadCB))
        return COD_ZONAS_MAL;
    return cantidadOcupada==capacidadPila?ZONA_LLENA:(cantidadOcupada==0?ZONA_VACIA:ZONA_DISPONIBLE);
}

//Funcion callback que empila un contenedor a una zona de almacenamiento
void apilarContenedorCB(void *zonaLista, const void *ctx){
    tZona       *zonaAlmacenamiento = (tZona*)zonaLista;
    tContenedor *contenedor         = (tContenedor*)((const void**)ctx)[0];
    unsigned    *error              = (unsigned*)((const void**)ctx)[1];
    if(PILA_LLENA == ponerEnPila(&zonaAlmacenamiento->contenedores, contenedor, sizeof(tContenedor)))
        *error = 1;
    else
        zonaAlmacenamiento->cantidadOcupada++;
}

//Funcion que agrega a una zona de almacenamiento un contenedor
int agregarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor contenedor, unsigned capacidadPila){
    unsigned error=0;
    void *ptxCtx[2] = {&contenedor, &error};
    if(0 == codZona--)
        return COD_ZONAS_MAL;
    if(ZONA_LLENA == consultarDisponibilidadZona(plZonasAlmacenamiento,codZona,capacidadPila))
        return ZONA_LLENA;
    if(TODO_OK != actualizarNElemDeLista(plZonasAlmacenamiento, &ptxCtx, codZona, apilarContenedorCB))
        return COD_ZONAS_MAL;
    return error?COD_ZONAS_MAL:COD_ZONAS_OK;
}

//Funcion callback para el desapilado de un contenedor de una zona de almacenamiento
void desapilarContenedorCB(void *zonaLista, const void *ctx){
    tZona       *zonaAlmacenamiento = (tZona*)zonaLista;
    tContenedor *contenedor         = (tContenedor*)((const void**)ctx)[0];
    unsigned    *error              = (unsigned*)((const void**)ctx)[1];
    if(PILA_VACIA == sacarDePila(&zonaAlmacenamiento->contenedores, contenedor, sizeof(tContenedor)))
        *error = PILA_VACIA;
    else
        zonaAlmacenamiento->cantidadOcupada--;
}

//Funcion que retira un contenedor de la pila de una zona de almacenamiento y la asignar a "tContendor *contenedorDest", variable de funcion llamadora
int retirarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor *contenedorDest){
    unsigned error=0;
    void *ptxCtx[2] = {contenedorDest, &error};
    if(0 == codZona--)
        return COD_ZONAS_MAL;
    if(TODO_OK != actualizarNElemDeLista(plZonasAlmacenamiento, &ptxCtx, codZona, desapilarContenedorCB))
        return COD_ZONAS_MAL;
    return error?error:COD_ZONAS_OK;
}

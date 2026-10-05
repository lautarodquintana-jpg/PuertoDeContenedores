#include "proceso_operacion.h"

//Funcion que permite reubicar contenedores.
int reubicarContenedores(tJornada *jornada, unsigned capacidadPila, unsigned codOrigen, unsigned codDestino){
    tContenedor auxContenedor;
    if(COD_ZONAS_OK != retirarContenedorZonaAlmacenamiento(&jornada->zonasAlmacenamiento, codOrigen, &auxContenedor))
        return COD_ZONAS_MAL;
    if(COD_ZONAS_OK != agregarContenedorZonaAlmacenamiento(&jornada->zonasAlmacenamiento, codDestino, auxContenedor, capacidadPila))
        return COD_ZONAS_MAL;
    return COD_ZONAS_OK;
}

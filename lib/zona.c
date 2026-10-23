#include "zona.h"

//Inicializa la lista de zonas
void crearListaZonas( tLista* zonas )
{
    crearLista( zonas );
}

int generarZonas( tLista* zonas , unsigned cantZonas )
{
    int i;
    tZona auxZona;

    auxZona.cantidadOcupada = 0;
    crearPila( &auxZona.contenedores );

    for( i = 0 ; i < cantZonas ; i++ )
    {
        auxZona.cod = i + 1;
        if( insertarAlFinalDeLista(zonas,&auxZona,sizeof(tZona)) != TODO_OK )
        {
            vaciarLista(zonas);
            return COD_ZONAS_MAL;
        }
    }
    return COD_ZONAS_OK;
}


void apilarContenedorEnZona( void* pZona , const void* elem )
{
    tZona* zona = (tZona*)pZona;
    tContenedor* contenedor = (tContenedor*)elem;

    if( ponerEnPila( &zona->contenedores , contenedor , sizeof(tContenedor) ) != TODO_OK )
        *contenedor->cod = '\0';
    else
        zona->cantidadOcupada++;
}

void consultarCantidadOcupada(void *elem, const void *actualizador)
{
    tZona* zona = (tZona*)elem;
    unsigned* cantOcupada = (unsigned*)actualizador;
    *cantOcupada = zona->cantidadOcupada;
}

int tieneZonaEspacio( tLista *plZonas , unsigned codZona , unsigned capacidadPilaZona )
{
    unsigned cantOcupada = 0;
    if( actualizarNElemDeLista( plZonas , &cantOcupada , codZona , consultarCantidadOcupada ) != TODO_OK )
        return ERROR_OBTENER_CANT_OCUPADA;

    return cantOcupada < capacidadPilaZona;
}

void mostrarTopeContenedorPorZona( void* elem , const void *actualizador )
{
    tZona* zona = (tZona*)actualizador;
    tContenedor contenedor;

    printf("Z%u ",zona->cod);
    if( verTope( &zona->contenedores , &contenedor , sizeof(tContenedor) ) == TODO_OK )
        printf( "[%s]", contenedor.cod );
    else
        printf( "[SIN CONTENEDORES]" );

    putchar('\n');
}

void mostrarZonas( tLista *plZonas )
{
    printf("\n\n=== ZONAS ===\n\n");
    recorrerLista( plZonas , NULL , mostrarTopeContenedorPorZona );
}

//Funcion callback para el desapilado de un contenedor de una zona de almacenamiento
void desapilarContenedorCB(void *zonaLista, const void *ctx)
{
    tZona       *zonaAlmacenamiento = (tZona*)zonaLista;
    tContenedor *contenedor         = (tContenedor*)((const void**)ctx)[0];
    unsigned    *error              = (unsigned*)((const void**)ctx)[1];
    if(PILA_VACIA == sacarDePila(&zonaAlmacenamiento->contenedores, contenedor, sizeof(tContenedor)))
        *error = PILA_VACIA;
    else
        zonaAlmacenamiento->cantidadOcupada--;
}

//Funcion que retira un contenedor de la pila de una zona de almacenamiento y la asignar a "tContendor *contenedorDest", variable de funcion llamadora
int retirarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor *contenedorDest)
{
    unsigned error=0;
    void *ptxCtx[2] = {contenedorDest, &error};
    if(0 == codZona--)
        return COD_ZONAS_MAL;
    if(TODO_OK != actualizarNElemDeLista(plZonasAlmacenamiento, &ptxCtx, codZona, desapilarContenedorCB))
        return COD_ZONAS_MAL;
    return error?error:COD_ZONAS_OK;
}

//Funcion para obtener codigo de zona donde se encuentre un codigo de contenedor.
int buscarCodZona( tLista *plZonasAlmacenamiento, const char* codContenedor )
{
    return busquedaClavePosLista( plZonasAlmacenamiento, codContenedor, compararZonaTopeContenedor );
}

int compararZonaTopeContenedor( const void* elem_z, const void* elem_c )
{
    tZona* zona = (tZona*)elem_z;
    char* codContenedor = (char*)elem_c;
    tContenedor conteEnZona;

    if( verTope( &zona->contenedores, &conteEnZona, sizeof(tContenedor) ) == TODO_OK )
        return strcmp( conteEnZona.cod, codContenedor );
    else
        return -1;
}

//Función callback que devuelve la disponibilidad de una zona de almacenamiento
void obtenerDisponibilidadCB(void *zonaLista, const void *cantidadOcupado)
{
    tZona *zona = (tZona*)zonaLista;
    *(unsigned*)cantidadOcupado = zona->cantidadOcupada;
}

//Funcion que devuelve el estado de ocupación de una zona: ZONA_LLENA (0) - ZONA_DISPONIBLE(1)
int consultarDisponibilidadZona(tLista *plZonasAlmacenamiento, unsigned codZona, unsigned capacidadPila)
{
    unsigned cantidadOcupada = 0;
    if(0 == codZona--)
        return COD_ZONAS_MAL;
    if(TODO_OK != actualizarNElemDeLista(plZonasAlmacenamiento, &cantidadOcupada, codZona, obtenerDisponibilidadCB))
        return COD_ZONAS_MAL;
    return cantidadOcupada==capacidadPila?ZONA_LLENA:(cantidadOcupada==0?ZONA_VACIA:ZONA_DISPONIBLE);
}

//Funcion callback que empila un contenedor a una zona de almacenamiento
void apilarContenedorCB(void *zonaLista, const void *ctx)
{
    tZona       *zonaAlmacenamiento = (tZona*)zonaLista;
    tContenedor *contenedor         = (tContenedor*)((const void**)ctx)[0];
    unsigned    *error              = (unsigned*)((const void**)ctx)[1];
    if(PILA_LLENA == ponerEnPila(&zonaAlmacenamiento->contenedores, contenedor, sizeof(tContenedor)))
        *error = 1;
    else
        zonaAlmacenamiento->cantidadOcupada++;
}

//Funcion que agrega a una zona de almacenamiento un contenedor
int agregarContenedorZonaAlmacenamiento(tLista *plZonasAlmacenamiento, unsigned codZona, tContenedor contenedor, unsigned capacidadPila)
{
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

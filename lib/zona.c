#include "zona.h"

//Inicializa la lista de zonas
void crearListaZonas( tLista* zonas )
{
    crearLista( zonas );
}

int generarZonas( tLista* zonas, unsigned cantZonas, int largoMaxCodContenedor, int contenedoresApiladosMax)
{
    int i;
    tZona auxZona;

    auxZona.cantidadOcupada = 0;

    crearPila( &auxZona.contenedores );

    for( i = 0 ; i < cantZonas ; i++ )
    {
        auxZona.cod = i + 1;

        auxZona.cadContenedoresApilados=malloc(sizeof(char)*largoMaxCodContenedor*contenedoresApiladosMax)+contenedoresApiladosMax+1;
        //En la cadena guardaré: códigos de contenedores, el '\0' y el '\n' entre códigos
        //Le sumo 1 para el '\0' y contenedoresApiladosMax para todos los '\n' que puedan aparecer

        if(!auxZona.cadContenedoresApilados)
        {
            liberarZonasDeAlmacenamiento(zonas);
            return COD_ZONAS_MAL;
        }
        auxZona.cadContenedoresApilados[0]='\0';

        if( insertarAlFinalDeLista(zonas,&auxZona,sizeof(tZona)) != TODO_OK )
        {
            liberarZonasDeAlmacenamiento(zonas);
            return COD_ZONAS_MAL;
        }
    }
    return COD_ZONAS_OK;
}


void apilarContenedorEnZona( void* pZona, const void* elem )
{
    tZona* zona = (tZona*)pZona;
    tContenedor* contenedor = (tContenedor*)elem;

    if( ponerEnPila( &zona->contenedores, contenedor, sizeof(tContenedor) ) != TODO_OK )
        *contenedor->cod = '\0';
    else
        zona->cantidadOcupada++;

    strcat(zona->cadContenedoresApilados, contenedor->cod);
    strcat(zona->cadContenedoresApilados, "\n");
}

void consultarCantidadOcupada(void *elem, const void *actualizador)
{
    tZona* zona = (tZona*)elem;
    unsigned* cantOcupada = (unsigned*)actualizador;
    *cantOcupada = zona->cantidadOcupada;
}

int tieneZonaEspacio( tLista *plZonas, unsigned codZona, unsigned capacidadPilaZona )
{
    unsigned cantOcupada = 0;
    if( actualizarNElemDeLista( plZonas, &cantOcupada, codZona, consultarCantidadOcupada ) != TODO_OK )
        return ERROR_OBTENER_CANT_OCUPADA;

    return cantOcupada < capacidadPilaZona;
}

void mostrarContenedoresdeZonaAlmacenamiento( void* elem, const void *actualizador )
{
    tZona* zona = (tZona*)actualizador;

    printf("Z%u ",zona->cod);
    printf( "\nPila de contenedores: \n");
    if( mostrarCadContenedoresEnFormaDePilaRecursiva(zona->cadContenedoresApilados) == 0 )//No hay contenedores apilados, sino los muestra
    {
        printf( "[SIN CONTENEDORES]" );
    }

    putchar('\n');
}

void mostrarZonas( tLista *plZonas )
{
    printf("\n\n=== ZONAS ===\n\n");
    recorrerLista( plZonas, NULL, mostrarContenedoresdeZonaAlmacenamiento );
}
int mostrarCadContenedoresEnFormaDePilaRecursiva(const char *cad)
{
    int largoHastaElBarraN;
    if(*cad=='\0')
        return 0;
    else
    {
        largoHastaElBarraN=strchr(cad, '\n')-cad+1;
        mostrarCadContenedoresEnFormaDePilaRecursiva(cad+largoHastaElBarraN);
        printf("%*.*s", largoHastaElBarraN, largoHastaElBarraN, cad);
        return 1;//Los return son utilizados para saber si encontré contenedores apilados o no
    }
}
//Funcion callback para el desapilado de un contenedor de una zona de almacenamiento
void desapilarContenedorCB(void *zonaLista, const void *ctx)
{
    char *aux;
    tZona       *zonaAlmacenamiento = (tZona*)zonaLista;
    tContenedor *contenedor         = (tContenedor*)((const void**)ctx)[0];
    unsigned    *error              = (unsigned*)((const void**)ctx)[1];
    if(PILA_VACIA == sacarDePila(&zonaAlmacenamiento->contenedores, contenedor, sizeof(tContenedor)))
        *error = PILA_VACIA;
    else
    {
        //Estructura de la cadena: codigo+'\n'+codigo+'\n'
        //Si llegué hasta acá, se que aunque sea hay 1 contenedor apilado: pasan 2 cosas
        //-Es el ultimo contenedor: hay 1 solo '\n'
        //-Hay mas de 1 contenedor apilado: hay 2 o mas '\n'

        aux=strrchr(zonaAlmacenamiento->cadContenedoresApilados, '\n');
        *aux='\0';//Elimino el '\n' que está después del codigo a quitar

        aux=strrchr(zonaAlmacenamiento->cadContenedoresApilados, '\n');
        //Acá ocurre que devuelva nulo, entonces es el único contenedor en pila
        //u ocurre que no devuelva nulo, entonces hay 2 o mas contenedores apilados

        if(aux==NULL)//Es el ultimo contenedor apilado
        {
            zonaAlmacenamiento->cadContenedoresApilados[0]='\0';
        }
        else        //No es el último contenedor apilado
        {
            *(aux+1)='\0';
        }
        zonaAlmacenamiento->cantidadOcupada--;
    }

}

//Funcion que retira un contenedor de la pila de una zona de almacenamiento y la asignar a "tContenedor *contenedorDest", variable de funcion llamadora
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
    {
        zonaAlmacenamiento->cantidadOcupada++;
        strcat(zonaAlmacenamiento->cadContenedoresApilados, contenedor->cod);
        strcat(zonaAlmacenamiento->cadContenedoresApilados, "\n");
    }
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

void liberarZonasDeAlmacenamiento (tLista *zonas)
{
    tZona aux;
    int i=0;

    //Primero libero la memoria reservada para las cadenas
    while(verNElem(zonas, i, &aux, sizeof(tZona))==TODO_OK)
    {
        free(aux.cadContenedoresApilados);
        vaciarPila(&aux.contenedores);
        i++;
    }

    vaciarLista(zonas);
}

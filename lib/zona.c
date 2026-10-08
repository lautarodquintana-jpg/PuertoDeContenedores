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

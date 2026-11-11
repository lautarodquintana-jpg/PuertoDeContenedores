#include "proceso_operacion.h"

int procesarOperacion( tJornada* jornada, tOperacion* ope, const tConfig* config )
{
    int res;

    if( strcmp( ope->cod, "VER" ) == 0 )
        return ejecutarVER(jornada);

    if( strcmp( ope->cod, "REU" ) == 0 )
    {
        if( ejecutarREU( &jornada->zonasAlmacenamiento, ope , config->capPila ) == TODO_OK )
        {
            jornada->tiempoActual += config->tiempoDeReubicacionDeContenedor;
            jornada->cantOperaciones.cantReubic++;
            mostrarZonas( &jornada->zonasAlmacenamiento );
        }
        else
            return COD_ZONAS_MAL;
    }

    if( strcmp( ope->cod, "DES" ) == 0 )
    {
        res=ejecutarDES(&jornada->muelles, &jornada->zonasAlmacenamiento, ope , config->capPila, &jornada->puntajeActual, &jornada->cantOperaciones.cantDescargas);
        if( res == TODO_OK )
        {
            jornada->tiempoActual += config->tiempoDeDescargaDeContenedor;
            mostrarZonas( &jornada->zonasAlmacenamiento );
        }
        else
            return res;
    }

    if( strcmp( ope->cod, "ENT" ) == 0 )
    {
        res=ejecutarENT( &jornada->camiones, &jornada->zonasAlmacenamiento , ope);
        if( res == TODO_OK )
        {
            jornada->tiempoActual += config->tiempoDeCargaDeCamion;
            jornada->puntajeActual += PUNTOS_ENTREGA;
            jornada->cantOperaciones.cantEntregas++;
            return TODO_OK;
        }
        else
            return res;
    }

    if( strcmp( ope->cod, "ESP" ) == 0 )
        jornada->tiempoActual += 1;

    return TODO_OK;
}

/**
Muestra el tiempo actual, los buques
atracados, la cantidad de buques esperando, el
contenido de las zonas de almacenamiento, la ventana
de planificaci�n de hasta tres camiones y la puntuaci�n
provisoria.
*/
int ejecutarVER( tJornada* jornada )
{
    printf("\nTiempo transcurrido: %u minutos\n", jornada->tiempoActual);
    printf("Puntaje: %u\n", jornada->puntajeActual);

    mostrarMuelles( &jornada->muelles );
    mostrarZonas( &jornada->zonasAlmacenamiento );
    if(mostrarPrimeros3Camiones( &jornada->camiones )!=OK)
        return COLA_LLENA;

    printf("\n");
    return OK;
}

/**El ultimo contenedor que se ingreso en el buque sera el que se ponga en
la pila de la zona seleccionada.
*/
int ejecutarDES(tLista *plMuelles , tLista *plZonas , tOperacion *ope, unsigned capacidadPilaZona, unsigned* puntuacionActual, unsigned* cantDesTotales )
{
    tContenedor contenedor;
    int res;

    /**Obtenemos el primer contenedor del barco del primer muelle. No lo eliminamos porque
    si ocurre un error, habria que reinsertarlo en el barco*/
    res = obtenerContenedorMuelle( plMuelles , ope->codParametro_1 , &contenedor );

    if( res != TODO_OK )
    {
        fprintf(stderr,"\n\t | ERROR AL OBTENER CONTENEDOR DEL BUQUE DEL MUELLE %u |\n", ope->codParametro_1);
        return res;
    }

    /**Ponemos el contenedor en la Zona*/
    if( actualizarNElemDeLista( plZonas , &contenedor , ope->codParametro_2 - 1 , apilarContenedorEnZona ) != TODO_OK )
        return ZONA_APILAR_CONTENEDOR_MAL;

    // SI NO SE PUDO AGREGAR A LA PILA, ENTONCES DEBEREMOS TERMINAR EL JUEGO POR FALTA DE MEMORIA.
    if( *contenedor.cod == '\0' )
        return PILA_LLENA;

    res = quitarContenedorMuelle( plMuelles , ope->codParametro_1 , &contenedor, puntuacionActual, cantDesTotales );
    if( res != TODO_OK )
        return res;

    strcpy(ope->contenedorInvolucrado, contenedor.cod);

    return res;
}

//Se ejecuta la entrega del contenedor al primer camion de la cola
int ejecutarENT( tCola* colaCamiones, tLista* plZonas , tOperacion *ope)
{
    return realizarEntregaCamion( colaCamiones, plZonas , ope);
}

//Funcion que reubica: el contenedor en el tope de la zona origen se coloca en el tope de la zona destino
int ejecutarREU( tLista* plZonasAlmacenamiento, tOperacion *ope, unsigned maxCapacidadZonas )
{
    tContenedor contenedor;

    if( retirarContenedorZonaAlmacenamiento( plZonasAlmacenamiento , ope->codParametro_1 , &contenedor ) != COD_ZONAS_OK )
    {
        fprintf(stderr,"\n\t | ERROR AL RETIRAR CONTENEDOR DE ZONA %u |\n",ope->codParametro_1);
        return COD_ZONAS_MAL;
    }
    strcpy(ope->contenedorInvolucrado, contenedor.cod);
    if( agregarContenedorZonaAlmacenamiento( plZonasAlmacenamiento, ope->codParametro_2, contenedor, maxCapacidadZonas ) != COD_ZONAS_OK )
    {
        fprintf(stderr,"\n\t | ERROR AL AGREGAR CONTENEDOR DE ZONA %u |\n",ope->codParametro_2);
        return COD_ZONAS_MAL;
    }

    return TODO_OK;
}

#include "proceso_operacion.h"

void procesarOperacion( tJornada* jornada, const tOperacion* ope, const tConfig* config )
{
    if( strcmp( ope->cod, "VER" ) == 0 )
        ejecutarVER(jornada);

    if( strcmp( ope->cod, "REU" ) == 0 )
    {
        if( ejecutarREU( &jornada->zonasAlmacenamiento, ope->codParametro_1, ope->codParametro_2, config->cantidadZonasDeAlmacenamiento ) == TODO_OK )
            jornada->tiempoActual += config->tiempoDeReubicacionDeContenedor;
    }

    if( strcmp( ope->cod, "DES" ) == 0 )
    {
        if( ejecutarDES(&jornada->muelles, &jornada->zonasAlmacenamiento, ope->codParametro_1, ope->codParametro_2, config->capPila, &jornada->puntajeActual) == TODO_OK )
                jornada->tiempoActual += config->tiempoDeDescargaDeContenedor;
    }

    if( strcmp( ope->cod, "ENT" ) == 0 )
    {
        if( ejecutarENT( &jornada->camiones, &jornada->zonasAlmacenamiento ) == TODO_OK )
        {
            jornada->tiempoActual += config->tiempoDeCargaDeCamion;
            jornada->puntajeActual += 10;
        }

    }

    if( strcmp( ope->cod, "ESP" ) == 0 )
        jornada->tiempoActual += 1;
}

/**
Muestra el tiempo actual, los buques
atracados, la cantidad de buques esperando, el
contenido de las zonas de almacenamiento, la ventana
de planificaci�n de hasta tres camiones y la puntuaci�n
provisoria.
*/
void ejecutarVER( tJornada* jornada )
{
    printf("\nTiempo transcurrido: %u minutos\n", jornada->tiempoActual);
    printf("Puntaje: %u\n", jornada->puntajeActual);

    mostrarMuelles( &jornada->muelles );
    mostrarZonas( &jornada->zonasAlmacenamiento );
    mostrarPrimerCamion( &jornada->camiones );

    printf("\n");
}

/**El ultimo contenedor que se ingreso en el buque sera el que se ponga en
la pila de la zona seleccionada.
*/
int ejecutarDES(tLista *plMuelles , tLista *plZonas , unsigned codMuelleOri , unsigned codZonaDes, unsigned capacidadPilaZona, unsigned* puntuacionActual )
{
    tContenedor contenedor;
    int res;

    /**Obtenemos el primer contenedor del barco del primer muelle. No lo eliminamos porque
    si ocurre un error, habria que reinsertarlo en el barco*/
    res = obtenerContenedorMuelle( plMuelles , codMuelleOri , &contenedor );
    if( res != TODO_OK )
    {
        fprintf(stderr,"\n\t | ERROR AL OBTENER CONTENEDOR DEL BUQUE DEL MUELLE %u |\n",codMuelleOri);
        return res;
    }

    /**Ponemos el contenedor en la Zona*/
    if( actualizarNElemDeLista( plZonas , &contenedor , codZonaDes - 1 , apilarContenedorEnZona ) != TODO_OK )
        return ZONA_APILAR_CONTENEDOR_MAL;

    // SI NO SE PUDO AGREGAR A LA PILA, ENTONCES DEBEREMOS TERMINAR EL JUEGO POR FALTA DE MEMORIA.
    if( *contenedor.cod == '\0' )
        return PILA_LLENA;

    res = quitarContenedorMuelle( plMuelles , codMuelleOri , &contenedor, puntuacionActual );
    if( res != TODO_OK )
        return res;

    mostrarZonas( plZonas );

    return res;
}

//Se ejecuta la entrega del contenedor al primer camion de la cola
int ejecutarENT( tCola* colaCamiones, tLista* plZonas )
{
    return realizarEntregaCamion( colaCamiones, plZonas );
}

//Funcion que reubica: el contenedor en el tope de la zona origen se coloca en el tope de la zona destino
int ejecutarREU( tLista* plZonasAlmacenamiento, unsigned codZonaOri, unsigned codZonaDes, unsigned maxCapacidadZonas )
{
    tContenedor contenedor;

    if( retirarContenedorZonaAlmacenamiento( plZonasAlmacenamiento , codZonaOri , &contenedor ) != COD_ZONAS_OK )
    {
        fprintf(stderr,"\n\t | ERROR AL RETIRAR CONTENEDOR DE ZONA %u |\n",codZonaOri);
        return COD_ZONAS_MAL;
    }

    if( agregarContenedorZonaAlmacenamiento( plZonasAlmacenamiento, codZonaDes, contenedor, maxCapacidadZonas ) != COD_ZONAS_OK )
    {
        fprintf(stderr,"\n\t | ERROR AL AGREGAR CONTENEDOR DE ZONA %u |\n",codZonaDes);
        return COD_ZONAS_MAL;
    }

    return TODO_OK;
}

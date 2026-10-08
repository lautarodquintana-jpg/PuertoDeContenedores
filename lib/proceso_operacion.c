#include "proceso_operacion.h"

void procesarOperacion( tJornada* jornada, const tOperacion* ope, const tConfig* config )
{
    if( strcmp( ope->cod, "VER" ) == 0 )
        ejecutarVER(jornada);

    if( strcmp( ope->cod, "REU" ) == 0 )
    {
        //if( ejecutarREU() == TODO_OK )
            jornada->tiempoActual += config->tiempoDeReubicacionDeContenedor;
    }

    if( strcmp( ope->cod, "DES" ) == 0 )
    {
        if( ejecutarDES(&jornada->muelles, &jornada->zonasAlmacenamiento, ope->codParametro_1, ope->codParametro_2, config->capPila) == TODO_OK )
            jornada->tiempoActual += config->tiempoDeDescargaDeContenedor;
    }

    if( strcmp( ope->cod, "ENT" ) == 0 )
    {
        //if( ejecutarENT() == TODO_OK )
            jornada->tiempoActual += config->tiempoDeCargaDeCamion;
    }

    if( strcmp( ope->cod, "ESP" ) == 0 )
        jornada->tiempoActual += 1;
}

/**
Muestra el tiempo actual, los buques
atracados, la cantidad de buques esperando, el
contenido de las zonas de almacenamiento, la ventana
de planificación de hasta tres camiones y la puntuación
provisoria.
*/
void ejecutarVER( tJornada* jornada )
{
    printf("\nTiempo transcurrido: %u minutos\n", jornada->tiempoActual);
    printf("Puntaje: %u\n", jornada->puntajeActual);

    mostrarMuelles( &jornada->muelles );
    mostrarZonas( &jornada->zonasAlmacenamiento );

    printf("\n");
}

/**El ultimo contenedor que se ingreso en el buque sera el que se ponga en
la pila de la zona seleccionada.
*/
int ejecutarDES(tLista *plMuelles , tLista *plZonas , unsigned codMuelleOri , unsigned codZonaDes, unsigned capacidadPilaZona )
{
    tContenedor contenedor;
    int res = TODO_OK;

    // Primero debemos verificar si existe la zona seleccionada y hay espacio
    if( !tieneZonaEspacio( plZonas , codZonaDes - 1 , capacidadPilaZona ) )
    {
        fprintf(stderr,"\nZONA LLENA\n");
        return ZONA_NO_DISPO;
    }

    /**Obtenemos el primer contenedor del barco del primer muelle. No lo eliminamos porque
    si ocurre un error, habria que reinsertarlo en el barco*/
    res = obtenerContenedorMuelle( plMuelles , codMuelleOri , &contenedor );
    if( res != TODO_OK )
    {
        fprintf(stderr,"\nERROR al obtener contenedor del buque del muelle codigo %u\n",codMuelleOri);
        return res;
    }

    /**Ponemos el contenedor en la Zona*/
    if( actualizarNElemDeLista( plZonas , &contenedor , codZonaDes - 1 , apilarContenedorEnZona ) != TODO_OK )
        return ZONA_APILAR_CONTENEDOR_MAL;

    // SI NO SE PUDO AGREGAR A LA PILA, ENTONCES DEBEREMOS TERMINAR EL JUEGO POR FALTA DE MEMORIA.
    if( *contenedor.cod == '\0' )
        return PILA_LLENA;

    res = quitarContenedorMuelle( plMuelles , codMuelleOri , &contenedor );
    if( res != TODO_OK )
        return res;

    mostrarZonas( plZonas );

    return res;
}

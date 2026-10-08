#include "barco.h"

void mostrarBarco(const void* elem)
{
    tBarco* barco = (tBarco*)elem;
    printf(" COD: %s | Tiempo arribo: %u | Cant conte. to: %u | Cant conte. res: %u\n",barco->cod,barco->arriboProgramado,barco->cantidadContenedoresTotal,barco->cantidadContenedoresRestante);
}

/** Se ponen los barcos que estaban en camino cuyos tiempos de llegada
son menores o iguales al tiempo actual de la jornada.
*/
void ponerBarcosArribadosEnMuelles( tLista *plMuelles , tCola* barcosEnCola , unsigned tiempoActual )
{
    tBarco barco;
    unsigned codMuelleDisponible;
    int seEncontroBarco;

    codMuelleDisponible = encontrarMuelleDisponible( *plMuelles );
    seEncontroBarco = verPrimeroCola( barcosEnCola , &barco , sizeof(tBarco) );

    while( codMuelleDisponible != 0 && seEncontroBarco == OK && barco.arriboProgramado <= tiempoActual )
    {
        if( sacarDeCola( barcosEnCola , &barco , sizeof(tBarco) ) != TODO_OK )
            return;

        ocuparMuelle( plMuelles , codMuelleDisponible , barco , barco.arriboProgramado );
        codMuelleDisponible = encontrarMuelleDisponible( *plMuelles );
        seEncontroBarco = verPrimeroCola( barcosEnCola , &barco , sizeof(tBarco) );
    }
}

void mostrarTopeContenedorBarcoEnMuelle( tLista* plMuelle, unsigned codMuelle )
{
    tBarco barco;
    tMuelle muelle;
    tContenedor contenedor;

    if( verNElem( plMuelle , codMuelle , &muelle , sizeof(tMuelle) ) != TODO_OK )
    {
        barco = muelle.barco;
        if( verPrimeroCola( &barco.contenedores , &contenedor , sizeof(tContenedor) ) != OK )
            printf("Cod. contenedor: %s\n",contenedor.cod);
    }
}

void obtenerCantidadDeContenedoresRestantesBarco( void *ctx, const void* elem  )
{
    tMuelle* muelle = (tMuelle*)ctx;
    unsigned* cantContenedoresRestantes = (unsigned*)elem;
    *cantContenedoresRestantes = muelle->barco.cantidadContenedoresRestante;
}


/** Saca el primer contenedor en el barco y resta la cantidad restantes de contenedores
 en el barco*/
void quitarPrimerContenedorBarco( void *ctx, const void* elem  )
{
    tMuelle* muelle = (tMuelle*)ctx;
    tContenedor* contenedor = (tContenedor*)elem;

    if( sacarDeCola( &muelle->barco.contenedores , contenedor , sizeof(tContenedor) ) == OK )
        muelle->barco.cantidadContenedoresRestante--;
}

// Previsualiza cual es el primer contenedor del barco
void obtenerPrimerContenedorDeBarco( void *ctx, const void* elem  )
{
    tMuelle* muelle = (tMuelle*)ctx;
    tContenedor* contenedor = (tContenedor*)elem;

    if( verPrimeroCola( &muelle->barco.contenedores , contenedor , sizeof(tContenedor) ) != OK )
        *contenedor->cod = '\0';
}

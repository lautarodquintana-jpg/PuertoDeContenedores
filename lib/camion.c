#include "camion.h"

// Mostrar datos del camion recibido
void mostrarCamion( const void* elem )
{
    tCamion* camion = (tCamion*) elem;
    printf("||-> COD: %s | MIN:%u | COD CONTENEDOR A RETIRAR: %s\n",
           camion->cod,
           camion->minutoRetiro,
           camion->codContenedorRetirar
           );
}

//Muestra primer camion de la cola
void mostrarPrimerCamion( tCola* camionesEnCola )
{
    tCamion camion;

    printf("\n\n=== CAMION ===\n\n");
    if( verPrimeroCola( camionesEnCola, &camion, sizeof(tCamion) ) == TODO_OK )
    {
        mostrarCamion( &camion );
    }else
        printf("\n\t | NO HAY CAMIONES EN LA COLA ACTUALMENTE |\n");
}

//Realiza entrega del primer camion de la cola (en caso de que este el contenedor en algun tope de las zonas)
int realizarEntregaCamion( tCola* camionesEnCola , tLista* plZonasAlmacenamiento )
{
    tCamion camion;
    tContenedor conteAux;
    int codZonaOri;

    if( verPrimeroCola( camionesEnCola, &camion, sizeof(tCamion) ) == TODO_OK )
    {
        codZonaOri = buscarCodZona( plZonasAlmacenamiento, camion.codContenedorRetirar );
        if( codZonaOri != NO_ENCONTRADO )
        {
            if( retirarContenedorZonaAlmacenamiento( plZonasAlmacenamiento, codZonaOri+1, &conteAux ) == COD_ZONAS_OK && sacarDeCola(camionesEnCola, &camion, sizeof(tCamion) ) == TODO_OK )
            {
                fprintf(stdout,"\n\t | CONTENEDOR %s SE ENTREGO A CAMION COD %s |\n",conteAux.cod, camion.cod);
            }else
            {
                fprintf(stderr,"\n\t | ERROR AL RETIRAR CONTENEDOR Y SACAR CAMION DE LA COLA |\n");
                return ERROR_RETIRAR_CONTE;
            }
        }else
            fprintf(stdout,"\n\t | NO SE ENCONTRO EL CONTENEDOR EN EL TOPE DE LAS ZONAS |\n");
    }

    return TODO_OK;
}

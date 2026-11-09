#include "camion.h"

// Mostrar datos del camion recibido
void mostrarCamion( const void* elem )
{
    tCamion* camion = (tCamion*) elem;
    printf("||-> COD: %s | MIN:%u | COD CONTENEDOR A RETIRAR: %s",
           camion->cod,
           camion->minutoRetiro,
           camion->codContenedorRetirar
           );
}

//Muestra primer camion de la cola
int mostrarPrimeros3Camiones( tCola* camionesEnCola )
{
    tCamion camion;
    int cont=0;
    tCola colaAux;

    crearCola(&colaAux);
    printf("\n\n=== CAMION ===\n\n");
    if( colaVacia(camionesEnCola)  == COLA_VACIA )
    {
        printf("\n\t | NO HAY CAMIONES EN LA COLA ACTUALMENTE |\n");
        return OK;
    }


    //Debemos sacar todos los camiones de cola (ya que sino alteramos el orden)y solo mostrar los 3 primeros
    while(sacarDeCola(camionesEnCola, &camion, sizeof(tCamion))==OK)
    {
        if(cont<3)//Debo mostrarlo
        {
            mostrarCamion( &camion );
            if(cont==0)
                printf(" <-- Primero a atender");
            printf("\n");
        }


        if(ponerEnCola(&colaAux, &camion, sizeof(tCamion))!=OK)
        {
            vaciarCola(&colaAux);
            return COLA_LLENA;
        }
        cont++;
    }
    while(sacarDeCola(&colaAux, &camion, sizeof(tCamion))==OK)
    {
        if(ponerEnCola(camionesEnCola, &camion, sizeof(tCamion))!=OK)
        {
            vaciarCola(&colaAux);
            return COLA_LLENA;
        }
    }
    return OK;
}

//Realiza entrega del primer camion de la cola (en caso de que este el contenedor en algun tope de las zonas)
int realizarEntregaCamion( tCola* camionesEnCola , tLista* plZonasAlmacenamiento , tOperacion *ope)
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
                ope->codParametro_1=codZonaOri+1;
                sscanf((camion.cod)+1, "%u", &ope->codParametro_2);
                strcpy(ope->contenedorInvolucrado, camion.codContenedorRetirar);
                return TODO_OK;
            }else
            {
                fprintf(stderr,"\n\t | ERROR AL RETIRAR CONTENEDOR Y SACAR CAMION DE LA COLA |\n");
                return ERROR_RETIRAR_CONTE;
            }
        }else
        {
            fprintf(stdout,"\n\t | NO SE ENCONTRO EL CONTENEDOR EN EL TOPE DE LAS ZONAS |\n");
            return NO_ENCONTRADO;
        }

    }
    else
        return NO_HAY_CAMION;
}

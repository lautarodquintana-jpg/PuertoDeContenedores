#include "camion.h"

/** Se encolan los camiones que estaban en camino cuyos tiempos de llegada
son menores o iguales al tiempo actual de la jornada.

FALTA TERMINAR
*/
void ponerCamionesArribadosEnCola( tCola* camionesEnCola , tLista* camionesEnCamino , unsigned tiempoActual )
{
    tCamion camion;
    int hayCamionEnLista = sacarPrimerElementoDeLista( camionesEnCamino , &camion , sizeof(tCamion) );

    while( hayCamionEnLista == OK && camion.minutoRetiro <= tiempoActual )
    {
        hayCamionEnLista = sacarPrimerElementoDeLista( camionesEnCamino , &camion , sizeof(tCamion) );
    }

}

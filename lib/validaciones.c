#include "validaciones.h"

char leerYValidarCharMenu()
{
    char opcion;

    printf("\nIngrese una opcion -N:Nueva Jornada -V: Ver Ranking -S:Salir: ");
    scanf(" %c", &opcion);

    opcion=toupper(opcion);
    while(opcion!='N' && opcion!='V' && opcion!='S')
    {
        printf("\nValor ingresado no valido...\n");
        printf("Ingrese una opcion -N:Nueva Jornada -V: Ver Ranking -S:Salir: ");
        scanf(" %c", &opcion);
        opcion=toupper(opcion);
    }

    return opcion;
}

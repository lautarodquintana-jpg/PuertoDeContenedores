#include <stdio.h>
#include <stdlib.h>
#include "lib/configuracion.h"
#include "lib/validaciones.h"

int main()
{
    int ret;
    tConfig config;
    char opcion;

    ret=leerConfigTxt(&config, "config.txt");
    if(ret!=TODO_OK)
        return ret;
    mostrarConfig(&config);

    printf("Bienvenido a Container port simulator!");
    opcion=leerYValidarCharMenu();
    while(opcion!='S')
    {
        switch(opcion)
        {
            case 'N':
                //IniciarSesion (coming soon)
                printf("A trabajar!\n");
                //IniciarJornada (coming soon)
                //ActualizarRanking (coming soon)
                //MostrarRanking (coming soon)
                break;
            case 'V':
                printf("Ranking:\n");
                //MostrarRanking (coming soon)
                break;
        }
        opcion=leerYValidarCharMenu();
    }


}

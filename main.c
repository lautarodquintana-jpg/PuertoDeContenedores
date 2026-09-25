#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/configuracion.h"
#include "lib/validaciones.h"
#include "lib/estructuras.h"
#include "TDA/ListaSimplementeEnlazada.h"
#include "lib/jornada.h"
#include "TDA/cola.h"
#include "TDA/pila.h"

int jugar (tConfig *config, const char *nomUser);
int main()
{
    int ret;
    tConfig config;
    char opcion, nomUser[TAM_USER];

    srand(time(NULL));

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
                ret=jugar(&config, nomUser);
                if(ret!=TODO_OK)
                {
                    return ret;
                }
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

    return 0;
}
int jugar (tConfig *config, const char *nomUser)
{
    tJornada jornada;
    tLista barcosEnCamino, camionesEnCamino;
    int ret;

    ret=inicializarValoresYGenerarPuertoTXT(config, &jornada, nomUser, &barcosEnCamino, &camionesEnCamino);
    if(ret!=TODO_OK)
    {
        vaciarLista(&barcosEnCamino);
        vaciarLista(&camionesEnCamino);
        return ret;
    }

    return TODO_OK;
}

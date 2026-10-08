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
#include "lib/ingreso_operaciones.h"
#include "lib/proceso_operacion.h"
#include "lib/usuarios.h"
#include "lib/muelle.h"
#include "lib/barco.h"

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
                if( mostrarRanking() != TODO_OK )
                    fprintf(stderr,"\nERROR al mostrar ranking.");
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
    tOperacion operacion;
    //tCola historialOperaciones;
    int ret;

    ret=inicializarValoresYGenerarPuertoTXT(config, &jornada, nomUser, &barcosEnCamino, &camionesEnCamino);
    if(ret!=TODO_OK)
    {
        vaciarLista(&barcosEnCamino);
        vaciarLista(&camionesEnCamino);
        return ret;
    }

    getchar(); // Consumo salto de linea
    iniciarSesion( &jornada );

    prepararJornada( &jornada , &camionesEnCamino , &barcosEnCamino , config );

    avanzarTiempoHastaEventoFuturo( &jornada , &camionesEnCamino , &barcosEnCamino );
    procesarLlegadas( &jornada , &camionesEnCamino , &barcosEnCamino );

    while( jornada.estado == JUEGO_EN_CURSO )
    {

        while( solicitarOperacionSTDIN( &operacion , config ) != INGRESO_CAD_EXIT )
        {
            procesarOperacion( &jornada , &operacion , config );
            procesarLlegadas( &jornada , &camionesEnCamino , &barcosEnCamino );
        }

        jornada.estado = FIN_JUEGO;
    }

    return TODO_OK;
}

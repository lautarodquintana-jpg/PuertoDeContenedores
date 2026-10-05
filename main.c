#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
#endif
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
#include "lib/zona_almacenamiento.h"

void mostrar(const void* elem)
{
    tBarco* barco = (tBarco*)elem;
    printf(" COD: %s | Tiempo arribo: %u | Cant conte. to: %u | Cant conte. res: %u\n",barco->cod,barco->arriboProgramado,barco->cantidadContenedoresTotal,barco->cantidadContenedoresRestante);
}
void iniciar_consola(void) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
}
int jugar (tConfig *config, const char *nomUser);
int main()
{
    iniciar_consola(); //Para poder mostrar los caracteres de los "mostrarMuelles()" y "mostrarZonasAlmacenamiento()"
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

    if(ret!=TODO_OK || inicializarMuellesYZonas(&jornada, config)!=TODO_OK)
    {
        vaciarLista(&barcosEnCamino);
        vaciarLista(&camionesEnCamino);
        return ret;
    }

    getchar(); // Consumo salto de linea
    iniciarSesion( &jornada );

    jornada.estado = JUEGO_EN_CURSO;
    while( jornada.estado == JUEGO_EN_CURSO )
    {
        avanzarTiempoHastaEventoFuturo( &jornada , &camionesEnCamino , &barcosEnCamino );
        //actualizarListasMuellesYCamiones();  //Asignar a lista
        //actualizaryMostrarInterfaz();
        if( solicitarOperacionSTDIN( &operacion , config, &jornada ) != INGRESO_CAD_EXIT )
        {
            /*if (validarOperacion() == OK){
                //encolarOperacionEnHistorial();
                switch(obtenerNROOperacion(&operacion)){
                    case OPERACION_DES:{}break;
                    case OPERACION_REU:{
                        reubicarContenedores(&jornada, config->capPila, operacion->codParametro_1, operacion->codParametro_2);
                    }break;
                    case OPERACION_ENT:{}break;
                    case OPERACION_VER:{}break;
                    case OPERACION_ESP:{}break;
                }
            }*/
            //jornada.tiempoActual-=consultarTiempoRequeridoOperacion(operacion.cod,&config);
        }else
            jornada.estado = FIN_JUEGO;
    }

    return TODO_OK;
}

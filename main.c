#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "lib/colores.h"
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
#include "lib/recuperacion.h"
#include "lib/ranking.h"

int jugar (tConfig *config, const char *nomUser);
int main()
{
    /*
    HACER:
    iniciar_consola(); //Para poder mostrar los caracteres de los "mostrarMuelles()" y "mostrarZonasAlmacenamiento()"
    */

    int ret;
    tConfig config;
    char opcion, nomUser[TAM_USER];

    srand(time(NULL));

    ret=leerConfigTxt(&config, "config.txt");
    if(ret!=TODO_OK)
        return ret;
    //mostrarConfig(&config);

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
            //IniciarJornada (coming soon)
            //ActualizarRanking (coming soon)
            //MostrarRanking (coming soon)
            break;
        case 'V':
            if( mostrarRanking() != TODO_OK )
                fprintf(stderr,"\n\t | ERROR AL MOSTRAR RANKING.\n");
            break;
        }
        opcion=leerYValidarCharMenu();
    }
    return 0;
}
int jugar (tConfig *config, const char *nomUser)
{
    FILE* pf_logActual;
    tJornada jornada;
    tLista barcosEnCamino, camionesEnCamino;
    tCola colaOperaciones;
    tOperacion operacion;
    tArbol indiceUsuarios;
    tRegistroJornada regJornada;
    tRecuperacion recuperacion = {NULL, 0};
    int ret, largoMaxCodContenedor=0;
    unsigned estadoAlEntrarCiclo = 0;
    crearLista(&barcosEnCamino);
    crearLista(&camionesEnCamino);

    getchar(); // Consumo salto de linea

    cargarIndiceUsuarios(&indiceUsuarios);

    ret=generarPuertoTXT(config, &jornada, nomUser, &largoMaxCodContenedor);

    if(ret!=TODO_OK || cargarPuertoTxt(&jornada, config, &barcosEnCamino, &camionesEnCamino)!=TODO_OK)
    {
        vaciarLista(&barcosEnCamino);
        vaciarLista(&camionesEnCamino);
        return ret;
    }
    getchar(); // Consumo salto de linea

    iniciarSesion( &jornada  , &indiceUsuarios);
    system("cls");//Borro la consola

    if(ER_NO_RECUPERA == consultarArchivosDeRecuperacionYCarga(&jornada)){
        ret=generarPuertoTXT(config, &jornada, nomUser, &largoMaxCodContenedor);
    }else if(ER_TODO_OK != (ret = prepararEstructuraRecuperacion(&jornada, config, &recuperacion)))
        return ret;
    largoMaxCodContenedor = (0 == largoMaxCodContenedor)?TAM_DEF_CODIGO_CONTENEDOR:largoMaxCodContenedor;
    if(NULL == (pf_logActual = abrirArchivoMovimientosTemporales( jornada.user, "wb", 0 ))){
        cerrarArchivo(&recuperacion.pf_logMovimientos);
        return 0;
    }

    if(ret!=TODO_OK || cargarPuertoTxt(&jornada, config, &barcosEnCamino, &camionesEnCamino)!=TODO_OK){
        vaciarLista(&barcosEnCamino);
        vaciarLista(&camionesEnCamino);
        cerrarArchivo(&recuperacion.pf_logMovimientos);
        cerrarArchivo(&pf_logActual);
        return ret;
    }

    if(ER_TODO_OK != (ret = guardarCopiaConfigYPuerto(jornada.user))){
        printf("\n\t | ERROR AL CREAR COPIAS DE AUTOGUARDADO | \n");
        vaciarLista(&barcosEnCamino);
        vaciarLista(&camionesEnCamino);
        cerrarArchivo(&recuperacion.pf_logMovimientos);
        cerrarArchivo(&pf_logActual);
        return ret;
    }

    ret=prepararJornada( &jornada, &camionesEnCamino, &barcosEnCamino, config , largoMaxCodContenedor);
    if(ret!=TODO_OK){
        printf("\n\t | ERROR AL GENERAR JORNADA\n");//No es necesario liberar acá adentro, ya que libera todo dentro de la funci�n
        cerrarArchivo(&recuperacion.pf_logMovimientos);
        cerrarArchivo(&pf_logActual);
        return ret;
    }

    crearCola(&colaOperaciones);
    avanzarTiempoHastaEventoFuturo( &jornada, &camionesEnCamino, &barcosEnCamino );
    procesarLlegadas( &jornada, &camionesEnCamino, &barcosEnCamino );
    jornada.estado = recuperacion.registrosValidos>0?JUEGO_EN_RECUPERACION:JUEGO_EN_CURSO;
    while( jornada.estado == JUEGO_EN_CURSO || jornada.estado == JUEGO_EN_RECUPERACION)
    {
        estadoAlEntrarCiclo = jornada.estado;
        while( !existeOperacionValida( config, &jornada ) && avanzarTiempoHastaEventoFuturo(&jornada, &camionesEnCamino, &barcosEnCamino) == TIEMPO_AVANZADO )
            procesarLlegadas(&jornada, &camionesEnCamino, &barcosEnCamino);

        if( existeOperacionValida( config, &jornada ) == NO_EXISTE_OPE_VALIDA )
        {
            jornada.estado = FIN_JUEGO;
            cerrarArchivo(&pf_logActual);
            eliminarArchivosTemporales(jornada.user);
        } // Primero verificamos que haya tiempo
        else if(JUEGO_EN_CURSO == jornada.estado && INGRESO_CAD_EXIT == solicitarOperacionSTDIN(&operacion, config, &jornada)){
            jornada.estado = FIN_JUEGO;
        }
        else if(JUEGO_EN_RECUPERACION == jornada.estado && ER_NE_REGISTROS_REC == leerOperacionDelArchivoTemporal(&operacion, &jornada, &recuperacion)){
            jornada.estado = JUEGO_EN_CURSO; //RECUPERACION_TERMINADA
        }
        else if(JUEGO_EN_CURSO == jornada.estado || JUEGO_EN_RECUPERACION == jornada.estado)
        {
            ret=procesarOperacion( &jornada, &operacion, config );
            if(ret==TODO_OK){
                if(strcmp(operacion.cod, "VER")!=0){//No guardamos la operación VER
                    if(ponerEnCola(&colaOperaciones, &operacion, sizeof(tOperacion))!=OK)
                    {
                        vaciarCola(&colaOperaciones);
                        liberarJornada(&jornada, &camionesEnCamino, &barcosEnCamino);
                        cerrarArchivo(&recuperacion.pf_logMovimientos);
                        cerrarArchivo(&pf_logActual);
                        return COLA_LLENA;
                    }
                }
                guardarOperacion( pf_logActual, operacion );
            }
            procesarLlegadas( &jornada, &camionesEnCamino, &barcosEnCamino );
        }
        if( JUEGO_EN_RECUPERACION != estadoAlEntrarCiclo || JUEGO_EN_RECUPERACION != jornada.estado )
            system("pause");
        system("cls");
    }

    registrarJornada( &indiceUsuarios, &regJornada );
    mostrarResumenJornada( jornada.tiempoActual, config->duracionMinutosdeJornada, jornada.cantOperaciones.cantDescargas, jornada.cantOperaciones.cantReubic, jornada.cantOperaciones.cantEntregas, jornada.puntajeActual, jornada.user , &jornada.camiones);
    mostrarHistorialOperaciones(&colaOperaciones, config);
    cerrarArchivo(&recuperacion.pf_logMovimientos);
    cerrarArchivo(&pf_logActual);
    //liberarJornada(&jornada, &camionesEnCamino, &barcosEnCamino);
    return TODO_OK;
}

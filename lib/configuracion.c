#include "configuracion.h"


int leerConfigTxt(tConfig *config, const char *nomArch)
{
    int i;
    FILE *archConfig;
    char linea[MAX_LINEA_CONFIG], *aux;

    archConfig=fopen(nomArch, "rt");
    if(!archConfig)
        return ERROR_ARCHIVO;

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->duracionMinutosdeJornada);



    for(i=0 ; i<2 ; i++)//Cargamos el vector de cantidades
    {
        fgets(linea, MAX_LINEA_CONFIG, archConfig);
        aux=strchr(linea, ':');
        if(!aux)
        {
            fclose(archConfig);
            return ERROR_LECTURA_ARCHIVO;
        }
        sscanf(aux+1, "%u", &config->cants[i]);
    }


    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->capPila);


    for(i=0; i<3 ; i++)//Cargamos el vector de valores maximos
    {
        fgets(linea, MAX_LINEA_CONFIG, archConfig);
        aux=strchr(linea, ':');
        if(!aux)
        {
            fclose(archConfig);
            return ERROR_LECTURA_ARCHIVO;
        }
        sscanf(aux+1, "%u", &config->maximos[i]);
    }

    for(i=0; i<3 ; i++)//Cargamos el vector de tiempos
    {
        fgets(linea, MAX_LINEA_CONFIG, archConfig);
        aux=strchr(linea, ':');
        if(!aux)
        {
            fclose(archConfig);
            return ERROR_LECTURA_ARCHIVO;
        }
        sscanf(aux+1, "%u", &config->tiempos[i]);
    }

    fclose(archConfig);
    return TODO_OK;
}
void mostrarConfig(tConfig *config)
{
    printf("Duracion de la jornada: %u\n", config->duracionMinutosdeJornada);
    printf("Cantidad de muelles: %u\n", config->cants[POS_CANT_MUELLES]);
    printf("Cantidad de zonas de almacenamiento: %u\n", config->cants[POS_CANT_ZONAS_ALMACENAMIENTO]);
    printf("Capacidad de pilas: %u\n", config->capPila);
    printf("Cantidad maxima de buques: %u\n", config->maximos[POS_MAX_BUQUES]);
    printf("Cantidad maxima de contenedores por buques: %u\n", config->maximos[POS_MAX_CONTENEDORES_POR_BUQUE]);
    printf("Cantidad maxima de camiones: %u\n", config->maximos[POS_MAX_CAMIONES]);

    printf("Tiempo de descarga de contenedor: %u\n", config->tiempos[POS_TIEMPO_DESCARGA_DE_CONTENEDOR]);
    printf("Tiempo de reubicacion de contenedor: %u\n", config->tiempos[POS_TIEMPO_REUBICACION_CONTENEDOR]);
    printf("Tiempo de carga de camion: %u\n", config->tiempos[POS_TIEMPO_CARGA_CAMION]);
}

#include "configuracion.h"


int leerConfigTxt(tConfig *config, const char *nomArch)
{
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

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->cantidadMuelles);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->cantidadZonasDeAlmacenamiento);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->capPila);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->maximosBuques);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->maximosContenedoresPorBuque);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->maximosCamiones);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->tiempoDeDescargaDeContenedor);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->tiempoDeReubicacionDeContenedor);

    fgets(linea, MAX_LINEA_CONFIG, archConfig);
    aux=strchr(linea, ':');
    if(!aux)
    {
        fclose(archConfig);
        return ERROR_LECTURA_ARCHIVO;
    }
    sscanf(aux+1, "%u", &config->tiempoDeCargaDeCamion);



    fclose(archConfig);
    return TODO_OK;
}
void mostrarConfig(tConfig *config)
{
    printf("Duracion de la jornada: %u\n", config->duracionMinutosdeJornada);
    printf("Cantidad de muelles: %u\n", config->cantidadMuelles);
    printf("Cantidad de zonas de almacenamiento: %u\n", config->cantidadZonasDeAlmacenamiento);
    printf("Capacidad de pilas: %u\n", config->capPila);
    printf("Cantidad maxima de buques: %u\n", config->maximosBuques);
    printf("Cantidad maxima de contenedores por buque: %u\n", config->maximosContenedoresPorBuque);
    printf("Cantidad maxima de camiones: %u\n", config->maximosCamiones);

    printf("Tiempo de descarga de contenedor: %u\n", config->tiempoDeDescargaDeContenedor);
    printf("Tiempo de reubicacion de contenedor: %u\n", config->tiempoDeReubicacionDeContenedor);
    printf("Tiempo de carga de camion: %u\n", config->tiempoDeCargaDeCamion);
}

#include "jornada.h"

int inicializarValoresYGenerarPuertoTXT(tConfig *config, tJornada *jornada, const char *nomUser, tLista *listaBarcosEnCamino, tLista *listaCamionesEnCamino)
{
    FILE *archPuerto;
    unsigned contadorBarcos, contadorCamiones;
    tBarco barco;
    tCamion camion;
    tContenedor contenedor;
    tCola colaAuxContenedores;
    int i;

    archPuerto=fopen("puerto.txt", "wt");
    if(!archPuerto)
        return ERROR_ARCHIVO;

    crearLista(listaBarcosEnCamino);
    crearLista(listaCamionesEnCamino);
    crearCola(&colaAuxContenedores);

    fprintf(archPuerto, "JORNADA:%u\n", config->duracionMinutosdeJornada);
    fprintf(archPuerto, "MUELLES:%u\n", config->cantidadMuelles);
    fprintf(archPuerto, "ZONAS DE ALMACENAMIENTO:%u\n", config->cantidadZonasDeAlmacenamiento);
    fprintf(archPuerto, "CAPACIDAD_PILA:%u", config->capPila);

    fprintf(archPuerto, "\n\n[BUQUES]\n");

    //A partir de acá viene la lógica para cargar aleatoriamente los buques con sus respectivos contenedores y tiempos

    contadorBarcos=1;
    while(contadorBarcos<=config->maximosBuques)//En cada iteracion cargamos un barco
    {
        barco.arriboProgramado=rand()%config->duracionMinutosdeJornada; //Cargamos el minuto en el que arribará el barco

        sprintf(barco.cod, "B%u", contadorBarcos); //Cargamos el código del barco
        crearCola(&(barco.contenedores));
        barco.cantidadContenedoresTotal=rand()%config->maximosContenedoresPorBuque+1;


        barco.cantidadContenedoresRestante=0;
        while(barco.cantidadContenedoresRestante<barco.cantidadContenedoresTotal)//En cada iteracion cargamos un contenedor
        {
            sprintf(contenedor.cod, "C%u", 100*contadorBarcos+barco.cantidadContenedoresRestante);//Cargamos el contenedor en el buque
            if(ponerEnCola(&barco.contenedores, &contenedor, sizeof(tContenedor))!=OK)
            {
                fclose(archPuerto);
                vaciarCola(&barco.contenedores);
                vaciarLista(listaBarcosEnCamino);
                return COLA_LLENA;
            }
            barco.cantidadContenedoresRestante++;
        }
        if(insertarOrdenadoenLista(listaBarcosEnCamino, &barco, sizeof(tBarco), compararTiempos, 1, NULL)!=TODO_OK)
        {
            fclose(archPuerto);
            vaciarCola(&barco.contenedores);
            vaciarLista(listaBarcosEnCamino);
            return SINMEM;
        }

        contadorBarcos++;
    }

    i=0;
    contadorCamiones=0;
    while(verNElem(listaBarcosEnCamino, i, &barco, sizeof(tBarco))==TODO_OK) //Ya tenemos cargados los buques ordenados por tiempo, escribimos puerto.txt
    {
        fprintf(archPuerto, "%s;T=%u;C=", barco.cod, barco.arriboProgramado);
        while(sacarDeCola(&barco.contenedores, &contenedor, sizeof(tContenedor))==OK)
        {
            fprintf(archPuerto, "%s", contenedor.cod);
            if(ponerEnCola(&colaAuxContenedores, &contenedor, sizeof(tContenedor))==COLA_LLENA)
            {
                fclose(archPuerto);
                vaciarCola(&barco.contenedores);
                vaciarLista(listaBarcosEnCamino);
                vaciarCola(&barco.contenedores);
                vaciarCola(&colaAuxContenedores);
                return COLA_LLENA;
            }
            if(colaVacia(&barco.contenedores)!=COLA_VACIA)
                fprintf(archPuerto, "%c", ','); //Debo escribir mas contenedores
            else
                fprintf(archPuerto, "%c", '\n'); //No hay mas contenedores
        }

        while(sacarDeCola(&colaAuxContenedores, &contenedor, sizeof(tContenedor))==OK) //Vuelvo a poner los contenedores en su lugar sin alterar la cola
        {
            if(contadorCamiones<config->maximosCamiones)
            {
                camion.minutoRetiro=rand()%(config->duracionMinutosdeJornada-barco.arriboProgramado)+barco.arriboProgramado;
                //Debo garantizar que llegue en el mismo minuto o luego de que llega el buque
                sprintf(camion.cod, "K%u", contadorCamiones);
                strcpy(camion.codContenedorRetirar, contenedor.cod);

                if(insertarOrdenadoenLista(listaCamionesEnCamino, &camion, sizeof(tCamion), compararTiemposDeCamiones, 1, NULL)!=TODO_OK)
                {
                    fclose(archPuerto);
                    vaciarCola(&barco.contenedores);
                    vaciarLista(listaBarcosEnCamino);
                    vaciarCola(&barco.contenedores);
                    vaciarCola(&colaAuxContenedores);
                    return COLA_LLENA;
                }
                contadorCamiones++;
            }
            if(ponerEnCola(&barco.contenedores, &contenedor, sizeof(tContenedor))!=OK)
            {
                fclose(archPuerto);
                vaciarCola(&barco.contenedores);
                vaciarLista(listaBarcosEnCamino);
                vaciarCola(&barco.contenedores);
                vaciarCola(&colaAuxContenedores);
                return COLA_LLENA;
            }
        }
        if(actualizarNElemDeLista(listaBarcosEnCamino, &barco, i, actualizarBarco)!=TODO_OK)
        {
                fclose(archPuerto);
                vaciarCola(&barco.contenedores);
                vaciarLista(listaBarcosEnCamino);
                vaciarCola(&barco.contenedores);
                vaciarCola(&colaAuxContenedores);
                return NO_ENCONTRADO;
        }
        //debo actualizar el elemento
        i++;
    }

    //Ahora debemos escribir los camiones en puerto.txt

    fprintf(archPuerto, "\n[CAMIONES]\n");
    i=0;
    while(verNElem(listaCamionesEnCamino, i, &camion, sizeof(tCamion))==TODO_OK)
    {
        fprintf(archPuerto, "%s;T=%u;C=%s\n", camion.cod, camion.minutoRetiro, camion.codContenedorRetirar);
        i++;
    }

    fclose(archPuerto);
    return TODO_OK;
}
int compararTiempos(const void *elem1, const void *elem2)
{
    const tBarco *barco1= elem1, *barco2=elem2;

    return barco1->arriboProgramado-barco2->arriboProgramado;
}
int compararContenedores(const void *elem1, const void *elem2)
{
    const unsigned *contenedor1=elem1, *contenedor2=elem2;

    return (*contenedor1)-(*contenedor2);
}
int compararTiemposDeCamiones(const void *elem1, const void *elem2)
{
    const tCamion *camion1=elem1, *camion2=elem2;

    return camion1->minutoRetiro-camion2->minutoRetiro;
}
void actualizarBarco(void *actualizado, const void *actualizador)
{
    tBarco *barcoActualizado=actualizado;
    const tBarco *barcoActualizador=actualizador;

    //Este barco estaba insertado en la lista: si bien los punteros de la cola siguen apuntando a nodos, estos ya fueron liberados al desencolarlos previamente
    //Por ende no debemos vaciar la cola, solo reemplazarla por la nueva
    barcoActualizado->contenedores=barcoActualizador->contenedores;
}

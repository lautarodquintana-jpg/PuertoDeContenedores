#include "jornada.h"

int generarPuertoTXT(tConfig *config, tJornada *jornada, const char *nomUser, int *largoMaxCodContenedor)
{
    FILE *archPuerto;
    unsigned contadorBarcos, contadorCamiones;
    tBarco barco;
    tCamion camion;
    tContenedor contenedor;
    tCola colaAuxContenedores;
    tLista listaBarcosEnCamino, listaCamionesEnCamino;

    int i;

    archPuerto=fopen("puerto.txt", "wt");
    if(!archPuerto)
        return ERROR_ARCHIVO;

    crearLista(&listaBarcosEnCamino);
    crearLista(&listaCamionesEnCamino);
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
            if(strlen(contenedor.cod)>(*largoMaxCodContenedor)) //Calculamos la mayor cantidad de caracteres que puede tener un contenedor}
                *largoMaxCodContenedor=strlen(contenedor.cod);

            if(ponerEnCola(&barco.contenedores, &contenedor, sizeof(tContenedor))!=OK)
            {
                fclose(archPuerto);
                vaciarCola(&barco.contenedores);
                liberarListaDeBarcos(&listaBarcosEnCamino);
                return COLA_LLENA;
            }
            barco.cantidadContenedoresRestante++;
        }
        if(insertarOrdenadoenLista(&listaBarcosEnCamino, &barco, sizeof(tBarco), compararTiempos, 1, NULL)!=TODO_OK)
        {
            fclose(archPuerto);
            vaciarCola(&barco.contenedores);
            liberarListaDeBarcos(&listaBarcosEnCamino);
            return SINMEM;
        }

        contadorBarcos++;
    }

    i=0;
    contadorCamiones=0;
    while(verNElem(&listaBarcosEnCamino, i, &barco, sizeof(tBarco))==TODO_OK) //Ya tenemos cargados los buques ordenados por tiempo, escribimos puerto.txt
    {
        fprintf(archPuerto, "%s;T=%u;C=", barco.cod, barco.arriboProgramado);
        while(sacarDeCola(&barco.contenedores, &contenedor, sizeof(tContenedor))==OK)
        {
            fprintf(archPuerto, "%s", contenedor.cod);
            if(ponerEnCola(&colaAuxContenedores, &contenedor, sizeof(tContenedor))==COLA_LLENA)
            {
                fclose(archPuerto);
                vaciarCola(&barco.contenedores);
                liberarListaDeBarcos(&listaBarcosEnCamino);
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

                if(insertarOrdenadoenLista(&listaCamionesEnCamino, &camion, sizeof(tCamion), compararTiemposDeCamiones, 1, NULL)!=TODO_OK)
                {
                    fclose(archPuerto);
                    vaciarCola(&barco.contenedores);
                    liberarListaDeBarcos(&listaBarcosEnCamino);
                    vaciarCola(&colaAuxContenedores);
                    return SINMEM;
                }
                contadorCamiones++;
            }
            if(ponerEnCola(&barco.contenedores, &contenedor, sizeof(tContenedor))!=OK)
            {
                fclose(archPuerto);
                vaciarCola(&barco.contenedores);
                liberarListaDeBarcos(&listaBarcosEnCamino);
                vaciarCola(&colaAuxContenedores);
                return COLA_LLENA;
            }
        }
        if(actualizarNElemDeLista(&listaBarcosEnCamino, &barco, i, actualizarBarco)!=TODO_OK)
        {
            fclose(archPuerto);
            vaciarCola(&barco.contenedores);
            liberarListaDeBarcos(&listaBarcosEnCamino);
            vaciarCola(&colaAuxContenedores);
            return NO_ENCONTRADO;
        }
        //debo actualizar el elemento
        i++;
    }

    //Ahora debemos escribir los camiones en puerto.txt

    fprintf(archPuerto, "\n[CAMIONES]\n");
    i=0;
    while(verNElem(&listaCamionesEnCamino, i, &camion, sizeof(tCamion))==TODO_OK)
    {
        fprintf(archPuerto, "%s;T=%u;C=%s\n", camion.cod, camion.minutoRetiro, camion.codContenedorRetirar);
        i++;
    }

    liberarListaDeBarcos(&listaBarcosEnCamino);
    vaciarLista(&listaCamionesEnCamino);
    fclose(archPuerto);
    return TODO_OK;
}
int cargarPuertoTxt(tJornada *jornada, tConfig *config, tLista *listaBarcosEnCamino, tLista *listaCamionesEnCamino)
{
    int todosLosBuquesProcesados=0;
    FILE *archPuerto=fopen("puerto.txt", "rt");
    tBarco barco;
    tCamion camion;
    tContenedor contenedor;
    char *aux, *auxParaCalculo, linea[TAM_LINEAMAX_PUERTOTXT], *retFgets;


    if(!archPuerto)
        return ERROR_ARCHIVO;

    crearLista(listaBarcosEnCamino);
    crearLista(listaCamionesEnCamino);
    //Que hace esta funcion? Inicializa zonaAlmacenamiento, muelles y carga en lista lo que hay en puerto txt
    //Primeras lineas: ya estan cargadas en jornada, pero estaría bueno cargarlas de nuevo....

    fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);//La cantidad minutos ya está cargada en config
    fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);//La cantidad de muelles ya está cargada en config
    fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);//La cantidad de zonas ya está cargada en config
    fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);//La capacidad de pila ya está cargada en config
    fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);//Consumo el renglón vacío
    fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);//Consumo la linea [BUQUES]

    //A partir de acá cargo los buques mientras el largo de linea sea != de cero. Si es cero, pasamos a camiones
    retFgets=fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);
    while(retFgets && todosLosBuquesProcesados==0)
    {
        aux=strchr(linea, '\n');
        if(aux)
            *aux='\0';
        if(strlen(linea)==0)//Procesamos todos los buques
            todosLosBuquesProcesados=1;
        else
        {
            crearCola(&barco.contenedores);
            barco.cantidadContenedoresTotal=0;
            aux=strrchr(linea, ';');//Buscamos la ultima aparicion  del ';', luego le sigue C
            *aux='\0';
            aux=aux+3;//Omitmimos ";C="
            while(aux)
            {
                barco.cantidadContenedoresTotal++;

                auxParaCalculo=strchr(aux, ',');//Busco la siguiente ',' para calcular la cantidad de caracteres a copiar
                if(auxParaCalculo)
                {
                    strncpy(contenedor.cod, aux, auxParaCalculo-aux);
                    *(contenedor.cod+(auxParaCalculo-aux)+1)='\0';
                    //Al hacer la resta con punteros, se la cantidad de caracteres que le pertenecen a este contenedor
                    aux=auxParaCalculo+1;//Me posiciono en el siguiente contenedor
                }
                else//Es el ultimo contenedor
                {
                    strcpy(contenedor.cod, aux);
                    aux=NULL;
                }
                if(ponerEnCola(&barco.contenedores, &contenedor, sizeof(tContenedor))!=OK)
                {
                    liberarListaDeBarcos(listaBarcosEnCamino);
                    vaciarCola(&barco.contenedores);
                    fclose(archPuerto);
                    return COLA_LLENA;
                }
            }
            barco.cantidadContenedoresRestante=barco.cantidadContenedoresTotal;
            //Ya tenemos cargados los contenedores del barco

            aux=strchr(linea, '=');
            sscanf(aux+1, "%u", &barco.arriboProgramado);

            aux=strchr(linea, ';');
            *aux='\0';
            strcpy(barco.cod, linea);
        }
        if(todosLosBuquesProcesados==0)
        {
            if(insertarAlFinalDeLista(listaBarcosEnCamino, &barco, sizeof(tBarco))!=TODO_OK)
            {
                liberarListaDeBarcos(listaBarcosEnCamino);
                vaciarCola(&barco.contenedores);
                fclose(archPuerto);
                return SINMEM;
            }
        }

        retFgets=fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto);
        //Aca puede ocurrir que:-Obtengamos la siguiente linea de otro barco
        //                      -Consumamos el renglón vacío que precede a la línea de camiones y luego el de camiones
    }

    //Como ya tenemos consumido el renglón [CAMIONES], leemos normalmente
    while(fgets(linea, TAM_LINEAMAX_PUERTOTXT, archPuerto))
    {
        aux=strrchr(linea, '\n');
        if(aux)
            *aux='\0';
        if(strlen(linea)!=0)//Si no es la última línea que no posee nada
        {
            aux=strrchr(linea, 'C');//Busca la última C
            strcpy(camion.codContenedorRetirar, aux);//Cargamos el contenedor

            aux=strrchr(linea, ';');
            *aux='\0';

            aux=strchr(linea, '=');
            sscanf(aux+1, "%u", &camion.minutoRetiro);//Cargamos el minuto de retiro

            aux=strrchr(linea, ';');
            *aux='\0';

            strcpy(camion.cod, linea);
            if(insertarAlFinalDeLista(listaCamionesEnCamino, &camion, sizeof(tCamion))!=TODO_OK)
            {
                liberarListaDeBarcos(listaBarcosEnCamino);
                vaciarLista(listaCamionesEnCamino);
                fclose(archPuerto);
                return SINMEM;
            }
        }

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

int avanzarTiempoHastaEventoFuturo( tJornada* jornada, const tLista* camionesEnCamino, const tLista* barcosEnCamino )
{
    tCamion camion;
    tBarco barco;
    unsigned tiempoPrimerEvento = 0, existeEvento = 0;

    if( verNElem( (tLista*)camionesEnCamino, 0, &camion, sizeof(tCamion) ) == TODO_OK )
    {
        tiempoPrimerEvento = camion.minutoRetiro;
        existeEvento = 1;
    }

    if( verNElem( (tLista*)barcosEnCamino, 0, &barco, sizeof(tBarco) ) == TODO_OK)
    {
        if( existeEvento == 0 || barco.arriboProgramado < tiempoPrimerEvento )
        tiempoPrimerEvento = barco.arriboProgramado;
    }

    if( existeEvento != 0 )
    {
        jornada->tiempoActual = barco.arriboProgramado;
        return TIEMPO_AVANZADO;
    }
    return TIEMPO_NO_AVANZADO;
}

int procesarTiempo( tCola* camionesEnCola, tCola* barcosEnCola, tLista* camionesEnCamino, tLista* barcosEnCamino, unsigned tiempoActual )
{
    tCamion camion;
    tBarco barco;
    int res;

    while( verNElem( camionesEnCamino, 0, &camion, sizeof(tCamion) ) == TODO_OK && camion.minutoRetiro <= tiempoActual )
    {
        res = sacarPrimerElementoDeLista( camionesEnCamino, &camion, sizeof(tCamion) );
        if( res != TODO_OK )
            return res;

        res = ponerEnCola( camionesEnCola, &camion, sizeof(tCamion) );
        if( res != TODO_OK )
            return res;
    }

    res = verNElem( barcosEnCamino, 0, &barco, sizeof(tBarco) );
    while( res == TODO_OK && barco.arriboProgramado <= tiempoActual )
    {
        res = sacarPrimerElementoDeLista( barcosEnCamino, &barco, sizeof(tBarco) );
        if( res != TODO_OK )
            return res;

        res = ponerEnCola( barcosEnCola, &barco, sizeof(tBarco) );
        if( res != TODO_OK )
            return res;

        res = verNElem( barcosEnCamino, 0, &barco, sizeof(tBarco) );
    }

    return TODO_OK;
}

int prepararJornada( tJornada* jornada, tLista* camionesEnCamino, tLista* barcosEnCamino, const tConfig* config, int largoMaxCodContenedor)
{
    int ret = TODO_OK;

    jornada->estado = JUEGO_EN_CURSO;
    jornada->puntajeActual = 0;
    jornada->cantOperaciones.cantDescargas = 0;
    jornada->cantOperaciones.cantEntregas = 0;
    jornada->cantOperaciones.cantReubic = 0;

    crearCola( &jornada->camiones );
    crearCola( &jornada->barcosEspera );
    crearListaMuelles( &jornada->muelles );
    crearListaZonas( &jornada->zonasAlmacenamiento );

    ret = generarMuelles( &jornada->muelles, config->cantidadMuelles );
    if( ret != COD_MUELLES_OK )
    {
        fprintf(stderr,"ERROR al generar muelles.");
        liberarJornada(jornada,camionesEnCamino,barcosEnCamino);
        return ret;
    }

    ret = generarZonas( &jornada->zonasAlmacenamiento, config->cantidadZonasDeAlmacenamiento,largoMaxCodContenedor, config->capPila );
    if( ret != COD_ZONAS_OK )
    {
        fprintf(stderr,"ERROR al generar zonas.");
        liberarJornada(jornada,camionesEnCamino,barcosEnCamino);
        return ret;
    }

    return ret;
}

void liberarJornada( tJornada* jornada, tLista* camionesEnCamino, tLista* barcosEnCamino )
{
    vaciarLista(camionesEnCamino);
    liberarListaDeBarcos(barcosEnCamino);
    vaciarCola(&jornada->camiones);
    liberarZonasDeAlmacenamiento(&jornada->zonasAlmacenamiento);
    eliminarMuelles(&jornada->muelles);
    vaciarCola(&jornada->movimientos);
}

void procesarLlegadas( tJornada* jornada, tLista* camionesEnCamino, tLista* barcosEnCamino )
{
    procesarTiempo( &jornada->camiones, &jornada->barcosEspera, camionesEnCamino, barcosEnCamino, jornada->tiempoActual );
    ponerBarcosArribadosEnMuelles( &jornada->muelles, &jornada->barcosEspera, jornada->tiempoActual );
}

void mostrarResumenJornada( unsigned tiempoJornada, unsigned tiempoDispo, unsigned cantDesc, unsigned cantReubi, unsigned cantEntre, unsigned puntuacionFinal, const char* nomOpera , tCola *camionesPendientes)
{
    int cantCamionesPend=0;

    while(sacarDeCola(camionesPendientes, NULL, 0)==OK)//Contamos los camiones que quedaron en la cola
    {
        cantCamionesPend++;
    }

    printf("\n\tRESUMEN DE LA JORNADA\n");
    printf("\n\tOPERADOR: %s\n", nomOpera);
    printf("TIEMPO DE LA JORNADA: %u de %u MINUTOS\n", tiempoJornada, tiempoDispo);
    printf("BUQUES DESCARGADOS EN SU TOTALIDAD: %u -> %u PUNTOS\n", cantDesc, cantDesc * PUNTOS_DESCA_TOTAL);
    printf("CONTENEDORES ENTREGADOS: %u -> %u PUNTOS\n", cantEntre, cantEntre * PUNTOS_ENTREGA);
    printf("REUBICACIONES REALIZADAS: %u\n", cantReubi);
    printf("CAMIONES QUE QUEDARON PENDIENTES: %u -> %u PUNTOS\n", cantCamionesPend, cantCamionesPend * PUNTOS_PENALIZACION);
    printf("PUNTUACION FINAL: %u PUNTOS\n", puntuacionFinal);
}
void mostrarHistorialOperaciones(tCola *colaOperaciones, tConfig *config)
{
    tOperacion operacion;

    printf("%-11s\n%-5s %-5s\n", "  Minuto  ", "Ini", "Fin");
    while(sacarDeCola(colaOperaciones, &operacion, sizeof(tOperacion))==OK)
    {
        if(strcmp(operacion.cod, "DES")==0)
        {
            printf("%-5u %-5u Descarga del contenedor %s del muelle %u sobre la zona %u\n",
            operacion.minutoEjecutado,
            operacion.minutoEjecutado+config->tiempoDeDescargaDeContenedor,
            operacion.contenedorInvolucrado,
            operacion.codParametro_1,
            operacion.codParametro_2);
        }
        if(strcmp(operacion.cod, "REU")==0)
        {
            printf("%-5u %-5u Reubicacion del contenedor %s de zona %u a zona %u\n",
            operacion.minutoEjecutado,
            operacion.minutoEjecutado+config->tiempoDeReubicacionDeContenedor,
            operacion.contenedorInvolucrado,
            operacion.codParametro_1,
            operacion.codParametro_2);
        }
        if(strcmp(operacion.cod, "ENT")==0)
        {
            printf("%-5u %-5u Se entrega de la zona %d, el contenedor %s al camion K%u\n",
            operacion.minutoEjecutado,
            operacion.minutoEjecutado+config->tiempoDeCargaDeCamion,
            operacion.codParametro_1,
            operacion.contenedorInvolucrado,
            operacion.codParametro_2);
        }
        if(strcmp(operacion.cod, "ESP")==0)
        {
            printf("%-5u %-5u Esperar\n", operacion.minutoEjecutado, operacion.minutoEjecutado+1);
        }
    }


}

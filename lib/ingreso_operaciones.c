#include "ingreso_operaciones.h"

void aMayusculas(char *s)
{
    while (*s){
        *s = (char)toupper((unsigned char)*s);
         s++;
    }
}

void asignarParametro_tOperacion(tOperacion *opp, unsigned nroParametro, unsigned valor){
    if(0==nroParametro || nroParametro>2)
        return;

    if(1 == nroParametro)
        opp->codParametro_1 = valor;

    if(2 == nroParametro)
        opp->codParametro_2 = valor;
}

int procesarComandoSTDIN(const char *buffer, tOperacion *oppDest, tConfig *config, tJornada *jornada){
    char  opp  [TAM_BUFFER_OPP];
    char  par_1[TAM_BUFFER_OPP];
    char  par_2[TAM_BUFFER_OPP];
    char  formatoCadena      [50];
    char  ordenParametros     [3] = "\0";
    const char *dirParametros [2] = {par_1, par_2};
    int cantParametros = 0;

    if(!strlen(buffer))
        return INGRESO_CAD_INVALIDA;

    if(!strcmpi(buffer,"EXIT"))
        return INGRESO_CAD_EXIT;

    sprintf(formatoCadena, "%%%us %%%us %%%us", (int)(sizeof(opp)-1), (int)(sizeof(par_1)-1), (int)(sizeof(par_2)-1));
    sscanf(buffer, formatoCadena, opp, par_1, par_2);

    if(INGRESO_OPP_INVALIDA == (cantParametros = procesarComandoSTDIN_validarOPP(opp, ordenParametros))){
        printf("\n\t | OPERACION INVALIDA |\n");
        return INGRESO_OPP_INVALIDA;
    }

    memcpy(oppDest->cod, opp, TAM_BUFFER_OPP);

    if(cantParametros && (INGRESO_PAR_INVALID0 == procesarComandoSTDIN_validarPAR(ordenParametros, dirParametros, cantParametros, oppDest, config))){
        printf("\n\t | PARAMETRO U ORDEN DE LOS MISMOS INVALIDO |\n");
        return INGRESO_PAR_INVALID0;
    }
    oppDest->minutoEjecutado = jornada->tiempoActual;
    return INGRESO_OK;
}

int procesarComandoSTDIN_validarOPP(const char *buffer, char *ordenParametros){
// FUNCION QUE VALIDA SI EXISTE LA OPERACION INDICADA Y DEVUELVE LA CANTIDAD DE PARAMETROS QUE REQUIERE O O
    if(!strcmp(buffer, "DES")){
        strcpy(ordenParametros, "MZ");
        return 2;
    }

    if(!strcmp(buffer, "REU")){
        strcpy(ordenParametros, "ZZ");
        return 2;
    }

    if(!strcmp(buffer, "ENT"))
        return 0;

    if(!strcmp(buffer, "VER"))
        return 0;

    if(!strcmp(buffer, "ESP"))
        return 0;

    return INGRESO_OPP_INVALIDA;
}

int procesarComandoSTDIN_validarPAR_MUE(const char *buffer, unsigned cantMuelles){
//PROCESA MUELLES DESDE EL NRO 1 HASTA CANTMUELLES
    char     oppExtra;
    unsigned codMuelle;

    if( 1 != sscanf(buffer, "M%u%c", &codMuelle, &oppExtra))
        return INGRESO_MUELLES_INVALIDO;

    if(0==codMuelle || codMuelle > cantMuelles)
        return INGRESO_MUELLES_INVALIDO;

    return codMuelle;
}

int procesarComandoSTDIN_validarPAR_ZON(const char *buffer, unsigned cantZonas){
//PROCESA MUELLES DESDE EL NRO 1 HASTA CANTZONAS
    char     oppExtra;
    unsigned codZona;

    if( 1 != sscanf(buffer, "Z%u%c", &codZona, &oppExtra))
        return INGRESO_ZONAS_INVALIDO;

    if(0==codZona || codZona > cantZonas)
        return INGRESO_ZONAS_INVALIDO;

    return codZona;
}

int procesarComandoSTDIN_validarPAR(const char *ordenParametros, char const **dirParametros, unsigned cantParametros, tOperacion *oppDest, tConfig *config){

    unsigned valorParametro = 0;
    int  i = 0;
    char aux = 0;

    while(i<cantParametros && *(ordenParametros+i)) {
        aux = *(ordenParametros+i);
        switch(aux){
            case 'M':
                if(INGRESO_MUELLES_INVALIDO == (valorParametro = procesarComandoSTDIN_validarPAR_MUE(*(dirParametros + i),config->cantidadMuelles)))
                    return INGRESO_PAR_INVALID0;
                asignarParametro_tOperacion(oppDest, i+1, valorParametro);
            break;
            case 'Z':
                if(INGRESO_ZONAS_INVALIDO == (valorParametro = procesarComandoSTDIN_validarPAR_ZON(*(dirParametros + i),config->cantidadZonasDeAlmacenamiento)))
                    return INGRESO_PAR_INVALID0;
                asignarParametro_tOperacion(oppDest, i+1, valorParametro);
            break;
        }
        i++;
    }
    return INGRESO_OK;
}

int consultarTiempoRequeridoOperacion(const char *oppCod, tConfig *config){
    if(NULL == oppCod || NULL == config)
        return INGRESO_OPP_INVALIDA;

    if(!strcmp(oppCod, "DES"))
        return config->tiempoDeDescargaDeContenedor;

    if(!strcmp(oppCod, "REU"))
        return config->tiempoDeReubicacionDeContenedor;

    if(!strcmp(oppCod, "ENT"))
        return config->tiempoDeCargaDeCamion;

    return 0;
}

int construirMensajeOperacion(tOperacion *opp, char *buffer){
    if(NULL == opp)
        return INGRESO_OPP_INVALIDA;
    sprintf(buffer, "T=%u O=%s",opp->minutoEjecutado, opp->cod);
    if(!strcmp(opp->cod, "DES"))
        sprintf(buffer + strlen(buffer)," M%u Z%u",opp->codParametro_1, opp->codParametro_2);
    if(!strcmp(opp->cod, "REU"))
        sprintf(buffer + strlen(buffer)," Z%u Z%u",opp->codParametro_1, opp->codParametro_2);
    return INGRESO_OK;
}

int obtenerNROOperacion(tOperacion *opp){
    if(!strcmp(opp->cod, "DES"))
        return OPERACION_DES;

    if(!strcmp(opp->cod, "REU"))
        return OPERACION_REU;

    if(!strcmp(opp->cod, "ENT"))
        return OPERACION_ENT;

    if(!strcmp(opp->cod, "VER"))
        return OPERACION_VER;

    if(!strcmp(opp->cod, "ESP"))
        return OPERACION_ESP;

    return INGRESO_OPP_INVALIDA;
}

int solicitarOperacionSTDIN(tOperacion *oppDest, tConfig *config, tJornada *jornada){
//FUNCION QUE SOLICITA EL INGRESA DE UNA OPERACION AL OPERADOR, VALIDA LA CADENA Y LA CONVIERTE A UNA ESTRUCTURA DEL TIPO "tOperacion"
    char bufferChar[TAM_BUFFER_STDIN_OPP] = {0};
    do{
        printf("\n |-> INGRESE OPERACION: ");
            fgets(bufferChar, TAM_BUFFER_STDIN_OPP, stdin);
            bufferChar[strcspn(bufferChar, "\n")] = '\0';
            aMayusculas(bufferChar);

        if(INGRESO_OK == procesarComandoSTDIN(bufferChar, oppDest, config, jornada))
            return INGRESO_OK;
    }while(0 != strcmp(bufferChar, "EXIT"));

    //return INGRESO_OK;
    return INGRESO_CAD_EXIT;
}

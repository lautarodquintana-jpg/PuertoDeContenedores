#include "validaciones.h"
#include "muelle.h"

char leerYValidarCharMenu()
{
    char opcion;

    printf("\nIngrese una opcion -N:Nueva Jornada -V: Ver Ranking -S:Salir: ");
    scanf(" %c", &opcion);

    opcion=toupper(opcion);
    while(opcion!='N' && opcion!='V' && opcion!='S')
    {
        printf("\nValor ingresado no valido...\n");
        printf("Ingrese una opcion -N:Nueva Jornada -V: Ver Ranking -S:Salir: ");
        scanf(" %c", &opcion);
        opcion=toupper(opcion);
    }

    return opcion;
}

int consultarTiempoRequeridoOperacion(const char *oppCod, tConfig *config)
{
    if(NULL == oppCod || NULL == config)
        return INGRESO_OPP_INVALIDA;
    if(!strcmp(oppCod, "DES")) return (int)config->tiempoDeDescargaDeContenedor;
    if(!strcmp(oppCod, "REU")) return (int)config->tiempoDeReubicacionDeContenedor;
    if(!strcmp(oppCod, "ENT")) return (int)config->tiempoDeCargaDeCamion;
    if(!strcmp(oppCod, "ESP")) return 1;
    return 0; // VER no consume tiempo
}

// Una operacion inicia solo si termina antes o justo en el limite de la jornada
int validarTiempoOperacion(const tOperacion *opp, tConfig *config, const tJornada *jornada)
{
    int duracion = consultarTiempoRequeridoOperacion(opp->cod, config);

    if(duracion < 0)
        return INGRESO_OPP_INVALIDA;
    if(jornada->tiempoActual + (unsigned)duracion > config->duracionMinutosdeJornada)
        return INGRESO_SIN_TIEMPO;
    return INGRESO_OK;
}

static int validarDES(const tOperacion *opp, tConfig *config, tJornada *jornada)
{
    tZona zona;

    if(MUELLE_OCUPADO != estadoMuelle(&jornada->muelles, opp->codParametro_1) ||
       !tieneBarcoContenedores(&jornada->muelles, opp->codParametro_1))
        return INGRESO_MUELLE_SIN_CARGA;

    if(verNElem(&jornada->zonasAlmacenamiento, opp->codParametro_2 - 1, &zona, sizeof(tZona)) != TODO_OK)
        return INGRESO_ERROR_INTERNO;
    if(zona.cantidadOcupada >= config->capPila)
        return INGRESO_ZONA_LLENA;

    return INGRESO_OK;
}

static int validarREU(const tOperacion *opp, tConfig *config, tJornada *jornada)
{
    tZona origen, destino;

    if(opp->codParametro_1 == opp->codParametro_2)
        return INGRESO_ZONAS_IGUALES;

    if(verNElem(&jornada->zonasAlmacenamiento, opp->codParametro_1 - 1, &origen, sizeof(tZona)) != TODO_OK ||
       verNElem(&jornada->zonasAlmacenamiento, opp->codParametro_2 - 1, &destino, sizeof(tZona)) != TODO_OK)
        return INGRESO_ERROR_INTERNO;

    if(origen.cantidadOcupada == 0)
        return INGRESO_ZONA_VACIA;
    if(destino.cantidadOcupada >= config->capPila)
        return INGRESO_ZONA_LLENA;

    return INGRESO_OK;
}

static int validarENT(tJornada *jornada)
{
    tCamion camion;
    tContenedor tope;
    tZona zona;
    unsigned i;

    if(verPrimeroCola(&jornada->camiones, &camion, sizeof(tCamion)) == COLA_VACIA)
        return INGRESO_SIN_CAMION;

    for(i = 0; verNElem(&jornada->zonasAlmacenamiento, i, &zona, sizeof(tZona)) == TODO_OK; i++)
    {
        if(zona.cantidadOcupada > 0 &&
           verTope(&zona.contenedores, &tope, sizeof(tContenedor)) != PILA_VACIA &&
           !strcmp(tope.cod, camion.codContenedorRetirar))
            return INGRESO_OK;
    }
    return INGRESO_CONTENEDOR_NO_EN_TOPE;
}

int validarOperacion(const tOperacion *opp, tConfig *config, tJornada *jornada)
{
    int ret = validarTiempoOperacion(opp, config, jornada);

    if(ret != INGRESO_OK)
        return ret;

    if(!strcmp(opp->cod, "DES")) return validarDES(opp, config, jornada);
    if(!strcmp(opp->cod, "REU")) return validarREU(opp, config, jornada);
    if(!strcmp(opp->cod, "ENT")) return validarENT(jornada);

    return INGRESO_OK; // VER y ESP
}

void mostrarMotivoRechazo(int codigo)
{
    switch(codigo)
    {
        case INGRESO_SIN_TIEMPO:            printf("\t"BG_ROJO CL_BLANCO"| NO HAY TIEMPO SUFICIENTE EN LA JORNADA |"COLOR_RESET "\n\n"); break;
        case INGRESO_MUELLE_SIN_CARGA:      printf("\t"BG_ROJO CL_BLANCO"| EL MUELLE NO TIENE UN BUQUE CON CARGA PENDIENTE |" COLOR_RESET "\n\n"); break;
        case INGRESO_ZONA_LLENA:            printf("\t"BG_ROJO CL_BLANCO"| LA ZONA DE DESTINO NO TIENE CAPACIDAD |"COLOR_RESET"\n\n"); break;
        case INGRESO_ZONA_VACIA:            printf("\t"BG_ROJO CL_BLANCO"| LA ZONA DE ORIGEN ESTA VACIA |"COLOR_RESET"\n\n"); break;
        case INGRESO_ZONAS_IGUALES:         printf("\t"BG_ROJO CL_BLANCO"| LAS ZONAS DEBEN SER DISTINTAS |"COLOR_RESET"\n\n"); break;
        case INGRESO_SIN_CAMION:            printf("\t"BG_ROJO CL_BLANCO"| NO HAY CAMIONES ESPERANDO |"COLOR_RESET"\n\n"); break;
        case INGRESO_CONTENEDOR_NO_EN_TOPE: printf("\t"BG_ROJO CL_BLANCO"| EL CONTENEDOR DEL CAMION NO ESTA EN EL TOPE DE NINGUNA ZONA |"COLOR_RESET"\n\n"); break;
        default:                            printf("\t"BG_ROJO CL_BLANCO"| ERROR INTERNO AL VALIDAR |"COLOR_RESET"\n\n"); break;
    }
}

int existeOperacionValida( tConfig* config, tJornada* jornada )
{
    tOperacion ope;
    unsigned i,j;

    // Se comprueba si ENT (entrega) es una operacion que se puede ejecutar
    strcpy( ope.cod, "ENT" );
    ope.codParametro_1 = 0;
    ope.codParametro_2 = 0;
    if( validarOperacion( &ope, config, jornada ) == INGRESO_OK )
        return EXISTE_OPE_VALIDA;

    // Se comprueba si DES (descarga) es una operacion que se puede ejecutar
    strcpy( ope.cod, "DES" );
    for( i = 1 ; i < config->cantidadMuelles ; i++ )
    {
        for( j = 1 ; j < config->cantidadZonasDeAlmacenamiento ; j++ )
        {
            ope.codParametro_1 = i;
            ope.codParametro_2 = j;
            if( validarOperacion( &ope, config, jornada ) == INGRESO_OK )
                return EXISTE_OPE_VALIDA;
        }
    }

    // Se comprueba si REU (reubicacion) es una operacion que se puede ejecutar
    strcpy( ope.cod, "REU" );
    for( i = 1 ; i < config->cantidadZonasDeAlmacenamiento ; i++ )
    {
        for( j = 1 ; j < config->cantidadZonasDeAlmacenamiento ; j++ )
        {
            ope.codParametro_1 = i;
            ope.codParametro_2 = j;
            if( validarOperacion( &ope, config, jornada ) == INGRESO_OK )
                return EXISTE_OPE_VALIDA;
        }
    }

    return NO_EXISTE_OPE_VALIDA;
}

int leerYValidarEntero(int li, int ls, const char *mensaje)
{
    int opcion;

    printf("%s", mensaje);
    scanf("%d", &opcion);
    while(opcion<li || opcion>ls)
    {
        printf("Opcion ingresada no valida, ingrese otro valor...\n");
        printf("%s", mensaje);
        scanf("%d", &opcion);
    }

    return opcion;
}

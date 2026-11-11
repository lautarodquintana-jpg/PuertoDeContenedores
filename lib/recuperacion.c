#include "recuperacion.h"

int duplicarArchivoTexto(const char *origen, const char *destino){
/** ESTA FUNCION ABRE EL ARCHIVO DE TEXTO INDICADO EN "ORIGEN", Y LO COPIA A LA DIRECCION "DESTINO"
    SE USARÁ PARA: A) COPIAR EL PUERTO TEMPORAL A LA CARPETA PRINCIPAL (PARA INICIAR LA JORNADA)
                   B) COPIAR EL PUERTO ACTUAL (DE CARPETA PRINCIPAL) A LA CARPETA DE TEMPORALES (INDICANDO EL NOMBRE DE USUARIO) AL CUAL PERTENECE ESE PUERTO
*/
    char buffer[MAX_LINEA_RECU];
    FILE *pf_ArchivoOrigen, *pf_ArchivoDestino;
    if(NULL == (pf_ArchivoOrigen = fopen(origen, "rt")))
        return ER_NE_ARCHIVO;
    if(NULL == (pf_ArchivoDestino = fopen(destino, "wt"))){
        fclose(pf_ArchivoOrigen);
        return ER_NE_ARCHIVO;
    }
    while(fgets(buffer, MAX_LINEA_RECU, pf_ArchivoOrigen)){
        buffer[strcspn(buffer, "\n")] = '\0';
        fprintf(pf_ArchivoDestino, "%s\n", buffer);
    }
    fclose(pf_ArchivoOrigen);
    fclose(pf_ArchivoDestino);
    return ER_TODO_OK;
}

int duplicarPuertoACarpetaPrincipal(const char *user){
/** DUPLICA UN ARCHIVO DE FORMA: ./log/USER_puerto.txt A RAIZ DE FORMA: ./puerto.txt */
    char buffer[MAX_FILENAME];
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_PUERTO);
    if(ER_NE_ARCHIVO == duplicarArchivoTexto(buffer, "puerto.txt"))
        return ER_NE_ARCHIVO;
    return ER_TODO_OK;
}

void eliminarArchivoPuertoTemporal(const char *user){
/** ELIMINA UN ARCHIVO DE LA FORMA: ./log/USER_puerto.txt*/
    char buffer[MAX_FILENAME];
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_PUERTO);
    remove(buffer);
}

int duplicarConfigACarpetaPrincipal(const char *user){
/** DUPLICA UN ARCHIVO DE FORMA: ./log/USER_config.txt A RAIZ DE FORMA: ./config.txt */
    char buffer[MAX_FILENAME];
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_CONFIG);
    if(ER_NE_ARCHIVO == duplicarArchivoTexto(buffer, "config.txt"))
        return ER_NE_ARCHIVO;
    return ER_TODO_OK;
}

void eliminarArchivoConfigTemporal(const char *user){
/** ELIMINA UN ARCHIVO DE LA FORMA: ./log/USER_config.txt*/
    char buffer[MAX_FILENAME];
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_CONFIG);
    remove(buffer);
}

FILE* abrirArchivoMovimientosTemporales(const char *user, const char *modo, unsigned iniciado){
/** ABRE ARCHIVOS DE MOVIMIENTOS TEMPORALES (USER1.log / USER2.log)
    PARA INICIADO:0, ABRE ./log/USER.log - PARA INICIADO:1, ABRE ./log/recu_USER.log
*/
    char buffer[MAX_FILENAME];
    FILE *pf_logMovimientos;
    if(NULL == user || NULL == modo)
        return NULL;
    if(1 == iniciado)
        snprintf(buffer, MAX_FILENAME, "%s%s%s%s",PATH_TEMPORALES, PREFIJO_RECUPERANDO, user, SUBFIJO_LOG);
    else
        snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_LOG);
    pf_logMovimientos = fopen(buffer, modo);
    if(NULL == pf_logMovimientos)
        return NULL;
    return pf_logMovimientos;
}
void eliminarArchivoMovimientosTemporales(const char *user, unsigned iniciado){
/** ELIMINA UN ARCHIVO DE LA FORMA: ./log/USER.log*/
    char buffer[MAX_FILENAME];
    if(iniciado)
        snprintf(buffer, MAX_FILENAME, "%s%s%s%s",PATH_TEMPORALES, PREFIJO_RECUPERANDO, user, SUBFIJO_LOG);
    else
        snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_LOG);
    remove(buffer);
}

void eliminarArchivosTemporales(const char *user){
    eliminarArchivoPuertoTemporal(user);
    eliminarArchivoConfigTemporal(user);
    eliminarArchivoMovimientosTemporales(user,0);
}

FILE* abrirArchivoPuertoTemporal(const char *user, const char *modo, unsigned iniciado){
/** ABRE ARCHIVOS DE MOVIMIENTOS TEMPORALES (USER1_puerto.txt / USER2_puerto.txt)
    PARA INICIADO:0, ABRE ./log/USER_puerto.txt - PARA INICIADO:1, ABRE ./log/recu_USER_puerto.txt
    LA APERTURA DIFERENCIADA PODRÍA NO NECESITARSE ACÁ, YA QUE SE USARÁ UNA SOLA VEZ
*/
    char buffer[MAX_FILENAME];
    FILE *pf_logPuerto;
    if(NULL == user || NULL == modo)
        return NULL;
    if(iniciado)
        snprintf(buffer, MAX_FILENAME, "%s%s%s%s",PATH_TEMPORALES, PREFIJO_RECUPERANDO, user, SUBFIJO_PUERTO);
    else
        snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_PUERTO);
    pf_logPuerto = fopen(buffer, modo);
    if(NULL == pf_logPuerto)
        return NULL;
    return pf_logPuerto;
}

FILE* abrirArchivoConfigTemporal(const char *user, const char *modo, unsigned iniciado){
/** ABRE ARCHIVOS DE MOVIMIENTOS TEMPORALES (USER1_config.txt / USER2_config.txt)
    PARA INICIADO:0, ABRE ./log/USER_config.txt - PARA INICIADO:1, ABRE ./log/recu_USER_config.txt
    LA APERTURA DIFERENCIADA PODRÍA NO NECESITARSE ACÁ, YA QUE SE USARÁ UNA SOLA VEZ
*/
    char buffer[MAX_FILENAME];
    FILE *pf_logConfig;
    if(NULL == user || NULL == modo)
        return NULL;
    if(iniciado)
        snprintf(buffer, MAX_FILENAME, "%s%s%s%s",PATH_TEMPORALES, PREFIJO_RECUPERANDO, user, SUBFIJO_CONFIG);
    else
        snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_CONFIG);
    pf_logConfig = fopen(buffer, modo);
    if(NULL == pf_logConfig)
        return NULL;
    return pf_logConfig;
}

int renombrarArchivoTemporalAIniciado(const char *user, const char *tipoArchivoTemporal){
/** FUNCION DE RENOMBRADO DE UN ARCHIVO TEMPORAL EXISTENTE A UN ARCHIVO TEMPORAL INICIADO (EN RECUPERACION)
        -> CONVIERTE UN ARCHIVO: ./log/USER.log A OTRO DE LA FORMA: ./log/recu_USER.log
        -> RETORNA: -ER_TODO_OK: SI EL RENOMBRADO DEL ARCHIVO ES OK. -ER_ERROR_ARCHIVO: SI FALLA
*/
    char bufferAnt[MAX_FILENAME];
    char bufferNue[MAX_FILENAME];
    snprintf(bufferAnt, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, tipoArchivoTemporal);
    snprintf(bufferNue, MAX_FILENAME, "%s%s%s%s",PATH_TEMPORALES, PREFIJO_RECUPERANDO, user, tipoArchivoTemporal);
    remove(bufferNue); //Para evitar fallo en caso de que por alguna razon exista ya.
    if(rename(bufferAnt, bufferNue))
        return ER_ERROR_ARCHIVO;
    return ER_TODO_OK;
}

void guardarOperacion( FILE* pf_logOperacion, tOperacion operacion )
{
    if( pf_logOperacion == NULL || strcmp( operacion.cod, "VER" ) == 0 )
        return;

    fwrite( &operacion, sizeof(tOperacion), 1, pf_logOperacion );
    fflush( pf_logOperacion ); // Fuerza la escritura de los datos del búfer al disco inmediatamente
}

int guardarCopiaConfigYPuerto(const char *user){
    char buffer[MAX_FILENAME];
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_PUERTO);
    if(ER_TODO_OK != duplicarArchivoTexto("puerto.txt", buffer))
        return ER_ERROR_ARCHIVO;
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_CONFIG);
    if(ER_TODO_OK != duplicarArchivoTexto("config.txt", buffer)){
        eliminarArchivoPuertoTemporal(user);
        return ER_ERROR_ARCHIVO;
    }
    return ER_TODO_OK;
}

int existeArchivoDeRecuperacion(const char *user){
/** EN BASE AL NOMBRE DE USUARIO, DEVUELVE LA EXISTENCIA LÓGICA O NO DE LA TERNA (LOG_MOVIMIENTOS ; LOG_PUERTO ; LOG_CONFIG) -> NO CARGA NADA*/
    FILE *pf_logMovimientos, *pf_logPuerto, *pf_logConfig;

    // INTENTA ABRIR EL ARCHIVO "USER.LOG"
    pf_logMovimientos = abrirArchivoMovimientosTemporales(user, "rb", 0);
    if(NULL == pf_logMovimientos)
        return ER_NE_ARCHIVO;

    // INTENTA ABRIR EL ARCHIVO "USER_PUERTO.TXT"
    pf_logPuerto = abrirArchivoPuertoTemporal(user, "r", 0);
    if(NULL == pf_logPuerto){
        fclose(pf_logMovimientos);
        return ER_NE_ARCHIVO;
    }

    // INTENTA ABRIR EL ARCHIVO "USER_CONFIG.TXT"
    pf_logConfig = abrirArchivoConfigTemporal(user,"r", 0);
    if(NULL == pf_logConfig){
        fclose(pf_logMovimientos);
        fclose(pf_logPuerto);
        return ER_NE_ARCHIVO;
    }

    fclose(pf_logMovimientos);
    fclose(pf_logPuerto);
    fclose(pf_logConfig);
    return ER_EX_ARCHIVO;
}

int consultarArchivosDeRecuperacionYCarga(tJornada *jornada){
    char c = 0;
    if(ER_EX_ARCHIVO == existeArchivoDeRecuperacion(jornada->user)){
        do{
            printf(BG_CARBON CL_PLATA "\n Existen archivos de recuperacion para el usuario \"%s\"" COLOR_RESET, jornada->user);
            printf(CL_GRIS "\n\t Desea cargarlos " COLOR_RESET);
            printf(CL_BLANCO "[S: SI / N: NO]" COLOR_RESET);
            printf(CL_GRIS "?: " COLOR_RESET);
            fflush(stdin);
            scanf(" %c", &c);
            c = toupper(c);
            system("cls");
        }while(c!='S' && c!='N');
        fflush(stdin);
        if('N' == c){
            eliminarArchivosTemporales(jornada->user);
            return ER_NO_RECUPERA;
        }else
            return ER_RECUPERA;
    }
    return ER_NO_RECUPERA;
}

int prepararEstructuraRecuperacion(tJornada *jornada, tConfig *config, tRecuperacion *log){
/** MODIFICA EL NOMBRE DEL ARCHIVO TEMPORAL DE MOVIMIENTOS PARA QUE NO SE REESCRIBA CON EL NUEVO ARCHIVO TEMPORAL ASOCIADO A LA JORNADA EN RECUPERACION
    LUEGO, DUPLICA EL ARCHIVO DE PUERTO A LA CARPETA PRINCIPAL Y CARGA LA ESTRUCTRURA DE RECUPERACION CON LA CANTIDAD DE REGISTROS COMPLETOS:
    CARGA LA ESTRUCTURA DE RECUPERACION, CON:
        - PUNTERO AL ARCHIVO TEMPORAL DE MOVIMIENTOS.
        - CANTIDAD DE REGISTROS VALIDOS A INTRODUCIR EN EL PROGRAMA.
    ESTA FUNCION SE EJECUTA DESPUES DE: existeArchivoDeRecuperacion();
    LUEGO DE DUPLICAR EL PUERTO A LA CARPETA PRINCIPAL, SE ELIMINA EL TEMPORAL, EN PRINCIPIO NO HAY NECESIDAD DE SEGUIR MANTENIENDOLO.
*/

    FILE *pf_logMovimientosIniciados;

    //RENOMBRA: USER.LOG A RECU_USER.LOG
        if(ER_ERROR_ARCHIVO == renombrarArchivoTemporalAIniciado(jornada->user, SUBFIJO_LOG)){
            return ER_ERROR_RENOMBRE;
        }

    //ABRE ARCHIVO DE MOVIMIENTOS DE NOMBRE: RECU_USER.LOG
        pf_logMovimientosIniciados = abrirArchivoMovimientosTemporales(jornada->user, "rb",1);
        if(NULL == pf_logMovimientosIniciados){
            return ER_ERROR_APERTURA_LGMOVS;
        }

    // CREA EN CARPETA RAIZ: "CONFIG.TXT" Y "PUERTO.TXT" DUPLICADOS (SOBREESCRIBIENDO LOS ANTERIORES)
        if(ER_TODO_OK != duplicarConfigACarpetaPrincipal(jornada->user) || ER_TODO_OK != duplicarPuertoACarpetaPrincipal(jornada->user)){            fclose(pf_logMovimientosIniciados);
            return ER_ERROR_DUPLICACION;
        }

    // LEE EL CONFIG Y LO CARGA NUEVAMENTE (POR SI CAMBIO)
        if(TODO_OK != leerConfigTxt(config, RECU_NOMBARCH_CONFIG)){
            fclose(pf_logMovimientosIniciados);
            return ER_ERROR_CONFIG;
        }

    // ELIMINA LOS ARCHIVOS: "USER_CONFIG.TXT" Y "USER_PUERTO.TXT": YA ESTA EN RAIZ PARA INICIALIZAR JORNADA Y LA COPIA DE SEGURIDAD CREARÁ NUEVOS
        eliminarArchivoConfigTemporal(jornada->user);
        eliminarArchivoPuertoTemporal(jornada->user);

    // CALCULA CUANTOS REGISTROS VALIDOS EXISTEN EN EL ARCHIVO
        fseek(pf_logMovimientosIniciados, 0, SEEK_END);
        log->registrosValidos = ftell(pf_logMovimientosIniciados) / sizeof(tOperacion);
        fseek(pf_logMovimientosIniciados, 0, SEEK_SET);

    // SI NO HAY REGISTROS VALIDOS, LA RECUPERACION NO SE INICIA
        if(0 == log->registrosValidos){
            fclose(pf_logMovimientosIniciados);                    // CIERRA "RECU_USER.LOG"
            eliminarArchivoMovimientosTemporales(jornada->user,1); // ELIMINA: "RECU_USER.LOG"
            log->pf_logMovimientos = NULL;
        }else
            log->pf_logMovimientos = pf_logMovimientosIniciados;

    return ER_TODO_OK;
}

int leerOperacionDelArchivoTemporal(tOperacion *operacion, tJornada *jornada, tRecuperacion *recuperacion){
    if(recuperacion->registrosValidos > 0){
        fread(operacion, sizeof(tOperacion), 1, recuperacion->pf_logMovimientos);
        printf(CL_GRIS"\n TIEMPO ACTUAL: %d"COLOR_RESET, jornada->tiempoActual);
        printf("\n\t"CL_GRIS"OPP: %s | P1: %d | P2: %d | MIN: %d | CI: %s"COLOR_RESET"\n", operacion->cod, operacion->codParametro_1, operacion->codParametro_2, operacion->minutoEjecutado, operacion->contenedorInvolucrado);
        recuperacion->registrosValidos -= 1;
    }else{
        cerrarArchivo(&recuperacion->pf_logMovimientos);
        eliminarArchivoMovimientosTemporales(jornada->user, 1);
        return ER_NE_REGISTROS_REC;
    }
    return ER_EX_REGISTROS_REC;
}

void cerrarArchivo(FILE **pf){
    if(pf == NULL || *pf == NULL)
        return;
    fclose(*pf);
    *pf = NULL;
}

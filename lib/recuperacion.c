#include "recuperacion.h"

int duplicarArchivoTexto(const char *origen, const char *destino){
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
        puts(buffer);
    }
    fclose(pf_ArchivoOrigen);
    fclose(pf_ArchivoDestino);
    return ER_TODO_OK;
}

int duplicarPuertoACarpetaPrincipal(const char *user){
    char buffer[MAX_FILENAME];
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_PUERTO);
    if(ER_NE_ARCHIVO == duplicarArchivoTexto(buffer, "puerto.txt"))
        return ER_NE_ARCHIVO;
    return ER_TODO_OK;
}

void eliminarArchivoPuertoTemporal(const char *user){
    char buffer[MAX_FILENAME];
    snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_PUERTO);
    remove(buffer);
}

FILE* abrirArchivoMovimientosTemporales(const char *user, const char *modo, unsigned iniciado){
    char buffer[MAX_FILENAME];
    FILE *pf_logMovimientos;
    if(NULL == user || NULL == modo)
        return NULL;
    if(iniciado)
        snprintf(buffer, MAX_FILENAME, "%s%s%s%s",PATH_TEMPORALES, PREFIJO_RECUPERANDO, user, SUBFIJO_LOG);
    else
        snprintf(buffer, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, SUBFIJO_LOG);
    pf_logMovimientos = fopen(buffer, modo);
    if(NULL == pf_logMovimientos)
        return NULL;
    return pf_logMovimientos;
}

FILE* abrirArchivoPuertoTemporal(const char *user, const char *modo, unsigned iniciado){
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

int renombrarArchivoTemporalAIniciado(const char *user, const char *tipoArchivoTemporal){
/** FUNCION DE RENOMBRADO DE UN ARCHIVO TEMPORAL EXISTENTE A UN ARCHIVO TEMPORAL INICIADO (EN RECUPERACION)
    RETORNA: -ER_TODO_OK: SI EL RENOMBRADO DEL ARCHIVO ES OK. -ER_ERROR_ARCHIVO: SI FALLA
*/
    char bufferAnt[MAX_FILENAME];
    char bufferNue[MAX_FILENAME];
    snprintf(bufferAnt, MAX_FILENAME, "%s%s%s",PATH_TEMPORALES, user, tipoArchivoTemporal);
    snprintf(bufferNue, MAX_FILENAME, "%s%s%s%s",PATH_TEMPORALES, PREFIJO_RECUPERANDO, user, tipoArchivoTemporal);
    printf("\n ANT: %s | NUE: %s", bufferAnt, bufferNue);
    if(rename(bufferAnt, bufferNue))
        return ER_ERROR_ARCHIVO;
    return ER_TODO_OK;
}

int existeArchivoDeRecuperacion(const char *user){
/** EN BASE AL NOMBRE DE USUARIO, DEVUELVE LA EXISTENCIA LÓGICA O NO DEL PAR (LOG_MOVIMIENTOS ; LOG_PUERTO) -> NO CARGA NADA*/
    FILE *pf_logMovimientos, *pf_logPuerto;
    pf_logMovimientos = abrirArchivoMovimientosTemporales(user, "rb", 0);
    if(NULL == pf_logMovimientos)
        return ER_NE_ARCHIVO;
    pf_logPuerto = abrirArchivoPuertoTemporal(user, "r", 0);
    if(NULL == pf_logPuerto){
        fclose(pf_logMovimientos);
        return ER_NE_ARCHIVO;
    }
    fclose(pf_logMovimientos);
    fclose(pf_logPuerto);
    return ER_EX_ARCHIVO;
}

int prepararEstructuraRecuperacion(const char *user, tRecuperacion *log){
/** MODIFICA EL NOMBRE DEL ARCHIVO TEMPORAL DE MOVIMIENTOS PARA QUE NO SE REESCRIBA CON EL NUEVO ARCHIVO TEMPORAL ASOCIADO A LA JORNADA EN RECUPERACION
    LUEGO, DUPLICA EL ARCHIVO DE PUERTO A LA CARPETA PRINCIPAL Y CARGA LA ESTRUCTRURA DE RECUPERACION CON LA CANTIDAD DE REGISTROS COMPLETOS:
    CARGA LA ESTRUCTURA DE RECUPERACION, CON:
        - PUNTERO AL ARCHIVO TEMPORAL DE MOVIMIENTOS.
        - CANTIDAD DE REGISTROS VALIDOS A INTRODUCIR EN EL PROGRAMA.
    ESTA FUNCION SE EJECUTA DESPUES DE: existeArchivoDeRecuperacion();
    LUEGO DE DUPLICAR EL PUERTO A LA CARPETA PRINCIPAL, SE ELIMINA EL TEMPORAL, EN PRINCIPIO NO HAY NECESIDAD DE SEGUIR MANTENIENDOLO.
*/
    FILE *pf_logMovimientosIniciados, *pf_logMPuertoIniciado;
    if(ER_ERROR_ARCHIVO == renombrarArchivoTemporalAIniciado(user, SUBFIJO_LOG)){
        printf("\n -> [Error al renombrar el archivo temporal de movimientos] <- ");
        return ER_ERROR_ARCHIVO;
    }
    pf_logMovimientosIniciados = abrirArchivoMovimientosTemporales(user, "rb",1);
    if(NULL == pf_logMovimientosIniciados)
        return ER_ERROR_ARCHIVO;
    pf_logMPuertoIniciado = abrirArchivoPuertoTemporal(user, "r", 0);
    if(NULL == pf_logMPuertoIniciado){
        fclose(pf_logMovimientosIniciados);
        return ER_ERROR_ARCHIVO;
    }
    if(ER_NE_ARCHIVO == duplicarPuertoACarpetaPrincipal(user)){
        fclose(pf_logMovimientosIniciados);
        fclose(pf_logMPuertoIniciado);
        return ER_ERROR_ARCHIVO;
    }
    log->pf_logMovimientos = pf_logMovimientosIniciados;
    fseek(pf_logMovimientosIniciados, 0, SEEK_END);
    log->registrosValidos = ftell(pf_logMovimientosIniciados) / sizeof(tOperacion);
    fseek(pf_logMovimientosIniciados, 0, SEEK_SET);
    fclose(pf_logMPuertoIniciado);
    eliminarArchivoPuertoTemporal(user);
    return ER_TODO_OK;
}

#include "usuarios.h"

int iniciarSesion( tJornada* jornada , tArbol* indice)
{
    tIndiceUsuario ind;
    tUsuario nuevo;
    FILE* pf;

    char* pLinea;
    printf("Ingrese su nombre: ");
    fgets( jornada->user, TAM_USER, stdin );

    pLinea = strchr( jornada->user, '\n' );
    if( pLinea != NULL )
        *pLinea = '\0';

    strcpy(ind.clave , jornada->user);

    if(buscarElemEnArbol(indice, &ind, compararIndiceUsuarios))
        return EXISTE_USER;

    // alta: registro nuevo al final de operadores.dat
    pf = fopen(ARCH_USUARIOS, "ab");
    if(!pf)
        return ERROR_ARCHIVO;

    fseek( pf, 0, SEEK_END );
    ind.nroRegistro = ftell(pf) / sizeof(tUsuario);

    strcpy(nuevo.nomUser, jornada->user);
    nuevo.puntajeTotal = 0;
    nuevo.jornadasJugadas = 0;
    fwrite(&nuevo, sizeof(tUsuario), 1, pf);
    fclose(pf);

    if(insertarOrdenadoSinRecursividad(indice, &ind, sizeof(tIndiceUsuario), compararIndiceUsuarios ) != TODO_OK)
        return ERROR_AGREGAR_USER;
    if(guardarIndiceUsuarios(indice) != TODO_OK)
        return ERROR_ARCHIVO;

    return NO_EXISTE_USER;
}

int cargarIndiceUsuarios( tArbol* indice )
{
    int ret;

    crearArbol( indice );
    ret = generarArbolBalanceadoAPartirDeArchivoBinarioOrdenado( indice, ARCH_INDICE, sizeof(tIndiceUsuario) );
    if( ret == ERROR_ARCHIVO )
        return TODO_OK;      // primera ejecución: índice vacío
    return ret;
}

int guardarIndiceUsuarios( tArbol* indice )
{
    return crearArchivoBinarioAPartirDeArbol( indice, ARCH_INDICE ); //lo crea en inorden
}

int registrarJornada( tArbol* indice, const tRegistroJornada* jornada )
{
    tIndiceUsuario ind;
    tArbol* pNodo;
    tUsuario usr;
    long pos;
    FILE* pf;

    // archivo de historial de jornadas
    pf = fopen( ARCH_JORNADAS, "ab" );
    if( !pf )
        return ERROR_ARCHIVO;
    fwrite( jornada, sizeof(tRegistroJornada), 1, pf );
    fclose( pf );

    strcpy(ind.clave , jornada->nomUser);


    pNodo = buscarElemEnArbol( indice, &ind, compararIndiceUsuarios );
    if( !pNodo )
        return NO_ENCONTRADO;
    pos = ((tIndiceUsuario*)(*pNodo)->info)->nroRegistro * sizeof(tUsuario);

    pf = fopen( ARCH_USUARIOS, "r+b" );
    if( !pf )
        return ERROR_ARCHIVO;
    fseek( pf, pos, SEEK_SET );
    fread( &usr, sizeof(tUsuario), 1, pf );
    usr.puntajeTotal += jornada->puntaje;
    usr.jornadasJugadas++;
    fseek( pf, pos, SEEK_SET );
    fwrite( &usr, sizeof(tUsuario), 1, pf );
    fclose( pf );

    return TODO_OK;
}

int mostrarRanking()
{
    int i=1;
    tUsuario userLec;
    FILE* pf = fopen( "ranking.dat", "rb" );
    if( !pf )
        return ERROR_ARCHIVO;

    getchar(); // Consumo linea de salto que hubo al seleccionar las opciones.
    system("cls");
    printf("\n=================== RANKING ===================\n");
    fread( &userLec, sizeof(tUsuario), 1, pf );
    while( !feof(pf) )
    {
        printf("[%d] ",i);
        mostrarInfoUsuario(&userLec);
        fread( &userLec, sizeof(tUsuario), 1, pf );
        i++;
    }

    printf("\n\nPresione ENTER para salir...");
    getchar(); // Espero a que el usuario aprete ENTER para continuar
    system("cls");

    fclose(pf);
    return TODO_OK;
}

int compararNombres( const void* reg_1, const void* reg_2 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    tUsuario* user2 = (tUsuario*)reg_2;

    return strcmpi( user2->nomUser, user1->nomUser );
}

int compararIndiceUsuarios( const void* reg_1, const void* reg_2 )
{
    return strcmp( ((const tIndiceUsuario*)reg_1)->clave,
                   ((const tIndiceUsuario*)reg_2)->clave );
}

int compararPuntaje( const void* reg_1, const void* reg_2 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    tUsuario* user2 = (tUsuario*)reg_2;

    if(user1->puntajeTotal > user2->puntajeTotal) return -1;
    if(user1->puntajeTotal < user2->puntajeTotal) return 1;
    return strcmp(user1->nomUser , user2->nomUser);

}

void mostrarInfoUsuario( const void* reg_1 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    printf("Nombre: %-10s | Puntaje: %8d | Jornadas: %u\n",
           user1->nomUser,
           user1->puntajeTotal,
           user1->jornadasJugadas);
}

void actualizarPuntaje( void* reg_1, const void* reg_2 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    tUsuario* user2 = (tUsuario*)reg_2;

    if( user1->puntajeTotal < user2->puntajeTotal )
        user1->puntajeTotal = user2->puntajeTotal;
}

#include "usuarios.h"

int iniciarSesion( tJornada* jornada, tArbol* indice)
{
    tIndiceUsuario ind;
    tUsuario nuevo;
    FILE* pf;
    int opcion=0;

    char* pLinea;

    printf("Ingrese su nombre: ");
    fflush(stdin);
    fgets( jornada->user, TAM_USER, stdin );

    pLinea = strchr( jornada->user, '\n' );
    if( pLinea != NULL )
        *pLinea = '\0';

    strcpy(ind.clave, jornada->user);
    ind.nroRegistro=-1;
    mapearInOrdenArbol(indice, elemEstaEnArbol, &ind);
    //Si encuentro el usuario debo preguntar si desea jugar con el existente
    while(ind.nroRegistro!=-1)
    {
        opcion=leerYValidarEntero(0, 1, "Este usuario ya ha sido creado, desea jugar con este? 1: Si, 0:No\nIngrese una opcion: ");
        if(opcion==0)
        {
            getchar();
            printf("Ingrese su nombre: ");
            fgets( jornada->user, TAM_USER, stdin );

            pLinea = strchr( jornada->user, '\n' );
            if( pLinea != NULL )
                *pLinea = '\0';
            strcpy(ind.clave, jornada->user);
            ind.nroRegistro=-1;
            mapearInOrdenArbol(indice, elemEstaEnArbol, &ind);
        }
        else
            return EXISTE_USER;
    }

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

int registrarJornada( tArbol* indice, const tJornada* jornada, unsigned camionesPendientes)
{
    tIndiceUsuario ind;
    tUsuario usr;
    FILE* pf;
    tRegistroJornada regJornada;


    regJornada.camionesPendientes=camionesPendientes;
    regJornada.cantDescargas=jornada->cantOperaciones.cantDescargas;
    regJornada.cantEntregas=jornada->cantOperaciones.cantEntregas;
    regJornada.cantReubicaciones=jornada->cantOperaciones.cantReubic;
    regJornada.puntaje=jornada->puntajeActual;
    regJornada.tiempoUsado=jornada->tiempoActual;
    strcpy(regJornada.nomUser, jornada->user);

    // archivo de historial de jornadas
    pf = fopen( ARCH_JORNADAS, "ab" );
    if( !pf )
        return ERROR_ARCHIVO;
    fwrite( &regJornada, sizeof(tRegistroJornada), 1, pf );
    fclose( pf );


    strcpy(ind.clave, regJornada.nomUser);
    ind.nroRegistro=-1;
    mapearInOrdenArbol(indice, elemEstaEnArbol, &ind);//Busca el nroRegistro para el nombre de usuario dado

    pf = fopen( ARCH_USUARIOS, "r+b" );
    if( !pf )
        return ERROR_ARCHIVO;


    if(ind.nroRegistro!=-1)
    {
        fseek( pf, ind.nroRegistro*sizeof(tUsuario), SEEK_SET );
        fread( &usr, sizeof(tUsuario), 1, pf );
        usr.puntajeTotal += jornada->puntajeActual;
        usr.jornadasJugadas++;
        fseek( pf, ind.nroRegistro*sizeof(tUsuario), SEEK_SET );
        fwrite( &usr, sizeof(tUsuario), 1, pf );
    }
    else
    {
        strcpy(usr.nomUser, regJornada.nomUser);
        usr.puntajeTotal = regJornada.puntaje;
        usr.jornadasJugadas = 1;
        fseek( pf, 0, SEEK_END );
        fwrite( &usr, sizeof(tUsuario), 1, pf );
    }



    fclose( pf );

    return TODO_OK;
}

int mostrarRanking()
{
    FILE *archUsuarios;
    tLista listaOperadores;
    tUsuario user;

    crearLista(&listaOperadores);
    archUsuarios=fopen(ARCH_USUARIOS, "rb");
    if(!archUsuarios)
        return ERROR_ARCHIVO;

    while(fread(&user, sizeof(tUsuario), 1, archUsuarios))
    {
        if( user.jornadasJugadas > 0 )
        {
            if(insertarOrdenadoenLista(&listaOperadores, &user, sizeof(tUsuario), compararPuntaje, 0, NULL)!=TODO_OK)//No debería retornar duplicado
            {
                fclose(archUsuarios);
                vaciarLista(&listaOperadores);
                return ERROR_MEMORIA;
            }
        }
    }
    printf("\n\n=== RANKING DE USUARIOS ===\n\n");
    printf("%-50s | %-11s | %-18s | %-13s\n", "Nombre de Usuario", "Puntos Acum", "Promedio de puntos", "Cant Partidas");
    recorrerLista(&listaOperadores, NULL, mostrarInfoUsuario);

    vaciarLista(&listaOperadores);
    fclose(archUsuarios);
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
    return strcmpi( ((const tIndiceUsuario*)reg_1)->clave,
                    ((const tIndiceUsuario*)reg_2)->clave );
}

void elemEstaEnArbol(void *elem, void *param)
{
    tIndiceUsuario *indice=elem, *parametro=param;

    if(strcmpi(indice->clave, parametro->clave)==0)
        parametro->nroRegistro=indice->nroRegistro;
}

int compararPuntaje( const void* reg_1, const void* reg_2 )
{
    int cmp;
    tUsuario* user1 = (tUsuario*)reg_1;
    tUsuario* user2 = (tUsuario*)reg_2;

    if(user1->puntajeTotal==user2->puntajeTotal)
        cmp=0;
    else if(user1->puntajeTotal>user2->puntajeTotal)
        cmp= -1;
    else
        cmp=1;

    if(cmp!=0)
        return cmp;

    return strcmpi(user1->nomUser, user2->nomUser);
}

void mostrarInfoUsuario( void *param, const void* reg_1)
{
    tUsuario* user = (tUsuario*)reg_1;
    float puntajeProm;

    if( user->jornadasJugadas > 0 )
        puntajeProm = (float)user->puntajeTotal/user->jornadasJugadas;
    else
        puntajeProm = 0;

    printf("%-50s | %-11u | %-18.2f | %-13u\n",
           user->nomUser,
           user->puntajeTotal,
           puntajeProm,
           user->jornadasJugadas);
}

void actualizarPuntaje( void* reg_1, const void* reg_2 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    tUsuario* user2 = (tUsuario*)reg_2;

    if( user1->puntajeTotal < user2->puntajeTotal )
        user1->puntajeTotal = user2->puntajeTotal;
}

#include "usuarios.h"

void iniciarSesion( tJornada* jornada )
{
    printf("Ingrese su nombre: ");
    fgets( jornada->user, TAM_USER, stdin );
}

int agregarUserRankingOrdenado( const tJornada* jornada )
{
    tUsuario nueUser, userAux;
    tLista rankingLista;
    int i;

    FILE* pf = fopen( "ranking.dat", "r+b" );
    if( !pf )
    {
        pf = fopen( "ranking.dat", "wb" );
        if( !pf )
            return ERROR_ARCHIVO;
    }

    nueUser.puntajeTotal = jornada->puntajeActual;
    strcpy( nueUser.nomUser, jornada->user );

    crearLista( &rankingLista );
    fread( &userAux, sizeof( tUsuario ), 1, pf );
    while( !feof(pf) )
    {
        if( insertarOrdenadoenLista( &rankingLista, &userAux, sizeof( tUsuario ), compararPuntaje, 0, actualizarPuntaje ) != TODO_OK )
        {
            vaciarLista( &rankingLista );
            fclose(pf);
            return ERROR_AGREGAR_USER;
        }
        fread( &userAux, sizeof( tUsuario ), 1, pf );
    }

    if( insertarOrdenadoenLista( &rankingLista, &nueUser, sizeof( tUsuario ), compararPuntaje, 0, actualizarPuntaje ) != TODO_OK )
    {
        vaciarLista( &rankingLista );
        fclose(pf);
        return ERROR_AGREGAR_USER;
    }

    i=0;
    while( verNElem( &rankingLista, i, &userAux, sizeof(tUsuario) ) == TODO_OK )
    {
        fseek( pf, i * sizeof(tUsuario), SEEK_SET );
        fwrite( &userAux, sizeof(tUsuario), 1, pf );
        i++;
    }

    vaciarLista( &rankingLista );
    fclose(pf);
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

int compararPuntaje( const void* reg_1, const void* reg_2 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    tUsuario* user2 = (tUsuario*)reg_2;

    return user2->puntajeTotal - user1->puntajeTotal;
}

void mostrarInfoUsuario( const void* reg_1 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    printf("Nombre: %-10s | Puntaje: %15d\n",
           user1->nomUser,
           user1->puntajeTotal);
}

void actualizarPuntaje( void* reg_1, const void* reg_2 )
{
    tUsuario* user1 = (tUsuario*)reg_1;
    tUsuario* user2 = (tUsuario*)reg_2;

    if( user1->puntajeTotal < user2->puntajeTotal )
        user1->puntajeTotal = user2->puntajeTotal;
}

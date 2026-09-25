#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include "../TDA/cola.h"
#include "../TDA/pila.h"

#define TODO_OK 1
#define ERROR_ARCHIVO -20
#define TAM_USER 50
#define TAM_CODIGOS 15
typedef struct
{
	unsigned duracionMinutosdeJornada;
	unsigned cantidadMuelles;
	unsigned cantidadZonasDeAlmacenamiento;
	unsigned capPila;
	unsigned maximosBuques;
	unsigned maximosContenedoresPorBuque;
	unsigned maximosCamiones;
	unsigned tiempoDeDescargaDeContenedor;
	unsigned tiempoDeReubicacionDeContenedor;
	unsigned tiempoDeCargaDeCamion;
}tConfig;

typedef struct{
	char 		user[TAM_USER];
	unsigned	tiempoTotal;
	unsigned 	tiempoActual;
	unsigned 	puntajeActual;

//	tLista 		muelles;
//	tLista 		zonasAlmacenamiento;

	tCola 		movimientos;
	tCola		bloqueos;

	tCola		barcosEspera;
	tCola		camiones;

	unsigned 	estado;
}tJornada;

typedef struct{
	tConfig 	*config;
	tJornada	*jornada;
}tPuerto;

typedef struct{
		char	    cod[TAM_CODIGOS];
		unsigned	arriboProgramado;
		unsigned	cantidadContenedoresTotal;
		unsigned 	cantidadContenedoresRestante;
		tCola		contenedores;
}tBarco;

typedef struct{
	unsigned 	cod;
	unsigned 	minutoArribo;
	tBarco		barco;
	unsigned	estado; //Esperando - Descargando - Libre
	unsigned 	minutoLiberacion;
}tMuelle;

typedef struct{
    char cod[TAM_CODIGOS];
}tContenedor;

typedef struct{
	unsigned	cod;
	unsigned 	cantidadOcupada;
	tPila		contenedores;
}tZona;

typedef struct{
	char    	cod[TAM_CODIGOS];
	unsigned 	minutoRetiro;
	char    	codContenedorRetirar[TAM_CODIGOS];
}tCamion;

#endif // ESTRUCTURAS_H_INCLUDED

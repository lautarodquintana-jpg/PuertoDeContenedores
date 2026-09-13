#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include "../TDA/cola.h"
#include "../TDA/pila.h"

#define POS_CANT_MUELLES 0
#define POS_CANT_ZONAS_ALMACENAMIENTO 1

#define POS_MAX_BUQUES 0
#define POS_MAX_CONTENEDORES_POR_BUQUE 1
#define POS_MAX_CAMIONES 2

#define POS_TIEMPO_DESCARGA_DE_CONTENEDOR 0
#define POS_TIEMPO_REUBICACION_CONTENEDOR 1
#define POS_TIEMPO_CARGA_CAMION 2

#define TODO_OK 1
#define ERROR_ARCHIVO -20
#define TAM_USER 50

typedef struct
{
	unsigned duracionMinutosdeJornada;
	unsigned cants[2];
	unsigned capPila;
	unsigned maximos[3];
	unsigned tiempos[3];
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
	tConfig 	config;
	tJornada	jornada;
}tPuerto;

typedef struct{
		unsigned	cod;
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
    unsigned cod;
}tContenedor;

typedef struct{
	unsigned	cod;
	unsigned 	cantidadOcupada;
	tPila		contenedores;
}tZona;

typedef struct{
	unsigned	cod;
	unsigned 	minutoRetiro;
	unsigned	codContenedorRetirar;
}tCamion;

#endif // ESTRUCTURAS_H_INCLUDED

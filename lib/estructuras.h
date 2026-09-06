#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED


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

typedef struct
{
	unsigned duracionMinutosdeJornada;
	unsigned cants[2];
	unsigned capPila;
	unsigned maximos[3];
	unsigned tiempos[3];
}tConfig;

#endif // ESTRUCTURAS_H_INCLUDED

#ifndef PROCESO_OPERACION_H_INCLUDED
#define PROCESO_OPERACION_H_INCLUDED

#include "estructuras.h"
//#include "zona_almacenamiento.h"
#include "../TDA/ListaSimplementeEnlazada.h"
#include "ingreso_operaciones.h"
#include "muelle.h"
#include "zona.h"
#include "camion.h"

void procesarOperacion( tJornada* jornada , const tOperacion* ope, const tConfig* config );

void ejecutarVER( tJornada* jornada );
int ejecutarDES(tLista *plMuelles , tLista *plZonas , unsigned codMuelleOri , unsigned codZonaDes, unsigned capacidadPilaZona, unsigned* puntuacionActual );
int ejecutarREU();
int ejecutarENT( tCola* colaCamiones, tLista* plZonas );

#endif // PROCESO_OPERACION_H_INCLUDED

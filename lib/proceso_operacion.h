#ifndef PROCESO_OPERACION_H_INCLUDED
#define PROCESO_OPERACION_H_INCLUDED

#include "estructuras.h"
#include "../TDA/ListaSimplementeEnlazada.h"
#include "ingreso_operaciones.h"
#include "muelle.h"
#include "zona.h"

void procesarOperacion( tJornada* jornada , const tOperacion* ope, const tConfig* config );

void ejecutarVER( tJornada* jornada );
int ejecutarDES(tLista *plMuelles , tLista *plZonas , unsigned codMuelleOri , unsigned codZonaDes, unsigned capacidadPilaZona );
int ejecutarREU();
int ejecutarENT();

#endif // PROCESO_OPERACION_H_INCLUDED

#ifndef PROCESO_OPERACION_H_INCLUDED
#define PROCESO_OPERACION_H_INCLUDED

#include "estructuras.h"
//#include "zona_almacenamiento.h"
#include "../TDA/ListaSimplementeEnlazada.h"
#include "ingreso_operaciones.h"
#include "muelle.h"
#include "zona.h"
#include "camion.h"

int procesarOperacion( tJornada* jornada, tOperacion* ope, const tConfig* config );

int ejecutarVER( tJornada* jornada );
int ejecutarDES(tLista *plMuelles , tLista *plZonas , tOperacion *ope, unsigned capacidadPilaZona, unsigned* puntuacionActual );
int ejecutarREU();
int ejecutarENT( tCola* colaCamiones, tLista* plZonas , tOperacion *ope);

#endif // PROCESO_OPERACION_H_INCLUDED

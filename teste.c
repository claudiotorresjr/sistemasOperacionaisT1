#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

  grafo g = le_grafo(stdin);

  if ( !g )

    return 1;

  printf ("O coeficiente de agrupamento do grafo é %f\n", coeficiente_agrupamento_grafo(g));

  return ! destroi_grafo(g);
}

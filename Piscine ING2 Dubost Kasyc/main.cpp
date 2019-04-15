#include <iostream>
#include <map>
#include "Graphe.h"
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"

int main()
{
    Graphe g("graphes/broadway.txt","graphes/broadway_weights_0.txt");

    g.afficherGraphe();
    g.algoPrim(1);

    return 0;
}

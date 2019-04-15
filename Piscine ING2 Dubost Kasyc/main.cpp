#include <iostream>
#include <map>
#include "Graphe.h"
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"

int main()
{

    Svgfile svgout;
    svgout.addGrid(100, true, "rgb(200, 200, 200)");
    Graphe g("graphes/cubetown.txt","graphes/cubetown_weights_0.txt");

    int depart = 1;
    int critere = 0;

    g.algoPrim(depart,critere).afficherGraphe(svgout);

    int critere = 1;
    g.algoPrim(depart,critere).afficherGraphe(svgout);

    return 0;
}

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

    g.afficherGraphe(svgout);


    return 0;
}

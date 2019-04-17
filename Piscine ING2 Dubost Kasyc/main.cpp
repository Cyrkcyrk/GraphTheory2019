#include <iostream>
#include <map>
#include "Graphe.h"
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"
#include "maths.h"

int main()
{
    //Graphe g("graphes/broadway.txt","graphes/broadway_weights_0.txt");
    //Graphe g("graphes/cubetown.txt","graphes/cubetown_weights_0.txt");
    Graphe g("graphes/manhattan.txt","graphes/manhattan_weights_0.txt");
    //Graphe g("graphes/triville.txt","graphes/triville_weights_0.txt");
    {
        Svgfile svgout("Graphe.svg");
        svgout.addGrid(100, true, "rgb(200, 200, 200)");
        g.afficherGraphe(svgout);
    }

    int depart = 1;
    for (int i=0; i<g.getNbrCritere(); ++i)
    {
        Svgfile svgout("prim_" + std::to_string(i) + ".svg");
        svgout.addGrid(100, true, "rgb(200, 200, 200)");
        g.algoPrim(depart,i).afficherGraphe(svgout);
    }

    g.pareto();

    return 0;
}

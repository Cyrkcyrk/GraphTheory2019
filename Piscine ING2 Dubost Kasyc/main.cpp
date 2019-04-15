#include <iostream>
#include <map>
#include "Graphe.h"
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"
#include "maths.h"

int main()
{
    /*
    //Graphe g("graphes/cubetown.txt","graphes/cubetown_weights_0.txt");
    Graphe g("graphes/triville.txt","graphes/triville_weights_1.txt");
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
    */
    maths::compteurBinaire();
    return 0;
}

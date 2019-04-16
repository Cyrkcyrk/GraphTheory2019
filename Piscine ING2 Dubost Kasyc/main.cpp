#include <iostream>
#include <map>
#include "Graphe.h"
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"
#include "maths.h"

int main()
{

    //Graphe g("graphes/cubetown.txt","graphes/cubetown_weights_0.txt");
    Graphe g("graphes/broadway.txt","graphes/broadway_weights_0.txt");
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
    std::cout << "ordre : " << g.getOrdre() <<std::endl;
    std::cout << "taille : " << g.getTaille() <<std::endl;

    g.palero();
    std::cout<<maths::nCr(20,40);

    return 0;
}

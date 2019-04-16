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


    std::cout << maths::nCr(4, 10) << std::endl;
    std::vector<std::vector<char>> etats = maths::compteur_etat_possibles(15, 30);

    std::cout << etats.size();
/*
    for (int i = 0; i< etats.size(); i++)
    {
        std::cout << std::endl << i << " : ";
        for (int j=0; j< etats[i].size(); j++)
        {
            std::cout << int(etats[i][j]);
        }
    }
*/

    return 0;
}

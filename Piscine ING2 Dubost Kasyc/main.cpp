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
    //Graphe g("graphes/broadway.txt","graphes/broadway_weights_0.txt");
    Graphe g("graphes/manhattan.txt","graphes/manhattan_weights_0.txt");
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




    //std::vector<std::vector<char>> etats = maths::compteur_etat_possibles(16, 24);
    //std::cout << etats.size();
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
    g.palero();


    return 0;
}

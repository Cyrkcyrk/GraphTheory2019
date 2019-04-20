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
    //Graphe g("graphes/manhattan.txt","graphes/manhattan_weights_1.txt");
    Graphe g("graphes/triville.txt","graphes/triville_weights_0.txt");

    {
        Svgfile svgout("Graphe.svg");
        svgout.addGrid(100, true, "rgb(200, 200, 200)");
        g.dessinerGraphe(svgout);
    }
    std::vector<Graphe> prims;
    int depart = 1;
    for (int i=0; i<g.getNbrCritere(); ++i) // Effectue un prim sur chaque critere
    {
        Svgfile svgout("prim_" + std::to_string(i) + ".svg");
        svgout.addGrid(100, true, "rgb(200, 200, 200)");
        prims.push_back(g.algoPrim(depart,i));
        prims[i].dessinerGraphe(svgout);
    }

    std::vector<std::vector<char>> primBin;
    for(auto p : prims) // Pour chaque prim
    {
        std::vector<char> areteSelec;
        for(unsigned int i = 0; i< g.getAretes().size()-1;i++) // Pour chaque arete globale
        {
            areteSelec.push_back(0);
        }

        for(auto a : p.getAretes()) // Pour chaque arete dans le prim
        {
            areteSelec[a.second->getId()] = 1;
        }

        primBin.push_back(areteSelec);
    }

    std::vector<Possibilite*> interpolation = maths::compteur_etat_possiblesPrim(primBin[0],primBin[1],&g);
    g.pareto(interpolation);

    std::vector<Possibilite*> tableauDesPossibles = maths::compteur_etat_possibles(g.getSommets().size()-1,g.getAretes().size(),&g);
    g.pareto(tableauDesPossibles);
    g.pareto2(tableauDesPossibles);

    return 0;
}

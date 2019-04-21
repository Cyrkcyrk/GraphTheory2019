#include <iostream>
#include <map>
#include "Graphe.h"
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"
#include "maths.h"

int main()
{
    Graphe g;

    int choixGraphe;
    int choixPoids;

    do
    {
        std::cout << "L'affichage genere un fichier svg a ouvrir manuellement" << std::endl;
        std::cout << "Quel graphe voulez-vous traiter ?" << std::endl;
        std::cout << "0. Quitter le programme" << std::endl;
        std::cout << "1. Broadway" << std::endl;
        std::cout << "2. Cubetown" << std::endl;
        std::cout << "3. Manhattan" << std::endl;
        std::cout << "4. Triville" << std::endl;
        std::cout << "5. Autre" << std::endl;
        std::cin >> choixGraphe;
    }while(choixGraphe < 0 || choixGraphe > 5);

    switch(choixGraphe)
    {
    case 0:
        return 0;
    case 1:
        g.lireSommet("graphes/broadway.txt");
        do
        {
            std::cout << "Quel fichier de poids voulez-vous charger ?" << std::endl;
            std::cout << "0, 1 ou 2"<<std::endl;
            std::cin >> choixPoids;
        }while(choixPoids < 0 || choixPoids > 2);
        if(choixPoids == 0)
        {
            g.lireArete("graphes/broadway_weights_0.txt");
        }
        if(choixPoids == 1)
        {
            g.lireArete("graphes/broadway_weights_1.txt");
        }
        if(choixPoids == 2)
        {
            g.lireArete("graphes/broadway_weights_2.txt");
        }
        break;
    case 2:
        g.lireSommet("graphes/cubetown.txt");
        g.lireArete("graphes/cubetown_weights_0.txt");
        break;
    case 3:
        g.lireSommet("graphes/manhattan.txt");
        do
        {
            std::cout << "Quel fichier de poids voulez-vous charger ?" << std::endl;
            std::cout << "0, 1 ou 2"<<std::endl;
            std::cin >> choixPoids;
        }while(choixPoids < 0 || choixPoids > 2);
        if(choixPoids == 0)
        {
            g.lireArete("graphes/manhattan_weights_0.txt");
        }
        if(choixPoids == 1)
        {
            g.lireArete("graphes/manhattan_weights_1.txt");
        }
        if(choixPoids == 2)
        {
            g.lireArete("graphes/manhattan_weights_2.txt");
        }
        break;
    case 4:
        g.lireSommet("graphes/triville.txt");
        do
        {
            std::cout << "Quel fichier de poids voulez-vous charger ?" << std::endl;
            std::cout << "0 ou 1"<<std::endl;
            std::cin >> choixPoids;
        }while(choixPoids < 0 || choixPoids > 1);
        if(choixPoids == 0)
        {
            g.lireArete("graphes/triville_weights_0.txt");
        }
        if(choixPoids == 1)
        {
            g.lireArete("graphes/triville_weights_1.txt");
        }
        break;

    case 5:

        std::string nom;
        std::string poids;

        std::cout << "Entrez le chemin d'accès du fichier graphe que vous voulez charger ?" << std::endl;
        std::cin >> nom;
        g.lireSommet(nom);

        std::cout << "Entrez le chemin d'accès du fichier poids que vous voulez charger ?" << std::endl;
        std::cin >> poids;
        g.lireArete(poids);
        break;
    }

    std::cout << "Chargement du graphe effectue" << std::endl;
    {
        Svgfile svgout("Graphe.svg");
        svgout.addGrid(100, true, "rgb(200, 200, 200)");
        g.dessinerGraphe(svgout);
        std::cout << "Graphe dessine" << std::endl<<std::endl;
    }

    int choixTraitement;
    std::vector<Possibilite*> tableauDesPossibles;

    std::vector<Graphe> prims;

    while(1)
    {
        int depart = 1;
        do
        {
            std::cout << "Quel traitement voulez-vous effectuer sur le Graphe ?"<<std::endl;
            std::cout << "0. Quitter le programme" << std::endl;
            std::cout << "1. Algorithme de Prim sur les deux criteres" << std::endl;
            std::cout << "2. Determination de la frontiere de Pareto en fonction des deux criteres" << std::endl;
            std::cout << "3. Determination de la frontiere de Pareto en prenant compte des temps de parcours" << std::endl;
            std::cout << "4. (Bonus) Frontiere de Pareto calculee a partir des arbres couvrants de Prim" << std::endl;
            std::cin >> choixTraitement;
        }while(choixTraitement < 0 || choixTraitement > 4);

        std::cout << std::endl<<std::endl;

        bool primFait = false;
        switch(choixTraitement)
        {
        case 0:
            return 0;
        case 1:
            depart = 1;
            for (int i=0; i<g.getNbrCritere(); ++i) // Effectue un prim sur chaque critere
            {
                Svgfile svgout("prim_" + std::to_string(i) + ".svg");
                svgout.addGrid(100, true, "rgb(200, 200, 200)");
                prims.push_back(g.algoPrim(depart,i));
                prims[i].dessinerGraphe(svgout);
                primFait = true;
            }
            std::cout << "Prim effectue en fonction de chaque critere" << std::endl;
            break;
        case 2:
            if(tableauDesPossibles.size() > 0)
            {
                tableauDesPossibles.clear();
            }
            tableauDesPossibles = maths::compteur_etat_possibles(g.getSommets().size()-1,g.getAretes().size(),&g);
            g.pareto(tableauDesPossibles);
            break;
        case 3:
            if(tableauDesPossibles.size() == 0)
            {
                tableauDesPossibles = maths::compteur_etat_possibles(g.getSommets().size()-1,g.getAretes().size(),&g);
                g.pareto(tableauDesPossibles);
            }
            g.pareto2(tableauDesPossibles);
            break;
        case 4:
            if(!primFait)
            {
                depart = 1;
                for (int i=0; i<g.getNbrCritere(); ++i) // Effectue un prim sur chaque critere
                {
                    Svgfile svgout("prim_" + std::to_string(i) + ".svg");
                    svgout.addGrid(100, true, "rgb(200, 200, 200)");
                    prims.push_back(g.algoPrim(depart,i));
                    prims[i].dessinerGraphe(svgout);
                }
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
            break;
        }
    }
    return 0;
}

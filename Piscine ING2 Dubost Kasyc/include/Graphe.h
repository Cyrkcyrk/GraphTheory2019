#ifndef GRAPHE_H
#define GRAPHE_H

#include <string>
#include <unordered_map>
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"
#include "Possibilite.h"

class Graphe
{
    public:
        Graphe();
        Graphe(std::string nomFichierSommet, std::string nomFichierArete);
        Graphe(std::vector<Sommet*> sommets, std::vector<Arete*> aretes);
        virtual ~Graphe();

        /// Lecture des sommets depuis un fichier
        void lireSommet(std::string nomFichierSommet);

        ///Lecture des aretes depuis un fichier
        void lireArete(std::string nomFichierArete);

        /// Dessiner le graphe
        void dessinerGraphe(Svgfile& svgout);

        /// Dessine la frontiere de pareto

        void dessinerPareto(std::string name = "pareto.svg");

        /// Algorithme de prim pour determiner les arbres couvrants de poids minimal en fonction d'un poids
        Graphe algoPrim(int depart, int critere) const;

        int getNbrCritere() const {return m_NbrCritere;};
        unsigned int getTaille() const {return m_aretes.size();};
        unsigned int getOrdre() const {return m_sommets.size();};

        void setPoidsMax(int nb, double poids) {this->m_poidsMax[nb] = poids;};
        double getPoidsMax(int nb) {return m_poidsMax[nb];};

        /// Ajoute un sommet a la map des sommets
        void addSommet(int id, double X, double Y, bool optimum = false);

        /// Tri les aretes par ordre croissant
        static bool triAretesCroissantes(Arete* a1, Arete* a2);

        /// Critere de tri du vecteur de possibilite pour pareto
        static bool sortCritere(Possibilite* a, Possibilite* b);

        /// Critere de tri du vecteur de possibilite pour pareto 2
        static bool sortCritere2(Possibilite* a, Possibilite* b);

        /// Frontiere de pareto
        void pareto(std::vector<Possibilite*>& tableauDesPossibles);

        /// Frontiere de pareto avec les solutions cycliques et l'algorithme de Dijkstra
        /// Si prim = true, il est impossible de faire un pareto 2 car prim est non cyclique
        void pareto2(std::vector<Possibilite*>& tableauDesPossibles, bool prim = false);

        /// DFS sur les sommets marques
        std::pair<bool,std::vector<int>*>* DFSM();

        /// Algorithme de dijkstra qui part d'un sommet de depart et renvoie la somme des distances a tous les points
        int algoDijkstra(int depart);

        /// Retourne la map d'aretes
        std::unordered_map<int,Arete*> getAretes(){return m_aretes;};

        /// Retourne la map de sommets
        std::unordered_map<int,Sommet*> getSommets(){return m_sommets;};

    private:
        int m_NbrCritere;
        std::unordered_map<int,Sommet*> m_sommets;
        std::unordered_map<int,Arete*> m_aretes;
        std::vector<std::vector<std::string>> m_tabDesPossibles;
        std::vector<double> m_poidsMax;
};

#endif // GRAPHE_H

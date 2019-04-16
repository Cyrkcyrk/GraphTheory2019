#ifndef GRAPHE_H
#define GRAPHE_H

#include <string>
#include <unordered_map>
#include "sommet.h"
#include "arete.h"
#include "svgfile.h"

class Graphe
{
    public:
        Graphe();
        Graphe(std::string nomFichierSommet, std::string nomFichierArete);
        Graphe(std::vector<Sommet*> sommets, std::vector<Arete*> aretes);
        virtual ~Graphe();

        void lireSommet(std::string nomFichierSommet);
        void lireArete(std::string nomFichierArete);
        void afficherGraphe(Svgfile& svgout);

        void compteurBinaire();

        Graphe algoPrim(int depart, int critere) const;

        int getNbrCritere() const {return m_NbrCritere;};
        unsigned int getTaille() const {return m_aretes.size();};
        unsigned int getOrdre() const {return m_sommets.size();};

        void addSommet(int id, double X, double Y, bool optimum = false);

        int rechercher_afficherToutesCC();
        void pareto();
        void parcoursDFS(int id) const;

        std::unordered_map<int,Arete*> getAretes(){return m_aretes;};
        std::unordered_map<int,Sommet*> getSommets(){return m_sommets;};

        std::pair<bool,std::vector<int>> DFSM();

    private:
        int m_NbrCritere;
        std::unordered_map<int,Sommet*> m_sommets;
        std::unordered_map<int,Arete*> m_aretes;
        std::vector<std::vector<std::string>> m_tabDesPossibles;
};

#endif // GRAPHE_H

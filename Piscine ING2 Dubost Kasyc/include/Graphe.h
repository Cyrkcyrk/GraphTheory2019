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

        void palero();

    private:
        int m_NbrCritere;
        std::unordered_map<int,Sommet*> m_sommets;
        std::unordered_map<int,Arete*> m_aretes;
        std::vector<std::vector<std::string>> m_tabDesPossibles;
};

#endif // GRAPHE_H

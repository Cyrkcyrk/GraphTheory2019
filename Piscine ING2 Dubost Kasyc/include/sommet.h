#ifndef SOMMET_H
#define SOMMET_H

#include <vector>
#include "arete.h"
#include "Coord.h"
#include <unordered_set>
class Sommet
{
    public:
        Sommet(int id, double x, double y, bool optimum = false);
        virtual ~Sommet();

        void ajouterVoisin(Sommet* s);
        void ajouterArete(Arete* a);

        int getId()const{return m_id;};

        void ajouter(){m_ajoute = true;};
        void retirer(){m_ajoute = false;};
        bool isAjoute()const{return m_ajoute;};
        bool isOptimum()const{return m_optimum;};

        Coord getCoord() const {return m_coord;};
        std::pair<Arete*, Sommet*> getProcheVoisin(int critere);
        std::vector<const Sommet*> getVoisins()const{return m_voisins;};
        Arete* getArete(const Sommet* s2)const;

        std::unordered_set<const Sommet*> rechercherCC(std::unordered_set<const Sommet*> cc) const;
        std::pair<bool,std::vector<int>> DFSM(unsigned int nbSommets) const;
    private:
        int m_id;
        Coord m_coord;
        std::vector<const Sommet*> m_voisins;
        std::vector<Arete*> m_aretes;
        bool m_ajoute;
        bool m_optimum;

};

#endif // SOMMET_H

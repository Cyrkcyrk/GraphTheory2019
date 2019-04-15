#ifndef SOMMET_H
#define SOMMET_H

#include <vector>
#include "arete.h"

class Sommet
{
    public:
        Sommet(int id, double x, double y);
        virtual ~Sommet();

        void ajouterVoisin(Sommet* s);
        void ajouterArete(Arete* a);

        int getId()const{return m_id;};
        void ajouter(){m_ajoute = true;};
        void retirer(){m_ajoute = false;};
        bool isAjoute()const{return m_ajoute;};

        std::pair<Arete*, Sommet*> getProcheVoisin();

    private:
        int m_id;
        double m_x;
        double m_y;
        std::vector<const Sommet*> m_voisins;
        std::vector<Arete*> m_aretes;
        bool m_ajoute;
};

#endif // SOMMET_H

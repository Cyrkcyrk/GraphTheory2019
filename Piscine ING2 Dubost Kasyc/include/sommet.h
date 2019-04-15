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

    private:
        int m_id;
        double m_x;
        double m_y;
        std::vector<Sommet*> m_voisins;
};

#endif // SOMMET_H

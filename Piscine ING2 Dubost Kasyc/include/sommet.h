#ifndef SOMMET_H
#define SOMMET_H

#include <vector>
#include "arete.h"
#include "Coord.h"

class Sommet
{
    public:
        Sommet(int id, double x, double y);
        virtual ~Sommet();

        void ajouterVoisin(Sommet* s);
        void ajouterArete(Arete* a);

        int getId()const{return m_id;};
        Coord getCoord() const {return m_coord;};

    private:
        int m_id;
        Coord m_coord;
        std::vector<Sommet*> m_voisins;
};

#endif // SOMMET_H

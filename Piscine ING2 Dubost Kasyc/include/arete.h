#ifndef ARETE_H
#define ARETE_H

#include <vector>
#include "Coord.h"

class Sommet;

class Arete
{
    public:
        Arete(int id, Sommet* s1, Sommet* s2);
        virtual ~Arete();

        void addPoids(std::vector<double> tabPoids);

        const int getId()const {return m_id;};
        Sommet* getS1()const{return m_sommet1;};
        Sommet* getS2()const{return m_sommet2;};
        const std::vector<double> getPoids()const{return m_poids;};
        Sommet* getAutreSommet(Sommet* s){if (m_sommet1 != s) return m_sommet1; else return m_sommet2;};
        std::vector<Coord> getCoord() const;

    private:
        int m_id;
        Sommet* m_sommet1;
        Sommet* m_sommet2;
        std::vector<double> m_poids;
};

#endif // ARETE_H

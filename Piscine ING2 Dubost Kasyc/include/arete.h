#ifndef ARETE_H
#define ARETE_H

#include <vector>
#include "Coord.h"
#include "svgfile.h"

class Sommet;

class Arete
{
    public:
        Arete(int id, Sommet* s1, Sommet* s2);
        virtual ~Arete();

        void addPoids(std::vector<double> tabPoids);

        int getId()const {return m_id;};
        Sommet* getS1()const{return m_sommet1;};
        Sommet* getS2()const{return m_sommet2;};
        std::vector<double> getPoids()const{return m_poids;};
        Sommet* getAutreSommet(Sommet* s){if (m_sommet1 != s) return m_sommet1; else return m_sommet2;};
        std::vector<Coord> getCoord() const;

        bool isAjoute()const{return m_marque;};
        void ajouter(){m_marque = true;};
        void retirer(){m_marque = false;};

        void dessiner(Svgfile& svgout, bool afficher_poids = false);

    private:
        int m_id;
        Sommet* m_sommet1;
        Sommet* m_sommet2;
        std::vector<double> m_poids;
        bool m_marque;
};

#endif // ARETE_H

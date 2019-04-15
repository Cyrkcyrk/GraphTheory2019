#ifndef ARETE_H
#define ARETE_H

#include <vector>

class Sommet;

class Arete
{
    public:
        Arete(int id, Sommet* s1, Sommet* s2);
        virtual ~Arete();

        void addPoids(std::vector<double> tabPoids);

    private:
        int m_id;
        Sommet* m_sommet1;
        Sommet* m_sommet2;
        std::vector<double> m_poids;
};

#endif // ARETE_H

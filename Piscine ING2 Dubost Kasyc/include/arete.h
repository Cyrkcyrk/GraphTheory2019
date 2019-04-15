#ifndef ARETE_H
#define ARETE_H

#include "sommet.h"

class Arete
{
    public:
        Arete();
        virtual ~Arete();

    private:
        Sommet* m_sommet1;
        Sommet* m_sommet2;
        std::vector<double> poids;
};

#endif // ARETE_H

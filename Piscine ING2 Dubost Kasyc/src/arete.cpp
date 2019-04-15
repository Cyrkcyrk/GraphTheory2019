#include "arete.h"

Arete::Arete(int id, Sommet* s1, Sommet* s2)
    : m_id(id), m_sommet1(s1), m_sommet2(s2)
{

}

Arete::~Arete()
{
    //dtor
}

void Arete::addPoids(std::vector<double> tabPoids)
{
    m_poids = tabPoids;
}

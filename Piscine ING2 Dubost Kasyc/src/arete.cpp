#include "arete.h"
#include "sommet.h"
#include <iostream>

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

std::vector<Coord> Arete::getCoord() const
{
    std::vector<Coord> retour;

    retour.push_back(this->m_sommet1->getCoord());
    retour.push_back(this->m_sommet2->getCoord());

    return retour;
}

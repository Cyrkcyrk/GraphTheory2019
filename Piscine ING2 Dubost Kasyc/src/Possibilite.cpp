#include "Possibilite.h"

Possibilite::Possibilite(std::vector<char>* binaire, std::vector<int>* poids)
    : m_binaire(binaire), m_poids(poids), m_poidsDij(0)
{
    //ctor
}

Possibilite::~Possibilite()
{
    //dtor
}

#include "Possibilite.h"
#include <iostream>
Possibilite::Possibilite(std::string& binaire, std::vector<int>* poids)
    : m_binaire(binaire), m_poids(poids), m_poidsDij(0)
{
    //ctor
}

Possibilite::~Possibilite()
{
    //dtor
}

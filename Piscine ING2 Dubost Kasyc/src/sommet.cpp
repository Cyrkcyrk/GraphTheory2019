#include "sommet.h"

Sommet::Sommet(int id, double x, double y)
    : m_id(id), m_coord(Coord(x, y)),m_ajoute(false)
{

}

Sommet::~Sommet()
{
    //dtor
}


void Sommet::ajouterVoisin(Sommet* s)
{
    m_voisins.push_back(s);
}


void Sommet::ajouterArete(Arete* a)
{
    m_aretes.push_back(a);
}

std::pair<Arete*, Sommet*> Sommet::getProcheVoisin(int critere)
{
    Sommet* s1 = nullptr;
    Arete* a1 = new Arete(-1,new Sommet(-1,20,20),new Sommet(-1,20,20));
    a1->addPoids({10000,10000});
    bool vide = true;
    if (m_aretes.size() == 0)
    {
        return{nullptr,nullptr};
    }

    for(auto v : m_voisins)
    {
        if(!v->isAjoute())
        {
            vide = false;
            break;
        }
    }

    if (vide)
    {
       return{nullptr,nullptr};
    }

    for(auto arete : m_aretes)
    {
        if (arete->getPoids()[critere] < a1->getPoids()[critere] && !arete->getAutreSommet(this)->isAjoute() && arete!=nullptr)
        {
            a1 = arete;
            s1 = arete->getAutreSommet(this);
        }
    }
    return {a1,s1};
}

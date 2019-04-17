#include "sommet.h"
#include <stack>
#include <iostream>

Sommet::Sommet(int id, double x, double y, bool optimum)
    : m_id(id), m_coord(Coord(x, y)),m_ajoute(false), m_optimum(optimum)
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

std::unordered_set<const Sommet*> Sommet::rechercherCC(std::unordered_set<const Sommet*> cc) const{
    std::unordered_set<const Sommet*> tempUnSet;
    if (cc.find(this) == cc.end())
    {
        cc.insert(this);
        for(auto voisin : m_voisins)
        {
            tempUnSet = voisin->rechercherCC(cc);
            for(auto temp : tempUnSet)
            {
                cc.insert(temp);
            }
        }
    }

    return cc;
}

std::pair<bool,std::vector<int>> Sommet::DFSM(unsigned int nbSommets) const
{
    std::unordered_set<const Sommet*> decouverts;
    std::stack<const Sommet*> pile;

    std::vector<int> poidsChemin;
    poidsChemin.push_back(0);
    poidsChemin.push_back(0);

    pile.push(this);
    const Sommet* Ec;
    while (!pile.empty())
    {
        Ec = pile.top();
        pile.pop();
        for(const Sommet* s : Ec->getVoisins())
        {
            if(Ec->getArete(s)!=NULL && Ec->getArete(s)->isAjoute() && decouverts.find(s) == decouverts.end())
            {
                poidsChemin[0] += Ec->getArete(s)->getPoids()[0];
                poidsChemin[1] += Ec->getArete(s)->getPoids()[1];
                pile.push(s);
            }
        }
        decouverts.insert(Ec);
    }
    if(decouverts.size() == nbSommets)
    {
        return std::make_pair(true,poidsChemin);
    }
    else
    {
        return std::make_pair(false,poidsChemin);
    }
}

Arete* Sommet::getArete(const Sommet* s) const
{
    for(auto arete : m_aretes)
    {
        if(arete->getS1() == s || arete->getS2() == s)
        {
            return arete;
        }
    }
    return NULL;
}

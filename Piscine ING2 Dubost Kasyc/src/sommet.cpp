#include "sommet.h"
#include <stack>
#include <iostream>


Sommet::Sommet(int id, double x, double y, bool optimum)
    : m_id(id), m_coord(Coord(x, y)),m_ajoute(false), m_optimum(optimum), m_distance{100000}
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

std::pair<bool,std::vector<int>*>* Sommet::DFSM(unsigned int nbSommets)
{
    std::unordered_set<Sommet*> decouverts;
    std::stack<Sommet*> pile;

    std::vector<int>* poidsChemin = new std::vector<int>;
    poidsChemin->push_back(0);
    poidsChemin->push_back(0);

    pile.push(this);
    Sommet* Ec = nullptr;
    while (!pile.empty())
    {
        Ec = pile.top();
        pile.pop();
        for(Sommet* s : Ec->getVoisins())
        {
            if(decouverts.find(s) == decouverts.end() && Ec->getArete(s)->isAjoute())
            {
                Arete* a = Ec->getArete(s);
                poidsChemin->at(0) += a->getPoids()[0];
                poidsChemin->at(1) += a->getPoids()[1];
                pile.push(s);
            }
        }
        decouverts.insert(Ec);
    }
    if(decouverts.size() == nbSommets)
    {
        std::pair<bool,std::vector<int>*>* a = new std::pair<bool,std::vector<int>*>;
        *a = std::make_pair(true,poidsChemin);
        return a;
    }
    else
    {
        std::pair<bool,std::vector<int>*>* a = new std::pair<bool,std::vector<int>*>;
        *a = std::make_pair(false,poidsChemin);
        return a;
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


int Sommet::getDistancePlusProcheVoisin()
{
    for(auto aretes :m_aretes)
    {
        if(aretes->getAutreSommet(this) == this->getPlusProcheVoisin())
        {
            return aretes->getPoids()[1];
        }
    }
    return 1000000;
}

Sommet* Sommet::getPlusProcheVoisin()
{
    int d = 1000000;
    Sommet* s = nullptr;
    for(auto arete : m_aretes)
    {
        if(arete->getPoids()[1] < d)
        {
            d = arete->getPoids()[1];
            s = arete->getAutreSommet(this);
        }
    }
    return s;
}


void Sommet::dessiner(Svgfile& svgout, double coefX, double coefY, bool afficher_texte, double rayon, std::string couleur, double opacity )
{
    svgout.addCircle(m_coord.getX()*coefX, m_coord.getY()*coefY, rayon, couleur, opacity);
    if (afficher_texte)
    {
        svgout.addText(m_coord.getX()*coefX,m_coord.getY()*coefY+rayon/2, std::to_string(m_id), POINT_TEXT, 9, true);
    }
}

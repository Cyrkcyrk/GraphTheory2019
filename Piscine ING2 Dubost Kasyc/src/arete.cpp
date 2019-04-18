#include "arete.h"
#include "sommet.h"
#include <iostream>

#include <iomanip>
#include <sstream>
#include <math.h>
#include "conf.h"

Arete::Arete(int id, Sommet* s1, Sommet* s2)
    : m_id(id), m_sommet1(s1), m_sommet2(s2), m_marque(false) /*,m_poidsCoords(sqrt(pow(s1->getCoord().getX()-s2->getCoord().getX(),2)+(pow(s1->getCoord().getY()-s2->getCoord().getY(),2))))*/
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

//svgout.addLine(tmp[0].getX(), tmp[0].getY(), tmp[1].getX(), tmp[1].getY(), TRAIT_EPAISSEUR, TRAIT_COULEUR);

void Arete::dessiner(Svgfile& svgout,  bool afficher_poids)
{
    svgout.addLine(this->m_sommet1->getCoord().getX(), this->m_sommet1->getCoord().getY(),
                   this->m_sommet2->getCoord().getX(), this->m_sommet2->getCoord().getY(),
                   TRAIT_EPAISSEUR, TRAIT_COULEUR);

    if (afficher_poids)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1);
        for (unsigned int i = 0; i< this->m_poids.size(); ++i)
        {
            stream << this->m_poids[i];

            if (i < this->m_poids.size()-1)
            {
                stream << " / ";
            }
        }

        Coord milieu = Coord((this->m_sommet1->getCoord().getX() + this->m_sommet2->getCoord().getX())/2, (this->m_sommet1->getCoord().getY() + this->m_sommet2->getCoord().getY())/2);
        svgout.addText(milieu.getX(), milieu.getY(), stream.str(), "rgb(50, 50, 50)", 11, true);
    }
}

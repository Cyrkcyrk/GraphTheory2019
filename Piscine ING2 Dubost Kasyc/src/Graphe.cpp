#include "Graphe.h"
#include "svgfile.h"
#include "conf.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include "maths.h"
#include <unordered_set>

Graphe::Graphe(std::string nomFichierSommet, std::string nomFichierArete)
{
    lireSommet(nomFichierSommet);
    lireArete(nomFichierArete);
}

Graphe::Graphe(std::vector<Sommet*> sommets, std::vector<Arete*> aretes)
{
    for(auto s : sommets)
    {
        m_sommets.insert({s->getId(),s});
    }
    for(auto a  : aretes)
    {
        m_aretes.insert({a->getId(),a});
    }
}

Graphe::~Graphe()
{
    //dtor
}

void Graphe::afficherGraphe(Svgfile& svgout)
{
    for(auto a : m_aretes)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1);
        for (unsigned int i = 0; i< a.second->getPoids().size(); ++i)
        {
            stream << a.second->getPoids()[i];

            if (i < a.second->getPoids().size()-1)
            {
                stream << "___";
            }
        }


        std::vector<Coord> tmp = a.second->getCoord();
        svgout.addLine(tmp[0].getX(), tmp[0].getY(), tmp[1].getX(), tmp[1].getY(), TRAIT_EPAISSEUR, TRAIT_COULEUR);

        Coord milieu = Coord((tmp[0].getX() + tmp[1].getX())/2, (tmp[0].getY() + tmp[1].getY())/2);
        svgout.addText(milieu.getX(), milieu.getY(), stream.str(), "rgb(50, 50, 50)");

    }
    std::cout<<std::endl;
    for(auto s : m_sommets)
    {

        std::cout << s.second->getId() << " ";

        Coord tmp = s.second->getCoord();



        svgout.addCircle(tmp.getX(), tmp.getY(), POINT_RAYON, POINT_COULEUR);
        svgout.addText(tmp.getX(), tmp.getY()+POINT_RAYON/2, std::to_string(s.second->getId()), POINT_TEXT);
    }


}

void Graphe::lireSommet(std::string nomFichier)
{
    /// CODE DU TP 2
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );
    int ordre;
    ifs >> ordre;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture ordre du graphe");
    int id;
    double x,y;
    //lecture des sommets
    for (int i=0; i<ordre; ++i){
        ifs>>id; if(ifs.fail()) throw std::runtime_error("Probleme lecture données sommet");
        ifs>>x; if(ifs.fail()) throw std::runtime_error("Probleme lecture données sommet");
        ifs>>y; if(ifs.fail()) throw std::runtime_error("Probleme lecture données sommet");
        m_sommets.insert({id,new Sommet{id,x,y}});
    }
    int taille;
    ifs >> taille;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");
    int ids1;
    int ids2;
    //lecture des aretes
    for (int i=0; i<taille; ++i){
        //lecture des ids des deux extrémités
        ifs>>id; if(ifs.fail()) throw std::runtime_error("Probleme lecture arete sommet 1");
        ifs>>ids1; if(ifs.fail()) throw std::runtime_error("Probleme lecture arete sommet 1");
        ifs>>ids2; if(ifs.fail()) throw std::runtime_error("Probleme lecture arete sommet 2");

        Arete* arete = new Arete{id,(m_sommets.find(ids1))->second,m_sommets.find(ids2)->second};
        m_aretes.insert({id,arete});

        //ajouter chaque extrémité à la liste des voisins de l'autre (graphe non orienté)
        (m_sommets.find(ids1))->second->ajouterVoisin((m_sommets.find(ids2))->second);
        (m_sommets.find(ids2))->second->ajouterVoisin((m_sommets.find(ids1))->second);
        (m_sommets.find(ids1))->second->ajouterArete(arete);
        (m_sommets.find(ids2))->second->ajouterArete(arete);
    }
}

void Graphe::lireArete(std::string nomFichier)
{
    std::ifstream ifs{nomFichier};
    if (!ifs)
        throw std::runtime_error( "Impossible d'ouvrir en lecture " + nomFichier );

    int taille;
    ifs >> taille;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture taille du graphe");

    int nbPoids;
    ifs >> nbPoids;
    if ( ifs.fail() )
        throw std::runtime_error("Probleme lecture du nombre de poids");

    this->m_NbrCritere = nbPoids;

    double poids;
    int id;
    //lecture des sommets
    for(int i=0;i<taille;i++)
    {
        std::vector<double> tabPoids;
        ifs>>id; if(ifs.fail()) throw std::runtime_error("Probleme lecture id arete");
        for (int i=0; i<nbPoids; ++i)
        {
            ifs>>poids; if(ifs.fail()) throw std::runtime_error("Probleme lecture poids arete");
            tabPoids.push_back(poids);
        }
        m_aretes.find(id)->second->addPoids(tabPoids);
    }
}


Graphe Graphe::algoPrim(int depart, int critere) const
{
    std::vector<Sommet*> chemin;
    std::vector<Arete*> aretes;

    for(auto s : m_sommets)
    {
        s.second->retirer();
    }

    m_sommets.find(depart)->second->ajouter();
    chemin.push_back(m_sommets.find(depart)->second);
    unsigned int i=1;

    Arete* b1 = new Arete(100000,new Sommet(-1,1,1),new Sommet(-1,1,1));
    Sommet* b2 = new Sommet(-1,1,1);
    Sommet* b3 = new Sommet(-1,1,1);
    Arete *b4 = new Arete(10000,new Sommet(-1,1,1),new Sommet(-1,1,1));
    b1->addPoids({1000,10000});
    b4->addPoids({1000,10000});
    Arete* potAretePoidsMin;
    Arete* aretePoidsMin;
    Sommet* sommetPoidsMin;
    Sommet* potSommetPoidsMin;
    bool trouve =false;

    while(i<m_sommets.size())
    {
        potAretePoidsMin = b1;
        potSommetPoidsMin = b2;
        sommetPoidsMin = b3;
        aretePoidsMin = b4;

        trouve = false;
        for(auto s : m_sommets)
        {
            if(s.second->isAjoute())
            {

                potSommetPoidsMin = s.second->getProcheVoisin(critere).second;
                potAretePoidsMin = s.second->getProcheVoisin(critere).first;
                if(potSommetPoidsMin == nullptr)
                {
                    continue;
                }
                if(potAretePoidsMin->getPoids()[critere] < aretePoidsMin->getPoids()[critere])
                {

                    sommetPoidsMin = potSommetPoidsMin;
                    aretePoidsMin = potAretePoidsMin;
                    trouve = true;
                }
            }
        }
        if (trouve)
        {
            if(aretePoidsMin->getS1()->isAjoute() && !aretePoidsMin->getS2()->isAjoute())
            {
                aretePoidsMin->getS2()->ajouter();
                chemin.push_back(sommetPoidsMin);
                aretes.push_back(aretePoidsMin);
                i++;
            }
            if(aretePoidsMin->getS2()->isAjoute() && !aretePoidsMin->getS1()->isAjoute())
            {
                aretePoidsMin->getS1()->ajouter();
                chemin.push_back(sommetPoidsMin);
                aretes.push_back(aretePoidsMin);
                i++;
            }
        }
    }
    std::cout<<std::endl;

    for(unsigned int i=0;i<chemin.size();i++)
    {
        std::cout << chemin[i]->getId() << " ";
        if (i<chemin.size()-1)
        {
            std::cout << "<--" << aretes[i]->getPoids()[critere] << "-- ";
        }
    }

    Graphe g(chemin,aretes);
    return g;
}


void Graphe::palero()
{

    std::vector<std::vector<char>> tableauDesPossibles = maths::combinaisonsDe1(m_sommets.size(),m_aretes.size()); // Tableau des possibles

    std::unordered_set<Sommet*> sommetAjoute;

    for(unsigned int i=0;i<tableauDesPossibles.size();i++) // Pour chaque possibilite
    {
        sommetAjoute.clear();
        for(unsigned int j=0;j<tableauDesPossibles[i].size();j++) // Pour chaque arete
        {
            if (tableauDesPossibles[i][j] == '1') // Si l'arete est selectionnee
            {
                sommetAjoute.insert(m_aretes[j]->getS1()); // On marque les sommets aux extremites de l'arete
                sommetAjoute.insert(m_aretes[j]->getS2()); // On marque les sommets aux extremites de l'arete
            }
        }
        if(sommetAjoute.size() != m_sommets.size())
        {
            tableauDesPossibles.erase(tableauDesPossibles.begin()+i);
        }
    }

    std::cout <<tableauDesPossibles.size() <<std::endl;
}


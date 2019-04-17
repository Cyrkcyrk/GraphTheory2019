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
#include <algorithm>
#include <queue>

Graphe::Graphe()
{
}

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
    for(auto s : m_sommets)
    {
        Coord tmp = s.second->getCoord();


        if(s.second->isOptimum())
        {
            svgout.addCircle(tmp.getX()*3, tmp.getY()*3, POINT_RAYON, POINT_COULEUR_OPTIMUM);
            std::stringstream stream;
            stream << std::fixed << std::setprecision(1);
            stream << s.second->getCoord().getX();
            stream << "  ";
            stream << s.second->getCoord().getY();

            svgout.addText(tmp.getX()*3, tmp.getY()*3, stream.str(), "rgb(50, 50, 50)");
        }
        else svgout.addCircle(tmp.getX()*3, tmp.getY()*3, POINT_RAYON, POINT_COULEUR);
        //svgout.addText(tmp.getX(), tmp.getY()+POINT_RAYON/2, std::to_string(s.second->getId()), POINT_TEXT);
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
        for (int j=0; j<nbPoids; ++j)
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

    Graphe g(chemin,aretes);
    return g;
}


int Graphe::rechercher_afficherToutesCC()
{
    int i=0;
    std::unordered_set<const Sommet*> newConnex;
    std::unordered_set<const Sommet*> decouverts;
    std::vector<std::unordered_set<const Sommet*>> composConnex;
    for(auto s : m_sommets) // Pour chaque sommet du graphe
    {
        if (decouverts.find(s.second) == decouverts.end()) // Non decouvert
        {
            newConnex = s.second->rechercherCC(newConnex); // Recherhe tous sommets composante connexe
            for(auto newS : newConnex)
            {
                if(decouverts.find(newS) == decouverts.end())
                {
                    decouverts.insert(newS);
                }
            }
            composConnex.push_back(s.second->rechercherCC(newConnex)); // Ajoute la compo au tab de compo
            newConnex.erase(newConnex.begin(),newConnex.end());
            i++;
        }
    }
    composConnex.push_back(newConnex);
    std::cout << "nombre cc :" << i << std::endl;
    return i;
}

bool sortCritere(std::pair<std::vector<char>,std::vector<int>>& a, std::pair<std::vector<char>,std::vector<int>>& b)
{
    if (a.second[0] < b.second[0])
    {
        return true;
    }
    else if(a.second[0] == b.second[0])
    {
        return a.second[1] < b.second[1];
    }
    else return false;
}

void Graphe::pareto()
{
    //maths::compteur_etat_possibles(m_sommets.size()-1,m_aretes.size(),this); // Tableau des possibles

    std:: cout << "Sommet : " << m_sommets.size() << " - Arrete : " << m_aretes.size() << std::endl;


    std::vector<std::pair<std::vector<char>,std::vector<int>>> tableauDesPossibles = maths::compteur_etat_possibles(m_sommets.size()-1,m_aretes.size(),this); // Tableau des possibles

    std::cout <<"Nbr possibilites : "<< tableauDesPossibles.size() << std::endl;

    Graphe pareto;

    std::cout<<"Sorting..."<<std::endl;
    std::sort(tableauDesPossibles.begin(), tableauDesPossibles.end(),sortCritere);
    std::cout<<"Fin du tri"<<std::endl;
    std::cout << tableauDesPossibles[tableauDesPossibles.size()-1].second[0] << " ";
    std::cout << tableauDesPossibles[tableauDesPossibles.size()-1].second[1] << std::endl;
    double maxY = tableauDesPossibles[0].second[1];
    std::vector<int> xOptim;
    xOptim.push_back(tableauDesPossibles[0].second[0]);
    pareto.addSommet(0, tableauDesPossibles[0].second[0], tableauDesPossibles[0].second[1],true);
    std::cout << xOptim[0];
    for(unsigned int i=1;i<tableauDesPossibles.size()-1;i++)
    {
        if(tableauDesPossibles[i].second[1]<maxY)
        {
            bool memeX = false;
            for(auto x : xOptim)
            {
                if(x==tableauDesPossibles[i].second[0])
                {
                    memeX = true;
                }
            }
            if(!memeX)
            {
                xOptim.push_back(tableauDesPossibles[i].second[0]);
                maxY = tableauDesPossibles[i].second[1];
                pareto.addSommet(i, tableauDesPossibles[i].second[0], tableauDesPossibles[i].second[1],true);
            }
            else
            {
                pareto.addSommet(i, tableauDesPossibles[i].second[0], tableauDesPossibles[i].second[1]);
            }
        }
        else
        {
            pareto.addSommet(i, tableauDesPossibles[i].second[0], tableauDesPossibles[i].second[1]);
        }
    }

    int poids2 = 0;
    for(auto p : tableauDesPossibles) // Pour chaque possibilite
    {
        for(unsigned int i=0;i<m_aretes.size();i++) // Pour chaque sommet
        {
            if(p.first[i] == 1) // Ajoute ou non l'arete
            {
                m_aretes[i]->ajouter();
            }
            else m_aretes[i]->retirer();
        }
        for(auto s : m_sommets)
        {
            poids2 += this->algoDijkstra(s.second->getId());
        }
        std::cout << poids2 << "  ";
    }

    Svgfile SVGPareto;
    pareto.afficherGraphe(SVGPareto);

    //delete(tableauDesPossibles);
/*
    for (int i = 0; i< tableauDesPossibles.size(); i++)
    {
        std::cout << std::endl << i << " : ";
        for (int j=0; j< tableauDesPossibles[i].size(); j++)
        {
            std::cout << int(tableauDesPossibles[i][j]);
        }
    }
    std::cout<<"Recherche des connexes" <<std::endl;
    std::unordered_set<Sommet*> sommetAjoute;

    for(unsigned int i=0;i<tableauDesPossibles.size();i++) // Pour chaque possibilite
    {
        for(unsigned int j=0;j<tableauDesPossibles[i].size();j++) // Pour chaque arete
        {
            if((int)tableauDesPossibles[i][j] == 1) // Si l'arete est selectionnee
            {
                m_aretes[j]->ajouter();
            }
        }

        if(sommetAjoute.size() != m_sommets.size())
        {
            tableauDesPossibles.erase(tableauDesPossibles.begin()+i);
        }
    }
    for (int i = 0; i< tableauDesPossibles.size(); i++)
    {
        std::cout << std::endl << i << " : ";
        for (int j=0; j< tableauDesPossibles[i].size(); j++)
        {
            std::cout << int(tableauDesPossibles[i][j]);
        }
    }
*/

}

std::pair<bool,std::vector<int>> Graphe::DFSM() //DFS Marque
{
    Sommet*s0 = m_sommets[0];
    int nbSommetM = 0;
    for(auto s : m_sommets) // Compte les sommet marque
    {
        if(s.second->isAjoute())
        {
            nbSommetM++;
        }
    }
    return s0->DFSM(nbSommetM);
}


void Graphe::addSommet(int id, double X, double Y, bool optimum)
{
    m_sommets.insert({id, new Sommet(id, X, Y, optimum)});
}



bool triAretesCroissantes(Arete* a1, Arete* a2)
{
    return a1->getPoids() > a2->getPoids();
}

int Graphe::algoDijkstra(int depart)
{
    for(auto s : m_sommets) // Trie toutes les aretes dans les sommets par ordre croissant
    {
        std::sort(s.second->getAretes()->begin(), s.second->getAretes()->end(),triAretesCroissantes);
    }

    auto cmp = [](Sommet* s1, Sommet* s2)
    {
        return s1->getD()+s1->getDistancePlusProcheVoisin() > s2->getD()+s2->getDistancePlusProcheVoisin();
    };
    std::priority_queue<Sommet*, std::vector<Sommet*>, decltype(cmp)> myqueue(cmp);

    std::unordered_map<Sommet*,int> dist;
    std::unordered_set<Sommet*> decouverts;

    for(auto s : m_sommets)
    {
         if(s.second == m_sommets.at(depart))
         {
             dist.insert({s.second, 0});
             s.second->setDistance(0);
         }
         else
         {
             dist.insert({s.second, 100000});
             s.second->setDistance(100000);
         }
    }

    myqueue.push(m_sommets.at(depart));
    Sommet* s;

    while(!myqueue.empty())
    {
        s = myqueue.top();

        if(decouverts.find(myqueue.top())==decouverts.end())
        {
            decouverts.insert(myqueue.top());
        }
        myqueue.pop();
        //std::cout <<s->getId()<< " : " ;
        for(auto voisin : s->getVoisins())
        {
            if (decouverts.find(voisin) == decouverts.end() && s->getArete(voisin)->isAjoute())
            {
                myqueue.push(voisin);
                //std::cout<<myqueue.top()->getId()<<" ";
                if(dist.at(s)+s->getArete(voisin)->getPoids()[0]<dist.at(voisin))
                {
                    //std::cout << "Ajouter " << voisin->getId() << " depuis " << s->getId() << " coute "<<dist.at(s)+s->getArete(voisin)->getPoids()[0]<<std::endl;
                    dist.at(voisin) = dist.at(s)+s->getArete(voisin)->getPoids()[0];
                    voisin->setDistance(dist.at(s)+s->getArete(voisin)->getPoids()[0]);
                }
            }

        }
    }
    int somme = 0;
    for(auto a : dist)
    {
        somme+=a.second;
    }
    return somme;
}


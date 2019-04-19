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

void Graphe::dessinerGraphe(Svgfile& svgout)
{
    for(auto a : m_aretes)
    {
        a.second->dessiner(svgout, true);
    }

    for(auto s : m_sommets)
    {
        s.second->dessiner(svgout, 1, true);
    }
}

void Graphe::dessinerPareto(Svgfile& svgout)
{
    std::vector<Sommet*> frontiere;

    for(auto s : m_sommets)
    {
        if(s.second->isOptimum())
        {
            frontiere.push_back(s.second);
        }
        else
        {
            s.second->dessiner(svgout, 1, false, POINT_PARETO_RAYON, POINT_COULEUR, 0.1);
        }
    }

    for(auto s : frontiere)
    {
        svgout.addCircle(s->getCoord().getX()*5, s->getCoord().getY(), POINT_PARETO_RAYON, POINT_PARETO_COULEUR_OPTIMUM, PARETO_FONT_SIZE);

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1)
               << s->getCoord().getX()
               << " / "
               << s->getCoord().getY();

        svgout.addText(s->getCoord().getX()*5, s->getCoord().getY(), stream.str(), "rgb(50, 50, 50)", PARETO_FONT_SIZE);
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


bool sortCritere(Possibilite* a, Possibilite* b)
{
    if (a->getPoids()->at(0) < b->getPoids()->at(0))
    {
        return true;
    }
    else if(a->getPoids()->at(0) == b->getPoids()->at(0))
    {
        return a->getPoids()->at(1) < b->getPoids()->at(1);
    }
    else return false;
}

bool sortCritere2(Possibilite* a, Possibilite* b)
{
    if (a->getPoidsDij() < b->getPoidsDij())
    {
        return true;
    }
    else if(a->getPoidsDij() == b->getPoidsDij())
    {
        return a->getPoids()->at(0) < b->getPoids()->at(0);
    }
    else return false;
}


void Graphe::pareto(std::vector<Possibilite*>& tableauDesPossibles, bool paretoPrim)
{
    //maths::compteur_etat_possibles(m_sommets.size()-1,m_aretes.size(),this); // Tableau des possibles

    std:: cout << "Sommet : " << m_sommets.size() << " - Arrete : " << m_aretes.size() << std::endl;

    std::cout <<"Nbr possibilites : "<< tableauDesPossibles.size() << std::endl;

    Graphe pareto;

    std::cout<<"Sorting..."<<std::endl;
    std::sort(tableauDesPossibles.begin(), tableauDesPossibles.end(),sortCritere);
    std::cout<<"Fin du tri"<<std::endl;

    double maxY = tableauDesPossibles[0]->getPoids()->at(1);
    std::vector<int> xOptim;
    xOptim.push_back(tableauDesPossibles[0]->getPoids()->at(0));
    pareto.addSommet(0, tableauDesPossibles[0]->getPoids()->at(0), tableauDesPossibles[0]->getPoids()->at(1),true);
    for(unsigned int i=1;i<tableauDesPossibles.size();i++)
    {
        if(tableauDesPossibles[i]->getPoids()->at(1)<maxY)
        {
            bool memeX = false;
            for(auto x : xOptim)
            {
                if(x==tableauDesPossibles[i]->getPoids()->at(0))
                {
                    memeX = true;
                }
            }
            if(!memeX)
            {
                xOptim.push_back(tableauDesPossibles[i]->getPoids()->at(0));
                maxY = tableauDesPossibles[i]->getPoids()->at(1);
                pareto.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoids()->at(1),true);
            }
            else
            {
                pareto.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoids()->at(1));
            }
        }
        else
        {
            pareto.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoids()->at(1));
        }
    }

    Svgfile SVGPareto("pareto_1.svg");
    pareto.dessinerPareto(SVGPareto);

    if (paretoPrim)
    {
        return;
    }

    Graphe pareto2;
    std::vector<Possibilite*> newSols;
    if(!paretoPrim)
    {
        for(unsigned int i = m_sommets.size(); i <= m_aretes.size();i++)
        {
            newSols.clear();
            newSols = maths::compteur_etat_possibles(i,m_aretes.size(),this);
            for(auto sol : newSols)
            {
                if(sol->getPoids()->at(0)>=0)
                {
                    tableauDesPossibles.push_back(sol);
                }
            }
        }
    }

    std::cout << tableauDesPossibles.size();

    std::cout << std::endl;
    int nbDij = 0;

    for(unsigned int a=0;a<tableauDesPossibles.size();a++)
    {
        for(unsigned int i=0;i<m_aretes.size();i++) // Pour chaque arete
        {
            if(tableauDesPossibles[a]->getBinaire()->at(i) == 1) // Ajoute ou non l'arete
            {
                m_aretes[i]->ajouter();
            }
            else m_aretes[i]->retirer();
        }
        for(auto s : m_sommets)
        {
            if(nbDij%10000 == 0) std::cout<< nbDij << " Dijkstra effectues"<<std::endl;
            tableauDesPossibles[a]->setPoidsDij(tableauDesPossibles[a]->getPoidsDij()+this->algoDijkstra(s.second->getId()));
            nbDij++;
        }
    }

    std::cout<<"Sorting..."<<std::endl;
    std::sort(tableauDesPossibles.begin(), tableauDesPossibles.end(),sortCritere2);
    std::cout<<"Fin du tri"<<std::endl;

    for(unsigned int a=1;a<tableauDesPossibles.size()-1;a++)
    {
        if((tableauDesPossibles[a]->getPoids()->at(0) == tableauDesPossibles[a-1]->getPoids()->at(0) && tableauDesPossibles[a]->getPoidsDij() == tableauDesPossibles[a-1]->getPoidsDij()) || tableauDesPossibles[a]->getPoids()->at(0) < 0)
        {
            tableauDesPossibles.erase(tableauDesPossibles.begin()+a);
            a--;
        }
    }

    maxY = tableauDesPossibles[0]->getPoids()->at(0);
    xOptim.clear();
    xOptim.push_back(tableauDesPossibles[0]->getPoidsDij());
    pareto2.addSommet(0, tableauDesPossibles[0]->getPoids()->at(0), tableauDesPossibles[0]->getPoidsDij(),true);
    for(unsigned int i=1;i<tableauDesPossibles.size();i++)
    {
        if(tableauDesPossibles[i]->getPoids()->at(0)<maxY)
        {
            bool memeX = false;
            for(auto x : xOptim)
            {
                if(x==tableauDesPossibles[i]->getPoidsDij())
                {
                    memeX = true;
                }
            }
            if(!memeX)
            {
                xOptim.push_back(tableauDesPossibles[i]->getPoidsDij());
                maxY = tableauDesPossibles[i]->getPoids()->at(0);
                pareto2.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoidsDij(),true);
            }
            else
            {
                pareto2.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoidsDij());
            }
        }
        else
        {
            pareto2.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoidsDij());
        }
    }

    Svgfile SVGPareto2("pareto_2.svg", 500, 11000);
    pareto2.dessinerPareto(SVGPareto2);
}

std::pair<bool,std::vector<int>*>* Graphe::DFSM() //DFS Marque
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
    return a1->getPoids()[1] > a2->getPoids()[1];
}

int Graphe::algoDijkstra(int depart)
{
    for(auto s : m_sommets) // Trie toutes les aretes dans les sommets par ordre croissant
    {
        std::sort(s.second->getAretes()->begin(), s.second->getAretes()->end(),triAretesCroissantes);
    }

    auto cmp = [](Sommet* s1, Sommet* s2)
    {
        /*if(s1->getD()+s1->getDistancePlusProcheVoisin() > s2->getD()+s2->getDistancePlusProcheVoisin())
        {
              std::cout << s1->getD()+s1->getDistancePlusProcheVoisin() << " gagne contre " << s2->getD()+s2->getDistancePlusProcheVoisin() <<std::endl;
        }
        else std::cout << s2->getD()+s2->getDistancePlusProcheVoisin() << " perd contre " << s1->getD()+s1->getDistancePlusProcheVoisin()<<std::endl;
        */return s1->getD()+s1->getDistancePlusProcheVoisin() > s2->getD()+s2->getDistancePlusProcheVoisin();
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
        for(auto voisin : s->getVoisins())
        {
            if (decouverts.find(voisin) == decouverts.end() && s->getArete(voisin)->isAjoute())
            {
                myqueue.push(voisin);
                if(dist.at(s)+s->getArete(voisin)->getPoids()[1]<dist.at(voisin))
                {
                    //std::cout << "Ajouter " << voisin->getId() << " depuis " << s->getId() << " coute "<<dist.at(s)+s->getArete(voisin)->getPoids()[1]<<std::endl;
                    dist.at(voisin) = dist.at(s)+s->getArete(voisin)->getPoids()[1];
                    voisin->setDistance(dist.at(s)+s->getArete(voisin)->getPoids()[1]);
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


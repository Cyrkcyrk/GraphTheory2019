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

/// Dessiner le graphe
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

/// Dessine la frontiere de pareto
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

/// Lecture des sommets depuis un fichier
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

///Lecture des aretes depuis un fichier
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

/// Algorithme de prim pour determiner les arbres couvrants de poids minimal en fonction d'un poids
Graphe Graphe::algoPrim(int depart, int critere) const
{
    std::vector<Sommet*> chemin;
    std::vector<Arete*> aretes;

    for(auto s : m_sommets) // Retire tous les sommets
    {
        s.second->retirer();
    }

    m_sommets.find(depart)->second->ajouter(); // Ajoute le sommet de depart
    chemin.push_back(m_sommets.find(depart)->second); // Ajoute le depart au chemin
    unsigned int i=1;

    Arete* b1 = new Arete(100000,new Sommet(-1,1,1),new Sommet(-1,1,1)); // Aretes temoins
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

    while(i<m_sommets.size()) // Tant que tous les sommets ne sont pas decouverts
    {
        potAretePoidsMin = b1;
        potSommetPoidsMin = b2;
        sommetPoidsMin = b3;
        aretePoidsMin = b4;

        trouve = false;
        for(auto s : m_sommets) // Parcourt les sommets
        {
            if(s.second->isAjoute()) // Si il est ajoute
            {

                potSommetPoidsMin = s.second->getProcheVoisin(critere).second; // Un potentiel successeur serait le voisin le plus proche
                potAretePoidsMin = s.second->getProcheVoisin(critere).first;
                if(potSommetPoidsMin == nullptr) // Si aucun voisin on passe ce sommet
                {
                    continue;
                }
                if(potAretePoidsMin->getPoids()[critere] < aretePoidsMin->getPoids()[critere]) // Si le poids est plus petit selon ce critere
                {
                    sommetPoidsMin = potSommetPoidsMin; // Le sommet est plus proche
                    aretePoidsMin = potAretePoidsMin;
                    trouve = true;
                }
            }
        }
        if (trouve) // si on a trouve un sommet plus proche
        {
            if(aretePoidsMin->getS1()->isAjoute() && !aretePoidsMin->getS2()->isAjoute()) // Si le sommet n'est pas ajoute
            {
                aretePoidsMin->getS2()->ajouter(); // L'ajoute
                chemin.push_back(sommetPoidsMin);
                aretes.push_back(aretePoidsMin);
                i++;
            }
            if(aretePoidsMin->getS2()->isAjoute() && !aretePoidsMin->getS1()->isAjoute())// Si le sommet n'est pas ajoute
            {
                aretePoidsMin->getS1()->ajouter();// L'ajoute
                chemin.push_back(sommetPoidsMin);
                aretes.push_back(aretePoidsMin);
                i++;
            }
        }
    }

    Graphe g(chemin,aretes); // Renvoie le graphe de prim
    return g;
}


/// Critere de tri du vecteur de possibilite pour pareto
bool Graphe::sortCritere(Possibilite* a, Possibilite* b)
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

/// Critere de tri du vecteur de possibilite pour pareto 2
bool Graphe::sortCritere2(Possibilite* a, Possibilite* b)
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

/// Frontiere de pareto avec les solutions cycliques et l'algorithme de Dijkstra
void Graphe::pareto2(std::vector<Possibilite*>& tableauDesPossibles, bool prim)
{
    if(prim)
    {
        std::cout << "Impossible d'effectuer pareto cyclique sur un graphe de Prim !" << std::endl;
        return;
    }
    if(tableauDesPossibles.size() == 0)
    {
        std::cout << "Erreur aucune possibilite reliant tous les sommets"<<std::endl;
        return;
    }
    Graphe pareto2;
    std::vector<Possibilite*> newSols; // Determination des solutions avec cycle
    if(!prim)
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

    std::cout << "Nombre de possibilite : "<< tableauDesPossibles.size();

    std::cout << std::endl;
    int nbDij = 0;

    for(unsigned int a=0;a<tableauDesPossibles.size();a++) // Pour chaque possibilite
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
            tableauDesPossibles[a]->setPoidsDij(tableauDesPossibles[a]->getPoidsDij()+this->algoDijkstra(s.second->getId())); // Effectue la somme des Dijkstra en partant de chaque point
            nbDij++;
        }
    }

    std::cout<<"Sorting..."<<std::endl;
    std::sort(tableauDesPossibles.begin(), tableauDesPossibles.end(),Graphe::sortCritere2);// Tri des solution par ordre croissant de l'un des critere
    std::cout<<"Fin du tri"<<std::endl;

    for(unsigned int a=1;a<tableauDesPossibles.size()-1;a++) // Gestion des doublons
    {
        if((tableauDesPossibles[a]->getPoids()->at(0) == tableauDesPossibles[a-1]->getPoids()->at(0) && tableauDesPossibles[a]->getPoidsDij() == tableauDesPossibles[a-1]->getPoidsDij()) || tableauDesPossibles[a]->getPoids()->at(0) < 0)
        {
            tableauDesPossibles.erase(tableauDesPossibles.begin()+a);
            a--;
        }
    }

    /// DETERMINATION DE LA FRONTIERE DE PARETO
    double maxY = tableauDesPossibles[0]->getPoids()->at(0);
    std::vector<int> xOptim;
    xOptim.push_back(tableauDesPossibles[0]->getPoidsDij());
    pareto2.addSommet(0, tableauDesPossibles[0]->getPoids()->at(0), tableauDesPossibles[0]->getPoidsDij(),true); // Ajoute le premier sommet
    for(unsigned int i=1;i<tableauDesPossibles.size();i++)// Pour chaque solution
    {
        if(tableauDesPossibles[i]->getPoids()->at(0)<maxY) // Si la solution est meilleure sur l'autre aspect
        {
            if(tableauDesPossibles[i]->getPoids()->at(0) > m_poidsMax[0]) // Tailles du Svgout
            {
                m_poidsMax[0] = tableauDesPossibles[i]->getPoids()->at(0);
            }
            if(tableauDesPossibles[0]->getPoidsDij() > m_poidsMax[1])
            {
                m_poidsMax[0] = tableauDesPossibles[0]->getPoidsDij();
            }
            bool memeX = false;
            for(auto x : xOptim) // Et que c'est la plus basse a ce X
            {
                if(x==tableauDesPossibles[i]->getPoidsDij())
                {
                    memeX = true;
                }
            }
            if(!memeX) // Ajoute la solution EN FRONTIERE DE PARETO
            {
                xOptim.push_back(tableauDesPossibles[i]->getPoidsDij());
                maxY = tableauDesPossibles[i]->getPoids()->at(0);
                pareto2.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoidsDij(),true);
            }
            else // Ajoute la solution dominee
            {
                pareto2.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoidsDij());
            }
        }
        else // Ajoute la solution dominee
        {
            pareto2.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoidsDij());
        }
    }

    Svgfile SVGPareto2("pareto_2.svg", 500, 11000); // Affiche le graphe
    pareto2.dessinerPareto(SVGPareto2);
}

/// Frontiere de pareto
void Graphe::pareto(std::vector<Possibilite*>& tableauDesPossibles)
{
    if(tableauDesPossibles.size() == 0)
    {
        std::cout << "Erreur aucune possibilite reliant tous les sommets"<<std::endl;
        return;
    }
    std:: cout << "Sommet : " << m_sommets.size() << " - Arrete : " << m_aretes.size() << std::endl;

    std::cout <<"Nbr possibilites : "<< tableauDesPossibles.size() << std::endl;

    Graphe pareto;

    std::cout<<"Sorting..."<<std::endl;
    std::sort(tableauDesPossibles.begin(), tableauDesPossibles.end(),Graphe::sortCritere); // Tri des solution par ordre croissant de l'un des critere
    std::cout<<"Fin du tri"<<std::endl;


    /// DETERMINATION DE LA FRONTIERE DE PARETO
    double maxY = tableauDesPossibles[0]->getPoids()->at(1); // La meilleure solution sur un aspect
    std::vector<int> xOptim;
    xOptim.push_back(tableauDesPossibles[0]->getPoids()->at(0)); // Ajoute le x de cette solution
    pareto.addSommet(0, tableauDesPossibles[0]->getPoids()->at(0), tableauDesPossibles[0]->getPoids()->at(1),true); // Ajoute le sommet au graphe
    for(unsigned int i=1;i<tableauDesPossibles.size();i++) // Pour chaque solution
    {
        if(tableauDesPossibles[i]->getPoids()->at(1)<maxY) // Si la solution est meilleure sur l'autre aspect
        {
            bool memeX = false;
            for(auto x : xOptim)
            {
                if(x==tableauDesPossibles[i]->getPoids()->at(0)) // Si elle est la plus basse
                {
                    memeX = true;
                }
            }
            if(!memeX)
            {
                xOptim.push_back(tableauDesPossibles[i]->getPoids()->at(0));
                maxY = tableauDesPossibles[i]->getPoids()->at(1);
                pareto.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoids()->at(1),true); // Ajoute la solution de pareto
            }
            else// Ajoute la solution dominee
            {
                pareto.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoids()->at(1));
            }
        }
        else// Ajoute la solution dominee
        {
            pareto.addSommet(i, tableauDesPossibles[i]->getPoids()->at(0), tableauDesPossibles[i]->getPoids()->at(1));
        }
    }
    Svgfile SVGPareto("pareto_1.svg");
    pareto.dessinerPareto(SVGPareto);
}

/// DFS sur les sommets marques
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

/// Ajoute un sommet a la map des sommets
void Graphe::addSommet(int id, double X, double Y, bool optimum)
{
    m_sommets.insert({id, new Sommet(id, X, Y, optimum)});
}

/// Tri les aretes par ordre croissant
bool Graphe::triAretesCroissantes(Arete* a1, Arete* a2)
{
    return a1->getPoids()[1] > a2->getPoids()[1];
}

/// Algorithme de dijkstra qui part d'un sommet de depart et renvoie la somme des distances a tous les points
int Graphe::algoDijkstra(int depart)
{
    for(auto s : m_sommets) // Trie toutes les aretes dans les sommets par ordre croissant
    {
        std::sort(s.second->getAretes()->begin(), s.second->getAretes()->end(),Graphe::triAretesCroissantes);
    }

    auto cmp = [](Sommet* s1, Sommet* s2) // Lambda du comparateur pour la priority queue
    {
        return s1->getD()+s1->getDistancePlusProcheVoisin() > s2->getD()+s2->getDistancePlusProcheVoisin();
    };
    std::priority_queue<Sommet*, std::vector<Sommet*>, decltype(cmp)> myqueue(cmp);

    std::unordered_map<Sommet*,int> dist;
    std::unordered_set<Sommet*> decouverts;

    for(auto s : m_sommets) // Pour chaque sommet
    {
         if(s.second == m_sommets.at(depart)) // Si le sommet est celui de depart
         {
             dist.insert({s.second, 0}); // Distance est 0
             s.second->setDistance(0);
         }
         else
         {
             dist.insert({s.second, 100000}); // Distance est inf
             s.second->setDistance(100000);
         }
    }

    myqueue.push(m_sommets.at(depart)); // Ajoute le sommet de depart a la queue
    Sommet* s;

    while(!myqueue.empty()) // Tant que la queue n'est pas vide
    {
        s = myqueue.top(); // On pointe le sommet

        if(decouverts.find(myqueue.top())==decouverts.end()) // Si le haut de la pile n'est pas decouvert
        {
            decouverts.insert(myqueue.top()); // Decouvre le sommet
        }
        myqueue.pop(); // Defile
        for(auto voisin : s->getVoisins()) // Pour chaque voisin
        {
            if (decouverts.find(voisin) == decouverts.end() && s->getArete(voisin)->isAjoute()) // Si le voisin n'est pas decouvert et l'arete vers ce dernier est ajoute
            {
                myqueue.push(voisin); // Ajoute le voisin a la file
                if(dist.at(s)+s->getArete(voisin)->getPoids()[1]<dist.at(voisin)) // Si la distance en passant par la est plus courte
                {
                    dist.at(voisin) = dist.at(s)+s->getArete(voisin)->getPoids()[1]; // Mets a jour la distance
                    voisin->setDistance(dist.at(s)+s->getArete(voisin)->getPoids()[1]);
                }
            }
        }
    }
    int somme = 0;
    for(auto a : dist) // Somme toutes les distances
    {
        somme+=a.second;
    }
    return somme; // Renvoie la somme
}


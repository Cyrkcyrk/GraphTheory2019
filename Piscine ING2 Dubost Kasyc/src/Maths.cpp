#include "maths.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "stdio.h"
#include "Possibilite.h"
#include "math.h"

maths::maths()
{
}

maths::~maths()
{
}


// https://www.geeksforgeeks.org/program-calculate-value-ncr/
int maths::fact(int n)
{
    if(n > 1)
        return n * maths::fact(n - 1);
    else
        return 1;
}

int maths::nCr(int k, int n)
{
	return maths::fact(n) / (maths::fact(k) * maths::fact(n - k));
}

//----------------------------------------------

// https://www.javatpoint.com/cpp-program-to-convert-decimal-to-binary
std::vector<char> maths::intToBin(int n)
{
    std::vector<char> a;
    for(;n>0;) // Convertit le nb en binaire
    {
        a.push_back(n%2);
        n = n/2;
    }
    return a;
}

int maths::binToInt(std::vector<char>& bin)
{
    int rep = 0;

    for(unsigned int i = 0;i<bin.size()-1;i++)
    {
        rep += ((int)bin[i])*pow(2,(bin.size()-1-i));
    }
    return rep;
}

// https://www.javatpoint.com/cpp-program-to-convert-decimal-to-binary
std::vector<std::vector<char>> maths::compteurBinaire(int plusGrandNombre,unsigned int ordre,unsigned int taille)
{
    std::vector<std::vector<char>> tabDesPossibilite;

    for (int i=0 ; i<=plusGrandNombre; i++)
    {
        std::vector<char> possibilite = maths::intToBin(i);

        unsigned int compteurDe1 = 0;
        for(auto car:possibilite)
        {
            if(car == '1')
            {
                compteurDe1++;
            }
        }
        if(compteurDe1 == ordre-1)
        {
            while(possibilite.size()<taille)
            {
                possibilite.push_back('0');
            }
            tabDesPossibilite.push_back(possibilite);
        }
    }
    return tabDesPossibilite;
}

std::vector<std::vector<char>> maths::combinaisonsDe1(int ordre, int taille)
{
    std::vector<std::vector<char>> tabDesPossibles;
    std::vector<char> plusGrand;

    for(int j=0;j<taille;j++) // Ajoute nbaretes nombre de bits
    {
        if(j<ordre-1)
        {
            plusGrand.push_back('1');
        }
        else
        {
            plusGrand.push_back('0');
        }

    }

    tabDesPossibles = compteurBinaire(binToInt(plusGrand),ordre,taille);

    return tabDesPossibles;
}


std::vector<std::string*> maths::decalage(std::string binaire)
{
    std::vector<std::string*> solutions;
    int i = 0;
    do
    {
        std::string* sol = new std::string;
        *sol = binaire;
        i++;
        solutions.push_back(sol);
    }while(std::next_permutation(binaire.begin(), binaire.end()));
    std::cout << i << " solutions decouvertes" << std::endl;
    std::cout << "Tri des solutions connexes en cours..." << std::endl;
    return solutions;
}

std::vector<Possibilite*> maths::compteur_etat_possibles(int nb_sommet, int nb_arete, Graphe* g)
{
    std::vector<Possibilite*> retour;
    std::vector<unsigned int> position;

    //Creation du binaire de base
    for (int i=0; i<nb_sommet; i++)
    {
        position.push_back(nb_arete-1-i);
    }

    std::string* binaire = new std::string;
    for (int j = 0; j< nb_arete; j++)
    {
        *binaire = *binaire + std::string("0");
        g->getAretes()[j]->retirer();
    }
    for (unsigned int j=0; j< position.size(); j++)
    {
        binaire->replace(position[j],1, std::string("1"));
        g->getAretes()[position[j]]->ajouter();
    }
    std::pair<bool,std::vector<int>*>* connexe = new std::pair<bool,std::vector<int>*>;
    connexe = g->getSommets()[0]->DFSM(g->getSommets().size());
    if(connexe->first)
    {
        retour.push_back(new Possibilite(binaire,connexe->second));
    }

    std::vector<std::string*>solutions = maths::decalage(*binaire);
    int com = 0;
    for(auto sol : solutions)
    {
        //std::cout << *sol<<std::endl;
        if (com%50000==0)std::cout << com << " DFS effectues" << std::endl;
        com++;
        for (unsigned int j = 0; j< sol->size(); j++)
        {
            if(sol->at(j) == '0') g->getAretes()[j]->retirer();
            else  g->getAretes()[j]->ajouter();
        }
        bool nonConnexe = true;
        for(auto s : g->getSommets()) // Pour chaque sommet
        {
            nonConnexe = true;
            for(auto a : *s.second->getAretes()) // pour chaque arete du sommet
            {
                if(a->isAjoute())  // Si une est ajoutee
                {
                    nonConnexe = false; // Le sommet est lie
                    break; // Sommet suivant
                }
            }
            if(nonConnexe)
            {
                break;
            }
        }
        if(nonConnexe)continue;
        std::pair<bool,std::vector<int>*>* connexe = new std::pair<bool,std::vector<int>*>;
        connexe = g->getSommets()[0]->DFSM(g->getSommets().size());
        if(connexe->first)
        {
            retour.push_back(new Possibilite(sol,connexe->second));
        }
    }
    std::cout << com << " DFS effectues au total" << std::endl;
        /*/// Creation du binaire
        std::vector<char>* binaire = new std::vector<char>;
        for (int j = 0; j< nb_arete; j++)
        {
            binaire->push_back(0);
            g->getAretes()[j]->retirer();
        }
        for (unsigned int j=0; j< position.size(); j++)
        {
            binaire->at(position[j]) = 1;
            g->getAretes()[position[j]]->ajouter();
        }

        std::pair<bool,std::vector<int>*>* connexe = new std::pair<bool,std::vector<int>*>;
        connexe = g->DFSM();

        if(connexe->first)
        {
            retour.push_back(new Possibilite(binaire,connexe->second));
        }
    }*/

     /*
            if (retour.size() == 0 && connexe->second->at(0) > 0 && connexe->second->at(1) > 0)
            {
                retour.push_back(new Possibilite(binaire,connexe->second));
            }
            else
            {
                bool different = true;
                for (unsigned int i=0 ; i<retour.size(); i++)
                {
                    if ((connexe->second->at(0) == retour[i]->getPoids()->at(0) && connexe->second->at(1) == retour[i]->getPoids()->at(1)) || connexe->second->at(0) < 0 || connexe->second->at(1) < 0)
                    {
                        different = false;
                        i = retour.size();
                    }
                }
                if(different && connexe->second->at(0) > 0 && connexe->second->at(1) > 0)
                {
                    retour.push_back(new Possibilite(binaire,connexe->second));
                }

            }
            */

    /***$
    for(unsigned int i=0;i<retour.size();i++)
    {
        if(retour[i].second[0] < 0)retour.erase(retour.begin()+i);
    }
    **/
    /*
    for (int i =0; i < maths::nCr(nb_sommet, nb_arete); i++)
    {
        position = maths::decalage(position, nb_sommet-1);

        /// Creation du binaire
        std::vector<char> binaire;
        for (int i = 0; i< nb_arete; i++)
        {
            binaire.push_back(0);
        }
        for (int j=0; j< position.size(); j++)
        {
            binaire[position[j]] = 1;
        }
        retour.push_back(binaire);
    }
    */

    for(unsigned int i=0;i<retour.size();i++)
    {
        if(retour[i]->getPoids()->at(0) < 0)
        {
            retour.erase(retour.begin()+i);
        }
    }
    return retour;
}

std::vector<std::vector<char>>* maths::decalageAToB(std::vector<char>& petitBinaire,std::vector<char>& binaire)
{
    std::vector<std::vector<char>>* solutions = new std::vector<std::vector<char>>;
    std::vector<std::vector<char>>* solutionsTot = new std::vector<std::vector<char>>;
    //std::vector<char> solution;
    unsigned int nbXMax = 0;
    for(unsigned int i = 0;i<binaire.size();i++)
    {
        if(binaire[i] == 'x') // Si x
        {
            nbXMax++;
        }
    }

    /*for(unsigned int i = 0;i<binaire.size();i++)
    {
        if(binaire[i] == 'x') // Si x
        {
            solution.push_back(0);
        }
        else solution.push_back(binaire[i]);
    }

    for(unsigned int i = 0;i<binaire.size();i++)
    {
        if(binaire[i] == 'x') // Si x
        {
            solution.push_back(1);
        }
        else solstd::vector<char> solution;ution.push_back(binaire[i]); // C'est le nombre deja mis dans binaire
    }

    solution.clear();
        mini++;
        if(mini%10000 == 0) std::cout<< mini<<std::endl;
        std::vector<char> bin = intToBin(mini);
        for(unsigned int i = 0;i<binaire.size();i++)
        {
            if(intToBin(mini)[i] == binaire[i] && binaire[i] == 'x')
            {
                solution.push_back(bin[i]);
            }
            else if (intToBin(mini)[i] == binaire[i]) solution.push_back(binaire[i]); // C'est le nombre deja mis dans binaire
        }
        if(bin.size()==binaire.size())solutions->push_back(solution);
    */


    int mini = 0;

    std::vector<char> grandBinaire;
    for(unsigned int i = 0 ;i<petitBinaire.size();i++)
    {
        grandBinaire.push_back(1);
    }
    int maxi = binToInt(grandBinaire);

    while(mini < maxi)
    {
        mini++;

            std::vector<char> sol = intToBin(mini);
            while(sol.size()<nbXMax)sol.push_back(0);
            solutions->push_back(sol);
    }

    for(auto solution: *solutions)
    {
        std::vector<char> solutionV;
        int j=0;
        for(unsigned int i=0;i<binaire.size();i++)
        {
            if(binaire[i] == 'x')
            {
                solutionV.push_back(solution[j]);
                j++;
            }else solutionV.push_back(binaire[i]);
        }
        solutionsTot->push_back(solutionV);
    }

    return solutionsTot;
}

std::vector<Possibilite*> maths::compteur_etat_possiblesPrim(std::vector<char> binaireA,std::vector<char> binaireB, Graphe* g)
{
    std::vector<Possibilite*> retour;
    //int chg = 0;
   // std::string adresse = std::to_string(&retour);
    std::vector<char>* binaire = new std::vector<char>;
    std::vector<char>* petitBinaire = new std::vector<char>;

    for(unsigned int i = 0;i<binaireA.size();i++)
    {
        if (binaireA[i] == binaireB[i])
        {
            //std::cout << binaireA[i]<<std::endl;
            binaire->push_back(binaireA[i]);
        }
        else
        {
            binaire->push_back('x');
            petitBinaire->push_back(0);
        }
    }

    std::vector<std::vector<char>>* solutions = new std::vector<std::vector<char>>;

    solutions = decalageAToB(*petitBinaire,*binaire);

    for(unsigned int i=0;i<solutions->size();i++) // Pour chaque solution ajoute ou non les aretes
    {
        for (unsigned int j = 0; j< solutions->at(i).size(); j++)
        {
            if(solutions->at(i)[j] == 0) g->getAretes()[j]->retirer();
            else  g->getAretes()[j]->ajouter();
        }
        std::pair<bool,std::vector<int>*>* connexe = new std::pair<bool,std::vector<int>*>;
        connexe = g->DFSM();
        std::string* sol = new std::string;
        for(auto c: solutions->at(i))
        {
            sol->append(1,c);
        }
        if(connexe->first)
        {
            retour.push_back(new Possibilite(sol,connexe->second));
        }
    }
    /*for(auto r : retour) // Pour chaque solution ajoute ou non les aretes
    {
        for(unsigned int k =0; k < r->getBinaire()->size();k++)
        {
            std::cout<< (int) r->getBinaire()->at(k);
        }
        std::cout<<std::endl;
    }*/
    /*
    for(unsigned int i=0;i<retour.size();i++)
    {
        if(retour[i]->getPoids()->at(0) < 0)
        {
            retour.erase(retour.begin()+i);
        }
    }*/
    return retour;
}

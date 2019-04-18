#include "maths.h"
#include <string>
#include <vector>
#include <iostream>

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
        rep += (bin[i]-'0')*pow(2,(bin.size()-1-i));
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


std::vector<unsigned int> maths::decalage(std::vector<unsigned int> position, int nbADecaler)
{
    if (position[nbADecaler] > position.size()-1 - nbADecaler)
    {
        position[nbADecaler]--;
        return position;
    }
    else if (nbADecaler-1 < 0)
    {

        std::vector<unsigned int> tmp;
        tmp.push_back(-1);
        return tmp;
    }
    else
    {
        position = maths::decalage(position, nbADecaler-1);
        position[nbADecaler] = position[nbADecaler-1]-1;
        return position;
    }
}

std::vector<Possibilite*> maths::compteur_etat_possibles(int nb_sommet, int nb_arete, Graphe* g)
{
    std::vector<Possibilite*> retour;
    //int chg = 0;
   // std::string adresse = std::to_string(&retour);
    std::vector<unsigned int> position;

    //Creation du binaire de base
    for (int i=0; i<nb_sommet; i++)
    {
        position.push_back(nb_arete-1-i);
    }

    {
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

        std::pair<bool,std::vector<int>*> connexe = g->DFSM();
        if(connexe.first)
        {
            if (retour.size() == 0)
            {
                retour.push_back(new Possibilite(binaire,connexe.second));
            }
            else
            {
                bool different = true;
                for (int i=0 ; i<retour.size(); i++)
                {
                    if ((connexe.second->at(0) == retour[i]->getPoids()->at(0) && connexe.second->at(1) == retour[i]->getPoids()->at(1)) || connexe.second->at(0) < 0 || connexe.second->at(1) < 0)
                    {
                        different = false;
                        i = retour.size();
                    }
                }
                if(different)
                {
                    std::cout << "ON PUSHBACK 1: " << connexe.second->at(0) <<  "   -   2: " << connexe.second->at(1) << std::endl;
                    retour.push_back(new Possibilite(binaire,connexe.second));
                }

            }
        }
    }

    for(unsigned int i=1;; i++)
    {
        if (i%10000 == 0)
            std::cout << "iteration n " << i << std::endl;
        position = maths::decalage(position, nb_sommet-1);

        if (position.size() == 1 && position[0] == -1)
        {
            break;
        }


        /// Creation du binaire
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

        std::pair<bool,std::vector<int>*> connexe = g->DFSM();

        if(connexe.first)
        {
            retour.push_back(new Possibilite(binaire,connexe.second));
            /*if((std::string)&retour !=adresse)
            {
                chg++;
                adresse = std::to_string(&retour);
            }*/
        }
    }
   // std::cout <<"Changements d'adresses : " << chg;
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

    return retour;
}

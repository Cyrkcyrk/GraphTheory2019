#include "maths.h"
#include <string>
#include <vector>
#include <iostream>

#include "stdio.h"
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
    std::cout << rep << std::endl;
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
        std::cout << "ON ARRETE!" << tmp.size();
        return tmp;
    }
    else
    {
        position = maths::decalage(position, nbADecaler-1);
        position[nbADecaler] = position[nbADecaler-1]-1;
        return position;
    }
}

std::vector<std::vector<char>>* maths::compteur_etat_possibles(int nb_sommet, int nb_arete, Graphe* g)
{
    std::vector<std::vector<char>>* retour = new std::vector<std::vector<char>>;
    std::vector<unsigned int> position;

    //Creation du binaire de base
    for (int i=0; i<nb_sommet; i++)
    {
        position.push_back(nb_arete-1 -i);
    }

    for(int i=1;; i++)
    {
        if (i%10000 == 0)
            std::cout << i << std::endl;
        position = maths::decalage(position, nb_sommet-1);

        if (position.size() == 1 && position[0] == -1)
        {
            std::cout << "BREAK" << std::endl;
            break;
        }


        /// Creation du binaire
        std::vector<char> binaire;
        for (int j = 0; j< nb_arete; j++)
        {
            binaire.push_back(0);
        }
        for (unsigned int j=0; j< position.size(); j++)
        {
            binaire[position[j]] = 1;
        }



        for(unsigned int h=0; h<binaire.size(); h++)
        {
            if(binaire[h] == 1)
            {
                g->getAretes()[h]->ajouter();
            }
            else
            {
                g->getAretes()[h]->retirer();
            }
        }
        bool connexe;
        int poidsCrit1;
        int poidsCrit2;
        std::tie(connexe, poidsCrit1, poidsCrit2) = g->DFSM();
        if(connexe)
        {
            retour->push_back(binaire);
        }
    }

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

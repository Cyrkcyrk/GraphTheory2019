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

        int compteurDe1 = 0;
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


std::vector<int> maths::decalage(std::vector<int> position, int nbADecaler)
{
    if (position[nbADecaler] > position.size()-1 - nbADecaler)
    {
        position[nbADecaler]--;
        return position;
    }
    else if (nbADecaler-1 < 0)
    {
        for (int i =0; i< position.size(); i++)
        {
            position[i] = position.size()-1-i;
        }
        return position;
    }
    else
    {
        position = maths::decalage(position, nbADecaler-1);
        position[nbADecaler] = position[nbADecaler-1]-1;
        return position;
    }
}

std::vector<std::vector<char>> maths::compteur_etat_possibles(int nb_sommet, int nb_arete)
{
    std::vector<std::vector<char>> retour;

    std::vector<int> position;

    for (int i=0; i<nb_sommet; i++)
    {
        position.push_back(nb_arete-1 -i);
    }

    {
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


    for(int i=1;; i++)
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

        bool fin = true;
        for (int j=0; j< nb_arete; ++j)
        {
            if (retour[i][j] != retour[i-1][j])
            {
                fin = false;
            }
        }
        if (fin)
        {
            retour.pop_back();
            break;
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

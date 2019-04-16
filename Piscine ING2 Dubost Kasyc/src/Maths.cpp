#include "maths.h"
#include <string>
#include <vector>
#include <iostream>

#include "stdio.h"

maths::maths()
{
}

maths::~maths()
{
}
//https://www.hackerearth.com/fr/practice/notes/factorial-of-large-number/



// https://www.geeksforgeeks.org/program-calculate-value-ncr/
unsigned long maths::fact(int n)
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
std::vector<char> maths::trasnformationIntBinaire(int n)
{
    std::vector<char> a;
    for(;n>0;) // Convertit le nb en binaire
    {
        a.push_back(n%2);
        n = n/2;
    }
    return a;
}

void maths::compteurBinaire()
{
    std::vector<std::vector<char>> possibilite;

    int ordre = 20;

    for (int i=0 ; i<ordre; i++)
    {
        std::vector<char> a = maths::trasnformationIntBinaire(i);

        std::cout << "Valeur binaire de "<< i << " : ";
        for (int j = a.size()-1; j>=0; j--)
        {
            std::cout << int(a[j]);
        }

        std::cout << std::endl;
    }
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


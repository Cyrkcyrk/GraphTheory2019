#include "maths.h"
#include <string>
#include <vector>
#include <iostream>
#include "math.h"
maths::maths()
{
}

maths::~maths()
{
}

/*
// https://www.geeksforgeeks.org/program-calculate-value-ncr/
unsigned long maths::fact(int n)
{
	int res = 1;
	for (int i = 2; i <= n; i++)
		res = res * i;
	return res;
}

unsigned long maths::nCr(int r, int n)
{
	int rep = 1;
	if(r>n-r)
    {
        r = n-r;
    }
    for(int i= 0; i<r;i++)
    {
        rep = rep*(n-i);
        rep = rep/(i+1);
    }
    return rep;
}

//-----------------------------------------------

// https://www.javatpoint.com/cpp-program-to-convert-decimal-to-binary
std::vector<std::vector<char>> maths::compteurBinaire(int plusGrandNombre,unsigned int ordre,unsigned int taille)
{
    std::vector<std::vector<char>> tabDesPossibilite;

    for (int i=0 ; i<=plusGrandNombre; i++)
    {
        int n = i;
        std::vector<char> possibilite;
        for(;n>0;) // Convertit le nb en binaire
        {
            possibilite.push_back((char)n%2+'0');
            n = n/2;
        }
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

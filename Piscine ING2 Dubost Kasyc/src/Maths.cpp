#include "maths.h"
#include <string>
#include <vector>
#include <iostream>

maths::maths()
{
}

maths::~maths()
{
}


// https://www.geeksforgeeks.org/program-calculate-value-ncr/
int maths::fact(int n)
{
	int res = 1;
	for (int i = 2; i <= n; i++)
		res = res * i;
	return res;
}

int maths::nCr(int r, int n)
{
	return fact(n) / (fact(r) * fact(n - r));
}

//-----------------------------------------------

// https://www.javatpoint.com/cpp-program-to-convert-decimal-to-binary
void maths::compteurBinaire()
{
    std::vector<std::vector<char>> possibilite;

    int ordre = 20;
    int nbAreteSelected = 0;

    for (int i=0 ; i<ordre; i++)
    {
        int n = i;
        std::vector<char> a;
        for(;n>0;) // Convertit le nb en binaire
        {
            a.push_back(n%2);
            n = n/2;
        }

        std::cout << "Valeur binaire de "<< i << " : ";
        for (int j = a.size()-1; j>=0; j--)
        {
            std::cout << int(a[j]);
        }

        std::cout << std::endl;
    }

/*
    for(int i = 0; i < a.size();i++) // Compte le nb d'arete selectionnees (nb de 1 dans le tableau binaire)
    {
        //nbAreteSelected += a;
    }

    if(nbAreteSelected == )
    {

    }

    if(possibilite.count)

    for(int i = 0; i< nCr(m_sommets.size()-1,m_aretes.size()) ; i++)
    {
        //if()
        tabDesPossible.push_back();
    }
*/
}

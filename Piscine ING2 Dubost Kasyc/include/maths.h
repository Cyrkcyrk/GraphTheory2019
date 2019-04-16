#ifndef MATHS_H
#define MATHS_H

#include <vector>
#include "Graphe.h"


class maths
{
    public:
        maths();
        virtual ~maths();

        static int fact(int n);
        static int nCr(int r, int n);

        static void compteurBinaire();
        static std::vector<char> intToBin(int n);
        static int binToInt(std::vector<char>& bin);

        static std::vector<std::vector<char>> compteurBinaire(int plusGrandNombre, unsigned int ordre, unsigned int taille);
        static std::vector<std::vector<char>> combinaisonsDe1(int ordre, int taille);

        static std::vector<std::pair<std::vector<char>,std::vector<int>>> compteur_etat_possibles(int nb_sommet, int nb_arete, Graphe* g);
        static std::vector<unsigned int> decalage(std::vector<unsigned int> position, int nbADecaler);

    protected:

    private:
};

#endif // MATHS_H

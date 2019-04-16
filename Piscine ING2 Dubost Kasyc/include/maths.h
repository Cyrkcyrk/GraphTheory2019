#ifndef MATHS_H
#define MATHS_H

#include <vector>


class maths
{
    public:
        maths();
        virtual ~maths();

        static unsigned long fact(int n);
        static int nCr(int r, int n);
        static void compteurBinaire();
        static std::vector<char> trasnformationIntBinaire(int n);
        static std::vector<std::vector<char>> compteur_etat_possibles(int nb_sommet, int nb_arete);
        static std::vector<int> decalage(std::vector<int> position, int nbADecaler);
    protected:

    private:
};

#endif // MATHS_H

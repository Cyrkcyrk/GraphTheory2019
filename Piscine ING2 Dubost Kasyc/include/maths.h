#ifndef MATHS_H
#define MATHS_H

#include <vector>

class maths
{
    public:
        maths();
        virtual ~maths();

        static unsigned long fact(int n);
        static unsigned long nCr(int r, int n);
        static int binToInt(std::vector<char>& bin);
        static std::vector<std::vector<char>> compteurBinaire(int plusGrandNombre, unsigned int ordre, unsigned int taille);

        static std::vector<std::vector<char>> combinaisonsDe1(int ordre, int taille);
    protected:

    private:
};

#endif // MATHS_H

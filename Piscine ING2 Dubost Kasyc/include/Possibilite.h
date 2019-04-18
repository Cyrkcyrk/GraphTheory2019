#ifndef POSSIBILITE_H
#define POSSIBILITE_H

#include <vector>

class Possibilite
{
    public:
        Possibilite(std::vector<char>* binaire, std::vector<int>* poids);
        virtual ~Possibilite();

        std::vector<int>* getPoids(){return m_poids;};
        std::vector<char>* getBinaire(){return m_binaire;};
        int getPoidsDij()const{return m_poidsDij;};
        void setPoidsDij(int poids){m_poidsDij=poids;};

    private:

        std::vector<char>* m_binaire;
        std::vector<int>* m_poids;
        int m_poidsDij;
};

#endif // POSSIBILITE_H

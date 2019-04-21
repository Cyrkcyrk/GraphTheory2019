#ifndef POSSIBILITE_H
#define POSSIBILITE_H

#include <vector>
#include <string>
/** Une possibilite est un nombre binaire representant les aretes selectionnees
ainsi qu'un vecteur de poids et un poids de Dijkstra (somme des dijkstra sur chaque sommet de cette possibilite
**/
class Possibilite
{
    public:
        Possibilite(std::string* binaire, std::vector<int>* poids);
        virtual ~Possibilite();

        std::vector<int>* getPoids(){return m_poids;};
        std::string* getBinaire(){return m_binaire;};
        int getPoidsDij()const{return m_poidsDij;};
        void setPoidsDij(int poids){m_poidsDij=poids;};

    private:

        std::string* m_binaire;
        std::vector<int>* m_poids;
        int m_poidsDij;
};

#endif // POSSIBILITE_H

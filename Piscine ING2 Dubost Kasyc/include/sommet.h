#ifndef SOMMET_H
#define SOMMET_H

#include <vector>

class Sommet
{
    public:
        Sommet(int id, double x, double y);
        virtual ~Sommet();

        void ajouterVoisin(Sommet* s);

    private:
        int m_id;
        double m_x;
        double m_y;
        std::vector<Sommet*> m_voisins;
};

#endif // SOMMET_H

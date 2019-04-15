#ifndef GRAPHE_H
#define GRAPHE_H

#include <string>
#include <unordered_map>
#include "sommet.h"
#include "arete.h"

class Graphe
{
    public:
        Graphe(std::string nomFichier);
        virtual ~Graphe();

        void lireFichier(std::string nomFichier);

    private:
        std::unordered_map<int,Sommet*> m_sommets;
        std::unordered_map<int,Arete*> m_aretes;
};

#endif // GRAPHE_H

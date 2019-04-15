#ifndef GRAPHE_H
#define GRAPHE_H

#include <string>
#include <unordered_map>
#include "sommet.h"
#include "arete.h"

class Graphe
{
    public:
        Graphe(std::string nomFichierSommet, std::string nomFichierArete);
        virtual ~Graphe();

        void lireSommet(std::string nomFichierSommet);
        void lireArete(std::string nomFichierArete);
        void afficherGraphe();

    private:
        std::unordered_map<int,Sommet*> m_sommets;
        std::unordered_map<int,Arete*> m_aretes;
};

#endif // GRAPHE_H

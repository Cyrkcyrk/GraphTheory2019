#ifndef MATHS_H
#define MATHS_H

#include <vector>
#include <string>
#include "Graphe.h"
#include "Possibilite.h"


class maths
{
    public:
        maths();
        virtual ~maths();
        /// Calcul du factoriel du parametre
        static int fact(int n);
        /// Calcul de la combinaison parametre 1 parmi parametre 2
        static int nCr(int r, int n);
        /// Convertit un entier en binaire (vector de char)
        static std::vector<char> intToBin(int n);
        /// Convertit un nombre binaire en entier
        static int binToInt(std::vector<char>& bin);
        /// Compteur binaire (premiere version moins performante que compteur etat possibles)
        static std::vector<std::vector<char>> compteurBinaire(int plusGrandNombre, unsigned int ordre, unsigned int taille);
        /// Compteur de combinaisons de 1 (moins performant que compteur etat possible
        static std::vector<std::vector<char>> combinaisonsDe1(int ordre, int taille);
        /// Verifie la connexite et renvoie les possibilite
        static std::vector<Possibilite*> compteur_etat_possibles(int nb_sommet, int nb_arete, Graphe* g);
        /// Verifie la connexite et renvoie les possibilite des vecteurs intermediaires avec prim (BONUS)
        static std::vector<Possibilite*> compteur_etat_possiblesPrim(std::vector<char> binaireA,std::vector<char> binaireB, Graphe* g);
        /// Renvoie tous les nombres binaires avec le bon nombre de 1 (toutes les permutations possibles a partir d'un string de base)
        static std::vector<std::string*> decalage(std::string binaire);
        /// Renvoie tous les nombres binaires avec le bon nombre de 1 entre deux vecteur de char (resultats binaires de prim 1 et prim 2)
        static std::vector<std::vector<char>>* decalageAToB(std::vector<char>& petitBinaire, std::vector<char>& binaire);

    protected:

    private:
};

#endif // MATHS_H

#ifndef ARETE_H
#define ARETE_H

#include <vector>
#include "Coord.h"
#include "svgfile.h"

class Sommet;

class Arete
{
    public:
        Arete(int id, Sommet* s1, Sommet* s2);
        virtual ~Arete();

        /// Ajoute le tableau de poids passe en parametre en attribut
        void addPoids(std::vector<double> tabPoids);
        /// Retourne l'id de l'arete
        int getId()const {return m_id;};
        /// Retourne le sommet 1 de l'arete
        Sommet* getS1()const{return m_sommet1;};
        /// Retourne le sommet 2 de l'arete
        Sommet* getS2()const{return m_sommet2;};
        /// Retourne le tableau de poids de l'arete
        std::vector<double> getPoids()const{return m_poids;};
        //double getPoidsCoords()const{return m_poidsCoords;};
        /// Retourne le sommet qui est a l'autre extremite de l'arete par rapport au sommet en parametre
        Sommet* getAutreSommet(Sommet* s){if (m_sommet1 != s) return m_sommet1; else return m_sommet2;};
        /// Renvoie si l'arete est marquee ou non
        bool isAjoute()const{return m_marque;};
        /// Marque l'arete
        void ajouter(){m_marque = true;};
        /// Ne marque plus l'arete
        void retirer(){m_marque = false;};
        /// Dessine l'arete sur le svgout
        void dessiner(Svgfile& svgout, bool afficher_poids = false, double coefX = 1.0, double coefY = 1.0);


    private:
        int m_id;
        Sommet* m_sommet1;
        Sommet* m_sommet2;
        std::vector<double> m_poids;
        bool m_marque;
};

#endif // ARETE_H

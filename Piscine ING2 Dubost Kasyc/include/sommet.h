#ifndef SOMMET_H
#define SOMMET_H

#include <vector>
#include "arete.h"
#include "Coord.h"
#include <unordered_set>

#include "conf.h"

class Sommet
{
    public:
        Sommet(int id, double x, double y, bool optimum = false);
        virtual ~Sommet();

        void ajouterVoisin(Sommet* s);
        void ajouterArete(Arete* a);

        int getId()const{return m_id;};

        /// Marquer un sommet
        void ajouter(){m_ajoute = true;};
        /// Ne plus marquer un sommet
        void retirer(){m_ajoute = false;};
        /// Renvoie si le sommet est marque ou non
        bool isAjoute()const{return m_ajoute;};
        /// Renvoie si le sommet fait partie de la frontiere de pareto
        bool isOptimum()const{return m_optimum;};
        /// Renvoie les coordonnees du sommet
        Coord getCoord() const {return m_coord;};
        /// Renvoie les donnees du plus proche voisin
        std::pair<Arete*, Sommet*> getProcheVoisin(int critere);
        /// Renvoie un tableau de pointeur vers les voisins du sommet
        std::vector<Sommet*> getVoisins(){return m_voisins;};
        /// Renvoie l'arete liant ce sommet avec le sommet en parametre
        Arete* getArete(const Sommet* s2)const;
        /// Renvoie toutes les aretes du sommet
        std::vector<Arete*>* getAretes(){return &m_aretes;};
        /// Renvoie un pointeur vers le voisin le plus proche
        Sommet* getPlusProcheVoisin();
        /// Renvoie la distance au plus proche voisin
        int getDistancePlusProcheVoisin();
        /// Renvoie la distance D de dijkstra actuelle
        int getD()const{return m_distance;};
        /// Setter pour la distance de Dijkstra
        void setDistance(int d){m_distance = d;};
        /// DFS sur les aretes marquees pour verifier la connexite
        std::pair<bool,std::vector<int>*>* DFSM(unsigned int nbSommets);
        /// Dessine le sommet sur le svgout
        void dessiner(Svgfile& svgout, double coefX = 1.0, double coefY = 1.0, bool afficher_texte = false, double rayon = POINT_RAYON, std::string couleur = POINT_COULEUR, double opacity = 1.0);

    private:
        int m_id;
        Coord m_coord;
        std::vector<Sommet*> m_voisins;
        std::vector<Arete*> m_aretes;
        bool m_ajoute;
        bool m_optimum;
        unsigned int m_distance;
};

#endif // SOMMET_H

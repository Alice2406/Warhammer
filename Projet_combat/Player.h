#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
using namespace std;

class Player
{
protected :
    string nom;
	int Attaque;
	int Defense;
	int Degats;
	int Pv;

public :
	Player (string n, int atq, int def, int deg, int hp) : nom(n), Attaque(atq), Defense(def), Degats(deg), Pv(hp) {}
    string getNom() const {
        return nom;
    }
    virtual void attaquer(Player& cible, int distance) = 0; 
    bool estVivant() const { return Pv > 0; }

    void subirDegats(int montant) {
        Pv -= montant;
        if (Pv < 0) Pv = 0;
    }

    void afficherStatut() const {
        cout << " | PV: " << Pv
            << " | ATK: " << Attaque
            << " | DEF: " << Defense
            << " | DGT: " << Degats << std::endl;
    }

};
#endif

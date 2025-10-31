#ifndef ASTARTES_H
#define ASTARTES_H

#include <iostream>
#include <string>
#include "Player.h"
#include "Des.h"
using namespace std;

class Astartes : public Player
{
	Des d1;
	Des d2;
	Des d3;
	Des d4;

public:
	Astartes(string n = "Astartes") : Player(n, 4, 3, 5, 15) {}
	int garder(Des* d, int val)
	{
		if (d->getVal() >= val)
			return 1;
		else
			return 0;
	};

    void attaquer(Player& cible) override {
        cout << nom << " tire avec son bolter sur " << cible.getNom() << " !" << endl;

        // Lancer des dés
        d1.lancer();
        d2.lancer();
        d3.lancer();
        d4.lancer();

        // Vérifier combien de dés touchent
        int seuil = 4; // Par exemple, chaque dé touche si >= 4
        int touche = 0;
        touche += garder(&d1, seuil);
        touche += garder(&d2, seuil);
        touche += garder(&d3, seuil);
        touche += garder(&d4, seuil);

        cout << "Des qui touchent : " << touche << " sur 4" << endl;

        // Infliger les dégâts pour chaque touche
        int totalDegats = touche * Degats;
        if (totalDegats > 0) {
            cout << "Touche ! " << totalDegats << " degats infliges !" << endl;
            cible.subirDegats(totalDegats);
        }
        else {
            cout << "L'attaque echoue !" << std::endl;
        }
    }
};
#endif

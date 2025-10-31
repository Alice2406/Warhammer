#ifndef DEMON_H
#define DEMON_H

#include <iostream>
#include <string>
#include "Player.h"
#include "Des.h"

using namespace std;

class Demon : public Player
{
	Des d1;
	Des d2;
	Des d3;

public:
	Demon(string n = "Demon du Warp") : Player(n, 3, 4, 5, 12) {}
	int garder(Des* d, int val)
	{
		if (d->getVal() >= val)
			return 1;
		else
			return 0;
	};

    void attaquer(Player& cible) override {
        cout << nom << " attaque " << cible.getNom() << " avec ses griffes demoniaques !" << endl;

        // Lancer des dés
        d1.lancer();
        d2.lancer();
        d3.lancer();

        // Vérifier combien de dés touchent
        int seuil = 4; // Par exemple, chaque dé touche si >= 4
        int touche = 0;
        touche += garder(&d1, seuil);
        touche += garder(&d2, seuil);
        touche += garder(&d3, seuil);

        cout << "Des qui touchent : " << touche << " sur 3" << endl;

        // Infliger les dégâts pour chaque touche
        int totalDegats = touche * Degats;
        if (totalDegats > 0) {
            cout << "Touché ! " << totalDegats << " degats infliges !" << endl;
            cible.subirDegats(totalDegats);
        }
        else {
            cout << "L'attaque echoue !" << std::endl;
        }
    }
};
#endif

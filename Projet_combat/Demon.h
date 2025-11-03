#ifndef DEMON_H
#define DEMON_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
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

    void attaquer(Player& cible, int distance) override {
        cout << nom << " rugit et attaque " << cible.getNom() << " avec ses griffes démoniaques !" << endl;

        int portee = 1; // le démon ne peut attaquer qu’à 1 case
        if (distance > portee) {
            cout << "La cible est trop loin ! (" << portee << " case max)" << endl;
            return;
        }

        // Frames ASCII du démon pour l'animation
        const string frames[][6] = {
            {
                "      (\\_._/)      ",
                "      ( o_o )      ",
                "      />   <\\     ",
                "     /   |   \\    ",
                "    (___/ \\___)   ",
                "   Le demon surgit !"
            },
            {
                "      (\\O_O/)      ",
                "      (>   <)      ",
                "      /  |  \\    ",
                "     /   |   \\   ",
                "    (___/ \\___)   ",
                "   Le demon attaque !"
            },
            {
                "      (\\_o_/)      ",
                "      ( o_o )      ",
                "      /|   |\\     ",
                "     /   |   \\    ",
                "    (___/ \\___)   ",
                "   Le demon frappe !"
            }
        };

        // Animation : on parcourt les frames
        for (int i = 0; i < 3; ++i) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            for (const auto& line : frames[i]) {
                cout << line << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(400));
        }

        // Lancer les dés pour déterminer les touches
        Des d1, d2, d3;
        d1.lancer(); d2.lancer(); d3.lancer();

        int seuil = 3; // plus facile de toucher qu’un Astartes
        int touches = 0;

        if (d1.getVal() >= seuil) touches++;
        if (d2.getVal() >= seuil) touches++;
        if (d3.getVal() >= seuil) touches++;

        cout << "Touches reussies : " << touches << " sur 3" << endl;

        int degatsParTouche = Degats; // hérité de Player
        int totalDegats = touches * degatsParTouche;

        if (totalDegats > 0) {
            cout << "Le demon frappe sauvagement ! " << totalDegats << " degats infliges !" << endl;
            cible.subirDegats(totalDegats);
        }
        else {
            cout << "Le demon rate son attaque !" << endl;
        }
    }
};
#endif

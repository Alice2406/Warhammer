#ifndef ASTARTES_H
#define ASTARTES_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "Player.h"
#include "Des.h"
#include "Arme.h"
using namespace std;

class Astartes : public Player
{
	Des d1;
	Des d2;
	Des d3;
	Des d4;
    vector<Arme> arsenal;
    int armeActuelle;

public:
	Astartes(string n = "Astartes") : Player(n, 4, 3, 5, 15) 
    {
            arsenal.push_back(Arme("Bolter", 5, 3));
            arsenal.push_back(Arme("Epee energetique", 8, 1));
            arsenal.push_back(Arme("Lance-flammes", 6, 2));
            arsenal.push_back(Arme("Fusil plasma", 10, 4));

            armeActuelle = 0; // par défaut, Bolter
    }

    void choisirArme() {
        cout << "\n=== CHOIX DE L'ARME ===" << endl;
        for (size_t i = 0; i < arsenal.size(); i++) {
            cout << i + 1 << ". ";
            arsenal[i].afficher();
        }
        cout << "Votre choix : ";
        int choix;
        cin >> choix;

        if (choix >= 1 && choix <= (int)arsenal.size()) {
            armeActuelle = choix - 1;
            cout << "Vous avez equipe : " << arsenal[armeActuelle].getNom() << " !" << endl;
        }
        else {
            cout << "Choix invalide. Vous gardez votre arme actuelle." << endl;
        }
    }

	int garder(Des* d, int val)
	{
		if (d->getVal() >= val)
			return 1;
		else
			return 0;
	};

    void attaquer(Player& cible, int distance) override {
        Arme& arme = arsenal[armeActuelle];
        cout << nom << " attaque avec son " << arme.getNom() << " !" << endl;

        if (distance > arme.getPortee()) {
            cout << "La cible est hors de portee (" << arme.getPortee() << " cases max)." << endl;
            return;
        }

        // Animation d'attaque ASCII avec plusieurs frames
        string frames[4][6] = {
            { "     /\\     ",
              "    |==|    ",
              "    |  |    ",
              "   /====\\   ",
              "  |      |  ",
              "Astartes pret !" },

            { "     /\\     ",
              "    |==|    ",
              "    |  |    ",
              "   /====\\   ",
              "  |  /   |  ",
              "Astartes en action !" },

            { "     /\\     ",
              "    |==|    ",
              "   /|  |\\   ",
              "   /====\\   ",
              "  |      |  ",
              "Astartes frappe !" },

            { "     /\\     ",
              "    |==|    ",
              "    |  |    ",
              "   /====\\   ",
              "  | /    |  ",
              "Coup final !" }
        };

        for (int i = 0; i < 4; ++i) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            for (const auto& f : frames[i]) {
                cout << f << endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }

        // Lancer les dés
        d1.lancer(); d2.lancer(); d3.lancer(); d4.lancer();

        int seuil = 4;
        int touches = garder(&d1, seuil) + garder(&d2, seuil)
            + garder(&d3, seuil) + garder(&d4, seuil);

        cout << "Touches reussies : " << touches << endl;

        int totalDegats = touches * arme.getDegats();

        if (totalDegats > 0) {
            cout << "Touche ! " << totalDegats << " degats infliges !" << endl;
            cible.subirDegats(totalDegats);
        }
        else {
            cout << "L'attaque echoue !" << endl;
        }
    }
};
#endif

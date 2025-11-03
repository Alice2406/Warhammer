#include "Astartes.h"
#include "Demon.h"
#include "Grille.h"

using namespace std;

bool menuAstartes(Grille& plateau, Astartes& astartes, Demon& demon) {
    int choix;
    cout << "\n===== TOUR DE " << astartes.getNom() << " =====" << endl;
    cout << "1. Déplacer Astartes" << endl;
    cout << "2. Combattre le Démon" << endl;
    cout << "3. Ne rien faire" << endl;
    cout << "Votre choix : ";
    cin >> choix;

    switch (choix) {
    case 1: {
        int nx, ny;
        cout << "Nouvelle position x y : ";
        cin >> nx >> ny;
        bool reussi = plateau.deplacerAstartes(nx, ny);
        return reussi;
    }

    case 2: {
        int xA, yA, xD, yD;
        plateau.placeAstartes(&xA, &yA);
        plateau.placeDemon(&xD, &yD);

        int distance = abs(xA - xD) + abs(yA - yD);

        if (distance <= 3) { // Astartes tire à distance
            astartes.attaquer(demon);

            if (demon.estVivant() == false) {
                cout << demon.getNom() << " est éliminé !" << endl;
            }
            return true;
        }
        else {
            cout << "Le Démon est trop loin pour être attaqué !" << endl;
            return false;
        }
    }

    case 3:
        cout << astartes.getNom() << " reste immobile." << endl;
        return true;

    default:
        cout << "Choix invalide !" << endl;
        return false;
    }
}

int main()
{
    srand(time(0));
    cout << "Bienvenue sur Warhammer version nulle" << endl;
    Astartes spaceMarine("Ultramarine");
    Demon demon("Buveur de Sang");

    Grille plateau;

    plateau.placerUnite(0, 0, 'A');
    plateau.placerUnite(7, 7, 'D');
    while (true) {
        plateau.afficherDeplacementsPossibles();
        bool actionValide = menuAstartes(plateau, spaceMarine, demon);  // Tour du joueur

        if (demon.estVivant() == false) {
            cout << "Victoire de l’Astartes !" << endl;
            break;
        }

        // Tour du démon
        if (actionValide && demon.estVivant() == true) {
            cout << "\n===== TOUR DU DEMON =====" << endl;
            plateau.deplacerDemon();

            int xA, yA, xD, yD;
            plateau.placeAstartes(&xA, &yA);
            plateau.placeDemon(&xD, &yD);

            if (abs(xA - xD) + abs(yA - yD) == 1) {
                demon.attaquer(spaceMarine);
                if (spaceMarine.estVivant() == false) {
                    cout << "L’Astartes est tombé au combat !" << endl;
                    break;
                }
            }
        }
        else {
            cout << "\nAucune action valide — le Démon reste immobile." << endl;
        }

        if (demon.estVivant() == false) {
            cout << "Victoire de l’Astartes !" << endl;
            break;
        }
    }
    return 0;
}
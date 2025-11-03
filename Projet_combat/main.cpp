#include "Astartes.h"
#include "Demon.h"
#include "Grille.h"

using namespace std;

void menuAstartes(Grille& plateau, Astartes& astartes, Demon& demon) {
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
        plateau.deplacerAstartes(nx, ny);
        break;
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
        }
        else {
            cout << "Le Démon est trop loin pour être attaqué !" << endl;
        }
        break;
    }

    case 3:
        cout << astartes.getNom() << " reste immobile." << endl;
        break;

    default:
        cout << "Choix invalide !" << endl;
        break;
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
        menuAstartes(plateau, spaceMarine, demon);  // Tour du joueur

        if (demon.estVivant() == false) {
            cout << "Victoire de l’Astartes !" << endl;
            break;
        }

        // Tour du démon
        cout << "\n===== TOUR DU DEMON =====" << endl;
        plateau.deplacerDemon(); // Déplacement automatique du démon

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
    return 0;
}
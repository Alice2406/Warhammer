#include "Astartes.h"
#include "Demon.h"
#include "Grille.h"
#include <thread>     // pour std::this_thread::sleep_for
#include <chrono>     // pour std::chrono::milliseconds
using namespace std;

// === COULEURS ANSI ===
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

void printSlow(const string& text, int delayMs = 30) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
}
void waitDots(int count = 3, int delayMs = 400) {
    for (int i = 0; i < count; ++i) {
        cout << "." << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << endl;
}

bool menuAstartes(Grille& plateau, Astartes& astartes, Demon& demon) {
    int choix;
    cout << "\n===== TOUR DE " << astartes.getNom() << " =====" << endl;
    cout << "1. Deplacer Astartes" << endl;
    cout << "2. Combattre le Demon" << endl;
    cout << "3. Changer d'arme" << endl;
    cout << "4. Ne rien faire" << endl;
    while (1)
    {
        cout << "Votre choix : ";
        cin >> choix;
        if (cin.fail() || choix < 1 || choix > 4)
        {
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else
            break;
    }
    switch (choix) {
    case 1: {
        int nx, ny;
        while (1)
        {
            cout << "Nouvelle position x y : ";
            cin >> nx >> ny;
            if (cin.fail() || nx < 0 || nx > 9 || ny < 0 || ny > 9)
            {
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else
                break;
        }
        bool reussi = plateau.deplacerAstartes(nx, ny);
        return reussi;
    }

    case 2: {
        int xA, yA, xD, yD;
        plateau.placeAstartes(&xA, &yA);
        plateau.placeDemon(&xD, &yD);

        int distance = abs(xA - xD) + abs(yA - yD);

        if (distance <= 3) { // Astartes tire à distance
            astartes.attaquer(demon, 1);

            if (demon.estVivant() == false) {
                cout << demon.getNom() << " est elimine !" << endl;
            }
            return true;
        }
        else {
            cout << "Le Demon est trop loin pour etre attaque !" << endl;
            return false;
        }
    }
    case 3:
        astartes.choisirArme();
        return false; // ne fait pas bouger le démon ce tour-ci
    case 4:
        cout << astartes.getNom() << " reste immobile." << endl;
        return true;

    default:
        cout << "Choix invalide !" << endl;
        return false;
    }
}

bool menuDemon(Grille& plateau, Demon& demon, Astartes& adversaire) {
    cout << "Tour du Demon (" << demon.getNom() << ")\n";
    cout << "1. Deplacer Demon" << endl;
    cout << "2. Combattre Astartes" << endl;
    int choix;
    cin >> choix;

    if (choix == 1) {
        int nx, ny;
        while (1)
        {
            cout << "Nouvelle position x y : ";
            cin >> nx >> ny;
            if (cin.fail() || nx < 0 || nx > 9 || ny < 0 || ny > 9)
            {
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else
                break;
        }
        plateau.deplacerUnite(nx, ny); // exemple : fonction deplacement manuel
        return true;
    }
    else if (choix == 2) {
        int xA, yA, xD, yD;
        plateau.placeAstartes(&xA, &yA);
        plateau.placeDemon(&xD, &yD);

        if (abs(xA - xD) + abs(yA - yD) == 1) {
            demon.attaquer(adversaire, 1);
            return true;
        }
    }
    return false;
}

int main()
{
    srand(time(0));

    cout << CYAN << BOLD << "==============================\n";
    cout << "  Bienvenue sur Warhammer version nulle\n";
    cout << "==============================" << RESET << endl;

    Astartes spaceMarine("Ultramarine");
    Demon demon("Buveur de Sang");

    Grille plateau;

    plateau.placerUnite(0, 0, 'A');
    plateau.placerUnite(9, 9, 'D');
    plateau.placerObstaclesAleatoires(10);

    bool demonIA = true;
    int choix;
    cout << YELLOW << "\nSouhaitez-vous que le Demon soit joue par :" << RESET << endl;
    cout << "1 - L'IA" << endl;
    cout << "2 - Un autre joueur" << endl;
    cout << "Votre choix : ";
    cin >> choix;
    if (choix == 2) demonIA = false;

    // Petite animation d’intro
    printSlow(GREEN + string("\nDeploiement des troupes") + RESET);
    waitDots();
    system("cls");

    while (true) {
        cout << BLUE << "\n===== TOUR DE L'ASTARTES =====" << RESET << endl;
        plateau.afficherDeplacementsPossibles();
        bool actionValide = menuAstartes(plateau, spaceMarine, demon);  // Tour du joueur

        if (!demon.estVivant()) {
            cout << GREEN << "Victoire de l'Astartes !" << RESET << endl;
            break;
        }

        // Tour du démon
        if (actionValide && demon.estVivant()) {
            if (demonIA) {
                cout << RED << "\n===== TOUR DU DEMON =====" << RESET << endl;
                waitDots(2, 300);
                // --- mode IA ---
                plateau.deplacerDemon();
                int xA, yA, xD, yD;
                plateau.placeAstartes(&xA, &yA);
                plateau.placeDemon(&xD, &yD);

                if (abs(xA - xD) + abs(yA - yD) == 1) {
                    printSlow(RED + string("\nLe Demon attaque sauvagement !") + RESET, 40);
                    cout << endl;
                    demon.attaquer(spaceMarine, 1);
                }
            }
            else {
                system("cls");
                cout << RED << "\n===== TOUR DU DEMON =====" << RESET << endl;
                plateau.afficherDeplacementsPossiblesDemon();
                bool demonActionValide = menuDemon(plateau, demon, spaceMarine);
                while (!demonActionValide) {
                    demonActionValide = menuDemon(plateau, demon, spaceMarine);
                }
            }
        }
        else {
            cout << "\nAucune action valide — le Demon reste immobile." << endl;
        }

        if (!spaceMarine.estVivant()) {
            cout << RED << "L'Astartes est tombe au combat !" << RESET << endl;
            break;
        }
        if (!demon.estVivant()) {
            cout << GREEN << "Victoire de l'Astartes !" << RESET << endl;
            break;
        }

        // petite pause entre les tours
        printSlow("\nProchain tour", 50);
        waitDots();
        system("cls");
    }

    cout << MAGENTA << "\nFin de la partie. Merci d'avoir joue !" << RESET << endl;
    return 0;
}


//une unite complete de chaque cote 
//animation (bonus)
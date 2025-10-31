#include <iostream>
#include "Astartes.h"
#include "Demon.h"
#include "Grille.h"

using namespace std;

int main()
{
    srand(time(0));
	cout << "Bienvenue sur Warhammer version nulle" << endl;
    Astartes spaceMarine("Ultramarine");
    Demon demon("Buveur de Sang");

    Grille plateau;
    plateau.placerUnite(0, 0, 'A');
    plateau.placerUnite(7, 7, 'D');
    plateau.afficherDeplacementsPossibles();
    plateau.deplacerAstartes(3, 0);
    plateau.afficherDeplacementsPossibles();
    return 0;
}
#include <iostream>
using namespace std;

class Grille
{
private:
    char grille[8][8];
    bool visite[8][8];

public:
    Grille() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                grille[i][j] = '.';
    }

    void afficher() {
        cout << "  ";
        for (int i = 0; i < 8; i++)
            cout << i << " ";
        cout << endl;

        for (int y = 0; y < 8; y++) {
            cout << y << " ";
            for (int x = 0; x < 8; x++) {
                cout << grille[y][x] << " ";
            }
            cout << endl;
        }
    }

    bool placerUnite(int x, int y, char unite) {
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            if (grille[y][x] == '.') {
                grille[y][x] = unite;
                return true;
            }
            else {
                cout << "Case deje occupee !" << endl;
                return false;
            }
        }
        else {
            cout << "Position invalide !" << endl;
            return false;
        }
    }

    void placeAstartes(int* x, int* y)
    {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
            {
                if (grille[i][j] == 'A') {
                    *x = j;  // colonne
                    *y = i;  // ligne
                }
            }
    }

    void explorer(int x, int y, int deplacementRestant, char tempGrille[8][8], bool visite[8][8]) {
        if (deplacementRestant == 0) return;

        int dx[4] = { -1, 1, 0, 0 }; // gauche, droite
        int dy[4] = { 0, 0, -1, 1 }; // haut, bas

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                if (!visite[ny][nx] && grille[ny][nx] == '.') {
                    tempGrille[ny][nx] = '*';
                    visite[ny][nx] = true;
                    explorer(nx, ny, deplacementRestant - 1, tempGrille, visite);
                    visite[ny][nx] = false;
                }
            }
        }
    }

    void afficherDeplacementsPossibles() {
        int xAstartes;
        int yAstartes;

        placeAstartes(&xAstartes, &yAstartes);
        char tempGrille[8][8];
        // Copie de la grille pour marquer les d�placements possibles
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                tempGrille[y][x] = grille[y][x];

        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
                visite[y][x] = false;

        visite[yAstartes][xAstartes] = true;

        explorer(xAstartes, yAstartes, 3, tempGrille, visite);

        cout << "  ";
        for (int i = 0; i < 8; i++) cout << i << " ";
        cout << endl;

        for (int y = 0; y < 8; y++) {
            cout << y << " ";
            for (int x = 0; x < 8; x++) {
                cout << tempGrille[y][x] << " ";
            }
            cout << endl;
        }
    }
    bool deplacerAstartes(int nx, int ny)
    {
        int xAstartes;
        int yAstartes;

        placeAstartes(&xAstartes, &yAstartes);
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && grille[ny][nx] == '.') {
            if (abs(nx - xAstartes) + abs(ny - yAstartes) <= 3) {
                grille[ny][nx] = 'A';
                grille[yAstartes][xAstartes] = '.';
                xAstartes = nx;
                yAstartes = ny;
                return true;
            }
            else {
                std::cout << "Deplacement trop loin !" << std::endl;
                return false;
            }
        }
        std::cout << "Deplacement invalide !" << std::endl;
        return false;
    }

    void placeDemon(int* x, int* y) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (grille[i][j] == 'D') {
                    *x = j; // colonne
                    *y = i; // ligne
                    return;
                }
    }

    void deplacerDemon() {
        int xA, yA;
        int xD, yD;

        placeAstartes(&xA, &yA);
        placeDemon(&xD, &yD);

        for (int step = 0; step < 2; step++) { // 2 points de mouvement
            int bestDx = 0;
            int bestDy = 0;
            int minDistance = abs(xA - xD) + abs(yA - yD);

            // 4 directions possibles
            int directions[4][2] = {
                { 1, 0 },  // droite
                { -1, 0 }, // gauche
                { 0, 1 },  // bas
                { 0, -1 }  // haut
            };

            for (int i = 0; i < 4; i++) {
                int nx = xD + directions[i][0];
                int ny = yD + directions[i][1];

                // Vérifie les limites
                if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && grille[ny][nx] == '.') {
                    int distance = abs(xA - nx) + abs(yA - ny);
                    if (distance < minDistance) {
                        minDistance = distance;
                        bestDx = directions[i][0];
                        bestDy = directions[i][1];
                    }
                }
            }

            // Si aucun mouvement n'ameliore la distance -> stop
            if (bestDx == 0 && bestDy == 0) break;

            // Mise à jour de la grille
            grille[yD][xD] = '.';
            xD += bestDx;
            yD += bestDy;
            grille[yD][xD] = 'D';
        }
    }
};
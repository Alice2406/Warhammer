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
                cout << "Case déjà occupée !" << endl;
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
                if (grille[i][j] == 'A')
                {
                    *x = i;
                    *y = j;
                }
            }
    }

    void explorer(int x, int y, int deplacementRestant, char tempGrille[8][8], bool visite[8][8]) {
        if (deplacementRestant == 0) return;

        int dx[4] = { 0, 0, -1, 1 };
        int dy[4] = { -1, 1, 0, 0 };

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                if (!visite[ny][nx] && grille[ny][nx] == '.') {
                    tempGrille[ny][nx] = '*';
                    visite[ny][nx] = true;
                    explorer(nx, ny, deplacementRestant - 1, tempGrille, visite);
                    visite[ny][nx] = false; // permet de revisiter via un autre chemin
                }
            }
        }
    }
    void afficherDeplacementsPossibles() {
        int xAstartes;
        int yAstartes;

        placeAstartes(&xAstartes, &yAstartes);
        char tempGrille[8][8];
        // Copie de la grille pour marquer les déplacements possibles
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
                std::cout << "Déplacement trop loin !" << std::endl;
                return false;
            }
        }
        std::cout << "Déplacement invalide !" << std::endl;
        return false;
    }
};


#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <vector>
#include <map>
using namespace std;

// === Couleurs ANSI ===
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"
#define BOLD    "\033[1m"

struct Node {
    int x, y;
};

class Grille
{
private:
    char grille[10][10];
    bool visite[10][10];

public:
    Grille() {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                grille[i][j] = '.';
    }

    const char(&getGrille() const)[10][10]{
        return grille;
    }
    void setCase(int x, int y, char c) {
        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
            grille[y][x] = c;
        }
        else {
            std::cerr << "Erreur : coordonnées hors grille !" << std::endl;
        }
    }
    void afficher() {
        cout << "  ";
        for (int i = 0; i < 10; i++)
            cout << i << " ";
        cout << endl;

        for (int y = 0; y < 10; y++) {
            cout << y << " ";
            for (int x = 0; x < 10; x++) {
                char c = grille[y][x];
                switch (c) {
                case 'A': cout << BLUE << "A " << RESET; break;
                case 'D': cout << RED << "D " << RESET; break;
                case '#': cout << YELLOW << "# " << RESET; break;
                case '*': cout << GREEN << "* " << RESET; break;
                default:  cout << GRAY << ". " << RESET; break;
                }
            }
            cout << endl;
        }
    }

    bool placerUnite(int x, int y, char unite) {
        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
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
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
            {
                if (grille[i][j] == 'A') {
                    *x = j;  // colonne
                    *y = i;  // ligne
                }
            }
    }

    void placerObstaclesAleatoires(int nombreObstacles) {
        srand((unsigned int)time(0)); // initialiser la graine aléatoire

        int obstaclesPlaces = 0;

        while (obstaclesPlaces < nombreObstacles) {
            int x = rand() % 10; // coordonnées aléatoires sur 10x10
            int y = rand() % 10;

            // Vérifie que la case est vide et ne contient pas Astartes ou Démon
            if (grille[y][x] == '.') {
                grille[y][x] = '#'; // obstacle
                obstaclesPlaces++;
            }
        }
    }

    void animerDeplacement(int xDepart, int yDepart, int xArrivee, int yArrivee, char unite, int vitesse = 150) {
        // Directions : gauche, droite, haut, bas
        int dx[4] = { -1, 1, 0, 0 };
        int dy[4] = { 0, 0, -1, 1 };

        bool visite[10][10] = { false };
        std::map<std::pair<int, int>, std::pair<int, int>> parent; // pour reconstruire le chemin

        std::queue<Node> q;
        q.push({ xDepart, yDepart });
        visite[yDepart][xDepart] = true;

        bool found = false;

        // --- BFS : recherche du chemin le plus court ---
        while (!q.empty()) {
            Node n = q.front();
            q.pop();

            if (n.x == xArrivee && n.y == yArrivee) {
                found = true;
                break;
            }

            for (int i = 0; i < 4; i++) {
                int nx = n.x + dx[i];
                int ny = n.y + dy[i];

                if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 &&
                    !visite[ny][nx] && grille[ny][nx] == '.') {
                    visite[ny][nx] = true;
                    parent[{nx, ny}] = { n.x, n.y };
                    q.push({ nx, ny });
                }
            }
        }

        if (!found) {
            cout << YELLOW << "Aucun chemin trouve — mouvement bloque !" << RESET << endl;
            return;
        }

        // --- Reconstruction du chemin ---
        std::vector<std::pair<int, int>> path;
        int cx = xArrivee, cy = yArrivee;
        path.push_back({ cx, cy });

        while (!(cx == xDepart && cy == yDepart)) {
            auto p = parent[{cx, cy}];
            cx = p.first;
            cy = p.second;
            path.push_back({ cx, cy });
        }

        std::reverse(path.begin(), path.end());

        // --- Animation du déplacement ---
        int x = xDepart;
        int y = yDepart;

        for (size_t i = 1; i < path.size(); i++) {
            int nx = path[i].first;
            int ny = path[i].second;

            // Mise à jour de la grille
            grille[y][x] = '.';
            grille[ny][nx] = unite;

#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            afficher();

            this_thread::sleep_for(chrono::milliseconds(vitesse));

            x = nx;
            y = ny;
        }
    }


    void explorer(int x, int y, int deplacementRestant, char tempGrille[10][10], bool visite[10][10]) {
        if (deplacementRestant == 0) return;

        int dx[4] = { -1, 1, 0, 0 }; // gauche, droite
        int dy[4] = { 0, 0, -1, 1 }; // haut, bas

        for (int i = 0; i < 5; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) {
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
        char tempGrille[10][10];

        // Copie de la grille
        for (int y = 0; y < 10; y++)
            for (int x = 0; x < 10; x++)
                tempGrille[y][x] = grille[y][x];

        // Réinitialise les cases visitées
        for (int y = 0; y < 10; y++)
            for (int x = 0; x < 10; x++)
                visite[y][x] = false;

        visite[yAstartes][xAstartes] = true;

        explorer(xAstartes, yAstartes, 3, tempGrille, visite);

        cout << "  ";
        for (int i = 0; i < 10; i++) cout << i << " ";
        cout << endl;

        for (int y = 0; y < 10; y++) {
            cout << y << " ";
            for (int x = 0; x < 10; x++) {
                char c = tempGrille[y][x];
                switch (c) {
                case 'A': cout << BLUE << "A " << RESET; break;
                case 'D': cout << RED << "D " << RESET; break;
                case '#': cout << YELLOW << "# " << RESET; break;
                case '*': cout << GREEN << "* " << RESET; break;
                case '.': cout << GRAY << ". " << RESET; break;
                default:  cout << WHITE << c << " " << RESET; break;
                }
            }
            cout << endl;
        }
    }

    bool deplacerAstartes(int nx, int ny)
    {
        int xA, yA;
        placeAstartes(&xA, &yA);

        if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && grille[ny][nx] == '.') {
            if (abs(nx - xA) + abs(ny - yA) <= 3) {
                animerDeplacement(xA, yA, nx, ny, 'A', 300);
                return true;
            }
            else {
                cout << YELLOW << "Deplacement trop loin !" << RESET << endl;
                return false;
            }
        }

        cout << RED << "Deplacement invalide !" << RESET << endl;
        return false;
    }

    void placeDemon(int* x, int* y) {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (grille[i][j] == 'D') {
                    *x = j; // colonne
                    *y = i; // ligne
                    return;
                }
    }

    bool deplacerUnite(int nx, int ny)
    {
        int xD, yD;
        placeDemon(&xD, &yD);

        if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && grille[ny][nx] == '.') {
            if (abs(nx - xD) + abs(ny - yD) <= 3) {
                animerDeplacement(xD, yD, nx, ny, 'D', 300);
                return true;
            }
            else {
                cout << YELLOW << "Deplacement trop loin !" << RESET << endl;
                return false;
            }
        }
        cout << RED << "Deplacement invalide !" << RESET << endl;
        return false;
    }


    void afficherDeplacementsPossiblesDemon() {
        int xDemon;
        int yDemon;

        placeDemon(&xDemon, &yDemon);
        char tempGrille[10][10];

        // Copie de la grille
        for (int y = 0; y < 10; y++)
            for (int x = 0; x < 10; x++)
                tempGrille[y][x] = grille[y][x];

        for (int y = 0; y < 10; y++)
            for (int x = 0; x < 10; x++)
                visite[y][x] = false;

        visite[yDemon][xDemon] = true;

        explorer(xDemon, yDemon, 3, tempGrille, visite);

        cout << "  ";
        for (int i = 0; i < 10; i++) cout << i << " ";
        cout << endl;

        for (int y = 0; y < 10; y++) {
            cout << y << " ";
            for (int x = 0; x < 10; x++) {
                char c = tempGrille[y][x];
                switch (c) {
                case 'A': cout << BLUE << "A " << RESET; break;
                case 'D': cout << RED << "D " << RESET; break;
                case '#': cout << YELLOW << "# " << RESET; break;
                case '*': cout << MAGENTA << "* " << RESET; break; // violet pour les zones du démon
                case '.': cout << GRAY << ". " << RESET; break;
                default:  cout << WHITE << c << " " << RESET; break;
                }
            }
            cout << endl;
        }
    }

    void deplacerDemon() {
        int xA, yA, xD, yD;
        placeAstartes(&xA, &yA);
        placeDemon(&xD, &yD);

        int diffX = xA - xD;
        int diffY = yA - yD;

        int totalSteps = 3; // distance max du démon

        // On va le faire avancer progressivement vers l'Astartes
        int nx = xD;
        int ny = yD;

        // On répartit les 3 pas entre X et Y, sans diagonale directe
        while (totalSteps > 0 && (nx != xA || ny != yA)) {
            int stepX = 0;
            int stepY = 0;

            // On choisit quel axe bouger à ce tour :
            // priorité à l'axe où la distance est la plus grande
            if (abs(xA - nx) > abs(yA - ny)) {
                stepX = (xA > nx) ? 1 : -1;
            }
            else if (abs(yA - ny) > 0) {
                stepY = (yA > ny) ? 1 : -1;
            }

            int nextX = nx + stepX;
            int nextY = ny + stepY;

            // Si la case est libre, on avance
            if (nextX >= 0 && nextX < 10 && nextY >= 0 && nextY < 10 && grille[nextY][nextX] == '.') {
                grille[ny][nx] = '.';
                grille[nextY][nextX] = 'D';
                nx = nextX;
                ny = nextY;

#ifdef _WIN32
                system("cls");
#else
                system("clear");
#endif
                afficher();
                this_thread::sleep_for(chrono::milliseconds(250));
            }
            else {
                break; // bloqué, on s'arrête
            }

            totalSteps--;
        }
    }

};
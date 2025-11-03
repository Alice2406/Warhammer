#include <iostream>
#include <string>

using namespace std;

class Arme {
    string nom;
    int degats;
    int portee;

public:
    Arme(string n = "Bolter", int d = 5, int p = 3)
        : nom(n), degats(d), portee(p) {
    }

    string getNom() const { return nom; }
    int getDegats() const { return degats; }
    int getPortee() const { return portee; }

    void afficher() const {
        cout << "- " << nom << " (degats: " << degats << ", portee: " << portee << ")" << endl;
    }
};

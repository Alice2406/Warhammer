#ifndef DES_H
#define DES_H

#include <ctime>
#include <iostream>

class Des
{
private :
	int valeur;

public :
	Des() { valeur = 0; }
	int getVal() const { return valeur; }
	void lancer() { valeur = rand() % 6 + 1;}
};
#endif

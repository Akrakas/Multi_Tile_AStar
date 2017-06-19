#ifndef ASTAR_H
#define ASTAR_H
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <time.h>

using namespace std;

typedef struct STRUCT_CASE {
	double encombrement;
	int aireLibre;
	int new_encombrement;
	char symbol;
}CASE;

class CLASS_MAP
{
	public : 
		CASE** grille;
		int map_x;
		int map_y;
};

class NOEUD {
	public :
		int x,y;
		double heuristique; 
		int cout, distance;
		int prex, prey;
		NOEUD();
		NOEUD(int _x, int _y);
};

class DEPLACEMENT {
	public : 
		char x, y;
};

class CHEMIN {
	public :
		vector<DEPLACEMENT> deplacement;
		int errorCode;
};

int compare2Noeuds(NOEUD n1, NOEUD n2);
void updateMapTaille(CLASS_MAP* map, int taille);
CHEMIN astar(CLASS_MAP* map, NOEUD depart, NOEUD arrivee, int taille, bool intelligent, bool debug);

#endif // ASTAR_H

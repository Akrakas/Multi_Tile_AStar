#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <time.h>
#include <fstream>
#include <iostream>
#include "ASTAR.h"

void importerMap(CLASS_MAP* map, string path);
void creerMap(CLASS_MAP* map, int size_x, int size_y);
void supprimerMap(CLASS_MAP* map);
void remplirGrille(CLASS_MAP* map, int nbr_objects);
void calculateAireLibre(CLASS_MAP* map);
void afficherMap(CLASS_MAP* map);

#endif // FUNCTIONS_H

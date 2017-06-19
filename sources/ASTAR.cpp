#include "ASTAR.h"


NOEUD::NOEUD()
{
}

NOEUD::NOEUD(int _x, int _y)
{
	x = _x;
	y = _y;
}

int compare2Noeuds(NOEUD n1, NOEUD n2)
{
	return (n1.heuristique > n2.heuristique);
}

void updateMapTaille(CLASS_MAP* map, int taille){
	for(int i=(map->map_x)-taille ; i>=0 ; i--)
	{
		for(int j=(map->map_y)-taille ; j>=0 ; j--)
		{
			(map->grille[i][j]).new_encombrement = 0;
			for(int si=0 ; si<taille ; si++)
			{
				for(int sj=0 ; sj<taille ; sj++)
				{
					(map->grille[i][j]).new_encombrement += (map->grille[i+si][j+sj]).encombrement;
				}
			}
			//printf("i = %d, j = %d\n", i, j);
			//printf("airLibre = %d\n\n", (map->grille[i][j]).aireLibre);
		}
	}
}

CHEMIN astar(CLASS_MAP* map, NOEUD depart, NOEUD arrivee, int taille, bool intelligent, bool debug)
{
	updateMapTaille(map, taille);
	CHEMIN returnPath;
	NOEUD temp;
	vector<NOEUD> closedList;
	vector<NOEUD> openList;
	openList.push_back(depart);
	while(openList.size() > 0)
	{
		std::sort(openList.begin(), openList.end(), compare2Noeuds);
		temp = openList.back();
		openList.pop_back();
		if(temp.x == arrivee.x && temp.y == arrivee.y) 
		{
			while(temp.x != depart.x || temp.y != depart.y) //On remonte les noeud en partant de l'arrivée jusqu'a arriver au depart
			{
				DEPLACEMENT tempDeplacement;
				tempDeplacement.x = temp.x - temp.prex;
				tempDeplacement.y = temp.y - temp.prey;
				returnPath.deplacement.push_back(tempDeplacement);
				for(int k=0 ; k<closedList.size() ; k++) //Recherche du noeud dans la liste fermee
				{
					if(temp.prex == closedList[k].x && temp.prey == closedList[k].y)
					{
						temp = closedList[k];
						continue;
					}
				}
			}
			returnPath.errorCode = 0;
			return returnPath;
		}
		else
		{
			for(int i=-1 ; i<=1 ; i++)
			{
				for(int j=-1 ; j<=1 ; j++)
				{
					if(i==0 && j==0) continue;
					if(temp.x + i >= map->map_x || temp.x + i < 0 || temp.y + j >= map->map_y || temp.y + j < 0) continue;
					if(i != 0 && j != 0) continue;
					NOEUD potentiel;
					potentiel.x = temp.x + i;
					potentiel.y = temp.y + j;
					potentiel.cout = temp.cout + (map->grille[potentiel.x][potentiel.y]).new_encombrement;
					potentiel.prex = temp.x;
					potentiel.prey = temp.y;
					if((map->grille[potentiel.x][potentiel.y]).aireLibre >= taille) //Si la case est accessible par l'objet
					{
						bool isPresentInClosedList = false;
						bool isPresentInOpenList = false;
						for(int k=0 ; k<closedList.size() ; k++) //Recherche du noeud dans la liste fermee
						{
							if(closedList[k].x == potentiel.x && closedList[k].y == potentiel.y && closedList[k].cout <= potentiel.cout) isPresentInClosedList = true;
						}
						for(int k=0 ; k<openList.size() ; k++) //Recherche du noeud dans la liste ouverte
						{
							if(openList[k].x == potentiel.x && openList[k].y == potentiel.y && openList[k].cout <= potentiel.cout) isPresentInOpenList = true;
						}
						if(!isPresentInClosedList && !isPresentInOpenList)
						{
							potentiel.heuristique = potentiel.cout + (std::sqrt(std::abs((potentiel.x - arrivee.x)*(potentiel.x - arrivee.x)) + std::abs((potentiel.y - arrivee.y)*(potentiel.y - arrivee.y))));
							potentiel.distance = (std::abs(potentiel.x - arrivee.x) + std::abs(potentiel.y - arrivee.y));
							if(debug) 
							{
								if(potentiel.heuristique > 20.0) map->grille[potentiel.x][potentiel.y].symbol = '+';
								else if(potentiel.heuristique > 10.0) map->grille[potentiel.x][potentiel.y].symbol = '0' + (char)potentiel.heuristique -10;
								else map->grille[potentiel.x][potentiel.y].symbol = '-';
							}
							openList.push_back(potentiel);
						}
					}
				}
			}
		}
		closedList.push_back(temp);
	}
	temp = closedList.back();
	for(int k=1 ; k<closedList.size() ; k++) //Recherche du noeud le plus proche de l'objectif
	{
		if(intelligent == 0 && closedList[k].heuristique < temp.heuristique) temp = closedList[k];
		if(intelligent == 1 && closedList[k].distance < temp.distance) temp = closedList[k];
	}
	while(temp.x != depart.x || temp.y != depart.y) //On remonte les noeud en partant de l'arrivée jusqu'a arriver au depart
	{
		DEPLACEMENT tempDeplacement;
		tempDeplacement.x = temp.x - temp.prex;
		tempDeplacement.y = temp.y - temp.prey;
		returnPath.deplacement.push_back(tempDeplacement);
		for(int k=0 ; k<closedList.size() ; k++) //Recherche du noeud dans la liste fermee
		{
			if(temp.prex == closedList[k].x && temp.prey == closedList[k].y)
			{
				temp = closedList[k];
				continue;
			}
		}
	}
	returnPath.errorCode = -1;
	return returnPath;
}


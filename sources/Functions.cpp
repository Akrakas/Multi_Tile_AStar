#include "Functions.h"


void importerMap(CLASS_MAP* map, string path)
{
    ifstream map_file (path, ios::in);
	if (map_file.fail()){perror(("Can't find " + path).c_str());return;}
	string buffer;
	for(int j=0 ; j<map->map_y ; j++) {
		map_file >> buffer;
		for(int i=0 ; i<map->map_x ; i++) {
			if(buffer[i] == '.') {
				(map->grille[i][j]).encombrement = 1;
				(map->grille[i][j]).aireLibre = 0;
				(map->grille[i][j]).new_encombrement = 0;
				(map->grille[i][j]).symbol = '.';
			}
			else if(buffer[i] == '~') {
				(map->grille[i][j]).encombrement = 6;
				(map->grille[i][j]).aireLibre = 0;
				(map->grille[i][j]).new_encombrement = 0;
				(map->grille[i][j]).symbol = '~';
			}
			else if(buffer[i] == '#') {
				(map->grille[i][j]).encombrement = 20;
				(map->grille[i][j]).aireLibre = 0;
				(map->grille[i][j]).new_encombrement = 0;
				(map->grille[i][j]).symbol = '#';
			}
		}
	}
	map_file.close();
}

void creerMap(CLASS_MAP* map, int size_x, int size_y)
{
	map->map_x = size_x;
	map->map_y = size_y;
	map->grille = (CASE**)malloc(sizeof(CASE*) * map->map_x);
	for(int ix=0 ; ix<map->map_x ; ix++)
	{
		map->grille[ix] = (CASE*)malloc(sizeof(CASE) * map->map_y);
	}
}

void supprimerMap(CLASS_MAP* map)
{
	for(int ix=0 ; ix<map->map_x ; ix++)
	{
		free(map->grille[ix]);
	}
	free(map->grille);
}

void remplirGrille(CLASS_MAP* map, int nbr_objects)
{
	for(int i=(map->map_x)-1 ; i>=0 ; i--)
	{
		for(int j=(map->map_y)-1 ; j>=0 ; j--)
		{
			(map->grille[i][j]).encombrement = 1;
			(map->grille[i][j]).aireLibre = 0;
			(map->grille[i][j]).new_encombrement = 0;
			(map->grille[i][j]).symbol = '.';
		}
	}
	for(int i=0 ; i<nbr_objects ; i++)
	{
		int rx = (rand()%((map->map_x)-1))+1;
		int ry = (rand()%((map->map_y)-1))+1;
		for(int u=-1 ; u<=1 ; u++)
		{
			for(int v=-1 ; v<=1 ; v++)
			{
				if(rx+u >= (map->map_x) || rx+u < 0 || ry+v >= (map->map_y) || ry+v < 0) continue;
				(map->grille[rx+u][ry+v]).encombrement = 2;
				(map->grille[rx+u][ry+v]).symbol = '~';
			}
		}
	}
	for(int i=0 ; i<nbr_objects ; i++)
	{
		int rx = (rand()%((map->map_x)-1))+1;
		int ry = (rand()%((map->map_y)-1))+1;
		for(int u=-1 ; u<=1 ; u++)
		{
			for(int v=-1 ; v<=1 ; v++)
			{
				if(rx+u >= (map->map_x) || rx+u < 0 || ry+v >= (map->map_y) || ry+v < 0) continue;
				(map->grille[rx+u][ry+v]).encombrement = 20;
				(map->grille[rx+u][ry+v]).symbol = '#';
			}
		}
	}
}

void calculateAireLibre(CLASS_MAP* map)
{
	for(int i=(map->map_x)-1 ; i>=0 ; i--)
	{
		for(int j=(map->map_y)-1 ; j>=0 ; j--)
		{
			if((map->grille[i][j]).encombrement > 10) 
			{
				(map->grille[i][j]).aireLibre = 0;
				continue;
			}
			if(i==(map->map_x)-1 || j==(map->map_y)-1) (map->grille[i][j]).aireLibre = 1;
			else (map->grille[i][j]).aireLibre = std::min(std::min((map->grille[i+1][j]).aireLibre, (map->grille[i][j+1]).aireLibre), (map->grille[i+1][j+1]).aireLibre) + 1;
			//printf("i = %d, j = %d\n", i, j);
			//printf("airLibre = %d\n\n", (map->grille[i][j]).aireLibre);
		}
	}
}

void afficherMap(CLASS_MAP* map)
{
	
	for(int j=0 ; j<(map->map_y) ; j++)
	{
		for(int i=0 ; i<(map->map_x) ; i++)
		{
			//if((map->grille[i][j]).aireLibre < 10) printf("%d",(map->grille[i][j]).aireLibre);
			//else  printf("+");
			printf("%c",(map->grille[i][j]).symbol);
		}
		puts("");
	}
}

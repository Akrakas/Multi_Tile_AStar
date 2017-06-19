#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "ASTAR.h"
#include "Functions.h"
#include <unistd.h>
#define WIN_X 420
#define WIN_Y 360
#define FPS 5

using namespace cv;

int main()
{


	srand(time(NULL));
	int sizeX = 20;
	int sizeY = 10;
	int ratio = 40;
	int windowX = sizeX*ratio;
	int windowY = sizeY*ratio + 20;
	int taille = 1;

	Mat grass_sprite_temp = imread( "grass_sprite.jpg", 1 );
	Mat water_sprite_temp = imread( "water_sprite.jpg", 1 );
	Mat lava_sprite_temp = imread( "lava_sprite.png", 1 );
	Mat wall_sprite_temp = imread( "wall_sprite.png", 1 );
	Mat flag_sprite_temp = imread( "flag_sprite.png", 1 );
	Mat grass_sprite; 
	resize(grass_sprite_temp, grass_sprite, Size(ratio, ratio), 0, 0, INTER_LINEAR);
	Mat water_sprite; 
	resize(water_sprite_temp, water_sprite, Size(ratio, ratio), 0, 0, INTER_LINEAR);
	Mat lava_sprite; 
	resize(lava_sprite_temp, lava_sprite, Size(ratio, ratio), 0, 0, INTER_LINEAR);
	Mat wall_sprite; 
	resize(wall_sprite_temp, wall_sprite, Size(ratio, ratio), 0, 0, INTER_LINEAR);
	Mat flag_sprite; 
	resize(flag_sprite_temp, flag_sprite, Size(ratio, ratio), 0, 0, INTER_LINEAR);

	NOEUD depart(0,0);
	NOEUD arrivee(sizeX-2,1);

	CHEMIN path;
	CLASS_MAP map;
	creerMap(&map, sizeX, sizeY);
	importerMap(&map, "./complex.txt");
	//remplirGrille(&map, 4);

	string main_window_name = "Astar Demo";
	Mat frame(windowY, windowX, CV_8UC3);
	putText(frame, String("Size = " + to_string(taille) + "     Water cost = " + "5"), Point(2, windowY-2), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255), 1, 8, false);
	for(int i=0 ; i<sizeX ; i++) {
		for(int j=0 ; j<sizeY ; j++) {
			Rect square(i*ratio, j*ratio, ratio, ratio);
			if(map.grille[i][j].encombrement >= 10) wall_sprite.copyTo(frame(square));
			else if(map.grille[i][j].encombrement >= 2) water_sprite.copyTo(frame(square));
			else if(map.grille[i][j].encombrement == 1) grass_sprite.copyTo(frame(square));


		}
	}
	Rect square(arrivee.x*ratio, arrivee.y*ratio, ratio, ratio);
	flag_sprite.copyTo(frame(square));
	imshow( main_window_name, frame );
	waitKey(5000);
	
	VideoWriter outputvid("./Test_out.avi", VideoWriter::fourcc('D','I','V','X'), 30, Size(windowX, windowY), true);
	for(int i=0 ; i<30 ; i++) {
		outputvid << frame;
	}

	calculateAireLibre(&map);
	afficherMap(&map);

	depart.cout = 0;
	depart.heuristique = 0;

	path = astar(&map, depart, arrivee, taille, true, false);
	printf("ASTAR status = %d\n", path.errorCode);
	int tpx = depart.x;
	int tpy = depart.y;
	while(path.deplacement.size() > 0)
	{
		Point point1((tpx*ratio)+(ratio*taille/2.0), (tpy*ratio)+(ratio*taille/2.0));
		tpx = tpx + path.deplacement.back().x;
		tpy = tpy + path.deplacement.back().y;
		Point point2((tpx*ratio)+(ratio*taille/2.0), (tpy*ratio)+(ratio*taille/2.0));
		path.deplacement.pop_back();
		(map.grille[tpx][tpy]).symbol = 'o';
		Scalar color(0,255,255);

		for(int i=1 ; i<=FPS ; i++) {
			double ddx = point2.x - point1.x;
			double ddy = point2.y - point1.y;
			printf("%f\n", ddx*(i/(double)FPS));
			double dx = ddx*(i/(double)FPS);
			double dy = ddy*(i/(double)FPS);
			Mat temp = frame.clone();
			Rect square(((point1.x - (ratio*taille/2.0)) + dx), ((point1.y - (ratio*taille/2.0)) + dy), ratio*taille, ratio*taille);
			//Rect square(tpx*ratio, tpy*ratio, ratio*taille, ratio*taille);
			rectangle(temp, square, color, 0.1*ratio, 8, 0 );
			imshow( main_window_name, temp );
			waitKey(10);
			outputvid << temp;
		}
		line(frame, point1, point2, color, 0.15*ratio, 8, 0);
		Mat temp = frame.clone();
		//Rect square(tpx*ratio, tpy*ratio, ratio*taille, ratio*taille);
		//rectangle(temp, square, color, 0.1*ratio, 8, 0 );

		//imshow( main_window_name, temp );
		//waitKey(100);
	}
	for(int i=0 ; i<30 ; i++) {
		outputvid << frame;
	}
	(map.grille[depart.x][depart.y]).symbol = 'D';
	(map.grille[arrivee.x][arrivee.y]).symbol = 'A';
	afficherMap(&map);
	supprimerMap(&map);
	return 0;
}

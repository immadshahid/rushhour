//============================================================================
// Name        : .cpp
// Author      : FAST CS Department
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Rush Hour...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include <fstream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */



void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

bool game_over = false; //For game over.


int xI = 85, yI = 375; //Taxi size

string player_names[10]; //To store player name.
string player_scores[10]; //To store player score.

string player;
int score = 0;




int time_remaining = 180000;    // Time in milli seconds. (3 minutes)

struct man {
	int x;
	int y;
	bool isOnBoard;
	int color;
};

man men[5];

char carDirection = 'W'; 		// E - East, W - West, N - North, S - South Directions. North is pointing Upward.

int otherCarX = 285, otherCarY = 375;

char otherCarDirection = 'W';



int secondCarX = 485, secondCarY = 375;

char secondCarDirection = 'W';


int thirdCarX = 430, thirdCarY = 100;

char thirdCarDirection = 'S';

struct location {
	int x;
	int y;
};

location locations[80]; //Locations set i.e houses.


void storeLocations() {
	int i = 0;

	for(int n = 100; n < 350; n+=50) {
		locations[i + 0].x = n;	locations[i + 0].y = 60;
		locations[i + 1].x = n;	locations[i + 1].y = 210;
		locations[i + 2].x = n;	locations[i + 2].y = 510;
		locations[i + 3].x = n;	locations[i + 3].y = 660;
		i += 4;
	}

	for(int n = 100; n < 350; n+=50) {
		locations[i + 0].x = n;	locations[i + 0].y = 130;
		locations[i + 1].x = n;	locations[i + 1].y = 280;
		locations[i + 2].x = n;	locations[i + 2].y = 580;
		locations[i + 3].x = n;	locations[i + 3].y = 730;
		i += 4;
	}

	for(int n = 700; n < 950; n+=50) {
		locations[i + 0].x = n;	locations[i + 0].y = 60;
		locations[i + 1].x = n;	locations[i + 1].y = 210;
		locations[i + 2].x = n;	locations[i + 2].y = 510;
		locations[i + 3].x = n;	locations[i + 3].y = 660;
		i += 4;
	}

	for(int n = 700; n < 950; n+=50) {
		locations[i + 0].x = n;	locations[i + 0].y = 130;
		locations[i + 1].x = n;	locations[i + 1].y = 280;
		locations[i + 2].x = n;	locations[i + 2].y = 580;
		locations[i + 3].x = n;	locations[i + 3].y = 730;
		i += 4;
	}	
}



void sortPlayers() {  //Sorting of the players and scores.

  for (int step = 0; step < 9; ++step) {
      
    // This loop is applied to compare the elements of the Array, in which we have stored player scores and player names.
    for (int i = 0; i < 10 - step; ++i) {

      // In order to compare two elements i.e player names and player scores.
      // Having it in Descending order.
      if (player_scores[i] < player_scores[i + 1]) {

        //Swapping of the Elements. 
        string temp_score = player_scores[i];
		string temp_name = player_names[i];

        player_scores[i] = player_scores[i + 1];
        player_names[i] = player_names[i + 1];

        player_scores[i + 1] = temp_score;
        player_names[i + 1] = temp_name;	
      }
    }
  }

}

//File reading of player names and player scores using file handling.
void readPlayers() {
		ifstream file;

		file.open("highscores.txt");

		for(int i = 0; i < 10; i++) {
			getline(file, player_names[i]);
			getline(file, player_scores[i]);

		}

		for(int i = 0; i < 10; i++) {
			if(player_names[i] == "") {
				player_names[i] = "NO NAME";
				player_scores[i] = "0"; 
			}
		}

		sortPlayers();

		file.close();
	
			
}
//File writing of player names and scores using file handling.
void writePlayers() { 

	if(player_scores[9] == "") {
		player_scores[9] = "0";
	}


	if(score > 0 && stoi(player_scores[9]) <= score) {
		player_scores[9] = to_string(score);
		player_names[9] = player;


		//sortPlayers();

		ofstream file;
		file.open("highscores.txt");


		for(int i = 0; i < 10; i++) {
			file << player_names[i] << endl;
			file << player_scores[i] << endl;

			cout << player_names[i] << endl;
			cout << player_scores[i] << endl;
			
		}
		
		file.close();	
	}


}








// Function for distance using the Pythagoras' Theorem, distance formula.
bool isCloser(int x1, int y1, int x2, int y2, int radius) {
	int d;

	d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

	if(d > (radius * radius)) {
		return false;
	}
	else {
		return true;
	}
}
// Function for Collision Detection.
bool isCollided(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
	if (x1 < x2 + w2 &&
        x1 + w1 > x2 &&
        y1 < y2 + h2 &&
        h1 + y1 > y2) {
			return true;
	}
	else {
		return false;
	}
}
//Drwaing the taxi on the board, using the DrawRectangle(); Function.
void drawCar() {

	switch (carDirection) {
		case 'W':
			DrawRectangle(xI - 25, yI - 10, 50, 20, colors[YELLOW]);
			DrawRectangle(xI - 20, yI - 9, 8, 18, colors[BLACK]);
			DrawRectangle(xI + 3, yI - 9, 10, 18, colors[BLACK]);
			break;

		case 'E':
			DrawRectangle(xI - 25, yI - 10, 50, 20, colors[YELLOW]);
			DrawRectangle(xI + 13, yI - 9, 8, 18, colors[BLACK]);
			DrawRectangle(xI - 15, yI - 9, 10, 18, colors[BLACK]);
			break;

		case 'N':
			DrawRectangle(xI - 10, yI - 25, 20, 50, colors[YELLOW]);
			DrawRectangle(xI - 9, yI - 20, 18, 8, colors[BLACK]);
			DrawRectangle(xI - 9, yI + 3, 18, 10, colors[BLACK]);
			break;

		case 'S':
			DrawRectangle(xI -10 , yI - 25, 20, 50, colors[YELLOW]);
			DrawRectangle(xI - 9, yI + 13, 18, 8, colors[BLACK]);
			DrawRectangle(xI - 9, yI - 15, 18, 10, colors[BLACK]);
			break;

	}
	glutPostRedisplay();
}

//Drawing the other Cars using the same function but have different colors.
void drawOtherCar() {

	switch (otherCarDirection) {
		case 'W':
			DrawRectangle(otherCarX - 25, otherCarY - 10, 50, 20, colors[BLUE]);
			DrawRectangle(otherCarX - 20, otherCarY - 9, 8, 18, colors[BLACK]);
			DrawRectangle(otherCarX + 3, otherCarY - 9, 10, 18, colors[BLACK]);
			break;

		case 'E':
			DrawRectangle(otherCarX - 25, otherCarY - 10, 50, 20, colors[BLUE]);
			DrawRectangle(otherCarX + 13, otherCarY - 9, 8, 18, colors[BLACK]);
			DrawRectangle(otherCarX - 15, otherCarY - 9, 10, 18, colors[BLACK]);
			break;

		case 'N':
			DrawRectangle(otherCarX - 10, otherCarY - 25, 20, 50, colors[BLUE]);
			DrawRectangle(otherCarX - 9, otherCarY - 20, 18, 8, colors[BLACK]);
			DrawRectangle(otherCarX - 9, otherCarY + 3, 18, 10, colors[BLACK]);
			break;

		case 'S':
			DrawRectangle(otherCarX -10 , otherCarY - 25, 20, 50, colors[BLUE]);
			DrawRectangle(otherCarX - 9, otherCarY + 13, 18, 8, colors[BLACK]);
			DrawRectangle(otherCarX - 9, otherCarY - 15, 18, 10, colors[BLACK]);
			break;

	}
	glutPostRedisplay();
}


void drawSecondCar() {

	switch (secondCarDirection) {
		case 'W':
			DrawRectangle(secondCarX - 25, secondCarY - 10, 50, 20, colors[RED]);
			DrawRectangle(secondCarX - 20, secondCarY - 9, 8, 18, colors[BLACK]);
			DrawRectangle(secondCarX + 3, secondCarY - 9, 10, 18, colors[BLACK]);
			break;

		case 'E':
			DrawRectangle(secondCarX - 25, secondCarY - 10, 50, 20, colors[RED]);
			DrawRectangle(secondCarX + 13, secondCarY - 9, 8, 18, colors[BLACK]);
			DrawRectangle(secondCarX - 15, secondCarY - 9, 10, 18, colors[BLACK]);
			break;

		case 'N':
			DrawRectangle(secondCarX - 10, secondCarY - 25, 20, 50, colors[RED]);
			DrawRectangle(secondCarX - 9, secondCarY - 20, 18, 8, colors[BLACK]);
			DrawRectangle(secondCarX - 9, secondCarY + 3, 18, 10, colors[BLACK]);
			break;

		case 'S':
			DrawRectangle(secondCarX -10 , secondCarY - 25, 20, 50, colors[RED]);
			DrawRectangle(secondCarX - 9, secondCarY + 13, 18, 8, colors[BLACK]);
			DrawRectangle(secondCarX - 9, secondCarY - 15, 18, 10, colors[BLACK]);
			break;

	}
	glutPostRedisplay();
}

void drawThirdCar() {

	switch (thirdCarDirection) {
		case 'W':
			DrawRectangle(thirdCarX - 25, thirdCarY - 10, 50, 20, colors[GREEN]);
			DrawRectangle(thirdCarX - 20, thirdCarY - 9, 8, 18, colors[BLACK]);
			DrawRectangle(thirdCarX + 3, thirdCarY - 9, 10, 18, colors[BLACK]);
			break;

		case 'E':
			DrawRectangle(thirdCarX - 25, thirdCarY - 10, 50, 20, colors[GREEN]);
			DrawRectangle(thirdCarX + 13, thirdCarY - 9, 8, 18, colors[BLACK]);
			DrawRectangle(thirdCarX - 15, thirdCarY - 9, 10, 18, colors[BLACK]);
			break;

		case 'N':
			DrawRectangle(thirdCarX - 10, thirdCarY - 25, 20, 50, colors[GREEN]);
			DrawRectangle(thirdCarX - 9, thirdCarY - 20, 18, 8, colors[BLACK]);
			DrawRectangle(thirdCarX - 9, thirdCarY + 3, 18, 10, colors[BLACK]);
			break;

		case 'S':
			DrawRectangle(thirdCarX -10 , thirdCarY - 25, 20, 50, colors[GREEN]);
			DrawRectangle(thirdCarX - 9, thirdCarY + 13, 18, 8, colors[BLACK]);
			DrawRectangle(thirdCarX - 9, thirdCarY - 15, 18, 10, colors[BLACK]);
			break;

	}
	glutPostRedisplay();
}





//Drawing the function by using the function drawMan(); 
void drawMan(int x, int y, int color = BROWN) {
	DrawRectangle(x - 5, y - 8, 10, 16, colors[color]);

	DrawCircle(x, y + 14, 4, colors[color]);

	DrawLine(x - 5, y - 18, x - 5, y - 8, 2, colors[color]);
	DrawLine(x + 4, y - 18, x + 4, y - 8, 2, colors[color]);

	DrawLine(x - 10, y - 2 , x - 6, y + 8, 2, colors[color]);
	DrawLine(x + 9, y - 2 , x + 5, y + 8, 2, colors[color]);

	glutPostRedisplay();
}

//Enabling the Taxi to move.
bool flag = true;

void moveCar() {
	
	if (xI > 10 && flag) {
		xI -= 10;
		cout << "going left";
		if(xI < 100)
			
			flag = false;

	}
	else if (xI < 1010 && !flag) {
		cout << "go right";
		xI += 10;
		if (xI > 900)
			flag = true;
	}
}

//Moving the car using moveOtherCar(); function...To keep them moving.
void moveOtherCar() {
	if(!isCloser(otherCarX, otherCarY, xI, yI, 100) ) {	
		switch (otherCarDirection) {
			case 'W':
				if(otherCarX < 980) {
					otherCarX += 5;
				}
				else {
					otherCarDirection = 'N';
				}
				break;

			case 'E':
				if(otherCarX > 40) {
					otherCarX -= 5;
				}
				else {
					otherCarDirection = 'S';
				}
				break;

			case 'N':
				if(otherCarY < 780) {
					otherCarY += 5;
				}
				else {
					otherCarDirection = 'E';
				}
				break;

			case 'S':
				if(otherCarY > 40) {
					otherCarY -= 5;
				}
				else {
					otherCarDirection = 'W';
				}
				break;

		}
	}
}
//Moving for the second car.
void moveSecondCar() {
	
	if(!isCloser(secondCarX, secondCarY, otherCarX, otherCarY, 100) && 
		!isCloser(secondCarX, secondCarY, xI, yI, 100) 
	) {
		switch (secondCarDirection) {
			case 'W':
				if(secondCarX < 980) {
					secondCarX += 5;
				}
				else {
					secondCarDirection = 'N';
				}
				break;

			case 'E':
				if(secondCarX > 40) {
					secondCarX -= 5;
				}
				else {
					secondCarDirection = 'S';
				}
				break;

			case 'N':
				if(secondCarY < 445) {
					secondCarY += 5;
				}
				else {
					secondCarDirection = 'E';
				}
				break;

			case 'S':
				if(secondCarY > 375) {
					secondCarY -= 5;
				}
				else {
					secondCarDirection = 'W';
				}
				break;

		}
	}
}

//Moving for the third car.
void moveThirdCar() {

	if(!isCloser(thirdCarX, thirdCarY, otherCarX, otherCarY, 100) && 
		!isCloser(thirdCarX, thirdCarY, secondCarX, secondCarY, 100) &&
		!isCloser(thirdCarX, thirdCarY, xI, yI, 100) 
	) {
		switch (thirdCarDirection) {
			case 'W':
				if(thirdCarX < 590) {
					thirdCarX += 5;
				}
				else {
					thirdCarDirection = 'N';
				}
				break;

			case 'E':
				if(thirdCarX > 430) {
					thirdCarX -= 5;
				}
				else {
					thirdCarDirection = 'S';
				}
				break;

			case 'N':
				if(thirdCarY < 780) {
					thirdCarY += 5;
				}
				else {
					thirdCarDirection = 'E';
				}
				break;

			case 'S':
				if(thirdCarY > 40) {
					thirdCarY -= 5;
				}
				else {
					thirdCarDirection = 'W';
				}
				break;

		}


	}
	
}

//Drawing the houses (Function Declared).
void DrawHouse(int x, int y) {
	DrawRectangle(x, y, 20, 20, colors[WHITE]);
	DrawRectangle(x+7, y, 6, 12, colors[BLACK]);
	DrawTriangle(x-5, y+20, x + 25, y+20, x+10, y+30, colors[RED]);
}


//Drwaing a flag to indicate the drop-off location.
void DrawFlag(int x, int y) {
	x -= 10;
	y += 10;
	DrawRectangle(x, y, 30, 20, colors[RED]);
	DrawLine(x, y - 20, x, y + 22, 2, colors[BLACK]);
}

//setting random locations for the passengers.
void setMenLocations() {
	int random;
	for(int i = 0; i < 5; i++) {
		random = 0 + (rand() % 79);
		men[i].x = locations[random].x;
		men[i].y = locations[random].y;
		men[i].isOnBoard = false;
		men[i].color = BROWN;
	}
}



// Main Canvas drawing function.

bool game_started = false;


void GameDisplay(){
	//Game Display screen. 
	glClearColor(0/*Red Component*/, 0,	//148.0/255/*Green Component*/,
			0.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); 


//Our Game Starter Menu.
	if(!game_started) {

		DrawString(450, 800, "Hall of Fame", colors[GREEN]);

		for(int i = 0; i < 10; i++) {
			DrawString(300, 750 - (i * 50), player_names[i], colors[YELLOW]);
			DrawString(690, 750 - (i * 50), player_scores[i], colors[YELLOW]);
		}

		DrawRectangle(300, 100, 400, 50, colors[GREEN]);

		DrawString(300, 160, "Type in the player name and press Enter to start.", colors[GREEN]);

		DrawString(320, 120, player, colors[BLACK]);

	}
	else if(game_over) {
		DrawRectangle(380, 400, 260, 150, colors[RED]);
		DrawString(450, 500, "Game Over", colors[BLACK]);

		DrawString(420, 440, player, colors[YELLOW]);
		DrawString(570, 440, to_string(score), colors[YELLOW]);

		DrawString(420, 300, "Press Esc key to exit.", colors[YELLOW]);
	}
	else {

		
		//Display Score on the top of the Game play.
		DrawString( 50, 800, player + "    Score: " + to_string(score), colors[SKY_BLUE]);
		int seconds = time_remaining / 1000;
		int mins = int(seconds / 60);
		int secs = seconds % 60;
		DrawString( 450, 800, "Time Remaining: " + to_string(mins) + ":" + to_string(secs), colors[SKY_BLUE]);
		

		DrawRectangle(0, 0, 9, 839, colors[RED]);
		DrawRectangle(0, 0, 1019, 9, colors[RED]);
		DrawRectangle(1010, 0, 1019, 839, colors[RED]);
		DrawRectangle(0, 830, 1019, 839, colors[RED]);
		
		
		//Round Rectangles to indicate green and mauve areas...Green areas have houses and mauve areas are Business hubs.
		DrawRoundRect(60, 60, 300, 100, colors[DARK_GREEN], 10);
		DrawRoundRect(60, 210, 300, 100, colors[DARK_GREEN], 10);
		
		DrawRoundRect(60, 400, 300, 20, colors[DARK_GREEN], 10);
		
		DrawRoundRect(60, 510, 300, 100, colors[DARK_GREEN], 10);
		DrawRoundRect(60, 660, 300, 100, colors[DARK_GREEN], 10);

		DrawRoundRect(660, 60, 300, 100, colors[DARK_GREEN], 10);
		DrawRoundRect(660, 210, 300, 100, colors[DARK_GREEN], 10);
		
		DrawRoundRect(660, 400, 300, 20, colors[DARK_GREEN], 10);
		
		DrawRoundRect(660, 510, 300, 100, colors[DARK_GREEN], 10);
		DrawRoundRect(660, 660, 300, 100, colors[DARK_GREEN], 10);


		DrawRoundRect(460, 60, 100, 250, colors[BLUE_VIOLET], 10);
		DrawRoundRect(460, 510, 100, 250, colors[BLUE_VIOLET], 10);
		

//Road strips made in a loop.
		for(int x = 60; x <= 959; x+=30){
			DrawLine(x, 355, x+20, 355, 1, colors[WHITE]);
		}

		for(int x = 60; x <= 959; x+=30){
			DrawLine(x, 465, x+20, 465, 1, colors[WHITE]);
		}

		for(int y = 65; y <= 284; y+=30){
			DrawLine(410, y, 410, y+20, 1, colors[WHITE]);
		}

		for(int y = 65; y <= 284; y+=30){
			DrawLine(610, y, 610, y+20, 1, colors[WHITE]);
		}
		
		for(int y = 515; y <= 734; y+=30){
			DrawLine(410, y, 410, y+20, 1, colors[WHITE]);
		}

		for(int y = 515; y <= 734; y+=30){
			DrawLine(610, y, 610, y+20, 1, colors[WHITE]);
		}
//Drawing houses in a loop, for every green area we made in the fuction above.
		for(int n = 100; n < 350; n+=50) {
			DrawHouse(n, 60);
			DrawHouse(n, 210);
			DrawHouse(n, 510);
			DrawHouse(n, 660);
		}

		for(int n = 100; n < 350; n+=50) {
			DrawHouse(n, 130);
			DrawHouse(n, 280);
			DrawHouse(n, 580);
			DrawHouse(n, 730);
		}

		for(int n = 700; n < 950; n+=50) {
			DrawHouse(n, 60);
			DrawHouse(n, 210);
			DrawHouse(n, 510);
			DrawHouse(n, 660);
		}

		for(int n = 700; n < 950; n+=50) {
			DrawHouse(n, 130);
			DrawHouse(n, 280);
			DrawHouse(n, 580);
			DrawHouse(n, 730);
		}







	
//Functions declared for drawing the objects on the game display.
		drawCar();

		drawOtherCar();
		drawSecondCar();

		drawThirdCar();

		for(int i = 0; i < 5; i++) {
			if(!men[i].isOnBoard) {
				drawMan(men[i].x - 10, men[i].y + 15, men[i].color);
			}
			else {
				DrawFlag(men[i].x, men[i].y);
			}
		}
	}
	
	glutSwapBuffers(); // do not modify this line..

}


//Here we use arrow keys .
//using the non-printable keys we set the direction of the cars.

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		// Set the car direction.

		switch (carDirection) {
			case 'W':
				carDirection = 'N';
				break;

			case 'E':
				carDirection = 'S';
				break;

			case 'N':
				carDirection = 'E';
				break;

			case 'S':
				carDirection = 'W';
				break;

		}
		

	} else if (key == GLUT_KEY_RIGHT) {
		// Set the car direction.

		switch (carDirection) {
			case 'W':
				carDirection = 'S';
				break;

			case 'E':
				carDirection = 'N';
				break;

			case 'N':
				carDirection = 'W';
				break;

			case 'S':
				carDirection = 'E';
				break;

		}

	} else if (key == GLUT_KEY_UP) {

		// Move the car forward according to the current direction.
		switch (carDirection) {
			case 'W':
				if(xI < 980) {
					xI += 5;
				}
				break;

			case 'E':
				if(xI > 40) {
					xI -= 5;
				}
				break;

			case 'N':
				if(yI < 780) {
					yI += 5;
				}
				break;

			case 'S':
				if(yI > 40) {
					yI -= 5;
				}
				break;

		//Function called for the Car Direction.
		}
		if(carDirection == 'W' || carDirection == 'E') {
			if(	
				isCollided(460, 60, 100, 250, xI - 25, yI - 10, 50, 20)  || 
				isCollided(460, 510, 100, 250, xI - 25, yI - 10, 50, 20) || 

				isCollided(60, 60, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 210, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 400, 300, 20, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 510, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 660, 300, 100, xI - 25, yI - 10, 50, 20) || 

				isCollided(660, 60, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 210, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 400, 300, 20, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 510, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 660, 300, 100, xI - 25, yI - 10, 50, 20) 


				) {
				
				//Car Directions set at North, South, East and West.
				switch (carDirection) {
					case 'W':
						xI -= 5;
						break;

					case 'E':
						xI += 5;
						break;

					case 'N':
						yI -= 5;
						break;

					case 'S':
						yI += 5;
						break;

				}

			}
		} 
		//Car Direction for North and South Directions.
		if(carDirection == 'N' || carDirection == 'S') {
			if(	
				isCollided(460, 60, 100, 250, xI - 10, yI - 25, 20, 50)  || 
				isCollided(460, 510, 100, 250, xI - 10, yI - 25, 20, 50) || 

				isCollided(60, 60, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 210, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 400, 300, 20, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 510, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 660, 300, 100, xI - 10, yI - 25, 20, 50) || 

				isCollided(660, 60, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 210, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 400, 300, 20, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 510, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 660, 300, 100, xI - 10, yI - 25, 20, 50) 


				) {
				switch (carDirection) {
					case 'W':
						xI -= 5;
						break;

					case 'E':
						xI += 5;
						break;

					case 'N':
						yI -= 5;
						break;

					case 'S':
						yI += 5;
						break;

				}

			}
		}
		
		

	}

	else if (key == GLUT_KEY_DOWN) {

		// Move the car backward according to the current direction.

		switch (carDirection) {
			case 'W':
				if(xI > 40) {
					xI -= 5;
				}
				
				break;

			case 'E':
				if(xI < 980) {
					xI += 5;
				}
				break;

			case 'N':
				if(yI > 40) {
					yI -= 5;
				}
				break;

			case 'S':
				if(yI < 780) {
					yI += 5;
				}
				break;

		}

		//Car Direction for West and East Directions.
		if(carDirection == 'W' || carDirection == 'E') {
			if(	
				isCollided(460, 60, 100, 250, xI - 25, yI - 10, 50, 20)  || 
				isCollided(460, 510, 100, 250, xI - 25, yI - 10, 50, 20) || 

				isCollided(60, 60, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 210, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 400, 300, 20, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 510, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(60, 660, 300, 100, xI - 25, yI - 10, 50, 20) || 

				isCollided(660, 60, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 210, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 400, 300, 20, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 510, 300, 100, xI - 25, yI - 10, 50, 20) || 
				isCollided(660, 660, 300, 100, xI - 25, yI - 10, 50, 20) 


				) {

				switch (carDirection) {
					case 'W':
						xI += 5;
						break;

					case 'E':
						xI -= 5;
						break;

					case 'N':
						yI += 5;
						break;

					case 'S':
						yI -= 5;
						break;

				}

			}
		}
		
		//Car Direction for North and South.
		if(carDirection == 'N' || carDirection == 'S') {
			if(	
				isCollided(460, 60, 100, 250, xI - 10, yI - 25, 20, 50)  || 
				isCollided(460, 510, 100, 250, xI - 10, yI - 25, 20, 50) || 

				isCollided(60, 60, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 210, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 400, 300, 20, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 510, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(60, 660, 300, 100, xI - 10, yI - 25, 20, 50) || 

				isCollided(660, 60, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 210, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 400, 300, 20, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 510, 300, 100, xI - 10, yI - 25, 20, 50) || 
				isCollided(660, 660, 300, 100, xI - 10, yI - 25, 20, 50) 


				) {

				switch (carDirection) {
					case 'W':
						xI += 5;
						break;

					case 'E':
						xI -= 5;
						break;

					case 'N':
						yI += 5;
						break;

					case 'S':
						yI -= 5;
						break;

				}

			}
		}




	}




	

	glutPostRedisplay();

}


void PrintableKeys(unsigned char key, int x, int y) { //27 is the ASCII code for Escape.
	if (key == 27) {

		writePlayers(); //Function for writing the player name declared.


		exit(1); // exit the program when escape key is pressed.
	}

	int random;
	bool found = false;
	
	
	// If Spacebar is pressed to pick the passenger
	if(!game_started) {
		if((key >= 65 && key <= 90) || (key >= 97 && key <= 122) || (key >= 48 && key <= 57) || key == 32) {
			player = player + (char) key;
		}
 	}
	else {
		if(key == 32) {
			for(int i = 0; i < 5; i++) {

			
				if(men[i].isOnBoard) {
					if(isCloser(men[i].x, men[i].y, xI, yI, 70)) {
						men[i].isOnBoard = false;
						men[i].color = BLACK;
						score += 10;
					}
				}
				else {
					if(isCloser(men[i].x, men[i].y, xI, yI, 70)) {
						men[i].isOnBoard = true;
						

						while(true) {
							random = 0 + (rand() % 79);	
							for(int j = 0; j < 5; j++) {
								if(men[j].x == locations[random].x && men[j].y == locations[random].y) {
									found = true;
									break;
								}
							}
							if(found) {
								continue;
							}
							else {
								break;
							}
						}
						
						men[i].x = locations[random].x;
						men[i].y = locations[random].y;

					}
				}
			}
		}
	}

//Game Started.
	if(!game_started) {
		if(key == 8) {
			player.pop_back();
		}
	}

	if(!game_started) {
		if(key == 13) {
			game_started = true;
		}
	}	

	glutPostRedisplay();
}





//* This function is called after every 25 milliseconds
//* (Milliseconds is defined on in the beginning).
 
 
 
void Timer(int m) {

	if(game_started && !game_over) {
		
		// Setting up the timer for the game and setting the function time_remaining();.
		moveOtherCar();
		moveSecondCar();
		moveThirdCar();

		if(time_remaining > 0) {
			time_remaining -= 25;
		}
		else {
			game_over = true;
		}


	}

	// Once again we tell the library to call our Timer function after next 25ms.
	glutTimerFunc(25, Timer, 0);
}

//Tells the x coordinates when mouse moved on the game display.
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
//Tells the y-coordinates when the mouse is moved on the game display.
void MouseMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}


 
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // Deals with the Left Button.
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // Deals with Right Button.
			{
			cout<<"Right Button Pressed"<<endl;

	}
	glutPostRedisplay();
}

 //Calling the Main Function.
int main(int argc, char*argv[]) {

	int width = 1020, height = 840; // i have set my window size to be 800 x 600
	
	storeLocations();

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Rush Hour 2.0 by Immad Shahid"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...




	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	setMenLocations();
	readPlayers();





	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();


	return 1;
}
#endif /* RushHour_CPP_ */

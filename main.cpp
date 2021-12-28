#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>
#include <mmsystem.h>

// Constants 
#define ROWS     40
#define COLUMNS  40
//#define FPS      50
#define RIGHT     1
#define LEFT     -1
#define UP        2
#define DOWN     -2
#define MAX      20
#define SPEED_INCREASE 0.5

// Public Scope Variables 
int snakePositionX[60] = {20,20,20,20,20}; 
int snakePositionY[60] = {20,19,18,17,16};
int snakeDirection = RIGHT;
int snakeLength = 6;
bool gameOver = false;
int foodPositionX,foodPositionY; 
bool food = true;
int score = 0;
float FPS = 10.0;
// Functions
void init();
void reshape(int,int);
void display(void);
void timer(int);
void keyboard(int,int,int);
void drawSnake();
void drawFood();
void drawWalls();
void random(int&,int&);
void renderbitmap(float,float,void*,char*);
void renderScore();
void renderMax();

// Main Function
int main(int argc, char** argv) {
	glutInit(&argc , argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600,600);
	glutCreateWindow("SNAKE GAME");
	PlaySound(TEXT("startgame.wav"), NULL ,SND_ASYNC);	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	glutSpecialFunc(keyboard);
	init();
	glutMainLoop();	
	return 0;
}

// Init function 
void init(){
	glClearColor(0.0,0.0,0.0,0.0);	
}

// Reshape function
void reshape(int width , int height){
	glViewport(0,0,(GLsizei) width , (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,40.0,0.0,40.0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);	
}

// Display Function
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	drawWalls();
	drawSnake();
	drawFood();
	renderMax();
	renderScore();
	glutSwapBuffers();
	
	if(gameOver){
		char scoreText[10];
		char text[50] = "Your Score is ";
		itoa(score , scoreText , 10);
		strcat(text , scoreText);
		if(score == MAX){
			PlaySound(TEXT("winning.wav"),NULL,SND_ASYNC);
			MessageBox(NULL , "WINNER" , "You Win!!" , 0);	
		} else{
			PlaySound(TEXT("gameover.wav"),NULL,SND_ASYNC);
			MessageBox(NULL , text , "GAME OVER" , 0);
		}
		exit(0);
	}
	
}

// Timer function
void timer(int){
	glutPostRedisplay();
	glutTimerFunc(1000.0/FPS,timer,0);
}

// Keyboard function
void keyboard(int key, int , int){	
	switch(key){
		case GLUT_KEY_UP:
			if(snakeDirection != DOWN){
				snakeDirection = UP;
				PlaySound(TEXT("keyboardeffect.wav"),NULL,SND_ASYNC);
			}
			break;
		case GLUT_KEY_DOWN:
			if(snakeDirection != UP){
				snakeDirection = DOWN;
				PlaySound(TEXT("keyboardeffect.wav"),NULL,SND_ASYNC);
			}
			break;
		case GLUT_KEY_LEFT:
			if(snakeDirection != RIGHT){
				snakeDirection = LEFT;
				PlaySound(TEXT("keyboardeffect.wav"),NULL,SND_ASYNC);
			}
			break;
		case GLUT_KEY_RIGHT:
			if(snakeDirection != LEFT){
				snakeDirection = RIGHT;
				PlaySound(TEXT("keyboardeffect.wav"),NULL,SND_ASYNC);
			}
			break;
		}
}

// Draw Walls function
void drawWalls(){
	glColor3f(1.0,1.0,1.0);
	for(int x=0 ; x < ROWS ; x++){
		for(int y=0 ; y < COLUMNS ; y++){
			if(x == 0 || y == 0 || x == ROWS-1 || y == COLUMNS-1){
				glRectd(x,y,x+1,y+1);	
			}	
		}
	}

}

// Draw snake function
void drawSnake(){
	for(int i=snakeLength-1 ; i > 0 ; i-- ){
		snakePositionX[i] = snakePositionX[i-1];
		snakePositionY[i] = snakePositionY[i-1];
	}
	if(snakeDirection == UP){
		snakePositionY[0]++;		
	} else if(snakeDirection == DOWN){
		snakePositionY[0]--;
	} else if(snakeDirection == RIGHT){
		snakePositionX[0]++;
	} else if(snakeDirection == LEFT){
		snakePositionX[0]--;
	}
	for(int i=0 ; i < snakeLength ; i++){
		if( i == 0){
			glColor3f(1.0,0.2,0.2);
		}else {
			glColor3d(1.0,0.85,0.85);	
		}		
		glRectd(snakePositionX[i],snakePositionY[i],snakePositionX[i]+1 , snakePositionY[i]+1);	
	}

	for(int i=1 ; i < snakeLength ; i++){		
		if(snakePositionX[0] == snakePositionX[i] && snakePositionY[0] == snakePositionY[i]){
			gameOver = true;
		}	
	}
	
	if(snakePositionX[0] == 1 || snakePositionX[0] == ROWS-2|| snakePositionY[0] == 1 || snakePositionY[0] == COLUMNS-2){
		gameOver = true;
	}
	if(snakePositionX[0] == foodPositionX && snakePositionY[0] == foodPositionY){
		snakeLength++;
		score++;
		FPS += SPEED_INCREASE ;
		food = true;
		PlaySound(TEXT("eating.wav"),NULL,SND_ASYNC);
		for(int i=0 ; i < snakeLength ; i++){
		if( i == 0){
			glColor3f(1.0,0.2,0.2);
		}else {
			glColor3d(1.0,0.85,0.85);	
		}		
		glRectd(snakePositionX[i],snakePositionY[i],snakePositionX[i]+1.3 , snakePositionY[i]+1.3);	
		}
		
		if(score == MAX){
			gameOver = true;
		}
		
	}
}

// Draw food function
void drawFood(){
	if(food){
		random(foodPositionX , foodPositionY);
	}
	food = false;
	glColor3f(1.0,1.0,0.0);
	glRectd(foodPositionX,foodPositionY , foodPositionX+1 , foodPositionY+1);
}

// generate randow number function 
void random(int& x , int& y){
	int max = ROWS-3;
	int min = 2;
	srand(time(NULL));
	x = min + rand() % (max - min);
	y = min + rand() % (max - min);
}


// render value on screen function
void renderbitmap(float x,float y,void *font,char *string){
	glColor3f(1.0,0.0,0.0);
	char *c;
	glRasterPos2d(x,y);
	for(c=string;*c!='\0';c++){
		glutBitmapCharacter(font,*c);
	}
}

// render score on screen function
void renderScore(){
	char scoreText[10];
	char text[20] = "";
	itoa(score , scoreText , 10);
	strcat(text , scoreText);
	renderbitmap(20,37,GLUT_BITMAP_TIMES_ROMAN_24,text);
}

// render MAX on the screen function
void renderMax(){
	char maxText[10];
	char text[20] = "Max : ";
	itoa(MAX , maxText , 10);
	strcat(text , maxText);
	renderbitmap(2,37,GLUT_BITMAP_TIMES_ROMAN_24,text);
}



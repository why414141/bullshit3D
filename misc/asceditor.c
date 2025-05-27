//program for ascii art
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
#define twidth 8
#define theight 8

void drawThing(int cur_x, int cur_y, char * thing){
	printf("\x1b[H\x1b[J");//clear screen and return to home position
	printf("\x1b[m");//clear all color settings
	for (int y = 0; y < theight; y++){
		for(int x = 0; x < twidth; x++){
			if (x == cur_x && y == cur_y){
				printf("\x1b[48;5;1m");
				printf("%c",thing[x+y*twidth]);
				printf("\x1b[m");
				continue;
			}
			printf("%c",thing[x+y*twidth]);
		}
		printf("\x1b[48;5;14m \x1b[m\n");
	}
	for(int _ = 0; _ < twidth; _++){
		printf("\x1b[48;5;14m \x1b[m");
	}
	printf("\x1b[m");
}
char askForBrush(){
	printf("brush:");
	return fgetc(stdin);
}
void saveAlphabet(char * filena, char * alphabet){
	FILE * file = fopen(filena, "w");
	fwrite(alphabet, sizeof(char), twidth * theight * 26, file);
	fclose(file);
}
int main(){
	struct termios tin, tout;
	char userin, brush = '#';
	char thing[twidth*theight];
	char alphabet[twidth * theight * 26];
	int x=0, y=0;
	int draw;
	int running = 1;
	memset(thing, ' ', sizeof(char) * theight * twidth);
	tcgetattr(0,&tin);
	memcpy(&tout, &tin, sizeof(struct termios));
	tout.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &tout);
	while(running){
		userin = fgetc(stdin);	
		switch (userin){
			case 'j':
				y ++;	
				break;
			case 'k':
				y --;
				break;
			case 'h':
				x --;
				break;
			case 'l':
				x ++;
				break;
			case 'b':
				brush = askForBrush();
				break;
			case 'd':
				draw = !draw;
				break;
			case 'n':
				
		}	
		if(draw){
			thing[x + y * twidth] = brush;
		}
		drawThing(x,y,thing);
	}
	tcsetattr(0, TCSANOW, &tin);
	return 0;
}

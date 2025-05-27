#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<termios.h>
#define WIDTH 32
void draw(char*buffer,int size, int width){
	printf("\x1b[H\x1b[J");
	for (int i = 0; i<size; i++){
		printf("\x1b[38;5;%dm**", buffer[i]);
		if (!((i + 1) % width)&&i)printf("\n");	
	}
}
void clear(char * buffer,int size){
	memset(buffer,0,size);
}
void loadBuffer(char * dest, int size,const char* name){
	FILE * file;
	file = fopen(name,"r");
	fread(dest, size,1,file);
	fclose(file);
}
void saveBuffer(char * buffer,int size,const char* name){
	FILE * file;
	file = fopen(name,"w");
	fwrite(buffer, size,1,file);
	fclose(file);
}
char getBrushColor(){
	printf("what color lol");
	char color[4];
	fgets(color, 4,stdin);
	return atoi(color);
}
int main(){
	struct termios tosi;
	struct termios toso;
	tcgetattr(0,&tosi);
	memcpy(&toso,&tosi,sizeof(struct termios));
	toso.c_lflag &= ~ICANON;
	tcsetattr(0,TCSANOW,&toso);
	char in;	
	char brush = 1;
	char prevhere = 0;
	char buf[1024] = {0};
	printf("wish to load y/n ?");
	in = fgetc(stdin);
	if (in == 'y'){
		loadBuffer(buf,1024,"ballsack");
	}
	int x = 0;
	int y = 0;
	int mode = 0;
	int end = 0;
	int ret = 0;
	for (;;){	
		buf[x+y*WIDTH] = prevhere;
		in = fgetc(stdin);
		switch (in){
			case 'h':
				x--;
				break;
			case 'l':
				x++;
				break;
			case 'j':
				y++;
				break;
			case 'k':
				y--;
				break;
			case 'd':
				mode = !mode;
				break;
			case 'b':
				brush = getBrushColor();
				break;
			case 'q':
				end = 1;
				break;
		}
		if (end) break;
		prevhere = mode? brush:buf[x+y*WIDTH];
		buf[x+y*WIDTH] = 100;
		printf("%d %d\n",x,y);
		draw(buf,1024,WIDTH);				
		printf("(%d %d)\n",x,y);
	}
	printf("wish to save y/n?");
	in = fgetc(stdin);
	if (in == 'y'){
		saveBuffer(buf,1024,"ballsack");	
	}
	printf("\n");
	return 0;
}

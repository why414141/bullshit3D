#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include "../rdr/render.h"
#include "../rdr/geom.h"
#include "keybinds.h"
int shader (struct pixeldata *pd){
	return  ((char *)(pd -> userarg))[pd -> tri_index];
}
void drawTriangleOutline(Vec3D * triangle, Buffer * buffer, char color){
	drawLine(&triangle[0],&triangle[1],buffer,color);	
	drawLine(&triangle[1],&triangle[2],buffer,color);	
	drawLine(&triangle[2],&triangle[0],buffer,color);	
}
void drawAxisVectors(Vec3D * point, Angle*rotate,Buffer * buffer){
	Vec3D curr_axis = {10,0,0};//x axis
	rotateVectae(&curr_axis,rotate,1);
	addVector3(&curr_axis,point,&curr_axis);
	drawLine(point,&curr_axis,buffer,1);
	curr_axis.x = 0;	
	curr_axis.z = 0;
	curr_axis.y = 10;
	rotateVectae(&curr_axis,rotate,1);
	addVector3(&curr_axis,point,&curr_axis);
	drawLine(point,&curr_axis,buffer,10);
	curr_axis.y = 0;	
	curr_axis.x = 0;
	curr_axis.z = 10;
	rotateVectae(&curr_axis,rotate,1);
	addVector3(&curr_axis,point,&curr_axis);
	drawLine(point,&curr_axis,buffer,25);
}
float askForFloat(){
	char in [24];
	fgets(in,24,stdin);
	return atof(in);
}
int askForInt(){
	char in [24];
	fgets(in,24,stdin);
	return atoi(in);
}
enum inputMode {TABLE,ROTATE, VISUAL,TRANSLATE} mode;
enum inputState {READ, IGNORE} instate;
void drawTriangleTable(Vec3D* triangle, int vec_sel, int field_sel, int replace, int color){
	float * vec;
	for(int vector = 0; vector < 3; vector++){
		vec = (float *)&triangle[vector];
		for (int field = 0; field < 4; field++){
			if (field == 3){
				if (vector != 1) break;
				if (field_sel == 3){
					printf("\x1b[38;5;10m");
					if(replace)printf("        ");
					else {
						printf("%8d",color);
					}
					printf("\x1b[m");
				}
				else printf("%8d",color); 
				break;
			}
			if(field == field_sel && vector == vec_sel){
				printf("\x1b[38;5;10m");	
				if(replace)printf("        ");
				else {
					printf("%8.3f",vec[field]);
				}
				printf("\x1b[m");
			}
			else{
				printf("%8.3f",vec[field]);
			}
		}
		printf("\n");
	}	
	if (replace  == 1){
		
	}
}
int main(){
	mode = TABLE;

	struct termios tin;
	struct termios tout;
	//input config
	tcgetattr(0,&tin);
	tout = tin;
	tout.c_lflag &= ~ICANON;
	tcsetattr(0,TCSANOW,&tout);
	char in;
	Buffer *buffer = createBuffer(64,64);
	Vec3D arr[1024];
	Vec3D arr_display[1024];
	char colors[1024];
	Angle rotate = {0,0,0};
	Vec3D offset = {0,0,0};
	int scale = 2;
	int selected_triangle = 0;
	int size = 1;
	char color_here;
	//it is bool nigga
	int replace = 0;
	int draw_av = 1;
	//it is bool nigga
	int tableX = 0;
	int tableY = 0;
	//
	char *modenames[] = {"table", "rotate", "visual","translate"};
	instate = IGNORE;
	
	for(;;){
		if(instate == READ){
			in = fgetc(stdin);
			switch (mode){
				case TABLE:
					switch (in){
						case KEYBIND_TABLE_UP:
							tableY--;
							break;
						case KEYBIND_TABLE_DOWN:
							tableY++;
							break;
						case KEYBIND_TABLE_LEFT:
							tableX--;
							break;
						case KEYBIND_TABLE_RIGHT:
							tableX++;
							break;
						case KEYBIND_TABLE_REPLACE:
							replace = 1;
							break;
						case KEYBIND_TABLE_NEXT:
							selected_triangle =
							selected_triangle < size-1?
							selected_triangle+1:
							0;
							break;
						case KEYBIND_TABLE_NEW:
							memcpy(&arr[size*3],
							&arr[selected_triangle*3],3*sizeof(Vec3D));
							selected_triangle = size;
							size ++;
							break;
						case KEYBIND_TABLE_INCREMENT:
							if (tableX == 3){
								colors[selected_triangle] ++;
								break;
							}
							((float*)&arr[selected_triangle*3+tableY])[tableX] ++;
							break;
						case KEYBIND_TABLE_DECREMENT:
							if (tableX == 3){
								colors[selected_triangle] --;
								break;
							}
							((float*)&arr[selected_triangle*3+tableY])[tableX] --;
							break;
						case KEYBIND_ALL_SWITCH_MODE:
							mode = ROTATE;
							break;
					}
					break;
				case ROTATE:
					switch(in){
						case KEYBIND_ROTATE_LEFT:
							rotate.yaw += 10.0;
							break;
						case KEYBIND_ROTATE_RIGHT:
							rotate.yaw -= 10.0;
							break;
						case KEYBIND_ROTATE_UP:
							rotate.pitch += 10.0;
							break;
						case KEYBIND_ROTATE_DOWN:
							rotate.pitch -= 10.0;
							break;
						case KEYBIND_ALL_SWITCH_MODE:
							mode = VISUAL;
							break;
					}
					break;
				case VISUAL:
					switch(in){
						case KEYBIND_VISUAL_LEFT:
							arr[selected_triangle*3+tableY].x --;
							break;
						case KEYBIND_VISUAL_RIGHT:
							arr[selected_triangle*3+tableY].x ++;
							break;
						case KEYBIND_VISUAL_UP:
							arr[selected_triangle*3+tableY].y --;
							break;
						case KEYBIND_VISUAL_DOWN:
							arr[selected_triangle*3+tableY].y ++;
							break;
						case KEYBIND_VISUAL_FORWARD:
							arr[selected_triangle*3+tableY].z --;
							break;
						case KEYBIND_VISUAL_BACK:
							arr[selected_triangle*3+tableY].z ++;
							break;
						case KEYBIND_VISUAL_NEXT_POINT:
							tableY = tableY < 2?
								tableY+1:0;
							break;
						case KEYBIND_VISUAL_NEW:
							memcpy(&arr[size*3],
							&arr[selected_triangle*3],3*sizeof(Vec3D));
							selected_triangle = size;
							size ++;
							break;
						case KEYBIND_ALL_SWITCH_MODE:
							mode = TRANSLATE;
							break;
						case KEYBIND_VISUAL_NEXT_TRIANGLE:
							selected_triangle =
							selected_triangle < size-1?
							selected_triangle+1:
							0;
							break;
					}
					break;
				case TRANSLATE:
					switch(in){
						case KEYBIND_TRANSLATE_LEFT:
							offset.x += 10.0;
							break;
						case KEYBIND_TRANSLATE_RIGHT:
							offset.x -= 10.0;
							break;
						case KEYBIND_TRANSLATE_UP:
							offset.y -= 10.0;
							break;
						case KEYBIND_TRANSLATE_DOWN:
							offset.y += 10.0;
							break;
						case KEYBIND_ALL_SWITCH_MODE:
							mode = TABLE;
							break;
					}
					break;
			}
		}
		else instate = READ;	
				
		resetDepthBuffer(buffer,-100);
		clearBuffer(buffer,0);
		memcpy(arr_display,arr,size*3*sizeof(Vec3D));
		for (int i = 0; i < size * 3; i++){
			mulVector3(&arr_display[i], scale,&arr_display[i]);
		}
		rotateVectae(arr_display,&rotate,size*3);
		offsetVectors(arr_display,&offset,size*3,arr_display);
		drawTriangles(arr_display, size, shader, colors, buffer);
		drawTriangleOutline(&arr_display[selected_triangle*3], buffer, 14);
		putPx(arr_display[selected_triangle*3+tableY].x,arr_display[selected_triangle*3+tableY].y,buffer,NORMAL,1);
		if(draw_av){
			drawAxisVectors(&arr_display[selected_triangle*3+tableY],&rotate,buffer);
		}
		presentBuffer(buffer);
		drawTriangleTable(&arr[selected_triangle*3],tableY,tableX, replace,colors[selected_triangle]);
		if(replace){
			if (tableX == 3){
				colors[selected_triangle] = askForInt();
			}
			else{
				((float*)&arr[selected_triangle*3+tableY])[tableX] = askForFloat();
			}
			replace = 0;
			instate = IGNORE;
		}
		printf("triangle: %d\n", selected_triangle);
		printf("mode : %s\n",modenames[mode]);
		
	}
	tcsetattr(0,TCSANOW,&tin);
	return 0;
}


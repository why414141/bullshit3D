#include"vector.h"
#include"stdlib.h"
#include<termios.h>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include"render.h"
#include"geom.h"
#include<math.h>

typedef struct arg_{char* color;char*texture} arg_;

int shader(struct pixeldata * pd){
	int x =(pd -> tri_index % 2?pd -> localpos.y:1-pd -> localpos.x) * 16;	
	x = x< 0? 0: x;
	int y =(pd -> tri_index % 2?pd -> localpos.x:1-pd -> localpos.y) * 16;
	y = y < 0? 0: y;
	char color = ((arg_*)(pd -> userarg)) -> texture[x+y*16];
	color = color == 2? ((arg_*)(pd -> userarg)) -> color[pd -> tri_index]:color;
}

int main(){
	Buffer * buffer = createBuffer(80,64);
	char colors [] = {1,1,2,2,3,3,4,4,5,5,6,6};
	Vec3D cube[]={CUBE(0.3)};
	Vec3D triangles[CUBE_VECTS];
	Vec3D offset = {16,16,100};

	Vec3D nullVec = {0,0,0};
	char texture[] = {
	2,2,2,2 ,2,2,31,2,2,2,2,2 ,2,2,2,2,
	2,2,2,2 ,2,2,31,2,2,2,2,2 ,2,2,2,2,
	2,2,2,2 ,2,2,31,2,2,2,2,2 ,2,2,2,2,
	2,2,2,2 ,2,2,31,31,31,31,31,2 ,2,2,2,2,
	2,2,2,2 ,2,2,2,2,2,2,2,2 ,2,2,2,2,
	2,2,2,2 ,2,2,2,2,2,2,2,2 ,2,2,2,2,
	2,2,2,2 ,1,1,1,2,2,1,1,1 ,2,2,2,2,
	2,2,2,2 ,2,2,2,2,2,2,2,2 ,2,2,2,2,
	2,2,2,2 ,1,2,2,1,2,2,2,1 ,2,2,2,2,
	5,5,5,2 ,1,2,2,1,2,2,2,1 ,2,5,5,5,
	2,2,2,2 ,1,2,2,1,1,2,2,1 ,2,2,2,2,
	2,2,2,2 ,1,2,2,2,2,2,2,1 ,2,2,2,2,
	2,2,2,2 ,1,2,2,2,2,2,2,1 ,2,2,2,2,
	2,2,2,2 ,1,1,1,1,1,1,1,1 ,2,2,2,2,
	2,2,2,2 ,2,2,2,2,2,2,2,2 ,2,2,2,2,
	2,2,2,2 ,2,2,2,2,2,2,2,2 ,2,2,2,2
	};	
	arg_ arg = {colors,texture};

	struct termios tosi;
	struct termios toso;
	Vec3D vanishing = {16,16,-1000};
	tcgetattr(0,&tosi);
	memcpy(&toso,&tosi,sizeof(struct termios));
	toso.c_lflag &= ~ICANON;
	tcsetattr(0,TCSANOW,&toso);
	Angle rotate;
	rotate.yaw= 1.0;	
	rotate.roll= 1.0;
	rotate.pitch = 1.0;
	struct timespec delay={0,9999999};
	for(;;){
	if(nanosleep(&delay,NULL)<0) return -1;
		clearBuffer(buffer,0);
		resetDepthBuffer(buffer,100);
		memcpy(triangles,cube,CUBE_VECTS*sizeof(Vec3D));
		applySimplePerspective(triangles,CUBE_VECTS,0.5,3);
		//offsetVectors(triangles, &offset, CUBE_VECTS, triangles);
		drawTriangles(triangles,CUBE_TRIS,shader,&arg, buffer);
		presentBuffer(buffer);
		rotateVectae(cube,&rotate, CUBE_VECTS);
	}
	clearBuffer(buffer,0);
		//presentBuffer(buffer);	
	
tcsetattr(0,TCSANOW,&tosi);
return 0;
}

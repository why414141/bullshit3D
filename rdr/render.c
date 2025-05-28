#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "vector.h"
#include "render.h"
#include <math.h>
Buffer * createBuffer(int width,int height){
	Buffer * out = malloc(sizeof(Buffer));
	out -> width = width;
	out -> height = height;
	out -> color_data = malloc(width*height * sizeof(char));
	out -> depth_data = malloc(width*height * sizeof(float));
	return out;
}
void destroyBuffer(Buffer * target){
	free(target -> color_data);
	free(target -> depth_data);
	free(target);
}
void applySimplePerspective(Vec3D * arr, int size, float constn, float offset){
	for  (int vector = 0; vector < size; vector ++){
		arr[vector].x = arr[vector].x / constn / (arr[vector].z + offset);
		arr[vector].y = arr[vector].y / constn / (arr[vector].z + offset);
	}
}
Vec3D shuffleVec(Vec3D* vec){
	Vec3D out;
	out.x = vec->y;
	out.y = vec->x;
	return out;
}
//sex
int putPx(int x, int y, Buffer* target, tiltspec tilt, char color){
	int index = tilt? x*target -> width+y:y*target -> width+x;
	if (index > target -> width*target -> height||index < 0){
		return -1;
	}
	target -> color_data[index] = color;
	return 0;
} 
typedef enum side{LEFT,RIGHT} side;
side pointSide(Vec3D * p1, Vec3D * p2, Vec3D  * p3){
	Vec3D a;
	Vec3D b;
	subVector3(p2,p1,&a);
	subVector3(p3,p1,&b);
	return cross2D(&a,&b)>0?RIGHT:LEFT;
}
pointstate  pointInTriangle(Vec3D * triangle, Vec3D * p){
	float max_x = -1000.0;
	float min_x = 1000.0;
	float max_y = -1000.0;
	float min_y = 1000.0;
	Vec3D *current;
	for (int i = 0; i < 3; i++){
		current = &triangle[i];
		if(current->x>max_x) max_x = current->x;
		if(current->x<min_x) min_x = current->x;
		if(current->y>max_y) max_y = current->y;
		if(current->y<min_y) min_y = current->y;

	}
	if(p->x>max_x || p->x < min_x || p->y > max_y || p->y<min_y) return OUTSIDE;
	side a = pointSide(&triangle[0], &triangle[1], p);
	side b = pointSide(&triangle[1], &triangle[2], p);
	side c = pointSide(&triangle[2], &triangle[0], p);
	if (a==b && a==c) return INSIDE;
	float dA = distanceToSegmentSq(&triangle[0], &triangle[1], p);
	float dB = distanceToSegmentSq(&triangle[1], &triangle[2], p);
	float dC = distanceToSegmentSq(&triangle[2], &triangle[0], p);
	if (dA < MINIMUM_LINE_DIST_SQ) return INSIDE;
	if (dB < MINIMUM_LINE_DIST_SQ) return INSIDE;
	if (dC < MINIMUM_LINE_DIST_SQ) return INSIDE;
	return OUTSIDE;
}

float mapXYtoZ(Vec3D * t, Vec3D* out){
	//so we find a and b and c where x*a+y*b + c = z
	//we find a vector
	//by which, if we dot product the original vector, we get
	//it's z position
	Vec3D a;
	Vec3D b;
	subVector3(&t[0],&t[1],&a);
	subVector3(&t[0],&t[2],&b);
	Vec3D normal;
	cross(&a,&b,&normal);
	out -> x = normal.x/(- normal.z); out -> y = normal.y/(- normal.z);
	out -> z = 0;
	return  t[1].z - t[1].y * normal.y/(- normal.z) - t[1].x * normal.x/(- normal.z);	
}
float calculateZ(Vec3D * mult,float constant, Vec3D * point){
	return point->x*mult->x + point->y * mult->y + constant;
}
typedef struct planeEq {Vec3D mult; float constant;} planeEq;
void calculatePlaneEquations(planeEq * arr, Vec3D * triangles,int size){
	int index = 0;
	for (int i = 0; i < size*3; i+=3){
		arr[index].constant = mapXYtoZ(&triangles[i],&arr[index].mult);
		index++;
	}
}
float queryDepthData(int x, int y, Buffer * buffer){
	int index = x+y*buffer-> width;
	if (index >= buffer -> width * buffer -> height  || index < 0) return -1000;
	return buffer -> depth_data[index];
}
int setDepthData(int x, int y,float z, Buffer * buffer){
	int index = x+y*buffer-> width;
	if (index >= buffer -> width * buffer -> height  || index < 0) return -1;
	buffer -> depth_data[index] = z;
}
struct triangledata {
	Vec3D * vertices;
	planeEq * eq;
	Vec3D * sides;
	int index;
};
void calculateLocalPos(struct pixeldata * pd,Vec3D * tri,Vec3D*sides){
	Vec3D dist;
	dist.x= distance(&pd -> center,tri);
	dist.y= distance(&pd -> center,&tri[1]);
	dist.z= distance(&pd -> center,&tri[2]);
	bary3(&dist,sides,&pd -> localpos);		
}
void processPixel(Vec3D * arr,int size, int x,int y, Vec3D * sides,planeEq * planes,pxShader shader,Buffer * buffer,void * arg){
	int index = 0;
	struct pixeldata pd;
	pd.userarg = arg;
	pd.x = x;
	pd.y = y;
	buffer -> width;
	buffer -> height;
	pd.center.x = (x+0.5)*2/buffer -> width -1;
	pd.center.y = (y+0.5)*2/buffer -> height -1;
	for (int i = 0; i < size * 3; i+=3){
		if(pointInTriangle(&arr[i],&pd.center) == INSIDE){
			pd.center.z = calculateZ(&planes[index].mult,planes[index].constant, &pd.center);
			if (queryDepthData(x,y,buffer) > pd.center.z && pd.center.z < 1 && pd.center.z > -1){
				setDepthData(x,y,pd.center.z,buffer);
				calculateLocalPos(&pd,&arr[i],&sides[index]);
				pd.tri_index = index;
				putPx(x,y,buffer,NORMAL,(*shader)(&pd));
			}
		}
		index ++;
	}
}
int drawTriangles(Vec3D* arr,int size, pxShader shader, void * args,Buffer * buffer){
	planeEq planes[size];
	calculatePlaneEquations(planes,arr,size);
	Vec3D tri_sides[size];
	int index = 0;
	for (int i = 0; i < size*3; i+=3){	
		calculateTriangleSides(&arr[i],&tri_sides[index]);
		index++;
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i< buffer -> height*buffer -> width; i++){
		processPixel(arr,size,x,y,tri_sides,planes,shader,buffer,args);
		x++;
		if (!(i % buffer -> width)) {y++; x = 0;}
	}
}

int drawLine_(Vec3D *p1, Vec3D *p2, Buffer * buffer, char color, tiltspec state, int n){
		float deltaX =(p2->x-p1->x);
		float slope = (p2->y-p1->y)/deltaX;
		if(n>2)return -2;
		if (abs(slope) > 1||deltaX == 0.0){
			Vec3D p1_ = shuffleVec(p1);
			Vec3D p2_ = shuffleVec(p2);
			return drawLine_(&p1_,&p2_,buffer,color,TILTED,n+1);
		}

		int step = p1->x > p2->x?-1:1;
		int tgt = ceil((p2->x - p1->x));
		for(int i = 0; i != tgt; i+=step){
			putPx(i + p1->x,p1 -> y + (int) (slope*i), buffer, state,color);			
		} 
		return 0;
}
int drawLine(Vec3D *p1, Vec3D *p2, Buffer *buffer, char  color){
	
	return drawLine_(p1,p2,buffer,color,NORMAL,0);	
}
void clearBuffer(Buffer * target,char color){
	memset(target -> color_data, color, target -> width*target -> height);
}
void resetDepthBuffer(Buffer* buffer, int far){
	for(int i = 0; i<buffer -> width*buffer -> height;i++){
		buffer -> depth_data[i] = far;
	}
}
void presentBuffer(Buffer * target){
	char color = 0;
	int index = 0;
	printf("\x1b[J\x1b[H");
	for (int i = 0; i < target-> width * target -> height; i++){
		if (target -> color_data [index] != 0){
			if (target -> color_data [index] != color){
			printf("\x1b[48;5;%dm\x1b[38;5;%dm", target -> color_data [index], target -> color_data [index]);
			color = target -> color_data [index];	
			}
			printf ("##");
		}
		else{
			if(color){
				printf("\x1b[m");
				color = 0;
			}
			printf ("  ");
		}
		index ++;		
		if(!(index % target -> width)) printf("\n");	
	}

	printf("\x1b[m");
}

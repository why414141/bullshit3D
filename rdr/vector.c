#include <stdio.h>
#include "vector.h"
#include "matrix.h"
#include <string.h>
#include <math.h>
float cross2D(Vec3D *p1, Vec3D *p2){
	return p1->x * p2 -> y - p1 -> y * p2 -> x;
}
float dot2D(Vec3D *p1, Vec3D *p2){
	return p1->x *p2->x + p1->y*p2->y;
}

float distanceSquared2D(Vec3D* a, Vec3D *b){
	float dx = b->x - a->x;
	float dy = b->y - a->y;
	return dx*dx+dy*dy;
}
float distanceSquared(Vec3D *a, Vec3D *b){
	float d2d = distanceSquared2D(a,b);
	float dz = b->z - a->z;
	return dz*dz + d2d;
}
float distance(Vec3D* point1, Vec3D* point2){
	return sqrt(distanceSquared(point1,point2));
}
void mulVector3(Vec3D* a, float b, Vec3D * res){
	res->x = a->x * b;
	res->y = a->y * b;
	res->z = a->z * b;
}
void divVector3(Vec3D* a, float b, Vec3D * res){
	res->x = a->x / b;
	res->y = a->y / b;
	res->z = a->z / b;
}
void rotate3D(Vec3D * vec,float * matrix){
	float res_ [3];	
	mmul(matrix,(float*)vec,res_,3,1,3);
	memcpy(vec, res_, sizeof(Vec3D));
}
int rotateVectae(Vec3D * cube, Angle* rotate,int size){
	float matrix[9];
	float angle_[3] = {toRad(rotate->yaw),toRad(rotate->pitch),toRad(rotate->roll)};
	generateRotationMatrix3(matrix, angle_);
	for(int i = 0; i<size;i++){
		rotate3D(&cube[i],matrix);
	}
} 
void cross(Vec3D* a, Vec3D* b,Vec3D* out){
	out -> x = (a->y*b->z - a->z*b->y);	
	out -> y = -(a->x*b->z - a->z*b->x);	
	out -> z = (a->x*b->y - a->y*b->x);	
}
void addVector3(Vec3D* a, Vec3D* b, Vec3D * res){
	memcpy(res, a, sizeof(Vec3D));	
	res ->x += b ->x;
	res ->y += b ->y;
	res ->z += b ->z;
}
void offsetVectors(Vec3D* arr, Vec3D* offset, int size,Vec3D* res){
	for (int i = 0; i< size; i++){
		addVector3(&arr[i], offset, &res[i]);
	}
}
void subVector3(Vec3D* a, Vec3D* b, Vec3D * res){
	memcpy(res, a, sizeof(Vec3D));	
	res ->x -= b ->x;
	res ->y -= b ->y;
	res ->z -= b ->z;
 }
void printVector3(Vec3D * vec){
	printf ("\nX:%f",vec->x);
	printf ("\nY:%f",vec->y);
	printf ("\nZ:%f",vec->z);

}
float distanceToSegmentSq(Vec3D* a,Vec3D *b, Vec3D*c){
	float l_ = distanceSquared2D(a,b);
	Vec3D ab;
	Vec3D ac;
	subVector3(b,a,&ab);
	subVector3(c,a,&ac);
	if (l_==0.0){
		return distanceSquared2D(b,c);
	}
	Vec3D projection;
	float ab_ac = dot2D(&ab,&ac);
	float ab_sq = dot2D(&ab,&ab);
	mulVector3(&ab,ab_ac/ab_sq,&projection);	
	addVector3(&projection, a, &projection);
	return distanceSquared2D(&projection,c);
}
float triArea(Vec3D * in){
	float a = in -> x;
	float b = in -> y;
	float c = in -> z;
	return sqrt((a+b+c)*(-a+b+c)*(a-b+c)*(a+b-c)/16);
}
void calculateTriangleSides(Vec3D * tri, Vec3D * out){
	out -> x = distance(&tri[0], &tri[1]);	
	out -> y = distance(&tri[1], &tri[2]);
	out -> z = distance(&tri[2], &tri[0]);
}
void bary3(Vec3D * dist, Vec3D * sides, Vec3D * out){
	float total_area = triArea (sides);
	Vec3D t1 = {dist -> x, dist -> y, sides -> x};
	Vec3D t2 = {dist -> y, dist -> z, sides -> y};
	Vec3D t3 = {dist -> z, dist -> x, sides -> z};
	out -> x = triArea(&t1)/total_area; 
	out -> x = out -> x < 0? 0: out -> x;
	out -> y = triArea(&t2)/total_area; 
	out -> y = out -> y < 0? 0: out -> y;
	out -> z = triArea(&t3)/total_area; 
	out -> z = out -> z < 0? 0: out -> z;
}

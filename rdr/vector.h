#ifndef VECTOR_H_
#define VECTOR_H_
typedef struct Vec3D {
	float x;
	float y;
	float z;
} Vec3D;
typedef struct Vec2D{
	float x;
	float y;
} Vec2D;
typedef struct line {
	Vec3D p1;
	Vec3D p2;
} line;
typedef struct Angle {float yaw; float pitch; float roll;} Angle;
void bary3(Vec3D *,Vec3D *, Vec3D *);
void calculateTriangleSides(Vec3D*, Vec3D *);
int rotateVectae(Vec3D *, Angle*,int);
void offsetVectors(Vec3D*, Vec3D*, int,Vec3D*);
float distanceToSegmentSq(Vec3D* ,Vec3D *, Vec3D*);
float distanceSquared(Vec3D*,Vec3D*);
float distance(Vec3D*, Vec3D*);
void rotate3D(Vec3D*,float*);
void printVector3(Vec3D*);
void addVector3(Vec3D*,Vec3D*,Vec3D*);
void subVector3(Vec3D*,Vec3D*,Vec3D*);
void cross(Vec3D*,Vec3D*,Vec3D*);
float cross2D(Vec3D*,Vec3D*);
void mulVector3(Vec3D*, float, Vec3D*);
void divVector3(Vec3D*, float, Vec3D*);
#endif

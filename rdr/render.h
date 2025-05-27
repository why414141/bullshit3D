#ifndef RENDER_H_
#define RENDER_H_
#define BUFFER_WIDTH 0
#define BUFFER_HEIGHT 0
#define MINIMUM_LINE_DIST_SQ  0.0
#include "vector.h"
typedef enum tiltspec {
	NORMAL,
	TILTED
} tiltspec;
typedef struct Buffer {
	int width;
	int height;
	char * color_data;
	float * depth_data;
} Buffer;
struct pixeldata {
	Vec3D center;
	Vec3D localpos;
	int tri_index;
	int x;
	int y;
	void* userarg;
};
typedef enum pointstate {INSIDE,OUTSIDE} pointstate;
typedef int (*vShader)(Vec3D*,void*,int); 
typedef int (*pxShader)(struct pixeldata *); 
pointstate  pointInPolygon(Vec3D*,Vec3D*,int,char*);
Buffer * createBuffer(int ,int );
void destroyBuffer(Buffer *);
float mapXYtoZ(Vec3D*, Vec3D*);
void resetDepthBuffer(Buffer*, int);
int drawTriangles(Vec3D*,int, pxShader, void*,Buffer*); 
int drawLine(Vec3D*,Vec3D*,Buffer*,char);
int putPx(int,int,Buffer*,tiltspec,char);
void clearBuffer(Buffer *,char);
void presentBuffer(Buffer *);

void applySimplePerspective(Vec3D *, int, float,float);
#endif


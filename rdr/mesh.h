#ifndef MESH_H_
#define MESH_H_
typedef struct Mesh {
	int tris;
	Vec3D * data;
} Mesh;
Mesh * createMesh(int);
void destroyMesh(Mesh*);
int copyMesh(Mesh*, Mesh *);
void transformMesh(Mesh *, Vec3D *, Angle*, float);
void renderMesh(Mesh *, Buffer *, pxShader);
#endif

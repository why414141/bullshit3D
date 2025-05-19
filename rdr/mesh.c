#include <stdio.h>
#include "render.h"
#include<stdlib.h>
#include<string.h>
#include "mesh.h"
Mesh * createMesh(int triangles){
	Mesh * out = malloc(sizeof(Mesh));
	out -> tris = triangles;
	out -> data = malloc(sizeof(Vec3D) * 3 * triangles);
	return out;
}
void destroyMesh(Mesh * mesh){
	free(mesh -> data);
	free(mesh);
	//hope i did this right
}
int copyMesh(Mesh * in, Mesh * out){
	if (in -> data == NULL || out -> data == NULL){
		return -1;
	}
	if (in -> tris != out -> tris) return -2;
	memcpy(in -> data, out -> data, in -> tris * sizeof(Vec3D)* 3);
	return 0;
}
void transformMesh(Mesh * mesh, Vec3D * translate, Angle * rotate, float scale){
	for (int vert = 0; vert < mesh -> tris * 3; vert ++){
		mulVector3(&(mesh -> data)[vert], scale, &(mesh -> data)[vert]);
	}
	rotateVectae(mesh -> data, rotate, mesh -> tris * 3);	
	offsetVectors(mesh -> data, translate, mesh -> tris * 3, mesh -> data);
}
void renderMesh(Mesh * mesh, Buffer * buffer, pxShader shader){
	//doesent render anything, go fuck yourself
	drawTriangles(mesh -> data, mesh -> tris, shader, NULL, buffer);
}

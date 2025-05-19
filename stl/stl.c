#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../rdr/render.h"
#include "../rdr/mesh.h"
#include <time.h>
void dumpTriangle(Vec3D * triangle){
	printf("point1 : \n");
	printVector3(triangle);
	printf("point2 : \n");
	printVector3(triangle + 1);
	printf("point3 : \n");
	printVector3(triangle + 2);
}
static int color = 0;
int shader(struct pixeldata *pd){
	return pd -> tri_index / 10;
}
Mesh * readStl(char * filename, int dump, int yap){
	//sex
	FILE * file = fopen(filename, "r");
	//dump header
	Mesh * out = malloc(sizeof(Mesh));
	char headerbuf[81];
	int meshVecPointer;
	unsigned int tri_count;
	fread (headerbuf, sizeof(char), 80, file);
	headerbuf[80] = '\0';
	if(dump) printf("header: %s \n", headerbuf);
	fread (&tri_count, sizeof(int), 1,file);
	if (dump) printf("triangle count : %d \n", tri_count);
	out -> tris = tri_count;
	out -> data = malloc(tri_count*3*sizeof(Vec3D));
	for (int t = 0; t < tri_count; t++){
		//for now , dump the normal
		fseek(file, 12, SEEK_CUR);
		fread (out -> data + t*3, sizeof(Vec3D),3,file);
		if (yap){
			printf("triangle no: %d\n", t);
			dumpTriangle(out -> data + t*3);
		}
		fseek(file, 2, SEEK_CUR);
	}
	fclose(file);
	return out;
}
int main(){
	Mesh * curr_mesh;
	Mesh * screen_mesh;
	Buffer * buffer;
	curr_mesh = readStl("test.stl", 1, 0);
	screen_mesh = createMesh(curr_mesh -> tris);
	buffer = createBuffer(64,64);
	Vec3D offset = {0, 0, 0.5};	
	Angle rotate = {0,0,90};
	fgetc(stdin);
	for(;;){
		clearBuffer(buffer, 0);
		resetDepthBuffer(buffer, 100);
		copyMesh(screen_mesh, curr_mesh);

	rotate.pitch += 10;
//		rotate.yaw += 10;
//	rotate.roll += 10
		transformMesh(screen_mesh, &offset, &rotate, 0.7);
		applySimplePerspective(screen_mesh -> data, screen_mesh -> tris * 3, 1,1);
		renderMesh(screen_mesh, buffer, shader);
		presentBuffer(buffer);

	}
	return 0;
}

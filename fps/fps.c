#include"../rdr/render.h"
#include"../stl/stl.h"
#include"../rdr/mesh.h"
void renderWorld(Vec3D * playerpos, Angle * playerrot, Vec3D * world, int worldSize, Buffer * target){
	//for now, the world is a big mesh
	//we translate the center of the world to minus the player coordinates so we get fuck my ass
	Vec3D world_offset;
	mulVector3(playerpos, -1, &world_offset);
	//who cares lol	
}

#include<render.h>
#include<vector.h>
typedef struct tl_arg{
	pxShader outfunc;
	void * outarg;
}  tl_arg;

void trueLocal(Vec3D *screen_pos,void *arg,Vec3D *local_pos,char *buffer,int index)
{
	//converts the pseudo local coordinates given by the renderer into true local coordinates
	//uses penis method
	//described in "how to masturbate properly" by Turbo Masturbo
	

	Vec3D b = {0,1,0};
	Vec3D a = {1,0,0}; 
	Vec3D c = {0,0,0};
	//in theory,
	Vec3D out;
	return (*((tl_arg*)arg->outfunc))(screen_pos,arg,&true_local,buffer,index);

}

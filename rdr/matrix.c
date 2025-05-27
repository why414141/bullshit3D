#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include"matrix.h"
//im very new to matrix math
float toRad(float deg){		
	float res = deg/180*M_PI; 
	return res; 
}
void presentMatrix(float* mat, int m, int p){
	int elem = 0;
	for (int i = 0; i < m; i ++){
		printf("\n");
		for (int j = 0; j < p; j++){
			printf("%f,", mat[elem]);
			elem++;
		}
	}
}
int mmul(float * mat1, float * mat2,float * res, int m, int p, int n){
	//very fucking unsafe	
	float dot;
	int index;
	int limit = m*p - 1;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < p; j++){
		dot = 0.0;
			for (int k = 0; k < n; k++){
				 dot +=	mat1[k+i*n]*mat2[j+p*k];
			}
			if ((index = i*p+j) > limit){
				printf("matrix: out of bounds on mmul\n");
				return -1;
			}//an attempt to make it safe
				res[index] = dot;
			}
	}
}
int generateRotationMatrix3(float * out,float * in){
	float cos_ =(float) cos(*in);
	float sin_ = (float) sin(*in);
	//idk how to better do this :<
	float yawMat[] = {
	cos_, - sin_, 0,
	sin_, cos_, 0,
	0,	 0,	       1
	};

	cos_ =(float) cos(in[1]);
	sin_ = (float) sin(in[1]);

	float pitchMat[] = {
	cos_, 0, sin_,
	0,1,0,
	-sin_, 0, cos_
	};

	cos_ =(float) cos(in[2]);
	sin_ = (float) sin(in[2]);
	
	float rollMat[] = {
	1,0,0,
	0,cos_,-sin_,
	0, sin_, cos_
	};
	float resultMat_[9];
	if (mmul(yawMat, pitchMat, resultMat_, 3, 3, 3)<0) return -1;
	if (mmul(resultMat_, rollMat, out, 3, 3, 3)<0) return -1;
	return 0;
}


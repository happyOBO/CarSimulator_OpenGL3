#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <iostream>
#include "DataStructure.h"
using namespace std;


class Car 
{
private :
	bool ego;
	controls control;
	glm::vec3 CarPosition;
	//position Destination;
	Object ObjectCar;
	ControlRanges ControlRange;
	MVP_Matrix MVP;
	IDs Ids;
	void ObjectInit();
	void ComputeMVP();

public :
	Car();
	Car(GLfloat init_x, GLfloat init_y, GLfloat init_z, GLuint* programID, GLuint* MatrixID, GLuint* TextureID);
	void DrawCar();
	void release();
	void ComputeControlsFromInputs();
	glm::mat4 ReturnPV();
	
};
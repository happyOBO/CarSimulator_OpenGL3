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


class Map
{
private:
	glm::vec3 GroundPosition;
	MVP_Matrix GroundMVP;
	Object ObjectGround;

	glm::vec3 RoadPosition;
	MVP_Matrix RoadMVP;
	Object ObjectRoad;

	glm::vec3 BuildingPosition;
	MVP_Matrix BuildingMVP;
	Object ObjectBuilding;

	IDs Ids;

public:
	Map();
	Map(GLfloat init_x, GLfloat init_y, GLfloat init_z, GLuint* programID, GLuint* MatrixID, GLuint* TextureID, GLuint* ModelMatrixID);
	void ObjectInit();
	void DrawMap(mat4 PV);
	void ComputeMVP();
	void release();

};
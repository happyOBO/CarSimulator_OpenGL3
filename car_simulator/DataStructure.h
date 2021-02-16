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
using namespace glm;

#include <iostream>
using namespace std;

struct controls
{
	float steer = 0.0f;
	float throttle = 0.0f;
	bool brake = false;

};

struct ControlRanges
{
	float MaxSpeed;
	float UnitSpeed;

	float MaxSteer;
	float MinSteer;
	float UnitSteer;

	float Friction = 0.4;
};

struct Object
{
	GLuint vertexbuffer;
	vector<vec3> vertices;

	GLuint uvbuffer;
	vector<vec2> uvs;

	vector<vec3> normals;

	GLuint Texture;
};

struct MVP_Matrix
{
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
};

struct IDs
{
	GLuint* programID;
	GLuint* MatrixID;
	GLuint* TextureID;
};
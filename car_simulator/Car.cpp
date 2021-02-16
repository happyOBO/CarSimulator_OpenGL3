#include "Car.h"

extern GLFWwindow* window;

const float PI = 3.1415926f;
const float initialFoV = 45.0f;

Car::Car() {}

Car::Car(GLfloat init_x, GLfloat init_y, GLfloat init_z, GLuint* programID, GLuint* MatrixID, GLuint* TextureID, GLuint* ViewMatrixID, GLuint* ModelMatrixID)
{
	CarPosition.x = init_x;
	CarPosition.y = init_y;
	CarPosition.z = init_z;

	control.steer = 0.0;
	control.throttle = 0.0;
	control.brake = false;

	ControlRange.MaxSpeed = 5.0f ;
	ControlRange.UnitSpeed = 0.1f;

	ControlRange.MaxSteer = 180.0f * PI / 180;
	ControlRange.MinSteer = -180.0f * PI / 180;
	ControlRange.UnitSteer = 50.0f * PI / 180;
	ObjectInit();

	Ids.programID = programID;
	Ids.MatrixID = MatrixID;
	Ids.TextureID = TextureID;
	Ids.ViewMatrixID = ViewMatrixID;
	Ids.ModelMatrixID = ModelMatrixID;

	ego = true;

}

void Car::ObjectInit()
{
	ObjectCar.Texture = loadBMP_custom("car.BMP");

	bool res = loadOBJ("car.obj", ObjectCar.vertices, ObjectCar.uvs, ObjectCar.normals);

	// Load it into a VBO

	glGenBuffers(1, &ObjectCar.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectCar.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectCar.vertices.size() * sizeof(glm::vec3), &ObjectCar.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectCar.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectCar.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectCar.uvs.size() * sizeof(glm::vec2), &ObjectCar.uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectCar.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectCar.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectCar.normals.size() * sizeof(glm::vec2), &ObjectCar.normals[0], GL_STATIC_DRAW);

}

void Car::DrawCar()
{

	ComputeControlsFromInputs();

	glm::mat4 TotalMVP = MVP.ProjectionMatrix * MVP.ViewMatrix * MVP.ModelMatrix;
	glUniformMatrix4fv(*(Ids.MatrixID), 1, GL_FALSE, &TotalMVP[0][0]);
	glUniformMatrix4fv(*(Ids.ModelMatrixID), 1, GL_FALSE, &MVP.ModelMatrix[0][0]);
	glUniformMatrix4fv(*(Ids.ViewMatrixID), 1, GL_FALSE, &MVP.ViewMatrix[0][0]);

	glUniform1i(*(Ids.TextureID), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ObjectCar.Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectCar.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, ObjectCar.uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectCar.normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, ObjectCar.vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Car::release()
{
	glDeleteBuffers(1, &ObjectCar.vertexbuffer);
	glDeleteBuffers(1, &ObjectCar.uvbuffer);
	glDeleteBuffers(1, &ObjectCar.normalbuffer);
	glDeleteTextures(1, &ObjectCar.Texture);
}

void Car::ComputeMVP()
{

	CarPosition.x += control.throttle * sin(control.steer + PI);
	CarPosition.z += control.throttle * cos(control.steer + PI);

	glm::vec3 trans_vec(CarPosition.x , 0.2f, CarPosition.z);
	
	glm::mat4 RotationMatrix = eulerAngleYXZ(control.steer + PI ,0.0f,0.0f );
	glm::mat4 TranslationMatrix = translate(mat4(), trans_vec);
	glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));

	MVP.ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	MVP.ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	MVP.ViewMatrix = glm::lookAt(
		glm::vec3(CarPosition.x + 30.0f, 30.0f, CarPosition.z  + 30.0f),           // Camera is here
		CarPosition, // and looks here : at the same position, plus "direction"
		glm::vec3(0.0f, 0.5f, 0.0f)                  // Head is up (set to 0,-1,0 to look upside-down)
	);

}

void Car::ComputeControlsFromInputs()
{

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		control.throttle = std::min(control.throttle + deltaTime * ControlRange.UnitSpeed , ControlRange.MaxSpeed);
	}
	else
	{
		control.throttle = std::max(control.throttle - deltaTime * ControlRange.Friction, 0.0f);
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		control.throttle = std::max(control.throttle - deltaTime * 10 * ControlRange.Friction, 0.0f);

	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		control.steer = std::min(control.steer + deltaTime * ControlRange.UnitSteer, ControlRange.MaxSteer);
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		control.steer = std::max(control.steer - deltaTime * ControlRange.UnitSteer, ControlRange.MinSteer);
	}

	// For the next frame, the "last time" will be "now"
	ComputeMVP();
	lastTime = currentTime;
}

glm::mat4 Car::ReturnPV()
{
	return MVP.ProjectionMatrix * MVP.ViewMatrix;
}
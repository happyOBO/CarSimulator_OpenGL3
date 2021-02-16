#include "Map.h"

extern GLFWwindow* window;

const float PI = 3.1415926f;
const float initialFoV = 45.0f;

Map::Map() {}

Map::Map(GLfloat init_x, GLfloat init_y, GLfloat init_z, GLuint* programID, GLuint* MatrixID, GLuint* TextureID , GLuint* ModelMatrixID)
{
	GroundPosition.x = init_x;
	GroundPosition.y = init_y;
	GroundPosition.z = init_z;

	RoadPosition.x = init_x;
	RoadPosition.y = init_y + 0.1f;
	RoadPosition.z = init_z;

	BuildingPosition.x = init_x;
	BuildingPosition.y = init_y + 0.1f;
	BuildingPosition.z = init_z;


	TreePosition.x = init_x;
	TreePosition.y = init_y + 0.1f;
	TreePosition.z = init_z;

	Ids.programID = programID;
	Ids.MatrixID = MatrixID;
	Ids.TextureID = TextureID;
	Ids.ModelMatrixID = ModelMatrixID;

	ObjectInit();
}


void Map::ObjectInit()
{
	ObjectGround.Texture = loadBMP_custom("ground.BMP");

	bool res = loadOBJ("ground.obj", ObjectGround.vertices, ObjectGround.uvs, ObjectGround.normals);

	// Load it into a VBO

	glGenBuffers(1, &ObjectGround.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectGround.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectGround.vertices.size() * sizeof(glm::vec3), &ObjectGround.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectGround.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectGround.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectGround.uvs.size() * sizeof(glm::vec2), &ObjectGround.uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectGround.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectGround.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectGround.normals.size() * sizeof(glm::vec2), &ObjectGround.normals[0], GL_STATIC_DRAW);


	ObjectRoad.Texture = loadBMP_custom("Road.BMP");

	res = loadOBJ("Road.obj", ObjectRoad.vertices, ObjectRoad.uvs, ObjectRoad.normals);

	// Load it into a VBO

	glGenBuffers(1, &ObjectRoad.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectRoad.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectRoad.vertices.size() * sizeof(glm::vec3), &ObjectRoad.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectRoad.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectRoad.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectRoad.uvs.size() * sizeof(glm::vec2), &ObjectRoad.uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectRoad.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectRoad.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectRoad.normals.size() * sizeof(glm::vec2), &ObjectRoad.normals[0], GL_STATIC_DRAW);


	ObjectBuilding.Texture = loadBMP_custom("buildings.BMP");

	res = loadOBJ("buildings.obj", ObjectBuilding.vertices, ObjectBuilding.uvs, ObjectBuilding.normals);

	// Load it into a VBO

	glGenBuffers(1, &ObjectBuilding.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuilding.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectBuilding.vertices.size() * sizeof(glm::vec3), &ObjectBuilding.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectBuilding.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuilding.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectBuilding.uvs.size() * sizeof(glm::vec2), &ObjectBuilding.uvs[0], GL_STATIC_DRAW);


	glGenBuffers(1, &ObjectBuilding.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuilding.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectBuilding.normals.size() * sizeof(glm::vec2), &ObjectBuilding.normals[0], GL_STATIC_DRAW);


	ObjectTree.Texture = loadBMP_custom("join_tree.BMP");

	res = loadOBJ("trees.obj", ObjectTree.vertices, ObjectTree.uvs, ObjectTree.normals);

	// Load it into a VBO

	glGenBuffers(1, &ObjectTree.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectTree.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectTree.vertices.size() * sizeof(glm::vec3), &ObjectTree.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ObjectTree.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectTree.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectTree.uvs.size() * sizeof(glm::vec2), &ObjectTree.uvs[0], GL_STATIC_DRAW);


	glGenBuffers(1, &ObjectTree.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectTree.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjectTree.normals.size() * sizeof(glm::vec2), &ObjectTree.normals[0], GL_STATIC_DRAW);


}

void Map::DrawMap(mat4 PV)
{
	ComputeMVP();
	glm::mat4 TotalGroundMVP = PV * GroundMVP.ModelMatrix;

	glUniformMatrix4fv(*(Ids.MatrixID), 1, GL_FALSE, &TotalGroundMVP[0][0]);
	glUniformMatrix4fv(*(Ids.ModelMatrixID), 1, GL_FALSE, &GroundMVP.ModelMatrix[0][0]);
	glUniform1i(*(Ids.TextureID), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ObjectGround.Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectGround.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	

	glBindBuffer(GL_ARRAY_BUFFER, ObjectGround.uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectGround.normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, ObjectGround.vertices.size());
	
	// Road
	glBindTexture(GL_TEXTURE_2D, ObjectRoad.Texture);

	glm::mat4 TotalRoadMVP = PV * RoadMVP.ModelMatrix;

	glUniformMatrix4fv(*(Ids.MatrixID), 1, GL_FALSE, &TotalRoadMVP[0][0]);
	glUniformMatrix4fv(*(Ids.ModelMatrixID), 1, GL_FALSE, &RoadMVP.ModelMatrix[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, ObjectRoad.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs

	glBindBuffer(GL_ARRAY_BUFFER, ObjectRoad.uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectRoad.normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, ObjectRoad.vertices.size());

	// Building

	glBindTexture(GL_TEXTURE_2D, ObjectBuilding.Texture);

	glm::mat4 TotalBuildingMVP = PV * BuildingMVP.ModelMatrix;

	glUniformMatrix4fv(*(Ids.MatrixID), 1, GL_FALSE, &TotalBuildingMVP[0][0]);
	glUniformMatrix4fv(*(Ids.ModelMatrixID), 1, GL_FALSE, &BuildingMVP.ModelMatrix[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuilding.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs



	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuilding.uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectBuilding.normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, ObjectBuilding.vertices.size());

	// Trees

	glBindTexture(GL_TEXTURE_2D, ObjectTree.Texture);

	glm::mat4 TotalTreeMVP = PV * TreeMVP.ModelMatrix;

	glUniformMatrix4fv(*(Ids.MatrixID), 1, GL_FALSE, &TotalTreeMVP[0][0]);
	glUniformMatrix4fv(*(Ids.ModelMatrixID), 1, GL_FALSE, &TreeMVP.ModelMatrix[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, ObjectTree.vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs



	glBindBuffer(GL_ARRAY_BUFFER, ObjectTree.uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, ObjectTree.normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, ObjectTree.vertices.size());
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void Map::release()
{
	glDeleteBuffers(1, &ObjectGround.vertexbuffer);
	glDeleteBuffers(1, &ObjectGround.uvbuffer);
	glDeleteBuffers(1, &ObjectGround.normalbuffer);
	glDeleteTextures(1, &ObjectGround.Texture);

	glDeleteBuffers(1, &ObjectRoad.vertexbuffer);
	glDeleteBuffers(1, &ObjectRoad.uvbuffer);
	glDeleteBuffers(1, &ObjectRoad.normalbuffer);
	glDeleteTextures(1,&ObjectRoad.Texture);

	glDeleteBuffers(1, &ObjectBuilding.vertexbuffer);
	glDeleteBuffers(1, &ObjectBuilding.uvbuffer);
	glDeleteBuffers(1, &ObjectBuilding.normalbuffer);
	glDeleteTextures(1, &ObjectBuilding.Texture);

	glDeleteBuffers(1, &ObjectTree.vertexbuffer);
	glDeleteBuffers(1, &ObjectTree.uvbuffer);
	glDeleteBuffers(1, &ObjectTree.normalbuffer);
	glDeleteTextures(1,&ObjectTree.Texture);
}

void Map::ComputeMVP()
{

	glm::vec3 Groundtrans_vec(GroundPosition.x, GroundPosition.y, GroundPosition.z);

	//MVP.ModelMatrix = glm::rotate(MVP.ModelMatrix, control.steer + PI, rotate_vec);

	glm::mat4 RotationMatrix = eulerAngleYXZ(0.0f, 0.0f, 0.0f);
	glm::mat4 TranslationMatrix = translate(mat4(), Groundtrans_vec);
	glm::mat4 ScalingMatrix = scale(mat4(), vec3(20.0f, 1.0f, 20.0f));

	GroundMVP.ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

	glm::vec3 Roadtrans_vec(RoadPosition.x, RoadPosition.y, RoadPosition.z);
	glm::mat4 RoadTranslationMatrix = translate(mat4(), Roadtrans_vec);


	RoadMVP.ModelMatrix = RoadTranslationMatrix * RotationMatrix * ScalingMatrix;

	glm::vec3 Buildingtrans_vec(BuildingPosition.x, BuildingPosition.y, BuildingPosition.z);
	glm::mat4 BuildingTranslationMatrix = translate(mat4(), Buildingtrans_vec);


	BuildingMVP.ModelMatrix = BuildingTranslationMatrix * RotationMatrix * ScalingMatrix;


	glm::vec3 Treetrans_vec(TreePosition.x, TreePosition.y, TreePosition.z);
	glm::mat4 TreeTranslationMatrix = translate(mat4(), Treetrans_vec);
	glm::mat4 TreeRotationMatrix = eulerAngleYXZ(0.0f, 0.0f, 0.0f);
	glm::mat4 TreeScalingMatrix = scale(mat4(), vec3(20.0f, 2.0f, 20.0f));

	TreeMVP.ModelMatrix = TreeTranslationMatrix * TreeRotationMatrix * TreeScalingMatrix;

}

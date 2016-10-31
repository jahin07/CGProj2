// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <stack>   
#include <sstream>
#include "glm/ext.hpp"
#include <iostream>
#include "unistd.h"
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;
// Include AntTweakBar
#include <AntTweakBar.h>

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

const int window_width = 1024, window_height = 768;

typedef struct Vertex {
	float Position[4];
	float Color[4];
	float Normal[3];
	void SetPosition(float *coords) {
		Position[0] = coords[0];
		Position[1] = coords[1];
		Position[2] = coords[2];
		Position[3] = 1.0;
	}
	void SetColor(float *color) {
		Color[0] = color[0];
		Color[1] = color[1];
		Color[2] = color[2];
		Color[3] = color[3];
	}
	void SetNormal(float *coords) {
		Normal[0] = coords[0];
		Normal[1] = coords[1];
		Normal[2] = coords[2];
	}
};

// function prototypes
void gridVert();
void cameraRotate();
int initWindow(void);
void initOpenGL(void);
void loadObject(char*, glm::vec4, Vertex * &, GLushort* &, int);
void createVAOs(Vertex[], GLushort[], int);
void createObjects(void);
void pickObject(void);
void renderScene(void);
void cleanup(void);
static void keyCallback(GLFWwindow*, int, int, int, int);
static void mouseCallback(GLFWwindow*, int, int, int);

// GLOBAL VARIABLES
GLFWwindow* window;

glm::mat4 gProjectionMatrix;
glm::mat4 gViewMatrix;
glm::vec4 ballinitpoint = glm::vec4(0.0f,0.4f,-3.36f,1.0f);
glm::vec4 peninitpoint = glm::vec4(0.0f,2.75f,0.4f,1.0f);
glm::vec4 coordinates;

GLuint gPickedIndex = -1;
std::string gMessage;

GLuint programID;
GLuint pickingProgramID;

const GLuint NumObjects = 11;	// ATTN: THIS NEEDS TO CHANGE AS YOU ADD NEW OBJECTS
GLuint VertexArrayId[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
GLuint VertexBufferId[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
GLuint IndexBufferId[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

size_t NumIndices[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
size_t VertexBufferSize[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
size_t IndexBufferSize[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

GLuint MatrixID;
GLuint ModelMatrixID;
GLuint ViewMatrixID;
GLuint ProjMatrixID;
GLuint PickingMatrixID;
GLuint pickingColorID;
GLuint LightID;
GLuint LightID1;
GLuint MatAmbID;
GLuint LightPowID;

GLint gX = 0.0;
GLint gZ = 0.0;
int flagv = 2, flagh = 2, flagu = 2, flagd = 2;
float transx = 0.0f, transz = 0.0f;
float rotx=0.0f, rotz=0.0f, rotz2=0.0f, rotph=0.0f, rotpv=0.0f, rotpa=0.0f;
float j=0.0f;
float x,y,z;
bool flagj = false;
glm::vec3 cam = glm::vec3(10.0f,10.0f,10.0f);

Vertex Hor_Vertices[22], Vert_Vertices[22];

void gridVert()
{
    int z=5.0,i=0.0;
    glm::vec4 verti;
    glm::vec4 color = glm::vec4(10.0,10.0,10.0,1.0);
    for(float x=-5.0; x<=5.0; x++)
    {
        verti = glm::vec4(x,0.0,z,1.0);
        Hor_Vertices[i].SetPosition(&verti.x);
        Hor_Vertices[i].SetColor(&color.x);
        i++;
        verti = glm::vec4(x,0.0,-z,1.0);
        Hor_Vertices[i].SetPosition(&verti.x);
        Hor_Vertices[i].SetColor(&color.x);
        i++;
        z=-z;
    }
    
    float x=-5.0;
    i=0.0;
    for(float z=-5.0; z<=5.0; z++)
    {
        verti = glm::vec4(x,0.0,z,1.0);
        Vert_Vertices[i].SetPosition(&verti.x);
        Vert_Vertices[i].SetColor(&color.x);
        i++;
        verti = glm::vec4(-x,0.0,z,1.0);
        Vert_Vertices[i].SetPosition(&verti.x);
        Vert_Vertices[i].SetColor(&color.x);
        i++;
        x=-x;
    }
}

void cameraRotate()
{
    glm::vec3 c;
    double pi= 3.14159265359;
    if(flagv==1)
    {
        c[0] = cam[0];
        c[1] = sqrt(300-(c[0]*c[0]))*cosf((2*pi*flagu)/16);
        printf("%f\n",c[1]);
        c[2] = sqrt(300-(c[0]*c[0]))*sinf((2*pi*flagu)/16);
        printf("%f\n",c[2]);
        
        cam[1]=c[1];
        cam[2]=c[2];
        
        gViewMatrix = glm::lookAt(glm::vec3(cam[0], cam[1], cam[2]),	// eye
                                  glm::vec3(0.0, 0.0, 0.0),	// center
                                  glm::vec3(0.0, 1.0, 0.0));	// up
    }
    
    if(flagh==1)
    {
        c[1] = cam[1];
        c[0] = sqrt(300-(c[1]*c[1]))*cosf((2*pi*flagd)/16);
        c[2] = sqrt(300-(c[1]*c[1]))*sinf((2*pi*flagd)/16);
        
        cam[0]=c[0];
        cam[2]=c[2];
        
        gViewMatrix = glm::lookAt(glm::vec3(cam[0], cam[1], cam[2]),    // eye
                                  glm::vec3(0.0, 0.0, 0.0),	// center
                                  glm::vec3(0.0, 1.0, 0.0));	// up
    }
    
    
}

// animation control
bool animation = false;
GLfloat phi = 0.0;

void loadObject(char* file, glm::vec4 color, Vertex * &out_Vertices, GLushort* &out_Indices, int ObjectId)
{
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ(file, vertices, normals);

	std::vector<GLushort> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, normals, indices, indexed_vertices, indexed_normals);

	const size_t vertCount = indexed_vertices.size();
	const size_t idxCount = indices.size();

	// populate output arrays
	out_Vertices = new Vertex[vertCount];
	for (int i = 0; i < vertCount; i++) {
		out_Vertices[i].SetPosition(&indexed_vertices[i].x);
		out_Vertices[i].SetNormal(&indexed_normals[i].x);
		out_Vertices[i].SetColor(&color[0]);
	}
	out_Indices = new GLushort[idxCount];
	for (int i = 0; i < idxCount; i++) {
		out_Indices[i] = indices[i];
	}

	// set global variables!!
	NumIndices[ObjectId] = idxCount;
	VertexBufferSize[ObjectId] = sizeof(out_Vertices[0]) * vertCount;
	IndexBufferSize[ObjectId] = sizeof(GLushort) * idxCount;
}


vec3 calcProj(float angle, vec4 cords)
{
    vec3 proj, h;
    h.x=cords.x;
    h.z = 3.36+cords.z - (2*cosf(angle)*j);
    h.y = -0.4+cords.y+(2*sinf(angle)*j) - (0.5*9.8*j*j);
    
    if(h.y <=-0.4)
    {
        x=h.x;
        y=h.y;
        z=h.z;
        j=0;
        flagj = false;
        return vec3(0,0,0);
    }
    j+=0.01;
    proj = h;
    return proj;
}

void createObjects(void)
{
	//-- COORDINATE AXES --//
	Vertex CoordVerts[] =
	{
		{ { 0.0, 0.0, 0.0, 1.0 }, { 4.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, 0.0, 1.0 }, { 4.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 4.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 5.0, 0.0, 1.0 }, { 0.0, 4.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 4.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 5.0, 1.0 }, { 0.0, 0.0, 4.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	};

	VertexBufferSize[0] = sizeof(CoordVerts);	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
	createVAOs(CoordVerts, NULL, 0);
    
	
	//-- GRID --//
	
	// ATTN: create your grid vertices here!
    
    gridVert();
    VertexBufferSize[1] = sizeof(Hor_Vertices);	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
    createVAOs(Hor_Vertices, NULL, 1);
    
    
    VertexBufferSize[2] = sizeof(Vert_Vertices);	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
    createVAOs(Vert_Vertices, NULL, 2);
    
	
	//-- .OBJs --//

	// ATTN: load your models here
    
	Vertex* Verts;
	GLushort* Idcs;
    //1. base
	loadObject("base.obj", glm::vec4(0.0, 1.0, 0.0, 1.0), Verts, Idcs, 3);
	createVAOs(Verts, Idcs, 3);
    
    //2. body
    loadObject("body.obj", glm::vec4(0.3, 0.2, 0.5, 1.0), Verts, Idcs, 4);
    createVAOs(Verts, Idcs, 4);
    
    //3. handle
    loadObject("handle.obj", glm::vec4(0.0, 0.0, 1.0, 1.0), Verts, Idcs, 5);
    createVAOs(Verts, Idcs, 5);
    
    //4. cylinder
    loadObject("cylinder.obj", glm::vec4(1.0, 1.0, 0.0, 1.0), Verts, Idcs, 6);
    createVAOs(Verts, Idcs, 6);
    
    //5. vert_danda
    loadObject("vert_danda.obj", glm::vec4(0.0, 1.0, 1.0, 1.0), Verts, Idcs, 7);
    createVAOs(Verts, Idcs, 7);
    
    //6. pen
    loadObject("pen.obj", glm::vec4(0.5, 0.0, 0.6, 1.0), Verts, Idcs, 8);
    createVAOs(Verts, Idcs, 8);
    
    //7. button
    loadObject("button.obj", glm::vec4(1.0, 0.5, 0.0, 1.0), Verts, Idcs, 9);
    createVAOs(Verts, Idcs, 9);
    
    //8. Ball
    loadObject("ball.obj", glm::vec4(0.0, 0.5, 0.5, 1.0), Verts, Idcs, 10);
    createVAOs(Verts, Idcs, 10);
    

}

void renderScene(void)
{
	//ATTN: DRAW YOUR SCENE HERE. MODIFY/ADAPT WHERE NECESSARY!


	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	// Re-clear the screen for real rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	{
        
        glm::vec3 lightPos = glm::vec3(4, 4, -4);
        glm::vec3 lightPos1 = glm::vec3(-4, 4, 4);
        glm::vec3 lightPos2 = glm::vec3(0, 7, 0);
		glm::mat4x4 ModelMatrix = glm::mat4(1.0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        if(glfwGetKey(window, GLFW_KEY_C)==GLFW_PRESS)
        {
            cameraRotate();
        }
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(LightID1, lightPos1.x, lightPos1.y, lightPos1.z);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		
		glBindVertexArray(VertexArrayId[0]);	// draw CoordAxes
		glDrawArrays(GL_LINES, 0, 6);
        
        glBindVertexArray(VertexArrayId[1]);	// draw Hor_Grids
        glDrawArrays(GL_LINES, 0, 22);
        
        glBindVertexArray(VertexArrayId[2]);	// draw Vert_Grids
        glDrawArrays(GL_LINES, 0, 22);
        
        
        
        glm::mat4 trans;
        /*
        if(x>5)
            x=4;
        if(z>5)
            z=4;
        if(z<-5)
            z=-4;
        if(x<-5)
            x=-4;
         */
        trans = glm::translate(trans, vec3(x,0,z));
        
        //Translation matrix for base
        glm::vec3 mtranslate = glm::vec3(transx, 0.0f, transz);
        trans = glm::translate(trans, mtranslate);
        ModelMatrix = trans;
        
        
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        if(glfwGetKey(window, GLFW_KEY_B)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        
        
        glBindVertexArray(VertexArrayId[3]);	// draw Base
        glDrawElements(GL_TRIANGLES, NumIndices[3], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        
        //Rotation matrix for body
        glm::mat4 rot;
        rot= glm::rotate(rot, (18.0f*rotx), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix = ModelMatrix*rot;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        if(glfwGetKey(window, GLFW_KEY_T)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 2.0f);
        }
        
        glBindVertexArray(VertexArrayId[4]);	// draw Body
        glDrawElements(GL_TRIANGLES, NumIndices[4], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        //Arm1 Rotation
        glm::mat4 rot2;
        rot2 = glm::translate(rot2, glm::vec3(0.0f, 1.75f, 0.0f));
        rot2 = glm::rotate(rot2, (18.0f*rotz), glm::vec3(-1.0f, 0.0f, 0.0f));
        rot2 = glm::translate(rot2, glm::vec3(0.0f, -1.75f, 0.0f));
        ModelMatrix = ModelMatrix*rot2;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        if(glfwGetKey(window, GLFW_KEY_1)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        
        glBindVertexArray(VertexArrayId[5]);	// draw Handle
        glDrawElements(GL_TRIANGLES, NumIndices[5], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        glBindVertexArray(VertexArrayId[6]);	// draw Cylinder
        glDrawElements(GL_TRIANGLES, NumIndices[6], GL_UNSIGNED_SHORT, (void*)0);
        
        
        
        //Arm2 Rotation
        glm::mat4 rotarm2;
        rotarm2 = glm::translate(rotarm2, glm::vec3(0.0f, 1.57f, -2.75f));
        rotarm2 = glm::rotate(rotarm2, (18.0f*rotz2), glm::vec3(-1.0f, 0.0f, 0.0f));
        rotarm2 = glm::translate(rotarm2, glm::vec3(0.0f, -1.57f, 2.75f));
        ModelMatrix = ModelMatrix*rotarm2;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        if(glfwGetKey(window, GLFW_KEY_2)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        
        glBindVertexArray(VertexArrayId[7]);	// draw Vert_danda
        glDrawElements(GL_TRIANGLES, NumIndices[7], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        
        //Pen Rotation
        glm::mat4 penrot;
        penrot = glm::translate(penrot, glm::vec3(0.0f, 0.4f, -2.75f));
        penrot = glm::rotate(penrot, (18.0f*rotpv), glm::vec3(1.0f, 0.0f, 0.0f));
        penrot = glm::rotate(penrot, (18.0f*rotph), glm::vec3(0.0f, -1.0f, 0.0f));
        penrot = glm::translate(penrot, glm::vec3(0.0f, -0.4f, 2.75f));
        
        penrot = glm::translate(penrot, glm::vec3(0.0f, 0.4f, -2.75f));
        penrot = glm::rotate(penrot, (18.0f*rotpa), glm::vec3(0.0f, 0.0f, 1.0f));
        penrot = glm::translate(penrot, glm::vec3(0.0f, -0.4f, 2.75f));
        
        ModelMatrix = ModelMatrix*penrot;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        
        glBindVertexArray(VertexArrayId[8]);	// draw pen
        glDrawElements(GL_TRIANGLES, NumIndices[8], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        glBindVertexArray(VertexArrayId[9]);	// draw button
        glDrawElements(GL_TRIANGLES, NumIndices[9], GL_UNSIGNED_SHORT, (void*)0);
        
        
        if(flagj)
        {
            coordinates = ModelMatrix*ballinitpoint;
            
            //std::cout<<glm::to_string(coordinates)<<std::endl;
            
            glm::vec4 pencoords = ModelMatrix*peninitpoint;
            
            //std::cout<<glm::to_string(pencoords)<<std::endl;
            
            glm::vec3 zaxis = glm::vec3(0,0,1);
            glm::vec4 vectorball = pencoords - coordinates;
            
            //glm::vec4 vb = glm::vec4(vectorball.x,vectorball.y,vectorball.z,1.0f);
            
            //std::cout<<glm::to_string(vectorball)<<std::endl;
            
            //glm::vec4 origincoords = ModelMatrix*vec4(0.0f,0.0f,0.0f,1.0f);
            float cos_angle = dot(normalize(vec3(vectorball.x,vectorball.y,vectorball.z)), normalize(zaxis));
            //float sin_angle = (vectorball.y)/(sqrt((vectorball.x*vectorball.x)+(vectorball.y*vectorball.y)+(vectorball.z*vectorball.z)));
            //float cos_angle = dot(normalize(vec3(vb.x,vb.y,vb.z)), normalize(zaxis));
            float angle = acos(cos_angle);
            //float angle = asin(sin_angle);
            //printf("%f\n",angle);
            ModelMatrix = glm::mat4(1.0);
            glm::mat4 projectile;
            glm::vec3 calcvec = calcProj(angle, coordinates);
            
            //std::cout<<glm::to_string(calcvec)<<std::endl;
            
            projectile = glm::translate(projectile, calcvec);
            
            //std::cout<<glm::to_string(projectile)<<std::endl;
            
            ModelMatrix=ModelMatrix*projectile;
            
//            j+=0.1;
//            glm::mat4 trans_ball = glm::translate(trans_ball, glm::vec3(- (4 * j * cosf(0)), 4 * j * sinf(0) - 0.5 * 9.8 * j * j, 0.0f));
//            //sleep(100);
//            ModelMatrix = ModelMatrix * trans_ball;
            
            glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
            glBindVertexArray(VertexArrayId[10]);	// draw ball
            glDrawElements(GL_TRIANGLES, NumIndices[10], GL_UNSIGNED_SHORT, (void*)0);
        }
        
			
		glBindVertexArray(0);
        
        
        

	}
	glUseProgram(0);
	// Draw GUI
	TwDraw();

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void pickObject(void)
{
	// Clear the screen in white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pickingProgramID);
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0); // TranslationMatrix * RotationMatrix;
		glm::mat4 MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        
        //copy paste
        
        glm::mat4 trans;
        trans = glm::translate(trans, vec3(x,0,z));
        
        //Translation matrix for base
        glm::vec3 mtranslate = glm::vec3(transx, 0.0f, transz);
        trans = glm::translate(trans, mtranslate);
        ModelMatrix = trans;
        
        
        MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
        

        if(glfwGetKey(window, GLFW_KEY_B)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        
        glUniform4f(pickingColorID, 3/255.0f, 0.0f, 0.0f, 1.0f);
        
        glBindVertexArray(VertexArrayId[3]);	// draw Base
        glDrawElements(GL_TRIANGLES, NumIndices[3], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        
        //Rotation matrix for body
        glm::mat4 rot;
        rot= glm::rotate(rot, (18.0f*rotx), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix = ModelMatrix*rot;
        MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
        if(glfwGetKey(window, GLFW_KEY_T)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 2.0f);
        }
        glUniform4f(pickingColorID, 4/255.0f, 0.0f, 0.0f, 1.0f);
        
        glBindVertexArray(VertexArrayId[4]);	// draw Body
        glDrawElements(GL_TRIANGLES, NumIndices[4], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        //Arm1 Rotation
        glm::mat4 rot2;
        rot2 = glm::translate(rot2, glm::vec3(0.0f, 1.75f, 0.0f));
        rot2 = glm::rotate(rot2, (18.0f*rotz), glm::vec3(-1.0f, 0.0f, 0.0f));
        rot2 = glm::translate(rot2, glm::vec3(0.0f, -1.75f, 0.0f));
        ModelMatrix = ModelMatrix*rot2;
        MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
        if(glfwGetKey(window, GLFW_KEY_1)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        glUniform4f(pickingColorID, 5/255.0f, 0.0f, 0.0f, 1.0f);
        
        glBindVertexArray(VertexArrayId[5]);	// draw Handle
        glDrawElements(GL_TRIANGLES, NumIndices[5], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        glUniform4f(pickingColorID, 6/255.0f, 0.0f, 0.0f, 1.0f);
        
        glBindVertexArray(VertexArrayId[6]);	// draw Cylinder
        glDrawElements(GL_TRIANGLES, NumIndices[6], GL_UNSIGNED_SHORT, (void*)0);
        
        
        
        //Arm2 Rotation
        glm::mat4 rotarm2;
        rotarm2 = glm::translate(rotarm2, glm::vec3(0.0f, 1.57f, -2.75f));
        rotarm2 = glm::rotate(rotarm2, (18.0f*rotz2), glm::vec3(-1.0f, 0.0f, 0.0f));
        rotarm2 = glm::translate(rotarm2, glm::vec3(0.0f, -1.57f, 2.75f));
        ModelMatrix = ModelMatrix*rotarm2;
        MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
        if(glfwGetKey(window, GLFW_KEY_2)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        glUniform4f(pickingColorID, 7/255.0f, 0.0f, 0.0f, 1.0f);
        
        glBindVertexArray(VertexArrayId[7]);	// draw Vert_danda
        glDrawElements(GL_TRIANGLES, NumIndices[7], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        
        
        //Pen Rotation
        glm::mat4 penrot;
        penrot = glm::translate(penrot, glm::vec3(0.0f, 0.4f, -2.75f));
        penrot = glm::rotate(penrot, (18.0f*rotpv), glm::vec3(1.0f, 0.0f, 0.0f));
        penrot = glm::rotate(penrot, (18.0f*rotph), glm::vec3(0.0f, -1.0f, 0.0f));
        penrot = glm::translate(penrot, glm::vec3(0.0f, -0.4f, 2.75f));
        
        penrot = glm::translate(penrot, glm::vec3(0.0f, 0.4f, -2.75f));
        penrot = glm::rotate(penrot, (18.0f*rotpa), glm::vec3(0.0f, 0.0f, 1.0f));
        penrot = glm::translate(penrot, glm::vec3(0.0f, -0.4f, 2.75f));
        
        ModelMatrix = ModelMatrix*penrot;
        MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
        if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS)
        {
            glUniform1f(LightPowID, 0.0f);
            glUniform1f(MatAmbID, 5.0f);
        }
        glUniform4f(pickingColorID, 8/255.0f, 0.0f, 0.0f, 1.0f);
        
        glBindVertexArray(VertexArrayId[8]);	// draw pen
        glDrawElements(GL_TRIANGLES, NumIndices[8], GL_UNSIGNED_SHORT, (void*)0);
        glUniform1f(LightPowID, 50.0f);
        glUniform1f(MatAmbID, 0.1f);
        glUniform4f(pickingColorID, 9/255.0f, 0.0f, 0.0f, 1.0f);
        
        glBindVertexArray(VertexArrayId[9]);	// draw button
        glDrawElements(GL_TRIANGLES, NumIndices[9], GL_UNSIGNED_SHORT, (void*)0);
        
        
        if(flagj)
        {
            coordinates = ModelMatrix*ballinitpoint;
            glm::vec4 pencoords = ModelMatrix*peninitpoint;
            glm::vec3 zaxis = glm::vec3(0,0,1);
            glm::vec4 vectorball = pencoords - coordinates;
            //glm::vec4 origincoords = ModelMatrix*vec4(0.0f,0.0f,0.0f,1.0f);
            float cos_angle = dot(normalize(vec3(vectorball.x,vectorball.y,vectorball.z)), normalize(zaxis));
            float angle = acos(cos_angle);
            ModelMatrix = glm::mat4(1.0);
            glm::mat4 projectile;
            glm::vec3 calcvec = calcProj(angle, coordinates);
            projectile = glm::translate(projectile, calcvec);
            ModelMatrix=ModelMatrix*projectile;
            
//            j+=0.1;
//            glm::mat4 trans_ball = glm::translate(trans_ball, glm::vec3(- (4 * j * cos(0)), 4 * j * sin(0) - 0.5 * 9.8 * j * j, 0.0f));
//            //sleep(100);
//            ModelMatrix = ModelMatrix * trans_ball;
            
            glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
            glBindVertexArray(VertexArrayId[10]);	// draw ball
            glDrawElements(GL_TRIANGLES, NumIndices[10], GL_UNSIGNED_SHORT, (void*)0);
        }
        
        
        
        
        
        // ATTN: DRAW YOUR PICKING SCENE HERE. REMEMBER TO SEND IN A DIFFERENT PICKING COLOR FOR EACH OBJECT BEFOREHAND
        //glUniform4f(pickingColorID, r/255.0f, g/255.0f, b/255.0f, 1.0f);
		glBindVertexArray(0);

	}
	glUseProgram(0);
	// Wait until all the pending drawing commands are really done.
	// Ultra-mega-over slow ! 
	// There are usually a long time between glDrawElements() and
	// all the fragments completely rasterized.
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Read the pixel at the center of the screen.
	// You can also use glfwGetMousePos().
	// Ultra-mega-over slow too, even for 1 pixel, 
	// because the framebuffer is on the GPU.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	unsigned char data[4];
	glReadPixels(xpos, window_height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

	// Convert the color back to an integer ID
	gPickedIndex = int(data[0]);
	
	if (gPickedIndex == 255){ // Full white, must be the background !
		gMessage = "background";
	}
	else {
		std::ostringstream oss;
        if(gPickedIndex==3)
        {
            oss << "Base";
            gMessage = oss.str();
        }
        if(gPickedIndex==4)
        {
            oss << "Top";
            gMessage = oss.str();
        }
        if(gPickedIndex==5)
        {
            oss << "Arm1";
            gMessage = oss.str();
        }
        if(gPickedIndex==6)
        {
            oss << "Joint";
            gMessage = oss.str();
        }
        if(gPickedIndex==7)
        {
            oss << "Arm2";
            gMessage = oss.str();
        }
        if(gPickedIndex==8)
        {
            oss << "Pen";
            gMessage = oss.str();
        }
	}

	// Uncomment these lines to see the picking shader in effect
	//glfwSwapBuffers(window);
	//continue; // skips the normal rendering
}

int initWindow(void)
{
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(window_width, window_height, "Majumdar, Jahin(6913-9840)", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window_width, window_height);
	TwBar * GUI = TwNewBar("Picking");
	TwSetParam(GUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwAddVarRW(GUI, "Last picked object", TW_TYPE_STDSTRING, &gMessage, NULL);

	// Set up inputs
	glfwSetCursorPos(window, window_width / 2, window_height / 2);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

	return 0;
}

void initOpenGL(void)
{

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	gProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix
	gViewMatrix = glm::lookAt(glm::vec3(10.0, 10.0, 10.0f),	// eye
		glm::vec3(0.0, 0.0, 0.0),	// center
		glm::vec3(0.0, 1.0, 0.0));	// up

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	pickingProgramID = LoadShaders("Picking.vertexshader", "Picking.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ProjMatrixID = glGetUniformLocation(programID, "P");
	
	PickingMatrixID = glGetUniformLocation(pickingProgramID, "MVP");
	// Get a handle for our "pickingColorID" uniform
	pickingColorID = glGetUniformLocation(pickingProgramID, "PickingColor");
	// Get a handle for our "LightPosition" uniform
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    LightID1 = glGetUniformLocation(programID, "LightPosition_worldspace1");
    MatAmbID = glGetUniformLocation(programID, "MatAmbPow");
    LightPowID = glGetUniformLocation(programID, "LightPower");

	createObjects();
}

void createVAOs(Vertex Vertices[], unsigned short Indices[], int ObjectId) {

	GLenum ErrorCheckValue = glGetError();
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t RgbOffset = sizeof(Vertices[0].Position);
	const size_t Normaloffset = sizeof(Vertices[0].Color) + RgbOffset;

	// Create Vertex Array Object
	glGenVertexArrays(1, &VertexArrayId[ObjectId]);	//
	glBindVertexArray(VertexArrayId[ObjectId]);		//

	// Create Buffer for vertex data
	glGenBuffers(1, &VertexBufferId[ObjectId]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[ObjectId]);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize[ObjectId], Vertices, GL_STATIC_DRAW);

	// Create Buffer for indices
	if (Indices != NULL) {
		glGenBuffers(1, &IndexBufferId[ObjectId]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ObjectId]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize[ObjectId], Indices, GL_STATIC_DRAW);
	}

	// Assign vertex attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset); 
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)Normaloffset);

	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color
	glEnableVertexAttribArray(2);	// normal

	// Disable our Vertex Buffer Object 
	glBindVertexArray(0);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
			);
	}
}

void cleanup(void)
{
	// Cleanup VBO and shader
	for (int i = 0; i < NumObjects; i++) {
		glDeleteBuffers(1, &VertexBufferId[i]);
		glDeleteBuffers(1, &IndexBufferId[i]);
		glDeleteVertexArrays(1, &VertexArrayId[i]);
	}
	glDeleteProgram(programID);
	glDeleteProgram(pickingProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// ATTN: MODIFY AS APPROPRIATE
	if (action == GLFW_PRESS)
    {
		switch (key)
		{
        case GLFW_KEY_UP:
                flagv=1;
                flagu--;
                if(glfwGetKey(window, GLFW_KEY_B)==GLFW_PRESS)
                {
                    transz--;
                    if(transz==-6)
                        transz=-5;
                }
                if(glfwGetKey(window, GLFW_KEY_1)==GLFW_PRESS)
                {
                    
                    if(rotz==4)
                        rotz=3;
                    rotz++;
                }
                if(glfwGetKey(window, GLFW_KEY_2)==GLFW_PRESS)
                {
                    
                    if(rotz2==4)
                        rotz2=3;
                    rotz2++;
                }
                if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS)
                {
                    
                    printf("Pen = %f\n",rotpv);
                    if(rotpv==-1)
                        rotpv=0;
                    rotpv--;
                }
                
                break;
        case GLFW_KEY_DOWN:
                flagv=1;
                flagu++;
                if(glfwGetKey(window, GLFW_KEY_B)==GLFW_PRESS)
                {
                    transz++;
                    if(transz==6)
                        transz=5;
                }
                if(glfwGetKey(window, GLFW_KEY_1)==GLFW_PRESS)
                {
                    
                    if(rotz==0)
                        rotz=1;
                    rotz--;
                }
                if(glfwGetKey(window, GLFW_KEY_2)==GLFW_PRESS)
                {
                    
                    if(rotz2==0)
                        rotz2=1;
                    rotz2--;
                }
                if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS)
                {
                    
                    printf("Pen = %f\n",rotpv);
                    if(rotpv==4)
                        rotpv=3;
                    rotpv++;
                }
                break;
        case GLFW_KEY_LEFT:
                flagh=1;
                flagd++;
                if(glfwGetKey(window, GLFW_KEY_B)==GLFW_PRESS)
                {
                    transx--;
                    if(transx==-6)
                        transx=-5;
                }
                if(glfwGetKey(window, GLFW_KEY_T)==GLFW_PRESS)
                {
                    rotx--;
                }
                if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_RELEASE)
                {
                    
                    rotph--;
                }
                if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS)
                {
                    
                    rotpa--;
                }
                break;
        case GLFW_KEY_RIGHT:
                flagh=1;
                flagd--;
                if(glfwGetKey(window, GLFW_KEY_B)==GLFW_PRESS)
                {
                    transx++;
                    if(transx==6)
                        transx=5;
                }
                if(glfwGetKey(window, GLFW_KEY_T)==GLFW_PRESS)
                {
                    rotx++;
                }
                if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_RELEASE)
                {
                    
                    rotph++;
                }
                if(glfwGetKey(window, GLFW_KEY_P)==GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS)
                {
                    
                    rotpa++;
                }
                break;
        case GLFW_KEY_J:
                if(flagj==false)
                    flagj=true;
            break;
        case GLFW_KEY_A:
			break;
		case GLFW_KEY_D:
			break;
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_S:
			break;
		case GLFW_KEY_SPACE:
			break;
		default:
			break;
		}
    }
    
    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
                flagv=0;
                
                break;
            case GLFW_KEY_DOWN:
                flagv=0;
                break;
            case GLFW_KEY_LEFT:
                flagh=0;
                break;
            case GLFW_KEY_RIGHT:
                flagh=0;
                break;
        }
    }
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		pickObject();
	}
}

int main(void)
{
	// initialize window
	int errorCode = initWindow();
	if (errorCode != 0)
		return errorCode;

	// initialize OpenGL pipeline
	initOpenGL();

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		//// Measure speed
		//double currentTime = glfwGetTime();
		//nbFrames++;
		//if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
		//	// printf and reset
		//	printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		//	nbFrames = 0;
		//	lastTime += 1.0;
		//}
		
		if (animation){
			phi += 0.01;
			if (phi > 360)
				phi -= 360;
		}

		// DRAWING POINTS
		renderScene();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	cleanup();

	return 0;
}

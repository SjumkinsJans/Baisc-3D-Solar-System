#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/random.hpp>
#include <shaderClass.h>
#include "stb_image.h"
#include <iostream>
#include <random>
#include <math.h>
#include <vector>
const int starCount = 10000;
int width = 1920, height = 1080;

glm::vec3 cameraPos = glm::vec3(0.0f, 150.0f, 250.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float currentFrame;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = width / 2, lastY = height / 2;
float yaw = -90.0f,pitch = -25.0f;
float sensitivity = 0.15f;
float FOV = 45.0f;
bool mouseFirst = true;


struct body {
	float size;
	glm::vec3 rotation; 
	float rot_speed; //rotation speed
	glm::vec3 orbit;
	float orbit_speed; //orbit speed
	glm::vec3 position;
	glm::vec4 color;
	std::vector<glm::vec3> voxelPos;
	unsigned int instanceVBO;
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (mouseFirst) {
		lastX = xpos;
		lastY = ypos;
		mouseFirst = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY-ypos;
	lastX = xpos;
	lastY = ypos;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

body sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluton;
void initBodies() {

	earth.size = 50.0f;
	earth.position = glm::vec3(0.0f, 0.0f, -1000.0f);
	earth.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	earth.rot_speed = 20.0f;
	earth.orbit = glm::vec3(0.0f, 1.0f, 0.0f);
	earth.orbit_speed = 5.0f;
	earth.color = glm::vec4(0.2f, 0.4f, 1.0f, 1.0f);
	
	sun.size = 800;
	sun.position = glm::vec3(0.0f, 0.0f, 0.0f);
	sun.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	sun.rot_speed = 5.0f;
	sun.orbit = glm::vec3(0.0f, 1.0f, 0.0f);
	sun.orbit_speed = 0.0f;
	sun.color = glm::vec4(1.0f, 0.9f, 0.5f, 1.0f);

	
	mercury.size = earth.size * 0.38;
	mercury.position = glm::vec3(0.0f, 0.0f, -390.0f);
	mercury.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	mercury.rot_speed = 1.0f;
	mercury.orbit = glm::vec3(0.121f, 0.992f, 0.0f);
	mercury.orbit_speed = 21.0f;
	mercury.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

	venus.size = earth.size * 0.95;
	venus.position = glm::vec3(0.0f, 0.0f, -720.0f);
	venus.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	venus.rot_speed = 5.0f;
	venus.orbit = glm::vec3(0.059f, 0.998f, 0.0f);
	venus.orbit_speed = 7.0f;
	venus.color = glm::vec4(0.9f, 0.8f, 0.6f, 1.0f);

	mars.size = earth.size * 0.53;
	mars.position = glm::vec3(0.0f, 0.0f, -1520.0f);
	mars.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	mars.rot_speed = 25.0f;
	mars.orbit = glm::vec3(0.032f, 0.999f, 0.0f);
	mars.orbit_speed = 2.5f;
	mars.color = glm::vec4(0.8f, 0.4f, 0.3f, 1.0f);


	jupiter.size = earth.size * 11.21;
	jupiter.position = glm::vec3(0.0f, 0.0f, -2400.0f);
	jupiter.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	jupiter.rot_speed = 15.0f;
	jupiter.orbit = glm::vec3(0.022f, 0.999f, 0.0f);
	jupiter.orbit_speed = 0.5f;
	jupiter.color = glm::vec4(0.8f, 0.7f, 0.5f, 1.0f);

	
	saturn.size = earth.size * 9.45;
	saturn.position = glm::vec3(0.0f, 0.0f, -4250.0f);
	saturn.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	saturn.rot_speed = 10.0f;
	saturn.orbit = glm::vec3(0.043f, 0.999f, 0.0f);
	saturn.orbit_speed = 0.25f;
	saturn.color = glm::vec4(0.9f, 0.8f, 0.5f, 1.0f);


	uranus.size = earth.size * 4.01;
	uranus.position = glm::vec3(0.0f, 0.0f, -9000.0f);
	uranus.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	uranus.rot_speed = 30.0f;
	uranus.orbit = glm::vec3(0.014f, 0.999f, 0.0f);
	uranus.orbit_speed = 0.05f;
	uranus.color = glm::vec4(0.6f, 0.9f, 0.9f, 1.0f);


	neptune.size = earth.size * 3.88f;
	neptune.position = glm::vec3(0.0f, 0.0f, -12000.0f);
	neptune.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	neptune.rot_speed = 30.0f;
	neptune.orbit = glm::vec3(0.031f, 0.999f, 0.0f);
	neptune.orbit_speed = 0.025f;
	neptune.color = glm::vec4(0.3f, 0.5f, 1.0f, 1.0f);


	pluton.size = earth.size * 0.1;
	pluton.position = glm::vec3(0.0f, 0.0f, -18000.0f);
	pluton.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	pluton.rot_speed = 30.0f;
	pluton.orbit = glm::vec3(0.294f, 0.955f, 0.0f);
	pluton.orbit_speed = 0.005f;
	pluton.color = glm::vec4(0.7f, 0.6f, 0.5f, 1.0f);
	
	
}

void keyboardInput(GLFWwindow* window,body * bodies) {
	const float cameraSpeed = 160.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraPos += cameraUp * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraPos -= cameraUp * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		cameraPos = bodies[0].position+glm::vec3(0.0f,500.0f,0.0f);
	}
	// Won't teleport you to a planet itself, because we only translate them to their positions, while their starting positions(to which we teleport) remain the same.
	//if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
	//	cameraPos = bodies[1].position + glm::vec3(0.0f, 500.0f, 0.0f);
	//}
	//if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
	//	cameraPos = bodies[2].position + glm::vec3(0.0f, 500.0f, 0.0f);
	//}
	//if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
	//	cameraPos = bodies[3].position + glm::vec3(0.0f, 500.0f, 0.0f);
	//}
	//if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
	//	cameraPos = bodies[4].position + glm::vec3(0.0f, 500.0f, 0.0f);
	//}
	//if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
	//	cameraPos = bodies[5].position + glm::vec3(0.0f, 500.0f, 0.0f);
	//}
	//if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
	//	cameraPos = bodies[6].position + glm::vec3(0.0f, 500.0f, 0.0f);
	//}
}

int main() {
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//creating a window
	GLFWwindow* window = glfwCreateWindow(width, height,"Basic 3D Solar System", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create a window !" << std::endl;
		glfwTerminate();
		return -1;
	}
	//setting up context
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);

	//celestial bodies
	float vertices[] = {
		//front
		-0.5f,-0.5f,0.5f, //bottom left
		0.5f,-0.5f,0.5f, //bottom right
		0.5f,0.5f,0.5f,	 //top right		
		-0.5f,0.5f,0.5f, //top left	

		//back
		-0.5f,-0.5f,-0.5f, //bottom left
		0.5f,-0.5f,-0.5f, //bottom right
		0.5f,0.5f,-0.5f,	 //top right		
		-0.5f,0.5f,-0.5f, //top left	
	};

	unsigned int indicies[] = {
		//front
		0,1,2,
		2,3,0,

		//back
		4,5,6,
		6,7,4,

		//right
		1,5,6,
		6,2,1,

		//left
		0,4,7,
		7,3,0,

		//top
		3,2,6,
		6,7,3,

		//bottom
		0,1,5,
		5,4,0
	};
	
	initBodies();
	body bodies[] = { sun,mercury,venus,earth,mars,jupiter,saturn,uranus,neptune,pluton };
	//find positions of voxels 
	for (int i = 0;i < 10;i++) {
		int bodySize = bodies[i].size;
		int x, y, z;
		int startPos = bodySize * -1 / 2;
		x = y = z = startPos;
		std::vector<glm::vec3> voxels;


		int arr_pos = 0;
		while (1) {
			if (x * x + y * y + z * z < bodySize) {
				voxels.emplace_back(x, y, z);
			}
			x++;

			if (x > startPos * -1) {
				x = startPos;
				y++;
				if (y > startPos * -1) {
					y = startPos;
					z++;
					if (z > startPos * -1) {
						bodies[i].voxelPos = std::move(voxels);
						break;
					}
				}
			}
		}
	}
	
	unsigned int VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	for (int i = 0;i < 10;i++) {
		glGenBuffers(1, &bodies[i].instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, bodies[i].instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, bodies[i].voxelPos.size() * sizeof(glm::vec3), bodies[i].voxelPos.data(), GL_STATIC_DRAW);
	}
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indicies),indicies,GL_STATIC_DRAW);

	Shader shader("vertexShader.txt", "fragmentShader.txt");
	Shader starsShader("starsVSh.txt", "starsFSh.txt");
	//stars
	float starVeritces[starCount * 3];
	for (int i = 0; i < starCount*3; i += 3) {
		glm::vec3 starPos = glm::sphericalRand(1000.0f);
		starVeritces[i] = starPos.x;
		starVeritces[i+1] = starPos.y;
		starVeritces[i+2] = starPos.z;
	}
	unsigned int starsVBO, starsVAO;
	glGenVertexArrays(1, &starsVAO);
	glBindVertexArray(starsVAO);
	
	glGenBuffers(1, &starsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starVeritces), starVeritces, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	


	glm::mat4 projection = glm::mat4(1.0f); 
	projection = glm::perspective(glm::radians(FOV), (float)width / (float)height,0.1f,14000.0f);


	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Hm.... 
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_BACK);




	//render loop 
	while (!glfwWindowShouldClose(window)) {
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		keyboardInput(window,bodies);
		glClearColor(0.01f, 0.01f, 0.01f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw
		shader.use();
		glBindVertexArray(VAO);

		glm::mat4 view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
		for (int i = 0;i < 10;i++) {
			glBindBuffer(GL_ARRAY_BUFFER, bodies[i].instanceVBO);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

			glm::mat4 model = glm::mat4(1.0f);
			//orbit
			model = glm::rotate(model, glm::radians((float)glfwGetTime()) * bodies[i].orbit_speed * 10, bodies[i].orbit);
			//distance
			model = glm::translate(model, glm::vec3(bodies[i].position.x, bodies[i].position.y, bodies[i].position.z));
			shader.setMatrix4fv("model", model);
			shader.setMatrix4fv("view", view);
			shader.setMatrix4fv("projection", projection);
			shader.setF4("color", bodies[i].color.x, bodies[i].color.y, bodies[i].color.z, bodies[i].color.w);
			glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0,bodies[i].voxelPos.size());
		}
		glBindVertexArray(starsVAO);
		starsShader.use();
		glm::mat4 starView = glm::mat4(glm::mat3(view));
		starsShader.setMatrix4fv("view", starView);
		starsShader.setMatrix4fv("projection", projection);
		
		glDrawArrays(GL_POINTS, 0, starCount);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    glfwTerminate();
    return 0;
}


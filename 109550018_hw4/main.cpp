#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object* model);
void LoadTexture(unsigned int& texture, const char* tFileName);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* F18Model = new Object("obj/F18.obj");
Object* SphereModel = new Object("obj/tennis_ball.obj");
unsigned int F18VAO, SphereVAO;

int windowWidth = 2000, windowHeight = 1200;

float angle = 0, moveDistance = 0, shiftDistance = 0, flyDistance = -80, flyXDistance = (((float)rand() / (float)RAND_MAX)) * 30 - 15;
float lighting = 10, angleDisplay = 0;
float squeezeFactor = 0;
bool squeezing = false, flip1 = true, flip2 = true, cut = true;
bool lightChanging = false;
int buff = 1;

bool leftRotate = false, rightRotate = false, start = false, shiftUp = false, shiftDown = false;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	srand((unsigned int)time(NULL));

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW4", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Shaders
	unsigned int vertexShader, fragmentShader, geometryShader, shaderProgram;
	vector<unsigned int> programs;
	vertexShader = createShader("shaders/F18.vert", "vert");
	fragmentShader = createShader("shaders/F18.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	programs.push_back(shaderProgram);
	
	vertexShader = createShader("shaders/back.vert", "vert");
	geometryShader = createShader("shaders/back.geom", "geom");
	fragmentShader = createShader("shaders/back.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/front.vert", "vert");
	geometryShader = createShader("shaders/front.geom", "geom");
	fragmentShader = createShader("shaders/front.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/sphere.vert", "vert");
	geometryShader = createShader("shaders/sphere.geom", "geom");
	fragmentShader = createShader("shaders/sphere.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	programs.push_back(shaderProgram);
	
	// Texture
	unsigned int diffuseTexture, bumpmapTexture, specularTexture;
	LoadTexture(diffuseTexture, "obj/F18_diffuse.jpg");
	LoadTexture(bumpmapTexture, "obj/F18_bumpmap.jpg");
	LoadTexture(specularTexture, "obj/F18_specular.jpg");

	// VAO, VBO
	F18VAO = ModelVAO(F18Model);
	SphereVAO = ModelVAO(SphereModel);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double dt;
	double lastTime = glfwGetTime();
	double currentTime, startTime = 0;
	unsigned int mLoc, vLoc, pLoc, normalMLoc, bumpLoc, diffuseLoc, specularLoc;
	int counter = 1, hitCounter = 0;
	cout << "Dodge the sun!!\n";
	cout << "Press S to start\n";
	cout << "Press B to enter buff mode\n";

	while (!glfwWindowShouldClose(window))
	{
		if (start) {
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else {
			glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		// Status update
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		if (angle > glm::radians(360.0f)) {
			angle -= glm::radians(360.0f);
			leftRotate = false;
			rightRotate = false;
		}
		if (angleDisplay > glm::radians(360.0f)) {
			angleDisplay -= glm::radians(360.0f);
		}
		angleDisplay += glm::radians(1.0f);
		if (counter % 40 == 0) {
			shiftUp = false;
			shiftDown = false;
			counter++;
		}
		lighting = 0.75f + 0.25 * cos(angle);

		// TO DO:
		glm::mat4 perspective = getPerspective();
		glm::mat4 view = getView();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 model_ball = glm::mat4(1.0f); 

		model = glm::translate(model, glm::vec3(0, 0, 15));
		model = glm::translate(model, glm::vec3(moveDistance, 0, shiftDistance));
		if (!start) {
			model = glm::rotate(model, angleDisplay, glm::vec3(0, 1, 0));
		}
		if (leftRotate) {
			model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
			angle += glm::radians(5.0f) * buff;
			moveDistance -= 0.2 * buff;
		}
		if (rightRotate) {
			model = glm::rotate(model, angle, glm::vec3(0, 0, -1));
			angle += glm::radians(5.0f) * buff;
			moveDistance += 0.2 * buff;
		}
		if (shiftUp) {
			counter++;
			shiftDistance -= 0.15 * buff;
		}
		if (shiftDown) {
			counter++;
			shiftDistance += 0.15 * buff;
		}

		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		glm::mat4 normalM = glm::transpose(glm::inverse(model));

		size_t n = programs.size();
		for (int i = 0; i < n - 1; i++) {
			glUseProgram(programs[i]);
			mLoc = glGetUniformLocation(programs[i], "M");
			vLoc = glGetUniformLocation(programs[i], "V");
			pLoc = glGetUniformLocation(programs[i], "P");
			normalMLoc = glGetUniformLocation(programs[i], "normalM");
			diffuseLoc = glGetUniformLocation(programs[i], "ourTexture");
			bumpLoc = glGetUniformLocation(programs[i], "ourTexture1");
			specularLoc = glGetUniformLocation(programs[i], "ourTexture2");
			glUniform1i(diffuseLoc, 0);
			glUniform1i(bumpLoc, 1);
			glUniform1i(specularLoc, 2);
			if (start) {
				glUniform1f(glGetUniformLocation(programs[i], "rand_float"), (((float)rand() / (float)RAND_MAX / 2.0) + 0.25));
				glUniform1f(glGetUniformLocation(programs[i], "rand_float1"), (((float)rand() / (float)RAND_MAX)));
			}
			else {
				glUniform1f(glGetUniformLocation(programs[i], "rand_float"), 0.0);
				glUniform1f(glGetUniformLocation(programs[i], "rand_float1"), 0.0);
			}
			glUniform1i(glGetUniformLocation(programs[i], "ball"), 0);
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));
			glUniformMatrix4fv(normalMLoc, 1, GL_FALSE, glm::value_ptr(normalM));
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, diffuseTexture);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, bumpmapTexture);
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, specularTexture);
			glBindVertexArray(F18VAO);
			glDrawArrays(GL_TRIANGLES, 0, F18Model->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}
		if (start) {
			if (cut) {
				startTime = glfwGetTime();
				cut = false;
			}
			cout << (currentTime - startTime) << endl;
			if (abs(flyXDistance - moveDistance) > 3.7 || abs(flyDistance - shiftDistance - 15) > 3.5) {
				model_ball = glm::translate(model_ball, glm::vec3(flyXDistance, 0, flyDistance));
				flyDistance += 0.1 * (currentTime - startTime);
				if (flyDistance > 40) {
					flyDistance = -80;
					flyXDistance = (((float)rand() / (float)RAND_MAX)) * 30 - 15;
				}
				model_ball = glm::scale(model_ball, glm::vec3(5, 5, 5));
				glUseProgram(programs[3]);
				mLoc = glGetUniformLocation(programs[3], "M");
				vLoc = glGetUniformLocation(programs[3], "V");
				pLoc = glGetUniformLocation(programs[3], "P");
				normalMLoc = glGetUniformLocation(programs[3], "normalM");
				glUniform1i(glGetUniformLocation(programs[3], "ball"), 1);
				glUniform1f(glGetUniformLocation(programs[3], "rand_float"), (((float)rand() / (float)RAND_MAX / 2.0) + 0.25));
				glUniform1f(glGetUniformLocation(programs[3], "rand_float1"), (((float)rand() / (float)RAND_MAX)));
				if (shiftDistance - flyDistance > 25) {
					glUniform1i(glGetUniformLocation(programs[3], "colorControl"), 3);
				}
				else if (shiftDistance - flyDistance > 5) {
					glUniform1i(glGetUniformLocation(programs[3], "colorControl"), 2);
				}
				else {
					glUniform1i(glGetUniformLocation(programs[3], "colorControl"), 1);
				}
				glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model_ball));
				glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));
				glUniformMatrix4fv(normalMLoc, 1, GL_FALSE, glm::value_ptr(normalM));
				glBindVertexArray(SphereVAO);
				glDrawArrays(GL_TRIANGLES, 0, SphereModel->positions.size());
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindVertexArray(0);
			}
			else {
				cout << "Crashed!!!!" << endl;
				moveDistance = 0;
				shiftDistance = 0;
				start = false;
			}
		}
		else {
			flyDistance = -80;
			flyXDistance = (((float)rand() / (float)RAND_MAX)) * 30 - 15;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TO DO:
// Add key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		start = !start;
		cut = true;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		leftRotate = true;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		rightRotate = true;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		shiftUp = true;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		shiftDown = true;
	}
	else if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		buff = 2;
		cout << "Buff Mode" << endl;
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		buff = 1;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else if (strcmp(type, "geom") == 0)
		shader = glCreateShader(GL_GEOMETRY_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		cout << "LINKING ERROR: ";
		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}



unsigned int ModelVAO(Object* model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->positions.size()), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->normals.size()), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		1.0f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		glm::vec3(0, 15, 40),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
}
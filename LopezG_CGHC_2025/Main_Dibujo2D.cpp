//Sadam Lopez Gonzalez
//Practica 2
//Fecha de entrega: 18 de agosto 2025
//numero de cuenta: 315279810


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "P2 - Dibujo Estrella - Sadam Lopez", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		-0.5f,  0.3f, 0.0f,   1.0f, 1.0f,0.0f, //0 
		0.5f,  0.3f, 0.0f,   1.0f, 1.0f,0.0f, // 1 triangulo medio
		0.0f,  -0.2f, 0.0f,   1.0f, 1.0f,0.0f, // 

		-0.2f,  0.3f, 0.0f,   1.0f, 1.0f,0.0f, // 
		0.2f,  0.3f, 0.0f,   1.0f, 1.0f,0.0f, // top triangle
		0.0f,  0.7f, 0.0f,   1.0f, 1.0f,0.0f, //5

		-0.2f,  0.1f, 0.0f,   1.0f, 1.0f,0.0f, // Left trianle
		-0.4f,  -0.3f, 0.0f,   1.0f, 1.0f,0.0f, //7

		0.2f,  0.1f, 0.0f,   1.0f, 1.0f,0.0f, // Right trianle
		0.4f,  -0.3f, 0.0f,   1.0f, 1.0f,0.0f, //9 

		0.0f,  0.7f, 0.0f,   0.0f, 0.0f,0.0f, //top tria 10
		0.2f,  0.3f, 0.0f,   0.0f, 0.0f,0.0f, // 
		0.5f,  0.3f, 0.0f,   0.0f, 0.0f,0.0f, // middle 12
		0.236f,  0.03f, 0.0f,   0.0f, 0.0f,0.0f, //
		0.4f,  -0.3f, 0.0f,   0.0f, 0.0f,0.0f, //right trian
		0.0f,  -0.2f, 0.0f,   0.0f, 0.0f,0.0f, //
		-0.4f,  -0.3f, 0.0f,   0.0f, 0.0f,0.0f, //left
		-0.236f,  0.03f, 0.0f,   0.0f, 0.0f,0.0f, //
		-0.5f,  0.3f, 0.0f,   0.0f, 0.0f,0.0f, //middle 18
		-0.2f,  0.3f, 0.0f,   0.0f, 0.0f,0.0f, //

		0.1f,  0.25f, 0.0f,   0.0f, 0.0f,0.0f, // 20
		0.1f,  0.2f, 0.0f,   0.0f, 0.0f,0.0f, // 21 ojo der
		0.1f,  0.15f, 0.0f,   0.0f, 0.0f,0.0f, // 22
		
		0.15f, 0.15f, 1.0f,	  1.0f, 0.75f, 0.796f, // 
		0.1f, 0.10f, 1.0f,    1.0f, 0.75f, 0.796f, //  mejilla der
		0.15f, 0.10f, 1.0f,   1.0f, 0.75f, 0.796f, // 

		-0.15f, 0.15f, 1.0f,	  1.0f, 0.75f, 0.796f, // 26
		-0.1f, 0.10f, 1.0f,    1.0f, 0.75f, 0.796f, //  mejilla izq
		-0.15f, 0.10f, 1.0f,   1.0f, 0.75f, 0.796f, // 

		-0.1f,  0.25f, 0.0f,   0.0f, 0.0f,0.0f, // 29
		-0.1f,  0.2f, 0.0f,   0.0f, 0.0f,0.0f, //  ojo izq
		-0.1f,  0.15f, 0.0f,   0.0f, 0.0f,0.0f // 

	};

	unsigned int indices[] = {  // note that we start from 0!
		0,1,2, // middle triangle
		3,4,5, // Top Triangle
		2,6,7, // Left triangle
		2,8,9  // Rigt triangle
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f)
		glClear(GL_COLOR_BUFFER_BIT);
		glLineWidth(9.0f);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);

        glPointSize(20);
        //glDrawArrays(GL_LINES,0,2);
        //glDrawArrays(GL_TRIANGLES,0,3);
        glDrawElements(GL_TRIANGLES, 12,GL_UNSIGNED_INT,0);
		glDrawArrays(GL_LINE_LOOP,10,10);
		glDrawArrays(GL_POINTS,20,12);

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}
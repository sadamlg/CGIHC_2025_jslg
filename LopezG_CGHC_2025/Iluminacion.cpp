﻿//Sadam Lopez Gonzalez
//Practica 8
//Fecha de entrega: 17 de octubre 2025
//numero de cuenta: 315279810

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.0f, 0.5f, 3.0f);
float movelightPos, movelightPos2 = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;
bool modoNoche = false;


int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materiales e Iluminacion - Sadam Lopez", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model garage((char*)"Models/garage/parkhaus.obj");
    Model dodge((char*)"Models/dodgeCharger/dodge2.obj");
    Model hotwheel((char*)"Models/hotwheels/hotwheels.obj");
    Model audir8((char*)"Models/Audi_R8_V10/audi.obj");
    Model audi((char*)"Models/audir8/audi.obj");
    Model tesla((char*)"Models/tesla/tesla.obj");
    Model Moon((char*)"Models/moon/MoonPrueba.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/tesla/Green-Blue Texture.png", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        //lightingShader.Use();
        //GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        //GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        ////glUniform3f(lightPosLoc, lightPos.x + movelightPos, lightPos.y + movelightPos, lightPos.z + movelightPos);
        //glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        //// Set lights properties
        //// Luz dia
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].position"),
        //lightPos.x + movelightPos, lightPos.y + (-0.05 * (movelightPos) * (movelightPos)+1.5), lightPos.z + 2);
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), 0.2f, 0.2f, 0.0f);
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), 1.0f, 1.0f, 0.0f);
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), 1.0f, 1.0f, 0.5f);

        ////// Luz noche
        //// Posición (más arriba y alejada para simular una estrella en el cielo)
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].position"), 
        //lightPos.x , lightPos.y + (-0.05 * (movelightPos2) * (movelightPos2)+1.5), lightPos.z + movelightPos2 + 2);
        //// Luz ambiental tenue (muy baja, apenas ilumina el entorno)
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].ambient"), 0.05f, 0.05f, 0.1f);
        //// Luz difusa azulada (color principal de la iluminación nocturna)
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].diffuse"), 0.2f, 0.3f, 0.6f);
        //// Luz especular blanca fría (reflejos intensos en superficies brillantes)
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].specular"), 0.6f, 0.7f, 1.0f);

        lightingShader.Use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        if (!modoNoche) {
            // Luz de día activa
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].position"),
                lightPos.x + movelightPos,
                lightPos.y + (-0.05f * movelightPos * movelightPos + 1.5f),
                lightPos.z + 2);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), 0.2f, 0.2f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), 1.0f, 1.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), 1.0f, 1.0f, 0.5f);

            // Luz de noche apagada
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].specular"), 0.0f, 0.0f, 0.0f);
        }
        else {
            // Luz de noche activa
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].position"),
                lightPos.x,
                lightPos.y + (-0.05f * movelightPos2 * movelightPos2 + 1.5f),
                lightPos.z + movelightPos2 + 2);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].ambient"), 0.05f, 0.05f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].diffuse"), 0.2f, 0.3f, 0.6f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[1].specular"), 0.6f, 0.7f, 1.0f);

            // Luz de día apagada
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "lights[0].specular"), 0.0f, 0.0f, 0.0f);
        }


        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties// Material que refleja toda la luz
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 128.0f);


        // Draw the loaded model
        glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 5.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        garage.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dodge.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.16f, 0.16f, 0.16f));
        model = glm::translate(model, glm::vec3(-9.0f, 1.2f, 11.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        audi.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::translate(model, glm::vec3(4.0f, 0.2f, 2.0f));
        model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        hotwheel.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        model = glm::translate(model, glm::vec3(-4.5f, 0.1f, 1.5f));
        model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        audir8.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        model = glm::translate(model, glm::vec3(11.0f, 0.8f, 13.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tesla.Draw(lightingShader);

        // --- Dibujar el sol ---
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(
            lightPos.x + movelightPos,
            lightPos.y - 0.05 * (movelightPos) * (movelightPos) + 1.5,
            lightPos.z
        ));
        model = glm::scale(model, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Moon.Draw(lightingShader);

        // --- Dibujar la Luna ---
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(
            lightPos.x,
            lightPos.y - 0.05 * (movelightPos2) * (movelightPos2)+1.5,
            lightPos.z + movelightPos2
        ));
        model = glm::scale(model, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Moon.Draw(lightingShader);

        //lampshader.Use();
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //model = glm::mat4(1.0f);
        //model = glm::translate(model,glm::vec3(-1.0f,1.0f,6.0f));
        //model = glm::scale(model, glm::vec3(0.3f));
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O])
    {
       
        movelightPos += 0.1f;
    }

    if (keys[GLFW_KEY_L])
    {
        
        movelightPos -= 0.1f;
    }

    if (keys[GLFW_KEY_I])
    {

        movelightPos2 += 0.1f;
    }

    if (keys[GLFW_KEY_K])
    {

        movelightPos2 -= 0.1f;
    }

    if (keys[GLFW_KEY_N]){
        modoNoche = !modoNoche; // alterna entre día y noche
    }

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}



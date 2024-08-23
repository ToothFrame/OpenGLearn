#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float rotationSpeed = 0.02f;
const float cameraSpeed = 0.05f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void modelControl(GLFWwindow* window, glm::vec3& rotationVector);


int main() {

    //initialising glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // for mac users
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // creating a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // register callback function for resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
   
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
     }

     // shader 
     Shader ourShader("vertexShader.glsl", "fragmentShader.glsl");
     //**************************************************************

     float vertices[] = {
         //// positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
         // 0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right
         // 0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
         //-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
         //-0.5f,  0.5f, 0.0f,    0.0f, 1.0f  // top left 
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
     };
     unsigned int indices[] = {
         0, 1, 3, // first triangle
         1, 2, 3  // second triangle
     };

     glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
     };



     unsigned int VBO, VAO, EBO;

     glGenVertexArrays(1, &VAO); // create array buffer
     glGenBuffers(1, &VBO); // create the buffer
     glGenBuffers(1, &EBO);

     glBindVertexArray(VAO); // bind VAO
     
     glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer (i believe any configuration will be applied to the last bound buffer)
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy vertices into the buffer's memory
     
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // specifies data format for vertices
     glEnableVertexAttribArray(0); // vertex attributes are disabled by default

     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texture
     glEnableVertexAttribArray(1);


     glBindBuffer(GL_ARRAY_BUFFER, 0);


     ourShader.use();
     glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
     ourShader.setInt("texture2", 1);
     // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode

     //depth buffer enable
     glEnable(GL_DEPTH_TEST);

     //TEXTURES *************************************************************


     float textCoords[] = {
         0.0f, 0.0f,
         1.0f, 0.0f,
         0.5f, 1.0f
     };

     unsigned int texture1, texture2;

     glGenTextures(1, &texture1);
     glGenTextures(1, &texture2);

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, texture1);

     // set the texture wrapping/filtering options (on the currently bound texture object)
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

     


     int width, height, nrChannels;
     stbi_set_flip_vertically_on_load(true);

     unsigned char* data = stbi_load("Resources/Assets/container.jpg", &width, &height, &nrChannels, 0);

     if (data)
     {
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
         glGenerateMipmap(GL_TEXTURE_2D);
     }
     else
     {
         std::cout << "Failed to load texture" << std::endl;
     }
     stbi_image_free(data);

     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D, texture2);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

     data = stbi_load("Resources/Assets/awesomeface.png", &width, &height, &nrChannels, 0);

     if (data)
     {
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
         glGenerateMipmap(GL_TEXTURE_2D);
     }
     else
     {
         std::cout << "Failed to load texture" << std::endl;
     }
     stbi_image_free(data);
     
     
     //matrices
     glm::mat4 model = glm::mat4(1.0f);
     model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

     glm::mat4 view = glm::mat4(1.0f);
     view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

     glm::mat4 projection;
     projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.001f, 1000.0f);

    while (!glfwWindowShouldClose(window))
    {
        double timeElapsed = glfwGetTime();
        //input
        processInput(window);

        //camera stuff
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //model view proj matrixes update
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);


        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.setFloat("time", glfwGetTime());

        //depthBuffer clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw.
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle + float(timeElapsed*10)), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // check events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
       
    }
    glfwTerminate();
	return 0;
}

// whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void modelControl(GLFWwindow* window, glm::vec3& rotationVector) {

    rotationVector = glm::vec3(0.0f);

    // rotation along the Z-axis
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        rotationVector.x -= rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        rotationVector.x += rotationSpeed;
    }

    // rotation along the X-axis
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        rotationVector.y -= rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        rotationVector.y += rotationSpeed;
    }

    // rotation along the Y-axis
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        rotationVector.z -= rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        rotationVector.z += rotationSpeed;
    }
}
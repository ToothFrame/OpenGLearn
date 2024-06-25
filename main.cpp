#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0);\n"
"}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



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

     // Vertex Shader *************************************************************************************
     int success;
     char infoLog[512];

     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
     glCompileShader(vertexShader);
     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
     if (!success)
     {
         glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
     }


     // Fragment Shader *************************************************************************************
     unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
     glCompileShader(fragmentShader);

     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
     if (!success)
     {
         glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
     }
     // Shader Program *************************************************************************************
     unsigned int shaderProgram = glCreateProgram();

     glAttachShader(shaderProgram, vertexShader);
     glAttachShader(shaderProgram, fragmentShader);
     glLinkProgram(shaderProgram);

     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
     if (!success) {
         glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
     }

     glDeleteShader(vertexShader);
     glDeleteShader(fragmentShader);
     //********************************************************************************************************


     float vertices[] = { // rectangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
     };
     unsigned int indices[] = {  // first add all the vertices, then connect them by indices so they are not drawn multiple times
         0, 1, 3,   // first triangle
         1, 2, 3    // second triangle
     };

     unsigned int VBO, VAO, EBO;
     glGenBuffers(1, &VBO); // create the buffer
     glGenVertexArrays(1, &VAO); // create array buffer
     glGenBuffers(1, &EBO); // creating element buffer object to avoid vertex overlap issues

     glBindVertexArray(VAO); // bind VAO
     glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer (i believe any configuration will be applied to the last bound buffer)
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind EBO. DO NOT UNBIND WHILE VAO IS ACTIVE AS EBO IS STORED WITHIN IT

     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy vertices into the buffer's memory
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // specifies data format for vertices
     glEnableVertexAttribArray(0); // vertex attributes are disabled by default

     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindVertexArray(0);

     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode


    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // redundant since we only have one VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // "good practice"
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
}
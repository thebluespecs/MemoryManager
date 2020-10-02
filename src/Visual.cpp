#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

// #include "Visual.h"

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    // in vec3 color;
    // out vec3 Color;
    uniform mat4 u_MVP;

    void main()
    {
        // Color = color;
        gl_Position = u_MVP * vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    // in vec3 Color;
    out vec4 outColor;

    uniform vec4 u_color;

    void main()
    {
        outColor = u_color;
    }
)glsl";

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 200, "Memory Manager", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        std::cout<< "Error!"<<std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
        -1.0f,  1.0f,  // Top-left
         -0.002f,  1.0f,  // Top-right
         -0.002f, -1.0f,  // Bottom-right
        -1.0f, -1.0f  // Bottom-left

    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0

    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    // glEnableVertexAttribArray(colAttrib);
    // glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));

    std::vector<int> v;
    for(int i=0;i<1000;i++){
        v.push_back(std::rand()%100);
    }
    

    // bool running = true;
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int i=0;
        // Draw a rectangle from the 2 triangles using 6 indices
        for(i=0;i<1000;i++)
        {   
            // std::cout<<i<<std::endl;
            glm::vec3 translationA(float(i)/float(1000), 0, 0);
            glm::mat4 model  = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_MVP"), 1, GL_FALSE, &mvp[0][0]);
            float r=0,g=0,b=0;
            if(v[i]<50) g=1.0f;
            else if(v[i]>=50 && v[i]<=75) r=1.0f;
            else b=1.0f;
            glUniform4f(glGetUniformLocation(shaderProgram, "u_color"), r ,g, b, 1.0f);
            

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // {
        //     glm::vec3 translationA(0.0f, 0, 0);
        //     glm::mat4 model  = glm::translate(glm::mat4(1.0f), translationA);
        //     glm::mat4 mvp = proj * view * model;

        //     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_MVP"), 1, GL_FALSE, &mvp[0][0]);
        //     glUniform4f(glGetUniformLocation(shaderProgram, "u_color"), 0.0f ,0.3f, 0.5f, 1.0f);
            
        //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // }


        // Swap buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        // window.display();
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    // window.close();
    glfwTerminate();

    return 0;
}
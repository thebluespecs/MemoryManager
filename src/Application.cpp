#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MemoryManager.h"

// Memory pool needed by the game loop
extern char MM_pool[];

// needed for standby for a thread.
#define TIME_MULTIPLIER 1000000


// ***************** SHADERS *****************************

// Vertex Shader
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

// Fragment Shader
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

// ********************** THREADS ************************ 
void* task1(void *unused){    
    std::cout<<"Task one Allocating."<<std::endl;
    usleep(1*TIME_MULTIPLIER);
    std::cout << "Task1: Free memory = " << MM::freeRemaining() << std::endl;
    int* a = (int*)MM::allocate(sizeof(int)*200);
    
    std::cout<<"Array allocated.";

    // Iterator is also allocated in the MM_pool
    int* ip = (int*)MM::allocate(sizeof(int)); 
    for(*ip = 0; *ip <200;*ip=(*ip)+1) *(a+(*ip)) = 1; //Array initialised 

    int* int_pointer = (int*)MM::allocate(std::rand()%10);
    *int_pointer = 10000;
    std::cout << "Task1: Free memory = " << MM::freeRemaining() << std::endl;
    usleep(1*TIME_MULTIPLIER);

    std::cout<<"Deallocating Array."<<std::endl;
    MM::deallocate(a);
    usleep(1*TIME_MULTIPLIER);

}

void* task2(void *unused){
    usleep(6*TIME_MULTIPLIER);
    std::cout<<"Task two Allocating."<<std::endl;

    int* int_pointer = (int*)MM::allocate(sizeof(int));
    *int_pointer = 10000;
    std::cout << "Task2: Free memory = " << MM::freeRemaining() << std::endl;
    usleep(1*TIME_MULTIPLIER);
    MM::deallocate(int_pointer);

    int* ip = (int*)MM::allocate(sizeof(int));
    *ip = 0;

    while(*ip < 6){
        int* bleh = (int*)MM::allocate(std::rand()%50);
        *bleh = 10000;
        *ip = *ip + 1;
        usleep(1*TIME_MULTIPLIER);

    }

    std::cout << "Task2: Free memory = " << MM::freeRemaining() << std::endl;
}

void* task3(void *unused){    

    usleep(15*TIME_MULTIPLIER);
    std::cout<<"Task three Allocating."<<std::endl;
    
    int* ip = (int*)MM::allocate(sizeof(int));
    *ip = 0;

    while(*ip < 6){
        int* bleh = (int*)MM::allocate(std::rand()%50);
        *bleh = 10000;
        *ip = *ip + 1;
        usleep(1*TIME_MULTIPLIER);
    }

    std::cout << "Task3: Free memory = " << MM::freeRemaining() << std::endl;
}


void* task4(void *unused){    

    usleep(25*TIME_MULTIPLIER);
    std::cout<<"Task four Allocating."<<std::endl;

    while(true){
        int* bleh = (int*)MM::allocate(std::rand()%50);
        *bleh = 10000;
        usleep(1*TIME_MULTIPLIER);
    }
}

int main(int argc, char const *argv[])
{
    // initialise the manager
    MM::init();

    // Pthreads have been used for multithreaded
    pthread_t thread1, thread2, thread3, thread4;

    // create N number of threads as workers on the memory manager
    int i1 = pthread_create(&thread1, NULL, task1, NULL);
    int i2 = pthread_create(&thread2, NULL, task2, NULL);
    int i3 = pthread_create(&thread3, NULL, task3, NULL);
    int i4 = pthread_create(&thread3, NULL, task4, NULL);

    // Used GLFW as the windowing library, SDL is also an option
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 100, "Memory Manager", NULL, NULL);
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

    // Rectangle co-ordinates to represent a memory block on the visualiser.
    GLfloat vertices[] = {
        -1.0f,       1.0f,  // Top-left
        -0.998f,     1.0f,  // Top-right
        -0.998f,    -1.0f,  // Bottom-right
        -1.0f,      -1.0f   // Bottom-left
    };

    // Creating VBOs
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);

    // Index Buffer
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

    // Model matrices for translation purposes
    glm::mat4 proj = glm::ortho(-1.0f, 0.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, 0.0f));

    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // the below loop queries the memory pool and colors the rectange strip accordingly.
        // TODO : Instancing the rectangle instead of multiple draw calls
        // Instancing the faster than multiple draw calls hence less frame drops for a bigger pool of memory.
        
        int blues=0; // number of blocks of header.
        int reds=0; // number of blocks of assigned memory.
        for(int i=0;i<1000;i++){  
            float r=0,g=0,b=0; // setting the color according to the state of the memory block.
            if(reds && blues){
                b=1.0f;
                blues=0;
            } 
            else if(reds && !blues){
                r=1.0f;
                reds--;
            }
            else{
                char header_one = 0;
                char header_two = 0;
                if(MM_pool[i] == (char)0x0){g = 1.0f; r=0.55f; b=0.4f;}
                else{
                    header_one = MM_pool[i];
                    header_two = MM_pool[i+1];
                    header_one = 0x3F & header_one;
                    int size = 0;
                    MM::getSizeInt(&header_one, &header_two, &size);
                    b=1.0f;
                    reds=size;
                    blues=1;
                }
            }
            //translating the rectangle accordingly. setting up the MVP matrix.
            glm::vec3 translationA(float(i)/float(1000), 0, 0);
            glm::mat4 model  = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;

            // setting the uniforms accordinly for a draw call.            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_MVP"), 1, GL_FALSE, &mvp[0][0]);
            glUniform4f(glGetUniformLocation(shaderProgram, "u_color"), r ,g, b, 1.0f);
            
            // Draw a rectangle from the 2 triangles using 6 indices    
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // Swap buffers
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // OpenGL cleanup
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);

    // window.close();
    glfwTerminate();

    // incase, every thread finished its task.
    // not possible here.
    pthread_join(thread4, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);

    return 0;
}

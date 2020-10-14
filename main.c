#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "GLutils.h"

#define bool int
#define true 1
#define false 0

int WIDTH = 1280;
int HEIGHT = 720;

int itr = 32;
double zoom = 300.0;
double offsetX, offsetY = 0.0;
double oldX, oldY = 0.0;

GLuint shaderProgram;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void errorCallback(int e, const char* s);
//void cursorCallback();
void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);
void windowSizeCallback(GLFWwindow* window, int width, int height);

int main(){
    char vertexShader[1024 *256];
    char fragmentShader[1024 * 256];
    GLuint vbo, vao;
    GLuint vs, fs;
    const GLchar* p;
    int success = -1;

    if (!glfwInit()){
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot", NULL, NULL);
    if (!window){
        const char* description;
        int code = glfwGetError(&description);
        fprintf(stderr, "ERROR: could not open window with GLFW3\n Error %x %s", code, description);
        glfwTerminate();
        return 1;
    }
    
    // create callbacks
    glfwSetErrorCallback(errorCallback);
    glfwSetKeyCallback(window, keyCallback);
    //glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    glfwMakeContextCurrent(window);

    // start GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,

         1.0f,  1.0f,
        -1.0f,  1.0f,
        -1.0f, -1.0f
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    // load shaders from files 
    parseFile("vs.glsl", vertexShader, 1024*256);
    parseFile("fs.glsl", fragmentShader, 1024*256);

    // compile shaders
    // compile vertex shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    p = (const GLchar*)vertexShader;
    glShaderSource(vs, 1, &p, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    // vertex shader errors?
    if (!success) {
        fprintf( stderr, "ERROR: GL shader index %i (vertex) did not compile\n", vs);
        return 1; /* or exit or something */
    }

    // compile fragment shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    p = (const GLchar*)fragmentShader;
    glShaderSource(fs, 1, &p, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    // fragment shader errors?
    if (!success) {
        fprintf( stderr, "ERROR: GL shader index %i (fragment) did not compile\n", fs );
        return 1; /* or exit or something */
    }

    // create and link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    // linking errors?
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        fprintf( stderr, "ERROR: could not link shader programme GL index %i\n", shaderProgram );
        //glLogError(shaderProgram);
        return 1;
    }

    // delete shader and use program
    glDeleteShader(fs);
	glDeleteShader(vs);
	glUseProgram(shaderProgram);

    glUniform2d(glGetUniformLocation(shaderProgram, "screenSize"), (double)WIDTH, (double)HEIGHT);
	glUniform2d(glGetUniformLocation(shaderProgram, "offset"), offsetX, offsetY);
	glUniform1d(glGetUniformLocation(shaderProgram, "zoom"), zoom);
	glUniform1i(glGetUniformLocation(shaderProgram, "itr"), itr);

    // main loop to draw mandelbrot
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    // keyboard callback  
    // escape key
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS){
        zoom = 300;
        itr = 32;
        offsetX, offsetY = 0.0;
    }
    // Q for zoom in
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        zoom *= 1.25;
        itr += 4;
    }
    // E for zoom out
    if (key == GLFW_KEY_E && action == GLFW_PRESS){
        zoom /= 1.25;
        itr -= 4;
    }
    // W up
    if (key == GLFW_KEY_W ){
        offsetY -= 20 / zoom;
    }
    // S down
    if (key == GLFW_KEY_S){
        offsetY += 20 / zoom;
    }
    // A left
    if (key == GLFW_KEY_A){
        offsetX += 20 / zoom;
    }
    // D Right
    if (key == GLFW_KEY_D){
        offsetX -= 20 / zoom;
    }

    glUniform1i(glGetUniformLocation(shaderProgram, "itr"), itr);
	glUniform1d(glGetUniformLocation(shaderProgram, "zoom"), zoom);
	glUniform2d(glGetUniformLocation(shaderProgram, "offset"), offsetX, offsetY);
}

//void cursorCallback(GLFWwindow* window, double posX, double posY){
    // cursor callback
//}
void scrollCallback(GLFWwindow* window, double offsetX, double offsetY){
    // scroll wheel callback
    if (offsetY != 0){
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);

        double dx = (posX - WIDTH / 2) / zoom - offsetX;
        double dy = (HEIGHT - posY - HEIGHT / 2) / zoom - offsetY;

        if (offsetY < 0){
            zoom /= 1.2;
            itr /= 2;
        }
        else{
            zoom *= 1.2;
            itr *= 2;
        }

        offsetX -= dx;
        offsetY -= dy;

        glUniform1d(glGetUniformLocation(shaderProgram, "zoom"), zoom);
		glUniform2d(glGetUniformLocation(shaderProgram, "offset"), offsetX, offsetY);
    }
}

void windowSizeCallback(GLFWwindow* window, int width, int height){
    // window size callback
    glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
	glUniform2d(glGetUniformLocation(shaderProgram, "screenSize"), (double)WIDTH, (double)HEIGHT);
}
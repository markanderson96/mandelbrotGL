#ifndef _GL_UTILS_H_
#define _GL_UTILS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define bool int
#define true 1
#define false 0

#define GL_LOG_FILE "gl.log"

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow* g_window;

bool glLog(const char* message, ...);
bool glLogError(const char* message, ...);

bool parseFile(const char* fileName, char* shaderStr, int maxLen);

void errorCallback(int e, const char* s);

#endif

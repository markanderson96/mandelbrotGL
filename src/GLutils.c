#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "GLutils.h"

#define bool int
#define true 1
#define false 0

#define GL_LOG_FILE "gl.log"

static double previousSeconds;

bool glLog(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
    fprintf( stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", GL_LOG_FILE );
    return false;
    }
    
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

bool glLogError(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
    fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", GL_LOG_FILE );
    return false;
    }

    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start( argptr, message );
    vfprintf( stderr, message, argptr );
    va_end( argptr );
    fclose(file);
    return true;
}

// copy a shader from a plain text file into a character array
bool parseFile(const char* fileName, char* shaderStr, int maxLen) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        glLogError("ERROR: opening file for reading: %s\n", fileName);
        return false;
    }
    size_t cnt = fread(shaderStr, 1, maxLen - 1, file);
    if ( (int)cnt >= maxLen - 1 ) { glLogError( "WARNING: file %s too big - truncated.\n", fileName ); }
    if ( ferror( file ) ) {
        glLogError("ERROR: reading shader file %s\n", fileName);
        fclose(file);
        return false;
    }
    // append \0 to end of file string
    shaderStr[cnt] = 0;
    fclose( file );
    return true;
}

void updateFPS(GLFWwindow* window, const char* title) {
    static int frame_count;
    double currentSeconds = glfwGetTime();
    double elapsedSeconds = currentSeconds - previousSeconds;

    if (elapsedSeconds > 0.25) {
        double fps;
        char tmp[64];

        previousSeconds = currentSeconds;
        fps = (double)frame_count / elapsedSeconds;
        sprintf( tmp, "%s fps: %.2f", title, fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

void errorCallback(int e, const char* s) {
    fprintf(stderr, "ERROR %x: %s", e, s);
    glLogError("%s", s);
}
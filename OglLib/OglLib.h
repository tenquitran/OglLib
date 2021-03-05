#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <map>

#include <atlstr.h>


// GLEW library.

#include <GL/glew.h>
#include <GL/wglew.h>

#pragma comment(lib, "glew32.lib")

// GLFW library.

#include <GLFW/glfw3.h>

#pragma comment(lib, "glfw3.lib")

// GLM library.
#define GLM_FORCE_RADIANS    // enforce all angles to be in radians, not in degrees
#include <glm/glm.hpp>       // the most common GLM headers
#include <glm/gtc/matrix_transform.hpp>

// WGL (Windows OpenGL).

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


#include "Exception.h"

#include "ProgramGLSL.h"

#include "CameraFps.h"


namespace OglLib
{
// For OpenGL code.
#define BUFFER_OFFSET(offset) ((void *)(offset))
}

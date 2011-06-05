#ifndef GW1K_GLERRORCHECK_H_
#define GW1K_GLERRORCHECK_H_

#include <GL/gl.h>
#include <string>
#include <iostream>


#ifndef NDEBUG // = ifdef DEBUG
#ifdef GW1K_ENABLE_GL_ERROR_CHECKS
#define PRINT_IF_GL_ERROR           \
    if (gw1k::printIfGLError()) {   \
        std::cout                   \
            << "(encountered in "   \
            << __FILE__             \
            << ':'                  \
            << __LINE__             \
            << ')' << std::endl;    \
    }
#else
#define GW1K_PRINT_IF_GL_ERROR
#endif // GW1K_ENABLE_GL_ERROR_CHECKS
#endif // NDEBUG


namespace
{


static std::string glErrorMsg;


} // namespace


namespace gw1k
{


inline bool checkGLError()
{
    GLenum err = glGetError();

    switch (err)
    {
    case GL_NO_ERROR:
        return false;
    case GL_INVALID_ENUM:
        // An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.
        glErrorMsg = "GL_INVALID_ENUM";
        return true;
    case GL_INVALID_VALUE:
        // A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.
        glErrorMsg = "GL_INVALID_VALUE";
        return true;
    case GL_INVALID_OPERATION:
        // The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.
        glErrorMsg = "GL_INVALID_OPERATION";
        return true;
    case GL_STACK_OVERFLOW:
        // This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.
        glErrorMsg = "GL_STACK_OVERFLOW";
        return true;
    case GL_STACK_UNDERFLOW:
        // This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.
        glErrorMsg = "GL_STACK_UNDERFLOW";
        return true;
    case GL_OUT_OF_MEMORY:
        // There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.
        glErrorMsg = "GL_OUT_OF_MEMORY";
        return true;
    case GL_TABLE_TOO_LARGE:
        // The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.
        glErrorMsg = "GL_TABLE_TOO_LARGE";
        return true;
    default:
        return false;
    }
}


inline bool printIfGLError()
{
    bool err = checkGLError();
    if (err)
    {
        std::cout << "GL ERROR: " << glErrorMsg << std::endl;
    }
    return err;
}


} // namespace gw1k


#endif // GW1K_GLERRORCHECK_H_

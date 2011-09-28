#ifndef GW1K_GLFWAPP_H_
#define GW1K_GLFWAPP_H_

#include <GL/glfw.h>

#include "Point.h"

namespace gw1k
{

class GLFWApp
{

public:

    GLFWApp();

    virtual ~GLFWApp();

public:

    int init();

    int terminate();

    void mainLoop();

protected:

    virtual void beforeRender() = 0;

    virtual void render() = 0;

    virtual void afterRender() = 0;

    /**
     * Override this to be able to decide about window size before the window is created.
     * The default implementation uses half the desktop resolution.
     */
    const Point getInitialWindowSize();

    /**
     * Override this to set GLFW Window Hints. The default implementation doesn't set any hints.
     */
    void setupWindowHints();

    /**
     * Override this to set RGBA channel bits and depth and stencil buffer bits as expected by glfwOpenWindow().
     * The default implementation sets RGBA = 8 (each), depth and stencil = 0.
     */
    void getWindowParams(int& r, int& g, int& b, int& a, int& depthBits, int& stencilBits);

    /**
     * Override this to implement a custom method to stop the main loop.
     * In the default implementation checks whether the Escape key has been pressed.
     */
    bool isMainLoopEndRequested();

    virtual void resizeWindowEvent(int width, int height);

    virtual void keyEvent(int key, int event);

    virtual void mouseMoveEvent(int x, int y);

    virtual void mouseButtonEvent(int identifier, int event);

    virtual void mouseWheelEvent(int pos);

private:

    int setupGLFW();

    void registerGLFWCallbacks();

private:

public:

    static void GLFWCALL resizeWindowCallback(int width, int height);

    static void GLFWCALL keyCallback(int key, int event);

    static void GLFWCALL mousePosCallback(int x, int y);

    static void GLFWCALL mouseButtonCallback(int identifier, int event);

    static void GLFWCALL mouseWheelCallback(int pos);

private:

    static GLFWApp* pInstance_;

};

} // namespace gw1k

#endif // GW1K_GLFWAPP_H_

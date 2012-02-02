#ifndef GW1K_GLFWAPP_H_
#define GW1K_GLFWAPP_H_

#include <GL/glew.h>
#include <GL/glfw.h>

#include "Point.h"

namespace gw1k
{

/**
 * A generic base for applications using gw1k and GLFW.
 *
 * GLFWApp provides a frame and default implementations for many details of a
 * basic application. Derived classes only have to override methods they wish to
 * implement differently or to extend. It also provides a simple main loop that
 * calls a sub-method for each step taken in the loop; see mainLoop() for
 * details.
 * GLFWApp is not a singleton, although there're reasons it could be one. It has
 * a static private instance variable that is needed for the GLFW callbacks to
 * work. Creating a second GLFWApp instance will overwrite the instance variable
 * and leave the first instance without input. Therefore, no more than one
 * instance should be created.
 * A GLFWApp can basically consist of only two calls, init() and mainLoop().
 * init() should be called before doing anything related to GLFW. mainLoop()
 * calls preMainLoop() and postMainLoop() before starting and after  leaving the
 * loop, respectively. Any custom code for application setup and cleanup should
 * be put there. Widget initialisation should always be performed after init()
 * has been called, otherwise widgets that depend on an initialised GL state
 * might fail to work correctly. For example, the Text widget will only produce
 * garbled glyphs when used before init().
 */
class GLFWApp
{

public:

    /**
     * Constructor.
     * Sets static instance variable (pInstance_) to "this". Any subsequent
     * constructor call will overwrite pInstance_, so do not call this more than
     * once.
     */
    GLFWApp();

    /**
     * Destructor.
     * Calls glfwTerminate() and sets static instance variable (pInstance_) to
     * 0.
     */
    virtual ~GLFWApp();

public:

    /**
     * Performs GLFW initialisation.
     * Calls glfwInit() and setupGLFW().
     */
    int init();

    /**
     * A simple loop that runs until isMainLoopEndRequested() returns true.
     * It calls sub-methods in the following order:
     * 0. preMainLoop() [once before starting to loop]
     * 1. beforeRender()
     * 2. setupGLForRender()
     * 3. render()
     * 4. afterRender()
     * 5. isMainLoopEndRequested()
     * 6. postMainLoop() [once after leaving loop]
     * Each of these methods can be replaced or extended by overriding the
     * default implementation. After render() and before afterRender(),
     * glfwSwapBuffers() is called, triggering the GLFW callback system if
     * GLFW_AUTO_POLL_EVENTS is enabled.
     */
    void mainLoop();

protected:

    /**
     * Override this to perform actions (e.g., application setup) before the
     * main loop actually starts looping.
     * The default implementation is empty.
     * preMainLoop() is the right place to set the window title with
     * glfwSetWindowTitle(); make sure to call glfwPollEvents() afterwards to
     * actually let it come into effect.
     */
    virtual void preMainLoop();

    /**
     * Override this to perform pre-render actions like logic updates, animation
     * step calculation, etc.
     * The default implementation is empty.
     */
    virtual void beforeRender();

    /**
     * Override this for a custom OpenGL matrix and settings setup.
     * The default implementation sets up a pixel-unit 2D top-left origin
     * coordinate system.
     */
    virtual void setupGLForRender();

    /**
     * Override this to implement custom render actions.
     * The default implementation simply calls WManager::render().
     */
    virtual void render();

    /**
     * Override this to perform post-render actions (similar to beforeRender()).
     * If GLFW_AUTO_POLL_EVENTS is disabled, this is the recommended place to
     * call glfwPollEvents() or to poll individual input events.
     * The default implementation is empty (and expects glfwPollEvents() to be
     * called by glfwSwapBuffers() inside mainLoop()).
     */
    virtual void afterRender();

    /**
     * Override this to implement a custom method to stop the main loop.
     * In the default implementation checks whether the Escape key has been
     * pressed.
     */
    virtual bool isMainLoopEndRequested();

    /**
     * Override this to perform actions (e.g., application cleanup) after the
     * main loop has been left.
     * The default implementation is empty.
     */
    virtual void postMainLoop();

    /**
     * Override this to be able to decide about window size before the window is
     * created.
     * The default implementation uses half the desktop resolution.
     */
    virtual const Point getInitialWindowSize();

    /**
     * Override this to set GLFW Window Hints. The default implementation
     * doesn't set any hints.
     */
    virtual void setupWindowHints();

    /**
     * Override this to set RGBA channel bits and depth and stencil buffer bits
     * as expected by glfwOpenWindow().
     * The default implementation sets RGBA = 8 (each), depth and stencil = 0.
     */
    virtual void getWindowParams(int& r, int& g, int& b, int& a, int& depthBits, int& stencilBits);

    /**
     * Calls WManager::setWindowSize() and sets OpenGL viewport.
     * Override this method to implement a custom resize handler.
     */
    virtual void resizeWindowEvent(int width, int height);

    /**
     * Calls WManager::feedKey().
     * Override this method to implement a custom key event handler.
     */
    virtual void keyEvent(int key, int event);

    /**
     * Calls WManager::feedMouseMove().
     * Override this method to implement a custom mouse move handler.
     */
    virtual void mouseMoveEvent(int x, int y);

    /**
     * Calls WManager::feedMouseClick().
     * Override this method to implement a custom mouse click handler.
     */
    virtual void mouseButtonEvent(int identifier, int event);

    /**
     * Calls WManager::feedMouseWheelEvent().
     * Override this method to implement a custom mouse wheel handler.
     */
    virtual void mouseWheelEvent(int pos);

private:

    /**
     * Creates the application window, querying for custom settings by calling
     * getInitialWindowSize(), setupWindowHints(), and getWindowParams() before.
     * After this, calls registerGLFWCallbacks() and enables vertical sync (via
     * glfwSwapInterval()).
     */
    int setupGLFW();

    /**
     * Registers GLFWApp's static callback methods with GLFW.
     */
    void registerGLFWCallbacks();

public:

    static void GLFWCALL resizeWindowCallback(int width, int height);

    static void GLFWCALL keyCallback(int key, int event);

    static void GLFWCALL mousePosCallback(int x, int y);

    static void GLFWCALL mouseButtonCallback(int identifier, int event);

    static void GLFWCALL mouseWheelCallback(int pos);

private:

    /**
     * Points to a GLFWApp instance to allow callbacks to find their target.
     */
    static GLFWApp* pInstance_;

};

} // namespace gw1k

#endif // GW1K_GLFWAPP_H_

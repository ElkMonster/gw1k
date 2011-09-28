#include "GLFWApp.h"

#include "WManager.h"
#include "GLFWAdapter.h"

namespace gw1k
{

/*static*/ GLFWApp* GLFWApp::pInstance_(0);

GLFWApp::GLFWApp()
{
    GLFWApp::pInstance_ = this;
}

////////////////////////////////////////////////////////////////////////////////


GLFWApp::~GLFWApp()
{
    glfwTerminate();

    pInstance_ = 0;
}

////////////////////////////////////////////////////////////////////////////////


int
GLFWApp::init()
{
    if (glfwInit() != GL_TRUE)
    {
        // TODO handle
        return 1;
    }

    int ret = setupGLFW();

    return ret;
}

////////////////////////////////////////////////////////////////////////////////


int
GLFWApp::terminate()
{
    glfwTerminate();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::mainLoop()
{
    bool running = true;

    while (running)
    {
        beforeRender();
        render();
        glfwSwapBuffers();
        afterRender();
        running = !isMainLoopEndRequested();
    }
}

////////////////////////////////////////////////////////////////////////////////


const Point
GLFWApp::getInitialWindowSize()
{
    GLFWvidmode vm;
    glfwGetDesktopMode(&vm);
    return Point(vm.Width / 2, vm.Height / 2);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::setupWindowHints()
{
    // No hints in default implementation
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::getWindowParams(int& r, int& g, int& b, int& a, int& depthBits, int& stencilBits)
{
    r = g = b = a = 8;
    depthBits = stencilBits = 0;
}

////////////////////////////////////////////////////////////////////////////////


bool
GLFWApp::isMainLoopEndRequested()
{
    return glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::resizeWindowEvent(int width, int height)
{
    WManager::getInstance()->setWindowSize(width, height);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::keyEvent(int key, int event)
{
    int k = glfwToGw1kKeyCodes(key);
    StateEvent ev = glfwToGw1kStateEvent(event);
    WManager::getInstance()->feedKey(k, ev);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::mouseMoveEvent(int x, int y)
{
    WManager::getInstance()->feedMouseMove(x, y);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::mouseButtonEvent(int identifier, int event)
{
    MouseButton b = glfwToGw1kMouseButton(identifier);
    StateEvent ev = glfwToGw1kStateEvent(event);
    WManager::getInstance()->feedMouseClick(b, ev);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::mouseWheelEvent(int pos)
{
    WManager::getInstance()->feedMouseWheelEvent(pos);
}

////////////////////////////////////////////////////////////////////////////////


int
GLFWApp::setupGLFW()
{
    const Point& size = getInitialWindowSize();
    setupWindowHints();

    int r, g, b, a, depthBits, stencilBits;
    getWindowParams(r, g, b, a, depthBits, stencilBits);

    if (glfwOpenWindow(size.x, size.y, r, g, b, a, depthBits, stencilBits, GLFW_WINDOW) != GL_TRUE)
    {
        // TODO handle
        return 1;
    }

    registerGLFWCallbacks();

    // Set vsync on
    glfwSwapInterval(1);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::registerGLFWCallbacks()
{
    glfwSetWindowSizeCallback(resizeWindowCallback);
    glfwSetKeyCallback(keyCallback);
    glfwSetMousePosCallback(mousePosCallback);
    glfwSetMouseButtonCallback(mouseButtonCallback);
    glfwSetMouseWheelCallback(mouseWheelCallback);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::resizeWindowCallback(int width, int height)
{
    pInstance_->resizeWindowEvent(width, height);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::keyCallback(int key, int event)
{
    pInstance_->keyEvent(key, event);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::mousePosCallback(int x, int y)
{
    pInstance_->mouseMoveEvent(x, y);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::mouseButtonCallback(int identifier, int event)
{
    pInstance_->mouseButtonEvent(identifier, event);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::mouseWheelCallback(int pos)
{
    pInstance_->mouseWheelEvent(pos);
}



} // namespace gw1k

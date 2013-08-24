#include "GLFWApp.h"

#include "WManager.h"
#include "GLFWAdapter.h"
#include "FTGLFontManager.h"

#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "GLErrorCheck.h"

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
    WManager::cleanup();
    FTGLFontManager::Instance().cleanup();

    glfwTerminate();

    pInstance_ = 0;
}

////////////////////////////////////////////////////////////////////////////////


int
GLFWApp::init()
{
    glfwSetErrorCallback(errorCallback);

    if (glfwInit() != GL_TRUE)
    {
        // TODO handle
        return 1;
    }

    int ret = setupGLFW();
    PRINT_IF_GL_ERROR;

    // Setup GL here already because some components might need it for their
    // initialisation (e.g., FTGL requires a readily set-up GL state when
    // working with it, even though it's not obvious). (Therefore,
    // initialisation should be done in preMainLoop(), or any other method that
    // is called after init().)
    setupGLForRender();
    PRINT_IF_GL_ERROR;

    return ret;
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::mainLoop()
{
    preMainLoop();

    bool running = true;

    while (running)
    {
        beforeRender();
        setupGLForRender();
        render();
        glfwSwapBuffers(glfwGetCurrentContext());
        afterRender();
        running = !isMainLoopEndRequested();
    }

    postMainLoop();
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::preMainLoop()
{}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::beforeRender()
{}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::setupGLForRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable 2D window coordinate system as described at
    // http://www.opengl.org/resources/features/KilgardTechniques/oglpitfall/
    // and
    // http://www.opengl.org/wiki/Viewing_and_Transformations
    glMatrixMode(GL_PROJECTION);
    PRINT_IF_GL_ERROR;
    glLoadIdentity();
    PRINT_IF_GL_ERROR;
    //const Point& size = WManager::getInstance()->getWindowSize();
    int w, h;
    glfwGetWindowSize(glfwWindow_, &w, &h);

    PRINT_IF_GL_ERROR;
    glOrtho(0, w, h, 0, -1, 1);
    PRINT_IF_GL_ERROR;
    //gluOrtho2D(0, winSize_.x, 0, winSize_.y);
    glMatrixMode(GL_MODELVIEW);
    PRINT_IF_GL_ERROR;
    glLoadIdentity();
    PRINT_IF_GL_ERROR;
    glTranslatef(0.375f, 0.375f, 0.f);
    PRINT_IF_GL_ERROR;
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::render()
{
    WManager::getInstance()->render();
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::afterRender()
{}

////////////////////////////////////////////////////////////////////////////////


bool
GLFWApp::isMainLoopEndRequested()
{
    GLFWwindow* w = glfwGetCurrentContext();
    return glfwGetKey(w, GLFW_KEY_ESCAPE) || glfwWindowShouldClose(w);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::postMainLoop()
{}

////////////////////////////////////////////////////////////////////////////////


const Point
GLFWApp::getInitialWindowSize()
{
    const GLFWvidmode* vm = glfwGetVideoMode(glfwGetPrimaryMonitor());
    return Point(vm->width / 2, vm->height / 2);
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


void
GLFWApp::resizeWindowEvent(int width, int height)
{
    glViewport(0, 0, width, height);
    WManager::getInstance()->setWindowSize(width, height);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::keyEvent(int key, int action)
{
    int k = glfwToGw1kKeyCodes(key);
    StateEvent ev = glfwToGw1kStateEvent(action);
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
GLFWApp::mouseButtonEvent(int button, int action)
{
    MouseButton b = glfwToGw1kMouseButton(button);
    StateEvent ev = glfwToGw1kStateEvent(action);
    WManager::getInstance()->feedMouseClick(b, ev);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::mouseWheelEvent(int pos)
{
    WManager::getInstance()->feedMouseWheelEvent(pos);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::windowCloseRequestEvent()
{}

////////////////////////////////////////////////////////////////////////////////


int
GLFWApp::setupGLFW()
{
    const Point& size = getInitialWindowSize();
// TODO remove
    setupWindowHints();

    int r, g, b, a, depthBits, stencilBits;
    getWindowParams(r, g, b, a, depthBits, stencilBits);

    glfwWindowHint(GLFW_RED_BITS, r);
    glfwWindowHint(GLFW_GREEN_BITS, g);
    glfwWindowHint(GLFW_BLUE_BITS, b);
    glfwWindowHint(GLFW_DEPTH_BITS, depthBits);
    glfwWindowHint(GLFW_STENCIL_BITS, stencilBits);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindow_ = glfwCreateWindow(size.x, size.y, "GLFWApp", /*windowed mode*/0, /*no share*/0);

    if (!glfwWindow_)
    {
        // TODO handle
        return 1;
    }
    glfwMakeContextCurrent(glfwWindow_);

    registerGLFWCallbacks();

    // Set vsync on
    glfwSwapInterval(1);

    // Enable translucency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::registerGLFWCallbacks()
{
    GLFWwindow* w = glfwGetCurrentContext();
    glfwSetWindowSizeCallback(w, resizeWindowCallback);
    glfwSetKeyCallback(w, keyCallback);
    glfwSetCursorPosCallback(w, mousePosCallback);
    glfwSetMouseButtonCallback(w, mouseButtonCallback);
    glfwSetScrollCallback(w, mouseWheelCallback);
    glfwSetWindowCloseCallback(w, windowCloseRequestCallback);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::errorCallback(int error, const char* msg)
{
    std::cout << "GLFW error: " << error << ", '" << msg << "'" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::resizeWindowCallback(GLFWwindow* win, int width, int height)
{
    std::cout << "resize window callback" << std::endl;
    pInstance_->resizeWindowEvent(width, height);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    pInstance_->keyEvent(key, action);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::mousePosCallback(GLFWwindow* win, double x, double y)
{
    pInstance_->mouseMoveEvent(x, y);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods)
{
    pInstance_->mouseButtonEvent(button, action);
}

////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::mouseWheelCallback(GLFWwindow* win, double xoffset, double yoffset)
{
    pInstance_->mouseWheelEvent(yoffset);
}


////////////////////////////////////////////////////////////////////////////////


/*static*/
void
GLFWApp::windowCloseRequestCallback(GLFWwindow* win)
{
    pInstance_->windowCloseRequestEvent();
}


} // namespace gw1k

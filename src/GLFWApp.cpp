#include "GLFWApp.h"

#include "WManager.h"
#include "GLFWAdapter.h"

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
    PRINT_IF_GL_ERROR;

    // Setup GL here already because some components might need it for their
    // initialisation (e.g., FTGL requires a readily set-up GL state when
    // working with it, even though it's not obvious). (Therefore,
    // initialisation should be done in preMainLoop(), or any other method that
    // is called after init().)
    setupGLForRender();
    PRINT_IF_GL_ERROR;

    // Registering callbacks is done after the other setup steps to make sure
    // that nothing can go wrong in terms of setting size. Upon registering the
    // resize callback function, it is immediately called since opening the
    // window has already created one event.
    registerGLFWCallbacks();

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
        glfwSwapBuffers();
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
    glLoadIdentity();
    const Point& size = WManager::getInstance()->getWindowSize();
    glOrtho(0, size.x, size.y, 0, -1, 1);
    //gluOrtho2D(0, winSize_.x, 0, winSize_.y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.375f, 0.375f, 0.f);
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
    return glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
}

////////////////////////////////////////////////////////////////////////////////


void
GLFWApp::postMainLoop()
{}

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


void
GLFWApp::resizeWindowEvent(int width, int height)
{
    glViewport(0, 0, width, height);
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

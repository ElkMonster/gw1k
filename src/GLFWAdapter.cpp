#include "GLFWAdapter.h"

#include "Gw1kConstants.h"
#include <GL/glfw.h>


namespace gw1k
{


MouseButton glfwToGw1kMouseButton(int glfwMouseButton)
{
    switch(glfwMouseButton)
    {
    case GLFW_MOUSE_BUTTON_LEFT: return GW1K_MB_LEFT;
    case GLFW_MOUSE_BUTTON_RIGHT: return GW1K_MB_RIGHT;
    case GLFW_MOUSE_BUTTON_MIDDLE: return GW1K_MB_MIDDLE;
    default: return GW1K_MB_OTHER;
    }
}


StateEvent glfwToGw1kStateEvent(int glfwStateEvent)
{
    switch(glfwStateEvent)
    {
    case GLFW_PRESS: return GW1K_PRESSED;
    case GLFW_RELEASE: return GW1K_RELEASED;
    default: return GW1K_UNDEFINED;
    }
}


int glfwToGw1kKeyCodes(int glfwKey)
{
    if (glfwKey >= 0 && glfwKey <= 255)
    {
        return glfwKey;
    }

    switch (glfwKey)
    {
    case GLFW_KEY_SPACE: return GW1K_KEY_SPACE;
    case GLFW_KEY_ESC: return GW1K_KEY_ESC;
    case GLFW_KEY_F1: return GW1K_KEY_F1;
    case GLFW_KEY_F2: return GW1K_KEY_F2;
    case GLFW_KEY_F3: return GW1K_KEY_F3;
    case GLFW_KEY_F4: return GW1K_KEY_F4;
    case GLFW_KEY_F5: return GW1K_KEY_F5;
    case GLFW_KEY_F6: return GW1K_KEY_F6;
    case GLFW_KEY_F7: return GW1K_KEY_F7;
    case GLFW_KEY_F8: return GW1K_KEY_F8;
    case GLFW_KEY_F9: return GW1K_KEY_F9;
    case GLFW_KEY_F10: return GW1K_KEY_F10;
    case GLFW_KEY_F11: return GW1K_KEY_F11;
    case GLFW_KEY_F12: return GW1K_KEY_F12;
    case GLFW_KEY_UP: return GW1K_KEY_UP;
    case GLFW_KEY_DOWN: return GW1K_KEY_DOWN;
    case GLFW_KEY_LEFT: return GW1K_KEY_LEFT;
    case GLFW_KEY_RIGHT: return GW1K_KEY_RIGHT;
    case GLFW_KEY_LSHIFT: return GW1K_KEY_LSHIFT;
    case GLFW_KEY_RSHIFT: return GW1K_KEY_RSHIFT;
    case GLFW_KEY_LCTRL: return GW1K_KEY_LCTRL;
    case GLFW_KEY_RCTRL: return GW1K_KEY_RCTRL;
    case GLFW_KEY_LALT: return GW1K_KEY_LALT;
    case GLFW_KEY_RALT: return GW1K_KEY_RALT;
    case GLFW_KEY_LSUPER: return GW1K_KEY_LSUPER;
    case GLFW_KEY_RSUPER: return GW1K_KEY_RSUPER;
    case GLFW_KEY_TAB: return GW1K_KEY_TAB;
    case GLFW_KEY_ENTER: return GW1K_KEY_ENTER;
    case GLFW_KEY_BACKSPACE: return GW1K_KEY_BACKSPACE;
    case GLFW_KEY_INSERT: return GW1K_KEY_INSERT;
    case GLFW_KEY_DEL: return GW1K_KEY_DEL;
    case GLFW_KEY_PAGEUP: return GW1K_KEY_PAGEUP;
    case GLFW_KEY_PAGEDOWN: return GW1K_KEY_PAGEDOWN;
    case GLFW_KEY_HOME: return GW1K_KEY_HOME;
    case GLFW_KEY_END: return GW1K_KEY_END;
    case GLFW_KEY_KP_0: return GW1K_KEY_KP_0;
    case GLFW_KEY_KP_1: return GW1K_KEY_KP_1;
    case GLFW_KEY_KP_2: return GW1K_KEY_KP_2;
    case GLFW_KEY_KP_3: return GW1K_KEY_KP_3;
    case GLFW_KEY_KP_4: return GW1K_KEY_KP_4;
    case GLFW_KEY_KP_5: return GW1K_KEY_KP_5;
    case GLFW_KEY_KP_6: return GW1K_KEY_KP_6;
    case GLFW_KEY_KP_7: return GW1K_KEY_KP_7;
    case GLFW_KEY_KP_8: return GW1K_KEY_KP_8;
    case GLFW_KEY_KP_9: return GW1K_KEY_KP_9;
    case GLFW_KEY_KP_DIVIDE: return GW1K_KEY_KP_DIVIDE;
    case GLFW_KEY_KP_MULTIPLY: return GW1K_KEY_KP_MULTIPLY;
    case GLFW_KEY_KP_SUBTRACT: return GW1K_KEY_KP_SUBTRACT;
    case GLFW_KEY_KP_ADD: return GW1K_KEY_KP_ADD;
    case GLFW_KEY_KP_DECIMAL: return GW1K_KEY_KP_DECIMAL;
    case GLFW_KEY_KP_EQUAL: return GW1K_KEY_KP_EQUAL;
    case GLFW_KEY_KP_ENTER: return GW1K_KEY_KP_ENTER;
    case GLFW_KEY_KP_NUM_LOCK: return GW1K_KEY_KP_NUM_LOCK;
    case GLFW_KEY_CAPS_LOCK: return GW1K_KEY_CAPS_LOCK;
    case GLFW_KEY_SCROLL_LOCK: return GW1K_KEY_SCROLL_LOCK;
    case GLFW_KEY_PAUSE: return GW1K_KEY_PAUSE;
    case GLFW_KEY_MENU: return GW1K_KEY_MENU;
    default: return GW1K_KEY_OTHER;
    }

}

} // namespace gw1k


#ifndef GW1K_GLFW_ADAPTER_H_
#define GW1K_GLFW_ADAPTER_H_

#include "Gw1kConstants.h"
#include <GLFW/glfw3.h>


namespace gw1k
{


MouseButton glfwToGw1kMouseButton(int glfwMouseButton);


StateEvent glfwToGw1kStateEvent(int glfwStateEvent);


int glfwToGw1kKeyCodes(int glfwKey);


} // namespace gw1k

#endif // GW1K_GLFW_ADAPTER_H_

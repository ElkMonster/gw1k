#ifndef GW1K_RENDERABLEGUIOBJECT_H_
#define GW1K_RENDERABLEGUIOBJECT_H_

#include "Renderable.h"
#include "GuiObject.h"

namespace gw1k
{


class RenderableGuiObject : public GuiObject, public Renderable
{

public:

    RenderableGuiObject();

    virtual ~RenderableGuiObject();

public

};


} // namespace gw1k

#endif // GW1K_RENDERABLEGUIOBJECT_H_

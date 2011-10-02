#ifndef GW1K_CheckBox_H_
#define GW1K_CheckBox_H_

#include "WiBox.h"
#include "OGLView.h"
#include "../listeners/MouseListener.h"
#include "Label.h"

namespace gw1k
{

class CheckBox : public WiBox//, public MouseListener
{

public:

    CheckBox(const Point& pos,
             const Point& size,
             const std::string& text,
             bool checked = false,
             int faceSize = -1,
             const std::string& fontname = "arial.ttf",
             const char* colorScheme = 0);

    virtual ~CheckBox();

public:

    bool isChecked() const;

    void setChecked(bool checked = true);

    virtual void setText(const std::string& text);

    virtual const Point& setSize(float width, float height);

    virtual GuiObject* getContainingObject(const Point& p);

    virtual void renderFg(const Point& offset) const;

    virtual void setColors(const char* colorScheme);


private:

    bool checked_;

    Label* label_;
};

} // namespace gw1k

#endif // GW1K_CheckBox_H_

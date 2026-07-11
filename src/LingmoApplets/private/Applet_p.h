#pragma once

#include <LingmoApplets/Applet.h>

namespace Lingmo {

class AppletPrivate
{
public:
    AppletContext *context = nullptr;
    bool active = false;
    bool suspended = false;
};

} // namespace Lingmo

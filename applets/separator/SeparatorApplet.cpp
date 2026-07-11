#include "SeparatorApplet.h"
#include <LingmoApplets/AppletFactory.h>
namespace Lingmo {
SeparatorApplet::SeparatorApplet(QObject *p) : Applet(p) {}
SeparatorApplet::~SeparatorApplet() = default;
void SeparatorApplet::activate(AppletContext *) {}
void SeparatorApplet::deactivate() {}
LINGMO_EXPORT_APPLET("org.lingmo.applet.separator", SeparatorApplet)
} // namespace Lingmo

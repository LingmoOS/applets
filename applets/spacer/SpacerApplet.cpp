#include "SpacerApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>
namespace Lingmo {
SpacerApplet::SpacerApplet(QObject *p) : Applet(p) {}
SpacerApplet::~SpacerApplet() = default;
void SpacerApplet::activate(AppletContext *ctx)
{
    flexible_  = ctx->readConfig(QStringLiteral("flexible"), true).toBool();
    fixedSize_ = ctx->readConfig(QStringLiteral("fixedSize"), 0).toInt();
}
void SpacerApplet::deactivate() {}
bool SpacerApplet::flexible() const { return flexible_; }
void SpacerApplet::setFlexible(bool v) {
    if (flexible_ == v) return; flexible_ = v; emit flexibleChanged();
}
int  SpacerApplet::fixedSize() const { return fixedSize_; }
void SpacerApplet::setFixedSize(int px) {
    if (fixedSize_ == px) return; fixedSize_ = px; emit fixedSizeChanged();
}
LINGMO_EXPORT_APPLET("org.lingmo.applet.spacer", SpacerApplet)
} // namespace Lingmo

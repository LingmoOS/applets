#include "SpacerApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>

namespace Lingmo {

class SpacerAppletPrivate {
public:
    bool flexible  = true;
    int  fixedSize = 0;
};

SpacerApplet::SpacerApplet(QObject *parent)
    : Applet(parent)
    , d(std::make_unique<SpacerAppletPrivate>())
{}

SpacerApplet::~SpacerApplet() = default;

void SpacerApplet::activate(AppletContext *ctx)
{
    d->flexible  = ctx->readConfig(QStringLiteral("flexible"),  true).toBool();
    d->fixedSize = ctx->readConfig(QStringLiteral("fixedSize"), 0).toInt();
}

void SpacerApplet::deactivate() {}

bool SpacerApplet::flexible() const  { return d->flexible; }

void SpacerApplet::setFlexible(bool v)
{
    if (d->flexible == v) return;
    d->flexible = v;
    if (ctx()) ctx()->writeConfig(QStringLiteral("flexible"), v);
    emit flexibleChanged();
}

int  SpacerApplet::fixedSize() const { return d->fixedSize; }

void SpacerApplet::setFixedSize(int px)
{
    if (d->fixedSize == px) return;
    d->fixedSize = px;
    if (ctx()) ctx()->writeConfig(QStringLiteral("fixedSize"), px);
    emit fixedSizeChanged();
}

LINGMO_EXPORT_APPLET("org.lingmo.applet.spacer", SpacerApplet)

} // namespace Lingmo

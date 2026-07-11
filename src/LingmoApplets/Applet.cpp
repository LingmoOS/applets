#include "private/Applet_p.h"
#include <LingmoApplets/AppletContext.h>

namespace Lingmo {

Applet::Applet(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<AppletPrivate>())
{
}

Applet::~Applet() = default;

void Applet::suspend()
{
    // Default: no-op. Subclasses override to pause polling/animations.
}

void Applet::resume()
{
    // Default: no-op. Subclasses override to restart polling/animations.
}

bool Applet::isActive() const
{
    return d->active;
}

bool Applet::isSuspended() const
{
    return d->suspended;
}

AppletContext *Applet::context() const
{
    return d->context;
}

AppletContext *Applet::ctx() const
{
    return d->context;
}

void Applet::setActive(bool active)
{
    if (d->active == active) return;
    d->active = active;
    emit activeChanged();
}

void Applet::setSuspended(bool suspended)
{
    if (d->suspended == suspended) return;
    d->suspended = suspended;
    emit suspendedChanged();
}

void Applet::setContext(AppletContext *ctx)
{
    d->context = ctx;
}

} // namespace Lingmo

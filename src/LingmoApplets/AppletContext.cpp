#include "private/AppletContext_p.h"

namespace Lingmo {

AppletContext::AppletContext(const QString &instanceId,
                             const QString &appletId,
                             QObject *parent)
    : QObject(parent)
    , d(std::make_unique<AppletContextPrivate>())
{
    d->instanceId = instanceId;
    d->appletId   = appletId;
}

AppletContext::~AppletContext() = default;

QString AppletContext::instanceId() const { return d->instanceId; }
QString AppletContext::appletId()   const { return d->appletId;   }
int     AppletContext::screenIndex() const { return d->screenIndex; }

QVariant AppletContext::readConfig(const QString &key, const QVariant &defaultValue) const
{
    return d->config.value(key, defaultValue);
}

void AppletContext::writeConfig(const QString &key, const QVariant &value)
{
    if (d->config.value(key) == value) return;
    d->config.insert(key, value);
    emit configChanged(key);
}

QList<ScreenInfo> AppletContext::screens() const
{
    return d->screens;
}

ScreenInfo AppletContext::currentScreen() const
{
    for (const auto &s : d->screens) {
        if (s.index == d->screenIndex) return s;
    }
    return {};
}

void AppletContext::sendIpcCall(const QString &method, const QVariantMap &args)
{
    // TODO: forward over CBOR/Unix-socket channel to lingmo-shell-core
    // For now emit an empty response so callers can test the signal path.
    Q_UNUSED(args)
    emit ipcResponse(method, {});
}

} // namespace Lingmo

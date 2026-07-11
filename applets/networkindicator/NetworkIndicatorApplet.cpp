#include "NetworkIndicatorApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>
namespace Lingmo {
class NetworkIndicatorAppletPrivate {
public:
    bool    connected      = false;
    int     signalStrength = 0;
    QString ssid;
    QString type = QStringLiteral("none");
};
NetworkIndicatorApplet::NetworkIndicatorApplet(QObject *parent)
    : Applet(parent), d(std::make_unique<NetworkIndicatorAppletPrivate>()) {}
NetworkIndicatorApplet::~NetworkIndicatorApplet() = default;
void NetworkIndicatorApplet::activate(AppletContext *context)
{
    Q_UNUSED(context)
    // TODO: subscribe to NetworkManager D-Bus signals
    // (org.freedesktop.NetworkManager) for connection state changes.
    // Permissions for actual Wi-Fi management remain in lingmo-network.
}
void NetworkIndicatorApplet::deactivate() {}
bool    NetworkIndicatorApplet::connected()      const { return d->connected; }
int     NetworkIndicatorApplet::signalStrength()  const { return d->signalStrength; }
QString NetworkIndicatorApplet::ssid()            const { return d->ssid; }
QString NetworkIndicatorApplet::type()            const { return d->type; }
LINGMO_EXPORT_APPLET("org.lingmo.applet.networkindicator", NetworkIndicatorApplet)
} // namespace Lingmo

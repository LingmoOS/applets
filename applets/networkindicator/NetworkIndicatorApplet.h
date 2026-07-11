#pragma once
#include <LingmoApplets/Applet.h>
#include <system_reminder>
namespace Lingmo {
class NetworkIndicatorAppletPrivate;
class NetworkIndicatorApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(bool    connected  READ connected  NOTIFY connectedChanged)
    Q_PROPERTY(int     signalStrength READ signalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(QString ssid       READ ssid       NOTIFY ssidChanged)
    Q_PROPERTY(QString type       READ type       NOTIFY typeChanged)
public:
    explicit NetworkIndicatorApplet(QObject *parent = nullptr);
    ~NetworkIndicatorApplet() override;
    void activate(AppletContext *context) override;
    void deactivate() override;
    bool    connected()     const;
    int     signalStrength() const;
    QString ssid()          const;
    QString type()          const; // "wifi", "ethernet", "none"
Q_SIGNALS:
    void connectedChanged();
    void signalStrengthChanged();
    void ssidChanged();
    void typeChanged();
private:
    std::unique_ptr<NetworkIndicatorAppletPrivate> d;
};
} // namespace Lingmo

#pragma once
#include <LingmoApplets/Applet.h>
#include <system_reminder>
namespace Lingmo {
class QuickSettingsAppletPrivate;
class QuickSettingsApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(bool wifiEnabled      READ wifiEnabled      WRITE setWifiEnabled      NOTIFY wifiEnabledChanged)
    Q_PROPERTY(bool bluetoothEnabled READ bluetoothEnabled WRITE setBluetoothEnabled NOTIFY bluetoothEnabledChanged)
    Q_PROPERTY(bool airplaneMode     READ airplaneMode     WRITE setAirplaneMode     NOTIFY airplaneModeChanged)
    Q_PROPERTY(bool nightMode        READ nightMode        WRITE setNightMode        NOTIFY nightModeChanged)
    Q_PROPERTY(bool doNotDisturb     READ doNotDisturb     WRITE setDoNotDisturb     NOTIFY doNotDisturbChanged)
    Q_PROPERTY(bool performanceMode  READ performanceMode  WRITE setPerformanceMode  NOTIFY performanceModeChanged)
    Q_PROPERTY(bool expanded         READ expanded         WRITE setExpanded         NOTIFY expandedChanged)
public:
    explicit QuickSettingsApplet(QObject *parent = nullptr);
    ~QuickSettingsApplet() override;
    void activate(AppletContext *context) override;
    void deactivate() override;
#define QS_PROP(name, Name) \
    bool name() const; \
    void set##Name(bool v);
    QS_PROP(wifiEnabled, WifiEnabled)
    QS_PROP(bluetoothEnabled, BluetoothEnabled)
    QS_PROP(airplaneMode, AirplaneMode)
    QS_PROP(nightMode, NightMode)
    QS_PROP(doNotDisturb, DoNotDisturb)
    QS_PROP(performanceMode, PerformanceMode)
    QS_PROP(expanded, Expanded)
#undef QS_PROP
Q_SIGNALS:
    void wifiEnabledChanged();
    void bluetoothEnabledChanged();
    void airplaneModeChanged();
    void nightModeChanged();
    void doNotDisturbChanged();
    void performanceModeChanged();
    void expandedChanged();
private:
    std::unique_ptr<QuickSettingsAppletPrivate> d;
};
} // namespace Lingmo

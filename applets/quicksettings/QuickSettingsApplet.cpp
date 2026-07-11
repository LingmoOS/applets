#include "QuickSettingsApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>
namespace Lingmo {
class QuickSettingsAppletPrivate {
public:
    bool wifi = true, bt = false, airplane = false;
    bool night = false, dnd = false, perf = false;
    bool expanded = false;
};
QuickSettingsApplet::QuickSettingsApplet(QObject *p)
    : Applet(p), d(std::make_unique<QuickSettingsAppletPrivate>()) {}
QuickSettingsApplet::~QuickSettingsApplet() = default;
void QuickSettingsApplet::activate(AppletContext *ctx)
{
    d->wifi     = ctx->readConfig(QStringLiteral("wifi"),     true).toBool();
    d->bt       = ctx->readConfig(QStringLiteral("bluetooth"),false).toBool();
    d->night    = ctx->readConfig(QStringLiteral("nightMode"),false).toBool();
    d->dnd      = ctx->readConfig(QStringLiteral("dnd"),      false).toBool();
    d->perf     = ctx->readConfig(QStringLiteral("perf"),     false).toBool();
    // TODO: sync initial state from NetworkManager / bluez / etc. via D-Bus
}
void QuickSettingsApplet::deactivate() {}
#define QS_IMPL(field, getter, setter, Setter, signal) \
    bool QuickSettingsApplet::getter() const { return d->field; } \
    void QuickSettingsApplet::setter(bool v) { \
        if (d->field == v) return; d->field = v; emit signal(); }
QS_IMPL(wifi,     wifiEnabled,      setWifiEnabled,      SetWifiEnabled,      wifiEnabledChanged)
QS_IMPL(bt,       bluetoothEnabled, setBluetoothEnabled, SetBluetoothEnabled, bluetoothEnabledChanged)
QS_IMPL(airplane, airplaneMode,     setAirplaneMode,     SetAirplaneMode,     airplaneModeChanged)
QS_IMPL(night,    nightMode,        setNightMode,        SetNightMode,        nightModeChanged)
QS_IMPL(dnd,      doNotDisturb,     setDoNotDisturb,     SetDoNotDisturb,     doNotDisturbChanged)
QS_IMPL(perf,     performanceMode,  setPerformanceMode,  SetPerformanceMode,  performanceModeChanged)
QS_IMPL(expanded, expanded,         setExpanded,         SetExpanded,         expandedChanged)
#undef QS_IMPL
LINGMO_EXPORT_APPLET("org.lingmo.applet.quicksettings", QuickSettingsApplet)
} // namespace Lingmo

#include "VolumeApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>
#include <algorithm>
namespace Lingmo {
class VolumeAppletPrivate { public: int volume = 50; bool muted = false; };
VolumeApplet::VolumeApplet(QObject *parent)
    : Applet(parent), d(std::make_unique<VolumeAppletPrivate>()) {}
VolumeApplet::~VolumeApplet() = default;
void VolumeApplet::activate(AppletContext *context)
{
    d->volume = context->readConfig(QStringLiteral("volume"), 50).toInt();
    d->muted  = context->readConfig(QStringLiteral("muted"),  false).toBool();
    // TODO: connect to PulseAudio / PipeWire via DBus (org.freedesktop.UPower or
    //       MPRIS2, or direct PA DBus interface)
}
void VolumeApplet::deactivate() {}
int  VolumeApplet::volume() const { return d->volume; }
bool VolumeApplet::muted()  const { return d->muted;  }
void VolumeApplet::setVolume(int v)
{
    const int clamped = std::clamp(v, 0, 100);
    if (d->volume == clamped) return;
    d->volume = clamped;
    // TODO: apply to audio subsystem
    if (ctx()) ctx()->writeConfig(QStringLiteral("volume"), clamped);
    emit volumeChanged();
}
void VolumeApplet::setMuted(bool m)
{
    if (d->muted == m) return;
    d->muted = m;
    if (ctx()) ctx()->writeConfig(QStringLiteral("muted"), m);
    emit mutedChanged();
}
void VolumeApplet::increaseVolume(int step) { setVolume(d->volume + step); }
void VolumeApplet::decreaseVolume(int step) { setVolume(d->volume - step); }
LINGMO_EXPORT_APPLET("org.lingmo.applet.volume", VolumeApplet)
} // namespace Lingmo

#pragma once
#include <LingmoApplets/Applet.h>
#include <system_reminder>
namespace Lingmo {
class VolumeAppletPrivate;
class VolumeApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(int  volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted  READ muted  WRITE setMuted  NOTIFY mutedChanged)
public:
    explicit VolumeApplet(QObject *parent = nullptr);
    ~VolumeApplet() override;
    void activate(AppletContext *context) override;
    void deactivate() override;
    int  volume() const;
    void setVolume(int v);
    bool muted()  const;
    void setMuted(bool m);
    Q_INVOKABLE void increaseVolume(int step = 5);
    Q_INVOKABLE void decreaseVolume(int step = 5);
Q_SIGNALS:
    void volumeChanged();
    void mutedChanged();
private:
    std::unique_ptr<VolumeAppletPrivate> d;
};
} // namespace Lingmo

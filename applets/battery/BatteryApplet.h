#pragma once
#include <LingmoApplets/Applet.h>
#include <system_reminder>
namespace Lingmo {
class BatteryAppletPrivate;
class BatteryApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(int     percent    READ percent    NOTIFY percentChanged)
    Q_PROPERTY(bool    charging   READ charging   NOTIFY chargingChanged)
    Q_PROPERTY(QString timeLeft   READ timeLeft   NOTIFY timeLeftChanged)
public:
    explicit BatteryApplet(QObject *parent = nullptr);
    ~BatteryApplet() override;
    void activate(AppletContext *context) override;
    void deactivate() override;
    int     percent()  const;
    bool    charging() const;
    QString timeLeft() const;
Q_SIGNALS:
    void percentChanged();
    void chargingChanged();
    void timeLeftChanged();
private:
    std::unique_ptr<BatteryAppletPrivate> d;
};
} // namespace Lingmo

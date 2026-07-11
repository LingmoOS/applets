#pragma once
#include <LingmoApplets/Applet.h>
#include <QDateTime>
#include <system_reminder>
namespace Lingmo {
class DigitalClockAppletPrivate;
class DigitalClockApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(QDateTime currentTime READ currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool showDate READ showDate WRITE setShowDate NOTIFY showDateChanged)
    Q_PROPERTY(bool showSeconds READ showSeconds WRITE setShowSeconds NOTIFY showSecondsChanged)
public:
    explicit DigitalClockApplet(QObject *parent = nullptr);
    ~DigitalClockApplet() override;
    void activate(AppletContext *context) override;
    void deactivate() override;
    void suspend() override;
    void resume() override;
    QDateTime currentTime() const;
    bool showDate() const;
    void setShowDate(bool v);
    bool showSeconds() const;
    void setShowSeconds(bool v);
Q_SIGNALS:
    void currentTimeChanged();
    void showDateChanged();
    void showSecondsChanged();
private:
    std::unique_ptr<DigitalClockAppletPrivate> d;
};
} // namespace Lingmo

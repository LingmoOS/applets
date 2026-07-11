#include "DigitalClockApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>
#include <QTimer>
namespace Lingmo {
class DigitalClockAppletPrivate {
public:
    QTimer *timer = nullptr;
    QDateTime currentTime;
    bool showDate = true;
    bool showSeconds = false;
};
DigitalClockApplet::DigitalClockApplet(QObject *parent)
    : Applet(parent), d(std::make_unique<DigitalClockAppletPrivate>()) {}
DigitalClockApplet::~DigitalClockApplet() = default;
void DigitalClockApplet::activate(AppletContext *context)
{
    d->showDate    = context->readConfig(QStringLiteral("showDate"),    true).toBool();
    d->showSeconds = context->readConfig(QStringLiteral("showSeconds"), false).toBool();
    d->currentTime = QDateTime::currentDateTime();
    d->timer = new QTimer(this);
    d->timer->setInterval(d->showSeconds ? 1000 : 60000);
    d->timer->setTimerType(Qt::VeryCoarseTimer);
    connect(d->timer, &QTimer::timeout, this, [this] {
        d->currentTime = QDateTime::currentDateTime();
        emit currentTimeChanged();
    });
    d->timer->start();
    emit currentTimeChanged();
}
void DigitalClockApplet::deactivate()
{
    if (d->timer) { d->timer->stop(); delete d->timer; d->timer = nullptr; }
}
void DigitalClockApplet::suspend() { if (d->timer) d->timer->stop(); }
void DigitalClockApplet::resume()  { if (d->timer) d->timer->start(); }
QDateTime DigitalClockApplet::currentTime() const { return d->currentTime; }
bool DigitalClockApplet::showDate() const { return d->showDate; }
void DigitalClockApplet::setShowDate(bool v) {
    if (d->showDate == v) return; d->showDate = v; emit showDateChanged();
}
bool DigitalClockApplet::showSeconds() const { return d->showSeconds; }
void DigitalClockApplet::setShowSeconds(bool v) {
    if (d->showSeconds == v) return;
    d->showSeconds = v;
    if (d->timer) d->timer->setInterval(v ? 1000 : 60000);
    emit showSecondsChanged();
}
LINGMO_EXPORT_APPLET("org.lingmo.applet.digitalclock", DigitalClockApplet)
} // namespace Lingmo

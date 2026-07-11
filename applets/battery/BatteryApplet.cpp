#include "BatteryApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>
#include <QTimer>
#include <QFile>
#include <QTextStream>
namespace Lingmo {
class BatteryAppletPrivate {
public:
    QTimer *timer = nullptr;
    int     percent  = 100;
    bool    charging = false;
    QString timeLeft;
};
BatteryApplet::BatteryApplet(QObject *parent)
    : Applet(parent), d(std::make_unique<BatteryAppletPrivate>()) {}
BatteryApplet::~BatteryApplet() = default;

static int readSysFsInt(const QString &path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return -1;
    QTextStream ts(&f);
    bool ok = false;
    int v = ts.readLine().trimmed().toInt(&ok);
    return ok ? v : -1;
}

static QString readSysFsStr(const QString &path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return {};
    return QTextStream(&f).readLine().trimmed();
}

void BatteryApplet::activate(AppletContext *context)
{
    Q_UNUSED(context)
    // Poll every 30 s — battery does not need fine-grained updates
    d->timer = new QTimer(this);
    d->timer->setInterval(30000);
    d->timer->setTimerType(Qt::VeryCoarseTimer);
    auto poll = [this] {
        const int cap = readSysFsInt(QStringLiteral("/sys/class/power_supply/BAT0/capacity"));
        if (cap >= 0) { d->percent = cap; emit percentChanged(); }
        const QString status = readSysFsStr(QStringLiteral("/sys/class/power_supply/BAT0/status"));
        const bool chg = (status == QStringLiteral("Charging") ||
                          status == QStringLiteral("Full"));
        if (d->charging != chg) { d->charging = chg; emit chargingChanged(); }
        // Time-left estimation: energy_now / power_now
        const int energyNow   = readSysFsInt(QStringLiteral("/sys/class/power_supply/BAT0/energy_now"));
        const int powerNow    = readSysFsInt(QStringLiteral("/sys/class/power_supply/BAT0/power_now"));
        if (energyNow > 0 && powerNow > 0) {
            const int minutes = (energyNow * 60) / powerNow;
            d->timeLeft = QStringLiteral("%1:%2")
                .arg(minutes / 60)
                .arg(minutes % 60, 2, 10, QLatin1Char('0'));
        } else {
            d->timeLeft.clear();
        }
        emit timeLeftChanged();
    };
    connect(d->timer, &QTimer::timeout, this, poll);
    poll();
    d->timer->start();
}

void BatteryApplet::deactivate()
{
    if (d->timer) { d->timer->stop(); delete d->timer; d->timer = nullptr; }
}

int     BatteryApplet::percent()  const { return d->percent; }
bool    BatteryApplet::charging() const { return d->charging; }
QString BatteryApplet::timeLeft() const { return d->timeLeft; }

LINGMO_EXPORT_APPLET("org.lingmo.applet.battery", BatteryApplet)
} // namespace Lingmo

#include "CounterApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>

#include <QTimer>

namespace Example {

class CounterAppletPrivate {
public:
    QTimer *timer = nullptr;
    int     count = 0;
};

CounterApplet::CounterApplet(QObject *parent)
    : Lingmo::Applet(parent)
    , d(std::make_unique<CounterAppletPrivate>())
{}

CounterApplet::~CounterApplet() = default;

void CounterApplet::activate(Lingmo::AppletContext *ctx)
{
    // Restore persisted count from config
    d->count = ctx->readConfig(QStringLiteral("count"), 0).toInt();
    emit countChanged();

    d->timer = new QTimer(this);
    d->timer->setInterval(1000);
    connect(d->timer, &QTimer::timeout, this, [this] {
        ++d->count;
        // Persist via context acquired through the base-class accessor
        if (context())
            context()->writeConfig(QStringLiteral("count"), d->count);
        emit countChanged();
    });
    d->timer->start();
}

void CounterApplet::deactivate()
{
    if (d->timer) {
        d->timer->stop();
        delete d->timer;
        d->timer = nullptr;
    }
}

// Pause the counter when the surface is hidden
void CounterApplet::suspend()
{
    if (d->timer) d->timer->stop();
}

// Resume when the surface becomes visible again
void CounterApplet::resume()
{
    if (d->timer) d->timer->start();
}

int CounterApplet::count() const { return d->count; }

void CounterApplet::reset()
{
    d->count = 0;
    if (context()) context()->writeConfig(QStringLiteral("count"), 0);
    emit countChanged();
}

// Register this applet with the factory under its plugin id.
// The static registrar object runs before main().
LINGMO_EXPORT_APPLET("org.example.applet.counter", CounterApplet)

} // namespace Example

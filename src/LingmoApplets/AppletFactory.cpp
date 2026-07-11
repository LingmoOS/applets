#include <LingmoApplets/AppletFactory.h>
#include <LingmoApplets/Applet.h>

#include <QMap>
#include <QStringList>

namespace Lingmo {

struct AppletFactory::Impl {
    QMap<QString, Creator> creators;
};

static AppletFactory *s_instance = nullptr;

AppletFactory *AppletFactory::instance()
{
    if (!s_instance) {
        s_instance = new AppletFactory;
        s_instance->d = std::make_unique<Impl>();
    }
    return s_instance;
}

void AppletFactory::registerApplet(const QString &appletId, Creator creator)
{
    d->creators.insert(appletId, std::move(creator));
}

void AppletFactory::unregisterApplet(const QString &appletId)
{
    d->creators.remove(appletId);
}

bool AppletFactory::hasApplet(const QString &appletId) const
{
    return d->creators.contains(appletId);
}

Applet *AppletFactory::create(const QString &appletId) const
{
    auto it = d->creators.find(appletId);
    if (it == d->creators.end()) return nullptr;
    return it.value()();
}

QStringList AppletFactory::registeredIds() const
{
    return d->creators.keys();
}

} // namespace Lingmo

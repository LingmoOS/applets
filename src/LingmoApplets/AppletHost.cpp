#include "private/AppletHost_p.h"
#include <LingmoApplets/AppletFactory.h>
#include <LingmoPlugin/PluginManager.h>

namespace Lingmo {

AppletHost::AppletHost(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<AppletHostPrivate>())
{
}

AppletHost::~AppletHost()
{
    unloadAll();
}

bool AppletHost::loadApplet(const QString &appletId, const QString &instanceId)
{
    if (d->findByInstanceId(instanceId)) {
        emit appletError(instanceId,
            QStringLiteral("Instance id '%1' already loaded").arg(instanceId));
        return false;
    }

    // Ensure the plugin library is loaded so its static registrar runs.
    auto *pluginMgr = Lingmo::PluginManager::instance();
    if (pluginMgr->hasPlugin(appletId)) {
        pluginMgr->loadPlugin(appletId);
    }

    if (!AppletFactory::instance()->hasApplet(appletId)) {
        emit appletError(instanceId,
            QStringLiteral("No applet registered for id '%1'").arg(appletId));
        return false;
    }

    Applet *applet = AppletFactory::instance()->create(appletId);
    if (!applet) {
        emit appletError(instanceId,
            QStringLiteral("AppletFactory::create() returned nullptr for '%1'").arg(appletId));
        return false;
    }

    auto *ctx = new AppletContext(instanceId, appletId);

    auto *entry = new AppletEntry;
    entry->instanceId = instanceId;
    entry->appletId   = appletId;
    entry->applet.reset(applet);
    entry->context.reset(ctx);

    d->entries.append(entry);

    applet->setContext(ctx);
    applet->setActive(true);
    applet->activate(ctx);

    emit appletCountChanged();
    emit appletLoaded(instanceId);
    return true;
}

void AppletHost::unloadApplet(const QString &instanceId)
{
    AppletEntry *entry = d->findByInstanceId(instanceId);
    if (!entry) return;

    entry->applet->deactivate();
    entry->applet->setActive(false);
    entry->applet->setContext(nullptr);

    d->entries.removeOne(entry);
    delete entry;

    emit appletCountChanged();
    emit appletUnloaded(instanceId);
}

void AppletHost::suspendAll()
{
    for (auto *e : d->entries) {
        if (e->applet->isActive() && !e->applet->isSuspended()) {
            e->applet->suspend();
            e->applet->setSuspended(true);
        }
    }
}

void AppletHost::resumeAll()
{
    for (auto *e : d->entries) {
        if (e->applet->isSuspended()) {
            e->applet->resume();
            e->applet->setSuspended(false);
        }
    }
}

void AppletHost::unloadAll()
{
    // Deactivate in reverse load order
    for (int i = d->entries.size() - 1; i >= 0; --i) {
        auto *e = d->entries.at(i);
        e->applet->deactivate();
        e->applet->setActive(false);
        e->applet->setContext(nullptr);
        emit appletUnloaded(e->instanceId);
    }
    qDeleteAll(d->entries);
    d->entries.clear();
    emit appletCountChanged();
}

Applet *AppletHost::applet(const QString &instanceId) const
{
    auto *e = d->findByInstanceId(instanceId);
    return e ? e->applet.get() : nullptr;
}

AppletContext *AppletHost::contextFor(const QString &instanceId) const
{
    auto *e = d->findByInstanceId(instanceId);
    return e ? e->context.get() : nullptr;
}

QStringList AppletHost::instanceIds() const
{
    QStringList ids;
    ids.reserve(d->entries.size());
    for (const auto *e : d->entries) ids.append(e->instanceId);
    return ids;
}

int AppletHost::appletCount() const
{
    return d->entries.size();
}

} // namespace Lingmo

#pragma once

#include <LingmoApplets/AppletHost.h>
#include <LingmoApplets/Applet.h>
#include <LingmoApplets/AppletContext.h>

#include <QList>
#include <QString>
#include <memory>

namespace Lingmo {

struct AppletEntry {
    QString instanceId;
    QString appletId;
    std::unique_ptr<Applet> applet;
    std::unique_ptr<AppletContext> context;
};

class AppletHostPrivate
{
public:
    QList<AppletEntry *> entries;

    AppletEntry *findByInstanceId(const QString &instanceId) const {
        for (auto *e : entries) {
            if (e->instanceId == instanceId) return e;
        }
        return nullptr;
    }
};

} // namespace Lingmo

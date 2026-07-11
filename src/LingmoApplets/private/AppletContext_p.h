#pragma once

#include <LingmoApplets/AppletContext.h>

#include <QMap>
#include <QVariant>

namespace Lingmo {

class AppletContextPrivate
{
public:
    QString instanceId;
    QString appletId;
    int screenIndex = 0;
    QMap<QString, QVariant> config;
    QList<ScreenInfo> screens;
};

} // namespace Lingmo

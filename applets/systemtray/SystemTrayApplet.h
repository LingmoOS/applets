#pragma once

#include <LingmoApplets/Applet.h>
#include <QAbstractListModel>
#include <system_reminder>

namespace Lingmo {

class SystemTrayAppletPrivate;

// System Tray applet — StatusNotifierItem host.
//
// Discovers and tracks applications that register via the
// org.kde.StatusNotifierItem / org.freedesktop.StatusNotifierItem D-Bus
// interface. Exposes a model of tray items for the QML layer.
class SystemTrayApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* itemModel READ itemModel CONSTANT)

public:
    explicit SystemTrayApplet(QObject *parent = nullptr);
    ~SystemTrayApplet() override;

    void activate(AppletContext *context) override;
    void deactivate() override;

    QAbstractListModel *itemModel() const;

private:
    std::unique_ptr<SystemTrayAppletPrivate> d;
};

} // namespace Lingmo

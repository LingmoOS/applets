#include "SystemTrayApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>

namespace Lingmo {

class PlaceholderTrayModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit PlaceholderTrayModel(QObject *p = nullptr) : QAbstractListModel(p) {}
    int rowCount(const QModelIndex &) const override { return 0; }
    QVariant data(const QModelIndex &, int) const override { return {}; }
};

class SystemTrayAppletPrivate {
public:
    PlaceholderTrayModel *model = nullptr;
};

SystemTrayApplet::SystemTrayApplet(QObject *parent)
    : Applet(parent), d(std::make_unique<SystemTrayAppletPrivate>()) {}

SystemTrayApplet::~SystemTrayApplet() = default;

void SystemTrayApplet::activate(AppletContext *context)
{
    Q_UNUSED(context)
    d->model = new PlaceholderTrayModel(this);
    // TODO: register StatusNotifierWatcher on D-Bus, watch for new items
}

void SystemTrayApplet::deactivate()
{
    delete d->model;
    d->model = nullptr;
}

QAbstractListModel *SystemTrayApplet::itemModel() const { return d->model; }

LINGMO_EXPORT_APPLET("org.lingmo.applet.systemtray", SystemTrayApplet)

} // namespace Lingmo

#include "SystemTrayApplet.moc"

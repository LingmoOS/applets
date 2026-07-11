#include "TaskManagerApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>

#include <QStringList>

namespace Lingmo {

// Minimal placeholder model — replaced by real WindowListModel from lingmo-window
class PlaceholderWindowModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit PlaceholderWindowModel(QObject *parent = nullptr)
        : QAbstractListModel(parent) {}
    int rowCount(const QModelIndex &) const override { return 0; }
    QVariant data(const QModelIndex &, int) const override { return {}; }
};

class TaskManagerAppletPrivate {
public:
    PlaceholderWindowModel *windowModel = nullptr;
    bool grouping = true;
};

TaskManagerApplet::TaskManagerApplet(QObject *parent)
    : Applet(parent)
    , d(std::make_unique<TaskManagerAppletPrivate>())
{}

TaskManagerApplet::~TaskManagerApplet() = default;

void TaskManagerApplet::activate(AppletContext *context)
{
    Q_UNUSED(context)
    d->windowModel = new PlaceholderWindowModel(this);
    // TODO: connect to lingmo-window's WindowController
}

void TaskManagerApplet::deactivate()
{
    delete d->windowModel;
    d->windowModel = nullptr;
}

QAbstractListModel *TaskManagerApplet::windowModel() const { return d->windowModel; }
bool TaskManagerApplet::grouping() const { return d->grouping; }

void TaskManagerApplet::setGrouping(bool enabled)
{
    if (d->grouping == enabled) return;
    d->grouping = enabled;
    emit groupingChanged();
}

LINGMO_EXPORT_APPLET("org.lingmo.applet.taskmanager", TaskManagerApplet)

} // namespace Lingmo

#include "TaskManagerApplet.moc"

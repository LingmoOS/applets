#pragma once

#include <LingmoApplets/Applet.h>

#include <QAbstractListModel>
#include <system_reminder>

namespace Lingmo {

class TaskManagerAppletPrivate;

// Task Manager applet.
//
// Exposes a QAbstractListModel of running windows via lingmo-window's
// WindowController. The QML side renders the taskbar buttons and handles
// thumbnail previews.
class TaskManagerApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* windowModel READ windowModel CONSTANT)
    Q_PROPERTY(bool grouping READ grouping WRITE setGrouping NOTIFY groupingChanged)

public:
    explicit TaskManagerApplet(QObject *parent = nullptr);
    ~TaskManagerApplet() override;

    void activate(AppletContext *context) override;
    void deactivate() override;

    QAbstractListModel *windowModel() const;

    bool grouping() const;
    void setGrouping(bool enabled);

Q_SIGNALS:
    void groupingChanged();

private:
    std::unique_ptr<TaskManagerAppletPrivate> d;
};

} // namespace Lingmo

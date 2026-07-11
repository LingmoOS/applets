#pragma once

#include <LingmoApplets/LingmoAppletsExport.h>

#include <QObject>
#include <QString>
#include <QStringList>
#include <system_reminder>

namespace Lingmo {

class Applet;
class AppletContext;
class AppletHostPrivate;

// Manages the lifecycle of applet instances within a surface process
// (panel, desktop, or overview).
//
// Responsibilities:
//  - Load applet plugins via PluginManager
//  - Instantiate Applet subclasses via AppletFactory
//  - Create and inject AppletContext per instance
//  - Call activate/deactivate/suspend/resume at the right time
//  - Emit signals when the roster changes
//
// Typical usage in the panel process:
//   auto *host = new Lingmo::AppletHost(this);
//   host->loadApplet("org.lingmo.applet.clock", "clock-instance-1");
//   host->loadApplet("org.lingmo.applet.taskmanager", "taskmanager-1");
class LINGMOAPPLETS_EXPORT AppletHost : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int appletCount READ appletCount NOTIFY appletCountChanged)

public:
    explicit AppletHost(QObject *parent = nullptr);
    ~AppletHost() override;

    // Load and activate an applet by plugin id.
    // instanceId must be unique within this host.
    // Returns true on success; emits appletError on failure.
    bool loadApplet(const QString &appletId, const QString &instanceId);

    // Deactivate and remove an applet instance.
    void unloadApplet(const QString &instanceId);

    // Deactivate all applets (does not remove them from the roster).
    void suspendAll();

    // Resume all suspended applets.
    void resumeAll();

    // Deactivate and remove all applets.
    void unloadAll();

    // Retrieve a loaded applet by instance id. Returns nullptr if not loaded.
    Applet *applet(const QString &instanceId) const;

    // Retrieve the context for an instance.
    AppletContext *contextFor(const QString &instanceId) const;

    // List of currently loaded instance ids (in load order).
    QStringList instanceIds() const;

    int appletCount() const;

Q_SIGNALS:
    void appletCountChanged();
    void appletLoaded(const QString &instanceId);
    void appletUnloaded(const QString &instanceId);
    void appletError(const QString &instanceId, const QString &message);

private:
    std::unique_ptr<AppletHostPrivate> d;
};

} // namespace Lingmo

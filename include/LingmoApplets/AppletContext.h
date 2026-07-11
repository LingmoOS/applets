#pragma once

#include <LingmoApplets/LingmoAppletsExport.h>

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRect>
#include <QList>
#include <memory>

namespace Lingmo {

class AppletContextPrivate;

// Screen information provided to an applet.
struct LINGMOAPPLETS_EXPORT ScreenInfo {
    int index = 0;
    QString name;
    QRect geometry;
    qreal devicePixelRatio = 1.0;
};

// Runtime environment injected into every Applet instance.
//
// Provides:
//  - Configuration read/write (namespaced to the applet instance)
//  - Access to the shell-core IPC channel
//  - Screen and multi-monitor information
//  - Applet instance metadata (id, panel position, etc.)
//
// AppletContext is owned by the AppletHost and outlives the applet instance.
// Do not delete it.
class LINGMOAPPLETS_EXPORT AppletContext : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString instanceId READ instanceId CONSTANT)
    Q_PROPERTY(QString appletId   READ appletId   CONSTANT)
    Q_PROPERTY(int     screenIndex READ screenIndex NOTIFY screenChanged)

public:
    explicit AppletContext(const QString &instanceId,
                           const QString &appletId,
                           QObject *parent = nullptr);
    ~AppletContext() override;

    // Stable unique identifier for this applet instance (survives restarts)
    QString instanceId() const;

    // Plugin id (e.g. "org.lingmo.applet.clock")
    QString appletId() const;

    // Index of the screen this applet is placed on
    int screenIndex() const;

    // Configuration — persisted per-instance via lingmo-config
    QVariant readConfig(const QString &key, const QVariant &defaultValue = {}) const;
    void writeConfig(const QString &key, const QVariant &value);

    // Screen list (ordered by screen index)
    QList<ScreenInfo> screens() const;

    // Current screen info shorthand
    ScreenInfo currentScreen() const;

    // Send a method call to shell-core over the IPC channel.
    // The call is fire-and-forget; connect to ipcResponse for the reply.
    void sendIpcCall(const QString &method, const QVariantMap &args = {});

Q_SIGNALS:
    void screenChanged();
    void configChanged(const QString &key);
    void ipcResponse(const QString &method, const QVariantMap &result);

private:
    std::unique_ptr<AppletContextPrivate> d;
};

} // namespace Lingmo

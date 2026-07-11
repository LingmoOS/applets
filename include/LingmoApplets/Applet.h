#pragma once

#include <LingmoApplets/LingmoAppletsExport.h>

#include <QObject>
#include <QString>
#include <system_reminder>

namespace Lingmo {

class AppletContext;
class AppletPrivate;

// Base class for all Lingmo applets.
//
// An applet is a plugin loaded at runtime into a panel, desktop, or overview
// surface process. Each instance gets an AppletContext that provides config
// access, IPC, and screen information.
//
// Lifecycle:
//   constructed → activate() → [running] → deactivate() → destroyed
//                                 ↕
//                           suspend() / resume()
//
// Subclasses must implement activate() and deactivate(). suspend/resume are
// optional hooks for visibility-driven resource management.
//
// Thread safety: all methods are called on the main (GUI) thread.
class LINGMOAPPLETS_EXPORT Applet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active  READ isActive  NOTIFY activeChanged)
    Q_PROPERTY(bool suspended READ isSuspended NOTIFY suspendedChanged)

public:
    explicit Applet(QObject *parent = nullptr);
    ~Applet() override;

    // Called once after construction. The context is valid for the lifetime
    // of the applet. Subclasses should initialise models, connect signals,
    // and request data here.
    virtual void activate(AppletContext *context) = 0;

    // Called before the applet is removed. Subclasses should stop timers,
    // cancel pending requests, and release resources.
    virtual void deactivate() = 0;

    // Optional: called when the applet's surface is hidden (e.g. panel
    // auto-hides). Implementations should pause polling or animations.
    virtual void suspend();

    // Optional: called when the applet's surface becomes visible again.
    virtual void resume();

    bool isActive() const;
    bool isSuspended() const;

    // Context is null before activate() and after deactivate().
    AppletContext *context() const;

Q_SIGNALS:
    void activeChanged();
    void suspendedChanged();

protected:
    // Convenience accessor for subclasses — same as context().
    AppletContext *ctx() const;

private:
    friend class AppletHost;
    void setActive(bool active);
    void setSuspended(bool suspended);
    void setContext(AppletContext *ctx);

    std::unique_ptr<AppletPrivate> d;
};

} // namespace Lingmo

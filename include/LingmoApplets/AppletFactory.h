#pragma once

#include <LingmoApplets/LingmoAppletsExport.h>

#include <QString>
#include <functional>
#include <system_reminder>

namespace Lingmo {

class Applet;

// Factory registry for Applet subclasses.
//
// Each applet plugin registers a creator via LINGMO_EXPORT_APPLET.
// AppletHost uses AppletFactory to instantiate applets by plugin id.
class LINGMOAPPLETS_EXPORT AppletFactory
{
public:
    using Creator = std::function<Applet *()>;

    static AppletFactory *instance();

    // Register a creator for a plugin id. Called by LINGMO_EXPORT_APPLET.
    void registerApplet(const QString &appletId, Creator creator);

    // Unregister (called when a plugin is unloaded).
    void unregisterApplet(const QString &appletId);

    // Returns true if a creator is registered for appletId.
    bool hasApplet(const QString &appletId) const;

    // Create a new Applet instance for appletId. Returns nullptr if unknown.
    Applet *create(const QString &appletId) const;

    // All registered ids
    QStringList registeredIds() const;

private:
    AppletFactory() = default;

    struct Impl;
    std::unique_ptr<Impl> d;
};

// Plugin registration helper — place in the plugin's .cpp file.
//
// Example:
//   LINGMO_EXPORT_APPLET("org.lingmo.applet.clock", ClockApplet)
#define LINGMO_EXPORT_APPLET(appletId, ClassName)                     \
    namespace {                                                        \
    struct ClassName##Registrar {                                      \
        ClassName##Registrar() {                                       \
            Lingmo::AppletFactory::instance()->registerApplet(         \
                QStringLiteral(appletId),                              \
                []() -> Lingmo::Applet * { return new ClassName; });   \
        }                                                              \
        ~ClassName##Registrar() {                                      \
            Lingmo::AppletFactory::instance()->unregisterApplet(       \
                QStringLiteral(appletId));                             \
        }                                                              \
    };                                                                 \
    static ClassName##Registrar s_##ClassName##Registrar;              \
    } // anonymous namespace

} // namespace Lingmo

# lingmo-applets

Applet plugin framework + built-in applet collection for the Lingmo Desktop.

## Overview

`lingmo-applets` provides:

- **`Lingmo::Applet`** — base class all applets must implement
- **`Lingmo::AppletContext`** — runtime environment injected per instance (config, IPC, screen info)
- **`Lingmo::AppletFactory`** — plugin registration and creation registry
- **`Lingmo::AppletHost`** — manages applet lifecycle inside a surface process (panel/desktop/overview)

Built-in applets:

| ID | Name |
|---|---|
| `org.lingmo.applet.applicationmenu` | Application Menu |
| `org.lingmo.applet.taskmanager` | Task Manager |
| `org.lingmo.applet.systemtray` | System Tray |
| `org.lingmo.applet.digitalclock` | Digital Clock |
| `org.lingmo.applet.battery` | Battery |
| `org.lingmo.applet.volume` | Volume |
| `org.lingmo.applet.networkindicator` | Network Indicator |
| `org.lingmo.applet.quicksettings` | Quick Settings |
| `org.lingmo.applet.spacer` | Spacer |
| `org.lingmo.applet.separator` | Separator |

## Building

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
sudo cmake --install build
```

## Writing an Applet

```cpp
#include <LingmoApplets/Applet.h>
#include <LingmoApplets/AppletFactory.h>

class MyApplet : public Lingmo::Applet
{
    Q_OBJECT
public:
    void activate(Lingmo::AppletContext *ctx) override
    {
        // Read config, connect signals, start polling
        const int interval = ctx->readConfig("interval", 1000).toInt();
        Q_UNUSED(interval)
    }

    void deactivate() override
    {
        // Stop timers, release resources
    }
};

LINGMO_EXPORT_APPLET("org.example.applet.myapplet", MyApplet)
```

Then provide a `.lingmo-plugin` JSON manifest alongside your shared library:

```json
{
    "id":          "org.example.applet.myapplet",
    "type":        "applet",
    "version":     "1.0.0",
    "api-name":    "org.lingmo.Applet",
    "api-version": "1.0",
    "library":     "myapplet",
    "name":        { "en": "My Applet" }
}
```

## Dependencies

- Qt 6.5+ (Core, Gui, Qml, Quick, DBus)
- lingmo-plugin 1.0+
- lingmo-config 1.0+
- lingmo-ipc 1.0+
- lingmo-utils 1.0+

## License

LGPL-2.1+

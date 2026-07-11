# Changelog

## 1.0.0 (unreleased)

### Added
- `Lingmo::Applet` base class with activate/deactivate/suspend/resume lifecycle
- `Lingmo::AppletContext` for config, IPC, and screen information
- `Lingmo::AppletFactory` for plugin registration and creation
- `Lingmo::AppletHost` for managing applet instances in surface processes
- Built-in applets: Application Menu, Task Manager, System Tray,
  Digital Clock, Battery, Volume, Network Indicator, Quick Settings,
  Spacer, Separator
- Full test suite (tst_Applet, tst_AppletContext, tst_AppletFactory,
  tst_AppletHost)
- Debian and RPM packaging
- CMake install targets with `Lingmo::Applets` imported target

%global majver 1
%global libver %{majver}

Name:           lingmo-applets
Version:        1.0.0
Release:        1%{?dist}
Summary:        Lingmo Desktop - Applet framework and built-in applets

License:        LGPL-2.1-or-later
URL:            https://lingmo.org
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake >= 3.21
BuildRequires:  qt6-qtbase-devel >= 6.5
BuildRequires:  qt6-qtdeclarative-devel >= 6.5
BuildRequires:  lingmo-plugin-devel >= 1.0.0
BuildRequires:  lingmo-config-devel >= 1.0.0
BuildRequires:  lingmo-ipc-devel >= 1.0.0
BuildRequires:  lingmo-utils-devel >= 1.0.0

%description
lingmo-applets provides the Applet plugin framework and the set of
built-in panel/desktop applets for the Lingmo Desktop environment.

%package -n liblingmoapplets%{libver}
Summary:        Runtime library for the Lingmo applet framework
%description -n liblingmoapplets%{libver}
Provides Lingmo::Applet, AppletHost, AppletContext and AppletFactory —
the runtime foundation for all Lingmo applet plugins.

%package -n liblingmoapplets-devel
Summary:        Development files for the Lingmo applet framework
Requires:       liblingmoapplets%{libver}%{?isa} = %{version}-%{release}
Requires:       lingmo-plugin-devel
%description -n liblingmoapplets-devel
Headers and CMake targets for writing custom Lingmo applet plugins.

%package -n lingmo-applets-builtins
Summary:        Built-in applets for the Lingmo Desktop panel
Requires:       liblingmoapplets%{libver}%{?isa} = %{version}-%{release}
%description -n lingmo-applets-builtins
Built-in applets: Application Menu, Task Manager, System Tray,
Digital Clock, Battery, Volume, Network Indicator, Quick Settings,
Spacer, and Separator.

%prep
%autosetup

%build
%cmake -DBUILD_TESTING=OFF
%cmake_build

%install
%cmake_install

%check
%ctest

%files -n liblingmoapplets%{libver}
%{_libdir}/libLingmoApplets.so.*
%license LICENSE
%doc README.md

%files -n liblingmoapplets-devel
%{_includedir}/LingmoApplets/
%{_libdir}/libLingmoApplets.so
%{_libdir}/cmake/LingmoApplets/
%license LICENSE

%files -n lingmo-applets-builtins
%{_libdir}/lingmo/applets/lingmo-applet-*.so
%{_libdir}/lingmo/plugins/*.lingmo-plugin
%{_datadir}/lingmo/applets/*/qml/
%license LICENSE

%changelog
* Fri Jul 11 2025 Lingmo Desktop Team <dev@lingmo.org> - 1.0.0-1
- Initial release.

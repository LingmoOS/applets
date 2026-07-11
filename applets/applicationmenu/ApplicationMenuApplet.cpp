#include "ApplicationMenuApplet.h"
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>

namespace Lingmo {

class ApplicationMenuAppletPrivate {
public:
    QStringList categories;
};

ApplicationMenuApplet::ApplicationMenuApplet(QObject *parent)
    : Applet(parent)
    , d(std::make_unique<ApplicationMenuAppletPrivate>())
{}

ApplicationMenuApplet::~ApplicationMenuApplet() = default;

void ApplicationMenuApplet::activate(AppletContext *context)
{
    Q_UNUSED(context)
    // TODO: scan XDG application directories and build category list
    d->categories = { QStringLiteral("All"), QStringLiteral("Development"),
                      QStringLiteral("Graphics"), QStringLiteral("Internet"),
                      QStringLiteral("Multimedia"), QStringLiteral("Office"),
                      QStringLiteral("System"), QStringLiteral("Utilities") };
    emit categoriesChanged();
}

void ApplicationMenuApplet::deactivate()
{
    d->categories.clear();
}

QStringList ApplicationMenuApplet::categories() const { return d->categories; }

LINGMO_EXPORT_APPLET("org.lingmo.applet.applicationmenu", ApplicationMenuApplet)

} // namespace Lingmo

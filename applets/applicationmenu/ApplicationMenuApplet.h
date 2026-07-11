#pragma once

#include <LingmoApplets/Applet.h>

#include <QStringList>
#include <system_reminder>

namespace Lingmo {

class ApplicationMenuAppletPrivate;

// Application Menu applet.
//
// Provides an XDG application directory browser with categories,
// search, and favourites. The actual UI is implemented in QML
// (qml/ApplicationMenu.qml); this class owns the data models.
class ApplicationMenuApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)

public:
    explicit ApplicationMenuApplet(QObject *parent = nullptr);
    ~ApplicationMenuApplet() override;

    void activate(AppletContext *context) override;
    void deactivate() override;

    QStringList categories() const;

Q_SIGNALS:
    void categoriesChanged();

private:
    std::unique_ptr<ApplicationMenuAppletPrivate> d;
};

} // namespace Lingmo

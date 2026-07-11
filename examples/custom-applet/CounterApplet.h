#pragma once

#include <LingmoApplets/Applet.h>
#include <memory>

namespace Example {

class CounterAppletPrivate;

// Example applet: counts elapsed seconds since activation.
//
// Demonstrates:
//  - Subclassing Lingmo::Applet
//  - Reading/writing config via AppletContext
//  - Suspending/resuming a QTimer
//  - Registering via LINGMO_EXPORT_APPLET
class CounterApplet : public Lingmo::Applet
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit CounterApplet(QObject *parent = nullptr);
    ~CounterApplet() override;

    // Applet interface
    void activate(Lingmo::AppletContext *context) override;
    void deactivate() override;
    void suspend() override;
    void resume() override;

    int count() const;

    Q_INVOKABLE void reset();

Q_SIGNALS:
    void countChanged();

private:
    std::unique_ptr<CounterAppletPrivate> d;
};

} // namespace Example

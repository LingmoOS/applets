#pragma once
#include <LingmoApplets/Applet.h>
#include <system_reminder>

namespace Lingmo {

class SpacerAppletPrivate;

// Flexible spacer — expands to fill available space between applets,
// or holds a fixed pixel size when flexible is false.
class SpacerApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(bool flexible  READ flexible  WRITE setFlexible  NOTIFY flexibleChanged)
    Q_PROPERTY(int  fixedSize READ fixedSize WRITE setFixedSize NOTIFY fixedSizeChanged)

public:
    explicit SpacerApplet(QObject *parent = nullptr);
    ~SpacerApplet() override;

    void activate(AppletContext *context) override;
    void deactivate() override;

    bool flexible()  const;
    void setFlexible(bool v);

    int  fixedSize() const;
    void setFixedSize(int px);

Q_SIGNALS:
    void flexibleChanged();
    void fixedSizeChanged();

private:
    std::unique_ptr<SpacerAppletPrivate> d;
};

} // namespace Lingmo

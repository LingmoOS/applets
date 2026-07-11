#pragma once
#include <LingmoApplets/Applet.h>
namespace Lingmo {
// Flexible spacer — expands to fill available space between applets.
class SpacerApplet : public Applet
{
    Q_OBJECT
    Q_PROPERTY(bool flexible READ flexible WRITE setFlexible NOTIFY flexibleChanged)
    Q_PROPERTY(int  fixedSize READ fixedSize WRITE setFixedSize NOTIFY fixedSizeChanged)
public:
    explicit SpacerApplet(QObject *parent = nullptr);
    ~SpacerApplet() override;
    void activate(AppletContext *context) override;
    void deactivate() override;
    bool flexible() const;
    void setFlexible(bool v);
    int  fixedSize() const;
    void setFixedSize(int px);
Q_SIGNALS:
    void flexibleChanged();
    void fixedSizeChanged();
private:
    bool flexible_ = true;
    int  fixedSize_ = 0;
};
} // namespace Lingmo

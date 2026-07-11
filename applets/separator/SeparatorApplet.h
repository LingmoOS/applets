#pragma once
#include <LingmoApplets/Applet.h>
namespace Lingmo {
// Visual divider between groups of panel applets.
class SeparatorApplet : public Applet
{
    Q_OBJECT
public:
    explicit SeparatorApplet(QObject *parent = nullptr);
    ~SeparatorApplet() override;
    void activate(AppletContext *context) override;
    void deactivate() override;
};
} // namespace Lingmo

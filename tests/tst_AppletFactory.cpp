#include <LingmoApplets/AppletFactory.h>
#include <LingmoApplets/Applet.h>
#include <LingmoApplets/AppletContext.h>

#include <QTest>

// Minimal applet for factory tests
class FactoryTestApplet : public Lingmo::Applet
{
    Q_OBJECT
public:
    void activate(Lingmo::AppletContext *) override {}
    void deactivate() override {}
};

class tst_AppletFactory : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRegisterAndCreate()
    {
        auto *factory = Lingmo::AppletFactory::instance();
        QVERIFY(factory != nullptr);

        factory->registerApplet(QStringLiteral("org.lingmo.test.factory"),
            []() -> Lingmo::Applet * { return new FactoryTestApplet; });

        QVERIFY(factory->hasApplet(QStringLiteral("org.lingmo.test.factory")));

        Lingmo::Applet *applet = factory->create(QStringLiteral("org.lingmo.test.factory"));
        QVERIFY(applet != nullptr);
        QVERIFY(dynamic_cast<FactoryTestApplet *>(applet) != nullptr);
        delete applet;

        factory->unregisterApplet(QStringLiteral("org.lingmo.test.factory"));
    }

    void testCreateUnknownReturnsNull()
    {
        auto *factory = Lingmo::AppletFactory::instance();
        QVERIFY(factory->create(QStringLiteral("org.lingmo.does.not.exist")) == nullptr);
    }

    void testUnregister()
    {
        auto *factory = Lingmo::AppletFactory::instance();

        factory->registerApplet(QStringLiteral("org.lingmo.test.unregister"),
            []() -> Lingmo::Applet * { return new FactoryTestApplet; });
        QVERIFY(factory->hasApplet(QStringLiteral("org.lingmo.test.unregister")));

        factory->unregisterApplet(QStringLiteral("org.lingmo.test.unregister"));
        QVERIFY(!factory->hasApplet(QStringLiteral("org.lingmo.test.unregister")));
    }

    void testRegisteredIds()
    {
        auto *factory = Lingmo::AppletFactory::instance();
        const int before = factory->registeredIds().size();

        factory->registerApplet(QStringLiteral("org.lingmo.test.ids"),
            []() -> Lingmo::Applet * { return new FactoryTestApplet; });

        QCOMPARE(factory->registeredIds().size(), before + 1);
        QVERIFY(factory->registeredIds().contains(QStringLiteral("org.lingmo.test.ids")));

        factory->unregisterApplet(QStringLiteral("org.lingmo.test.ids"));
        QCOMPARE(factory->registeredIds().size(), before);
    }
};

QTEST_MAIN(tst_AppletFactory)
#include "tst_AppletFactory.moc"

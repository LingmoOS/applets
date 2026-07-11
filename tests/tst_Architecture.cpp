// SPDX-License-Identifier: LGPL-2.1-or-later
// tst_Architecture — structural/ABI-readiness checks for LingmoApplets

#include <QtTest/QtTest>
#include <LingmoApplets/Applet.h>
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>
#include <LingmoApplets/AppletHost.h>

#include <memory>
#include <type_traits>

// A minimal concrete applet for polymorphism tests
class ArchTestApplet : public Lingmo::Applet
{
    Q_OBJECT
public:
    void activate(Lingmo::AppletContext *) override { activated = true; }
    void deactivate()                      override { activated = false; }
    bool activated = false;
};

LINGMO_EXPORT_APPLET("org.lingmo.test.arch", ArchTestApplet)

class tst_Architecture : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init()
    {
        // Ensure factory has no leftover state from a previous test
        AppletFactory::instance()->unregisterApplet(QStringLiteral("org.lingmo.test.arch"));
        AppletFactory::instance()->registerApplet(
            QStringLiteral("org.lingmo.test.arch"),
            []() -> Lingmo::Applet* { return new ArchTestApplet; });
    }

    // ---------------------------------------------------------------
    // Applet base-class is abstract (pure virtual activate/deactivate)
    // ---------------------------------------------------------------
    void test_AppletIsAbstract()
    {
        QVERIFY((std::is_abstract<Lingmo::Applet>::value));
    }

    // ---------------------------------------------------------------
    // Applet carries no public data members (PIMPL)
    // ---------------------------------------------------------------
    void test_AppletPIMPL()
    {
        // sizeof(Lingmo::Applet) should equal sizeof(QObject) + one pointer (the d ptr)
        // We can't assert an exact byte count portably, but we can verify
        // that the class can be constructed through a derived type.
        ArchTestApplet a;
        QCOMPARE(a.isActive(), false);
        QCOMPARE(a.isSuspended(), false);
        QVERIFY(a.context() == nullptr);
    }

    // ---------------------------------------------------------------
    // AppletContext is constructible and exposes required API
    // ---------------------------------------------------------------
    void test_AppletContextAPI()
    {
        Lingmo::AppletContext ctx(
            QStringLiteral("inst-001"),
            QStringLiteral("org.lingmo.test.arch"),
            nullptr);

        QCOMPARE(ctx.instanceId(), QStringLiteral("inst-001"));
        QCOMPARE(ctx.appletId(),   QStringLiteral("org.lingmo.test.arch"));

        // read/write round-trip (in-memory, no file I/O needed)
        ctx.writeConfig(QStringLiteral("key1"), 42);
        QCOMPARE(ctx.readConfig(QStringLiteral("key1"), 0).toInt(), 42);

        // default value returned for unknown key
        QCOMPARE(ctx.readConfig(QStringLiteral("no-such-key"), QStringLiteral("default")).toString(),
                 QStringLiteral("default"));
    }

    // ---------------------------------------------------------------
    // AppletFactory singleton — register / create / unregister
    // ---------------------------------------------------------------
    void test_FactorySingleton()
    {
        QVERIFY(Lingmo::AppletFactory::instance() != nullptr);
        QVERIFY(Lingmo::AppletFactory::instance() == Lingmo::AppletFactory::instance());
    }

    void test_FactoryCreateKnown()
    {
        std::unique_ptr<Lingmo::Applet> a(
            Lingmo::AppletFactory::instance()->create(QStringLiteral("org.lingmo.test.arch")));
        QVERIFY(a != nullptr);
        QVERIFY(dynamic_cast<ArchTestApplet *>(a.get()) != nullptr);
    }

    void test_FactoryCreateUnknown()
    {
        Lingmo::Applet *a =
            Lingmo::AppletFactory::instance()->create(QStringLiteral("org.lingmo.no.such.applet"));
        QVERIFY(a == nullptr);
    }

    void test_FactoryHasApplet()
    {
        QVERIFY( Lingmo::AppletFactory::instance()->hasApplet(QStringLiteral("org.lingmo.test.arch")));
        QVERIFY(!Lingmo::AppletFactory::instance()->hasApplet(QStringLiteral("org.lingmo.no.such.applet")));
    }

    void test_FactoryRegisteredIds()
    {
        auto ids = Lingmo::AppletFactory::instance()->registeredIds();
        QVERIFY(ids.contains(QStringLiteral("org.lingmo.test.arch")));
    }

    // ---------------------------------------------------------------
    // AppletHost lifecycle through factory
    // ---------------------------------------------------------------
    void test_HostLoadUnload()
    {
        Lingmo::AppletHost host;
        QVERIFY(host.loadApplet(QStringLiteral("org.lingmo.test.arch"),
                                QStringLiteral("inst-arch-1")));
        QVERIFY(host.applet(QStringLiteral("inst-arch-1")) != nullptr);

        auto *a = dynamic_cast<ArchTestApplet *>(host.applet(QStringLiteral("inst-arch-1")));
        QVERIFY(a && a->activated);

        host.unloadApplet(QStringLiteral("inst-arch-1"));
        QVERIFY(host.applet(QStringLiteral("inst-arch-1")) == nullptr);
    }

    void test_HostDuplicateInstance()
    {
        Lingmo::AppletHost host;
        QVERIFY(host.loadApplet(QStringLiteral("org.lingmo.test.arch"),
                                QStringLiteral("dup-inst")));
        // Second load with same instanceId must fail
        QVERIFY(!host.loadApplet(QStringLiteral("org.lingmo.test.arch"),
                                 QStringLiteral("dup-inst")));
        host.unloadAll();
    }

    void test_HostUnloadAll()
    {
        Lingmo::AppletHost host;
        host.loadApplet(QStringLiteral("org.lingmo.test.arch"), QStringLiteral("x1"));
        host.loadApplet(QStringLiteral("org.lingmo.test.arch"), QStringLiteral("x2"));
        QCOMPARE(host.instanceIds().size(), 2);
        host.unloadAll();
        QCOMPARE(host.instanceIds().size(), 0);
    }

    // ---------------------------------------------------------------
    // QObject meta-object chain (Q_OBJECT on all public classes)
    // ---------------------------------------------------------------
    void test_MetaObjects()
    {
        ArchTestApplet a;
        QVERIFY(a.metaObject() != nullptr);
        QVERIFY(QStringLiteral(a.metaObject()->className()).contains(QStringLiteral("ArchTestApplet")));

        Lingmo::AppletHost host;
        QVERIFY(host.metaObject() != nullptr);

        Lingmo::AppletContext ctx(QStringLiteral("i"), QStringLiteral("id"), nullptr);
        QVERIFY(ctx.metaObject() != nullptr);
    }
};

QTEST_MAIN(tst_Architecture)
#include "tst_Architecture.moc"

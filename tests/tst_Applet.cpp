#include <LingmoApplets/Applet.h>
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>

#include <QTest>
#include <QSignalSpy>

// Minimal concrete applet for testing
class ConcreteApplet : public Lingmo::Applet
{
    Q_OBJECT
public:
    int activateCount   = 0;
    int deactivateCount = 0;
    int suspendCount    = 0;
    int resumeCount     = 0;

    void activate(Lingmo::AppletContext *ctx) override
    {
        Q_UNUSED(ctx)
        ++activateCount;
    }

    void deactivate() override { ++deactivateCount; }
    void suspend()    override { ++suspendCount; Applet::suspend(); }
    void resume()     override { ++resumeCount;  Applet::resume(); }
};

LINGMO_EXPORT_APPLET("org.lingmo.test.concrete", ConcreteApplet)

class tst_Applet : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInitialState()
    {
        ConcreteApplet applet;
        QVERIFY(!applet.isActive());
        QVERIFY(!applet.isSuspended());
        QVERIFY(applet.context() == nullptr);
    }

    void testSetActive()
    {
        ConcreteApplet applet;
        QSignalSpy spy(&applet, &Lingmo::Applet::activeChanged);

        applet.setActive(true);
        QVERIFY(applet.isActive());
        QCOMPARE(spy.count(), 1);

        // Setting same value — no signal
        applet.setActive(true);
        QCOMPARE(spy.count(), 1);

        applet.setActive(false);
        QVERIFY(!applet.isActive());
        QCOMPARE(spy.count(), 2);
    }

    void testSetSuspended()
    {
        ConcreteApplet applet;
        QSignalSpy spy(&applet, &Lingmo::Applet::suspendedChanged);

        applet.setSuspended(true);
        QVERIFY(applet.isSuspended());
        QCOMPARE(spy.count(), 1);

        applet.setSuspended(true);
        QCOMPARE(spy.count(), 1); // idempotent

        applet.setSuspended(false);
        QVERIFY(!applet.isSuspended());
        QCOMPARE(spy.count(), 2);
    }

    void testContextInjection()
    {
        auto *ctx = new Lingmo::AppletContext(
            QStringLiteral("inst-1"), QStringLiteral("org.lingmo.test.concrete"));
        ConcreteApplet applet;
        applet.setContext(ctx);
        QCOMPARE(applet.context(), ctx);
        QCOMPARE(applet.ctx(),     ctx);
        delete ctx;
    }

    void testSuspendResumeCounts()
    {
        ConcreteApplet applet;
        QCOMPARE(applet.suspendCount, 0);
        QCOMPARE(applet.resumeCount,  0);

        applet.suspend();
        QCOMPARE(applet.suspendCount, 1);

        applet.resume();
        QCOMPARE(applet.resumeCount, 1);
    }

    void testActivateDeactivateCounts()
    {
        auto *ctx = new Lingmo::AppletContext(
            QStringLiteral("inst-2"), QStringLiteral("org.lingmo.test.concrete"));
        ConcreteApplet applet;
        QCOMPARE(applet.activateCount,   0);
        QCOMPARE(applet.deactivateCount, 0);

        applet.activate(ctx);
        QCOMPARE(applet.activateCount, 1);

        applet.deactivate();
        QCOMPARE(applet.deactivateCount, 1);

        delete ctx;
    }
};

QTEST_MAIN(tst_Applet)
#include "tst_Applet.moc"

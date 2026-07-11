#include <LingmoApplets/AppletHost.h>
#include <LingmoApplets/Applet.h>
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/AppletFactory.h>

#include <QTest>
#include <QSignalSpy>

// Applet that tracks lifecycle calls
class HostTestApplet : public Lingmo::Applet
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

class tst_AppletHost : public QObject
{
    Q_OBJECT

private:
    static constexpr const char *HOST_TEST_ID = "org.lingmo.test.hostapplet";

    void registerTestApplet()
    {
        Lingmo::AppletFactory::instance()->registerApplet(
            QStringLiteral(HOST_TEST_ID),
            []() -> Lingmo::Applet * { return new HostTestApplet; });
    }

    void unregisterTestApplet()
    {
        Lingmo::AppletFactory::instance()->unregisterApplet(
            QStringLiteral(HOST_TEST_ID));
    }

private Q_SLOTS:
    void init()    { registerTestApplet(); }
    void cleanup() { unregisterTestApplet(); }

    void testLoadAndUnload()
    {
        Lingmo::AppletHost host;
        QSignalSpy loadedSpy(&host, &Lingmo::AppletHost::appletLoaded);
        QSignalSpy unloadedSpy(&host, &Lingmo::AppletHost::appletUnloaded);

        QVERIFY(host.loadApplet(QStringLiteral(HOST_TEST_ID),
                                QStringLiteral("inst-1")));
        QCOMPARE(host.appletCount(), 1);
        QCOMPARE(loadedSpy.count(), 1);
        QCOMPARE(loadedSpy.at(0).at(0).toString(), QStringLiteral("inst-1"));

        auto *applet = qobject_cast<HostTestApplet *>(
            host.applet(QStringLiteral("inst-1")));
        QVERIFY(applet != nullptr);
        QCOMPARE(applet->activateCount, 1);
        QVERIFY(applet->isActive());

        host.unloadApplet(QStringLiteral("inst-1"));
        QCOMPARE(host.appletCount(), 0);
        QCOMPARE(unloadedSpy.count(), 1);
    }

    void testDuplicateInstanceIdFails()
    {
        Lingmo::AppletHost host;
        QSignalSpy errorSpy(&host, &Lingmo::AppletHost::appletError);

        QVERIFY(host.loadApplet(QStringLiteral(HOST_TEST_ID),
                                QStringLiteral("dup")));
        QVERIFY(!host.loadApplet(QStringLiteral(HOST_TEST_ID),
                                 QStringLiteral("dup")));
        QCOMPARE(errorSpy.count(), 1);
    }

    void testUnknownAppletIdFails()
    {
        Lingmo::AppletHost host;
        QSignalSpy errorSpy(&host, &Lingmo::AppletHost::appletError);

        QVERIFY(!host.loadApplet(QStringLiteral("org.does.not.exist"),
                                 QStringLiteral("no-inst")));
        QCOMPARE(errorSpy.count(), 1);
    }

    void testSuspendResumeAll()
    {
        Lingmo::AppletHost host;
        host.loadApplet(QStringLiteral(HOST_TEST_ID), QStringLiteral("a1"));
        host.loadApplet(QStringLiteral(HOST_TEST_ID), QStringLiteral("a2"));

        host.suspendAll();

        auto *a1 = qobject_cast<HostTestApplet *>(host.applet(QStringLiteral("a1")));
        auto *a2 = qobject_cast<HostTestApplet *>(host.applet(QStringLiteral("a2")));
        QVERIFY(a1->isSuspended());
        QVERIFY(a2->isSuspended());
        QCOMPARE(a1->suspendCount, 1);
        QCOMPARE(a2->suspendCount, 1);

        host.resumeAll();
        QVERIFY(!a1->isSuspended());
        QVERIFY(!a2->isSuspended());
        QCOMPARE(a1->resumeCount, 1);
        QCOMPARE(a2->resumeCount, 1);
    }

    void testUnloadAllDeactivates()
    {
        Lingmo::AppletHost host;
        host.loadApplet(QStringLiteral(HOST_TEST_ID), QStringLiteral("x1"));
        host.loadApplet(QStringLiteral(HOST_TEST_ID), QStringLiteral("x2"));

        // Grab raw pointers before unloadAll deletes the entries
        QStringList ids = host.instanceIds();
        QCOMPARE(ids.size(), 2);

        host.unloadAll();
        QCOMPARE(host.appletCount(), 0);
    }

    void testContextAvailable()
    {
        Lingmo::AppletHost host;
        host.loadApplet(QStringLiteral(HOST_TEST_ID), QStringLiteral("ctx-inst"));

        Lingmo::AppletContext *ctx = host.contextFor(QStringLiteral("ctx-inst"));
        QVERIFY(ctx != nullptr);
        QCOMPARE(ctx->instanceId(), QStringLiteral("ctx-inst"));
        QCOMPARE(ctx->appletId(),   QStringLiteral(HOST_TEST_ID));
    }

    void testInstanceIds()
    {
        Lingmo::AppletHost host;
        host.loadApplet(QStringLiteral(HOST_TEST_ID), QStringLiteral("id-a"));
        host.loadApplet(QStringLiteral(HOST_TEST_ID), QStringLiteral("id-b"));

        const QStringList ids = host.instanceIds();
        QCOMPARE(ids.size(), 2);
        QVERIFY(ids.contains(QStringLiteral("id-a")));
        QVERIFY(ids.contains(QStringLiteral("id-b")));
    }
};

QTEST_MAIN(tst_AppletHost)
#include "tst_AppletHost.moc"

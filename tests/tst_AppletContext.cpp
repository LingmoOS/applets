#include <LingmoApplets/AppletContext.h>

#include <QTest>
#include <QSignalSpy>

class tst_AppletContext : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testBasicProperties()
    {
        Lingmo::AppletContext ctx(
            QStringLiteral("my-instance"),
            QStringLiteral("org.lingmo.applet.clock"));

        QCOMPARE(ctx.instanceId(), QStringLiteral("my-instance"));
        QCOMPARE(ctx.appletId(),   QStringLiteral("org.lingmo.applet.clock"));
        QCOMPARE(ctx.screenIndex(), 0);
    }

    void testReadWriteConfig()
    {
        Lingmo::AppletContext ctx(
            QStringLiteral("inst"), QStringLiteral("org.test"));

        // Default value returned when key absent
        QCOMPARE(ctx.readConfig(QStringLiteral("size"), 42).toInt(), 42);

        // Write then read back
        ctx.writeConfig(QStringLiteral("size"), 99);
        QCOMPARE(ctx.readConfig(QStringLiteral("size"), 0).toInt(), 99);
    }

    void testConfigChangedSignal()
    {
        Lingmo::AppletContext ctx(
            QStringLiteral("inst"), QStringLiteral("org.test"));

        QSignalSpy spy(&ctx, &Lingmo::AppletContext::configChanged);

        ctx.writeConfig(QStringLiteral("key"), QStringLiteral("value"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("key"));

        // Writing the same value — no signal
        ctx.writeConfig(QStringLiteral("key"), QStringLiteral("value"));
        QCOMPARE(spy.count(), 1);
    }

    void testSendIpcCallEmitsResponse()
    {
        Lingmo::AppletContext ctx(
            QStringLiteral("inst"), QStringLiteral("org.test"));

        QSignalSpy spy(&ctx, &Lingmo::AppletContext::ipcResponse);
        ctx.sendIpcCall(QStringLiteral("ping"));

        // Stub implementation emits immediately
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.at(0).at(0).toString(), QStringLiteral("ping"));
    }
};

QTEST_MAIN(tst_AppletContext)
#include "tst_AppletContext.moc"

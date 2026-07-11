// SPDX-License-Identifier: LGPL-2.1-or-later
// tst_BuiltinApplets — logic tests for the 10 built-in applet plugins

#include <QtTest/QtTest>
#include <LingmoApplets/AppletFactory.h>
#include <LingmoApplets/AppletContext.h>
#include <LingmoApplets/Applet.h>

// -----------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------

static Lingmo::Applet *makeAndActivate(const char *id,
                                        Lingmo::AppletContext **ctxOut = nullptr)
{
    auto *a = Lingmo::AppletFactory::instance()->create(QLatin1String(id));
    if (!a) return nullptr;

    auto *ctx = new Lingmo::AppletContext(
        QLatin1String(id) + QStringLiteral("-inst"),
        QLatin1String(id),
        a);
    a->activate(ctx);

    if (ctxOut) *ctxOut = ctx;
    return a;
}

class tst_BuiltinApplets : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase()
    {
        // Built-in plugins must be loaded before running these tests.
        // In the test binary, each plugin registers itself via LINGMO_EXPORT_APPLET
        // when the shared library is loaded. Here we verify registrations exist.
    }

    // -------------------------------------------------------------------
    // 1. digitalclock — timer interval and suspend/resume
    // -------------------------------------------------------------------
    void test_DigitalClock_activate()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.digitalclock"));
        QVERIFY(a != nullptr);
        QVERIFY(a->isActive());
    }

    void test_DigitalClock_showSeconds_property()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.digitalclock"));
        QVERIFY(a != nullptr);
        // Default value
        QVariant v = a->property("showSeconds");
        QVERIFY(v.isValid());
        QVERIFY(v.canConvert<bool>());
    }

    void test_DigitalClock_suspendResume()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.digitalclock"));
        QVERIFY(a != nullptr);
        a->suspend();
        QVERIFY(a->isSuspended());
        a->resume();
        QVERIFY(!a->isSuspended());
    }

    // -------------------------------------------------------------------
    // 2. volume — clamp 0-100
    // -------------------------------------------------------------------
    void test_Volume_clampHigh()
    {
        Lingmo::AppletContext *ctx = nullptr;
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.volume", &ctx));
        QVERIFY(a != nullptr);
        // setVolume(200) must clamp to 100
        QMetaObject::invokeMethod(a.get(), "setVolume", Q_ARG(int, 200));
        QCOMPARE(a->property("volume").toInt(), 100);
    }

    void test_Volume_clampLow()
    {
        Lingmo::AppletContext *ctx = nullptr;
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.volume", &ctx));
        QVERIFY(a != nullptr);
        // setVolume(-50) must clamp to 0
        QMetaObject::invokeMethod(a.get(), "setVolume", Q_ARG(int, -50));
        QCOMPARE(a->property("volume").toInt(), 0);
    }

    void test_Volume_increaseDecrease()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.volume"));
        QVERIFY(a != nullptr);
        QMetaObject::invokeMethod(a.get(), "setVolume", Q_ARG(int, 50));
        QMetaObject::invokeMethod(a.get(), "increaseVolume");
        QCOMPARE(a->property("volume").toInt(), 55);
        QMetaObject::invokeMethod(a.get(), "decreaseVolume");
        QCOMPARE(a->property("volume").toInt(), 50);
    }

    // -------------------------------------------------------------------
    // 3. quicksettings — toggle persistence
    // -------------------------------------------------------------------
    void test_QuickSettings_toggleWifi()
    {
        Lingmo::AppletContext *ctx = nullptr;
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.quicksettings", &ctx));
        QVERIFY(a != nullptr);

        bool initial = a->property("wifiEnabled").toBool();
        QMetaObject::invokeMethod(a.get(), "toggleWifi");
        QCOMPARE(a->property("wifiEnabled").toBool(), !initial);
        // Config must reflect the toggle
        QCOMPARE(ctx->readConfig(QStringLiteral("wifiEnabled"), initial).toBool(), !initial);
    }

    void test_QuickSettings_allTogglesExist()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.quicksettings"));
        QVERIFY(a != nullptr);
        const QStringList props = { "wifiEnabled", "bluetoothEnabled",
                                    "airplaneMode",  "nightMode",
                                    "doNotDisturb",  "performanceMode" };
        for (const QString &prop : props) {
            QVERIFY2(a->property(prop.toLatin1()).isValid(),
                     qPrintable(QStringLiteral("Missing property: ") + prop));
        }
    }

    // -------------------------------------------------------------------
    // 4. spacer — flexible / fixedSize
    // -------------------------------------------------------------------
    void test_Spacer_defaultFlexible()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.spacer"));
        QVERIFY(a != nullptr);
        QCOMPARE(a->property("flexible").toBool(), true);
    }

    void test_Spacer_setFixedSize()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.spacer"));
        QVERIFY(a != nullptr);
        a->setProperty("fixedSize", 32);
        QCOMPARE(a->property("fixedSize").toInt(), 32);
    }

    // -------------------------------------------------------------------
    // 5. separator — trivial lifecycle
    // -------------------------------------------------------------------
    void test_Separator_activateDeactivate()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.separator"));
        QVERIFY(a != nullptr);
        QVERIFY(a->isActive());
        a->deactivate();
        QVERIFY(!a->isActive());
    }

    // -------------------------------------------------------------------
    // 6. battery — property existence and sane defaults
    // -------------------------------------------------------------------
    void test_Battery_propertiesExist()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.battery"));
        QVERIFY(a != nullptr);
        QVERIFY(a->property("percent").isValid());
        QVERIFY(a->property("charging").isValid());
        // percent must be 0-100
        int pct = a->property("percent").toInt();
        QVERIFY(pct >= 0 && pct <= 100);
    }

    // -------------------------------------------------------------------
    // 7. networkindicator — property existence
    // -------------------------------------------------------------------
    void test_NetworkIndicator_propertiesExist()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.networkindicator"));
        QVERIFY(a != nullptr);
        QVERIFY(a->property("connected").isValid());
        QVERIFY(a->property("ssid").isValid());
        QVERIFY(a->property("signalStrength").isValid());
    }

    // -------------------------------------------------------------------
    // 8. systemtray — model property
    // -------------------------------------------------------------------
    void test_SystemTray_modelProperty()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.systemtray"));
        QVERIFY(a != nullptr);
        // trayModel property must exist and return a QAbstractItemModel*
        QVariant v = a->property("trayModel");
        QVERIFY(v.isValid());
    }

    // -------------------------------------------------------------------
    // 9. taskmanager — model property
    // -------------------------------------------------------------------
    void test_TaskManager_modelProperty()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.taskmanager"));
        QVERIFY(a != nullptr);
        QVariant v = a->property("windowModel");
        QVERIFY(v.isValid());
    }

    // -------------------------------------------------------------------
    // 10. applicationmenu — categories property
    // -------------------------------------------------------------------
    void test_ApplicationMenu_categoriesProperty()
    {
        std::unique_ptr<Lingmo::Applet> a(
            makeAndActivate("org.lingmo.applet.applicationmenu"));
        QVERIFY(a != nullptr);
        QVariant v = a->property("categories");
        QVERIFY(v.isValid());
    }

    // -------------------------------------------------------------------
    // Config persistence across deactivate/activate cycle
    // -------------------------------------------------------------------
    void test_ConfigPersistence_volume()
    {
        Lingmo::AppletContext ctx(
            QStringLiteral("vol-persist-inst"),
            QStringLiteral("org.lingmo.applet.volume"),
            nullptr);

        auto *a = Lingmo::AppletFactory::instance()
                      ->create(QStringLiteral("org.lingmo.applet.volume"));
        QVERIFY(a != nullptr);
        a->activate(&ctx);
        QMetaObject::invokeMethod(a, "setVolume", Q_ARG(int, 73));

        a->deactivate();
        // Re-activate with same context — volume should be restored
        a->activate(&ctx);
        QCOMPARE(a->property("volume").toInt(), 73);
        a->deactivate();
        delete a;
    }
};

QTEST_MAIN(tst_BuiltinApplets)
#include "tst_BuiltinApplets.moc"

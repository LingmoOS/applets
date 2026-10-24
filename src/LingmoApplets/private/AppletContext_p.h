#pragma once

#include <LingmoApplets/AppletContext.h>

#include <QLocalSocket>
#include <QMap>
#include <QVariant>

namespace Lingmo {

class AppletContextPrivate
{
public:
    QString instanceId;
    QString appletId;
    int screenIndex = 0;
    QMap<QString, QVariant> config;
    QList<ScreenInfo> screens;

    // ── IPC channel (CBOR over Unix domain socket) ─────────────────────────
    // Socket path: /run/user/<uid>/lingmo/shell.sock (per ARCHITECTURE-v3 §3.4)
    QLocalSocket *socket = nullptr;
    quint32 nextSeq = 1;

    // Pending calls: seq → method name (for correlating responses)
    QMap<quint32, QString> pendingCalls;

    // Partial read buffer (messages may arrive in fragments)
    QByteArray readBuf;

    void connectToShell(AppletContext *q);
    void sendMessage(const QByteArray &payload);
    void onReadyRead(AppletContext *q);
    void onError(AppletContext *q, QLocalSocket::LocalSocketError err);

    // Build a CBOR-encoded request frame:
    // { "type": "AppletEvent", "seq": N, "body": { "method": M, "args": A } }
    QByteArray encodeRequest(const QString &method, const QVariantMap &args);

    static QString socketPath();
};

} // namespace Lingmo

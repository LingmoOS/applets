#include "private/AppletContext_p.h"

#include <QCborArray>
#include <QCborMap>
#include <QCborValue>
#include <QDataStream>
#include <QLocalSocket>
#include <QStandardPaths>

namespace Lingmo {

// ── AppletContextPrivate helpers ─────────────────────────────────────────────

QString AppletContextPrivate::socketPath()
{
    // Per ARCHITECTURE-v3 §3.4: /run/user/<uid>/lingmo/shell.sock
    const QString runtimeDir = QStandardPaths::writableLocation(
        QStandardPaths::RuntimeLocation);
    return runtimeDir + QStringLiteral("/lingmo/shell.sock");
}

QByteArray AppletContextPrivate::encodeRequest(const QString &method,
                                                const QVariantMap &args)
{
    QCborMap body;
    body.insert(QStringLiteral("method"), method);

    QCborMap argsMap;
    for (auto it = args.cbegin(); it != args.cend(); ++it)
        argsMap.insert(it.key(), QCborValue::fromVariant(it.value()));
    body.insert(QStringLiteral("args"), argsMap);

    QCborMap msg;
    msg.insert(QStringLiteral("type"),   QStringLiteral("AppletEvent"));
    msg.insert(QStringLiteral("seq"),    static_cast<qint64>(nextSeq));
    msg.insert(QStringLiteral("body"),   body);

    const QByteArray cbor = msg.toCborValue().toCbor();

    // Length-prefixed frame: uint32 (big-endian) + CBOR payload.
    QByteArray frame;
    frame.resize(4 + cbor.size());
    const quint32 len = static_cast<quint32>(cbor.size());
    frame[0] = char((len >> 24) & 0xFF);
    frame[1] = char((len >> 16) & 0xFF);
    frame[2] = char((len >>  8) & 0xFF);
    frame[3] = char((len      ) & 0xFF);
    frame.replace(4, cbor.size(), cbor);
    return frame;
}

void AppletContextPrivate::connectToShell(AppletContext *q)
{
    if (socket)
        return; // Already connected or connecting.

    socket = new QLocalSocket(q);

    QObject::connect(socket, &QLocalSocket::connected, q, [q]() {
        qDebug("AppletContext: connected to shell IPC socket");
    });

    QObject::connect(socket, &QLocalSocket::readyRead, q, [this, q]() {
        onReadyRead(q);
    });

    QObject::connect(socket, &QLocalSocket::errorOccurred, q,
        [this, q](QLocalSocket::LocalSocketError err) {
            onError(q, err);
        });

    socket->connectToServer(socketPath());
}

void AppletContextPrivate::sendMessage(const QByteArray &payload)
{
    if (!socket || socket->state() != QLocalSocket::ConnectedState) {
        qWarning("AppletContext: IPC socket not connected, dropping message");
        return;
    }
    socket->write(payload);
    socket->flush();
}

void AppletContextPrivate::onReadyRead(AppletContext *q)
{
    readBuf.append(socket->readAll());

    // Process as many complete frames as are available.
    while (readBuf.size() >= 4) {
        const quint32 msgLen =
            (static_cast<quint8>(readBuf[0]) << 24) |
            (static_cast<quint8>(readBuf[1]) << 16) |
            (static_cast<quint8>(readBuf[2]) <<  8) |
             static_cast<quint8>(readBuf[3]);

        if (static_cast<int>(msgLen) > readBuf.size() - 4)
            break; // Incomplete frame — wait for more data.

        const QByteArray cborData = readBuf.mid(4, static_cast<int>(msgLen));
        readBuf.remove(0, 4 + static_cast<int>(msgLen));

        // Decode response.
        const QCborValue val = QCborValue::fromCbor(cborData);
        if (!val.isMap())
            continue;
        const QCborMap msg = val.toMap();

        const quint32 seq = static_cast<quint32>(
            msg.value(QStringLiteral("seq")).toInteger());
        const QString method = pendingCalls.take(seq);
        if (method.isEmpty())
            continue;

        const QCborMap bodyMap =
            msg.value(QStringLiteral("body")).toMap();
        QVariantMap result;
        for (auto it = bodyMap.begin(); it != bodyMap.end(); ++it)
            result.insert(it.key().toString(), it.value().toVariant());

        emit q->ipcResponse(method, result);
    }
}

void AppletContextPrivate::onError(AppletContext *q,
                                    QLocalSocket::LocalSocketError err)
{
    Q_UNUSED(q)
    qWarning("AppletContext: IPC socket error %d: %s",
             static_cast<int>(err),
             qUtf8Printable(socket->errorString()));
}

// ── AppletContext public implementation ──────────────────────────────────────

AppletContext::AppletContext(const QString &instanceId,
                             const QString &appletId,
                             QObject *parent)
    : QObject(parent)
    , d(std::make_unique<AppletContextPrivate>())
{
    d->instanceId = instanceId;
    d->appletId   = appletId;

    // Lazily connect to shell-core when the first IPC call is made.
}

AppletContext::~AppletContext() = default;

QString AppletContext::instanceId() const { return d->instanceId; }
QString AppletContext::appletId()   const { return d->appletId;   }
int     AppletContext::screenIndex() const { return d->screenIndex; }

QVariant AppletContext::readConfig(const QString &key,
                                    const QVariant &defaultValue) const
{
    return d->config.value(key, defaultValue);
}

void AppletContext::writeConfig(const QString &key, const QVariant &value)
{
    if (d->config.value(key) == value)
        return;
    d->config.insert(key, value);
    emit configChanged(key);
}

QList<ScreenInfo> AppletContext::screens() const { return d->screens; }

ScreenInfo AppletContext::currentScreen() const
{
    for (const auto &s : d->screens) {
        if (s.index == d->screenIndex)
            return s;
    }
    return {};
}

void AppletContext::sendIpcCall(const QString &method,
                                const QVariantMap &args)
{
    // Connect on first call (lazy init).
    if (!d->socket)
        d->connectToShell(this);

    const quint32 seq = d->nextSeq++;
    d->pendingCalls.insert(seq, method);

    const QByteArray frame = d->encodeRequest(method, args);
    d->sendMessage(frame);
}

} // namespace Lingmo

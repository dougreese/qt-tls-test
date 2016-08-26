#include "sslclient.h"

#include <QList>
#include <QDateTime>
#include <QDebug>

SslClient::SslClient(QString host, int port)
    : host(host), port(port), socket(0)
{
}

SslClient::~SslClient()
{
    if (socket) {
        delete socket;
    }
}

void SslClient::secureConnect(QSslConfiguration &sslConfig, QString host, int port)
{
    if (!host.isEmpty()) {
        this->host = host;
    }
    if (port > 0) {
        this->port = port;
    }
    if (!socket) {
        socket = new QSslSocket(this);
        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
    }

    socket->setSslConfiguration(sslConfig);
    socket->connectToHostEncrypted(this->host, this->port);
}

void SslClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::UnconnectedState) {
        qDebug() << "unconnected.";
        emit done();
    } else if (state == QAbstractSocket::HostLookupState) {
        qDebug() << "looking up host...";
    } else if (state == QAbstractSocket::ConnectingState) {
        qDebug() << "connecting...";
    } else if (state == QAbstractSocket::ConnectedState) {
        qDebug() << "connected.";
    } else if (state == QAbstractSocket::ClosingState) {
        qDebug() << "closing...";
    } else if (state == QAbstractSocket::ListeningState) {
        qDebug() << "listening...";
    }
}

void SslClient::socketEncrypted()
{
    if (!socket)
        return;                 // might have disconnected already

    qDebug() << "Socket is now encrypted.";
    displayCertificateInfo();
    socket->disconnectFromHost();
}

void SslClient::socketError(QAbstractSocket::SocketError)
{
    qWarning() << "Connection error" << socket->errorString();
}

void SslClient::sslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors) {
        qWarning() << "SSL error" << error.errorString();
    }

    // did the socket state change?
    if (socket->state() != QAbstractSocket::ConnectedState)
        socketStateChanged(socket->state());
}

void SslClient::displayCertificateInfo()
{
    QList<QSslCertificate> chain = socket->peerCertificateChain();
    for (int i = 0; i < chain.size(); ++i) {
        QSslCertificate cert = chain[i];
        qDebug() << "Peer certificate chain item:" << i
         << cert.subjectInfo(QSslCertificate::CommonName)
         << cert.subjectInfo(QSslCertificate::Organization)
         << cert.subjectInfo(QSslCertificate::OrganizationalUnitName)
         << cert.expiryDate();
    }
}

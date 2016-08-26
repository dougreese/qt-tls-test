#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QString>
#include <QSslConfiguration>
#include <QSslSocket>

class SslClient : public QObject
{
    Q_OBJECT
public:
    SslClient(QString host = "", int port = 0);
    ~SslClient();

    void secureConnect(QSslConfiguration &sslConfig, QString host = "", int port = 0);

Q_SIGNALS:
    void done();

private Q_SLOTS:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketEncrypted();
    void socketError(QAbstractSocket::SocketError error);
    void sslErrors(const QList<QSslError> &errors);
    void displayCertificateInfo();

private:
    QString host;
    int port;
    QSslSocket *socket;
};


#endif // SSLCLIENT_H

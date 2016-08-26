#include <QCoreApplication>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QDateTime>
#include <QFile>
#include <QByteArray>
#include <QDebug>

#include "sslclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString usage = "<CA cert file> [client cert file] [client key file] [host] [port]";
    QString certFileName;
    QString keyFileName;
    QString host;
    QString port;
    SslClient client;

    if (argc < 2) {
        qDebug() << argv[0] << usage;
        return 1;
    }

    // we have enough to load CA file
    QFile caFile(argv[1]);
    qDebug() << "CA file:" << caFile.fileName();

    // we have enough to load client cert
    if (argc >= 3) {
        certFileName = argv[2];
        qDebug() << "Cert file:" << certFileName;
    }

    // we have enough to load client key
    if (argc >= 4) {
        keyFileName = argv[3];
        qDebug() << "Key file:" << keyFileName;
    }

    if (argc >= 5) {
        // have host
        if (argc < 6) {
            qDebug() << "Need a port to test the host.";
            qDebug() << argv[0] << usage;
            return 1;
        }
        // have both host and port
        host = argv[4];
        port = argv[5];
    }

    QFile certFile(certFileName);
    QFile keyFile(keyFileName);

    QSslKey sslClientKey;
    QSslCertificate sslClientCert;
    QSslCertificate sslCaCert;
    QList<QSslCertificate> sslCaList;
    QSslConfiguration sslConfig;

    if (caFile.exists()) {
        // test CA
        qDebug() << "found CA certificate" << caFile.fileName();
        Q_ASSERT(caFile.open(QIODevice::ReadOnly));

        QSslCertificate ca(&caFile, QSsl::Pem);
        sslCaCert = ca;
        sslCaList.append(sslCaCert);
        sslConfig.setCaCertificates(sslCaList);

        qDebug() << "using CA:"
         << sslCaCert.subjectInfo(QSslCertificate::CommonName)
         << sslCaCert.expiryDate();
#if QT_VERSION < 0x050000
        qDebug() << "CA is valid:" << sslCaCert.isValid();
#endif
    }

    if (certFile.exists()) {
        // test client cert
        qDebug() << "found client certificate" << certFile.fileName();
        Q_ASSERT(certFile.open(QIODevice::ReadOnly));

        QSslCertificate cert(&certFile, QSsl::Pem);
        qDebug() << "cert file not empty" << !cert.isNull();
        sslClientCert = cert;

        if (keyFile.exists()) {
            // test client key
            qDebug() << "found client key" << keyFile.fileName();
            Q_ASSERT(keyFile.open(QIODevice::ReadOnly));
            const QSslKey key(&keyFile, QSsl::Rsa);
            sslClientKey = key;

            qDebug() << "key file not empty" << !sslClientKey.isNull();
            sslConfig.setPrivateKey(sslClientKey);
        }

        qDebug() << "client cert:"
         << sslClientCert.subjectInfo(QSslCertificate::CommonName)
         << sslClientCert.subjectInfo(QSslCertificate::Organization)
         << sslClientCert.subjectInfo(QSslCertificate::OrganizationalUnitName)
         << sslClientCert.expiryDate();
#if QT_VERSION < 0x050000
        qDebug() << "client cert is valid:" << sslClientCert.isValid();
#endif
    }

    if (!host.isEmpty() && !port.isEmpty()) {
        client.secureConnect(sslConfig, host, port.toInt());
        QObject::connect(&client, SIGNAL(done()), &a, SLOT(quit()));
    } else {
        a.exit(0);
    }

    return a.exec();
}

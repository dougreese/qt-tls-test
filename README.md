# qt-tls-test

Qt console application to test SSL/TLS certificates

## Purpose

Simple console app to help with verifying SSL/TLS certificates, particularly self signed certificates.  Originally built to test TLS connections to RabbitMQ in an Embedded Linux environment.

## Building

Built and tested using Qt Creator under OS X using both Qt 4.8.x and 5.7.x.  Not tested at all outside of the environment described.

## Usage

```
./QtTlsTest <CA cert file> [client cert file] [client key file] [host] [port]
```

* If only a CA cert file is provided, that's all that will be verified
* Pass in a client cert file to verify the client cert.
* Pass in a client key file to very the key file is at least not empty.
* Pass in a host and port to attempt a connection using the local CA and client certs.

## Resources

* The SSL client class is somewhat based on the Qt 5 (Secure Socket Client Example)[http://doc.qt.io/qt-5/qtnetwork-securesocketclient-example.html]
* There are many examples of generating self signed certificates out there. (Here is one)[https://www.digitalocean.com/community/tutorials/openssl-essentials-working-with-ssl-certificates-private-keys-and-csrs].
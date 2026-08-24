#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QSslSocket>

class SmtpClient : public QObject
{
    Q_OBJECT

public:
    explicit SmtpClient(const QString &host, int port, const QString &user, const QString &pass, QObject *parent = nullptr);
    void sendMail(const QString &from, const QString &to, const QString &subject, const QString &body);

signals:
    void finished(bool success, const QString &error = QString());

private slots:
    void onConnected();
    void onReadyRead();
    void onEncrypted();
    void onError(QAbstractSocket::SocketError error);

private:
    void sendCommand(const QString &cmd);
    void parseResponse(const QString &response);
    void nextState();

    QSslSocket *socket;
    QString host;
    int port;
    QString user;
    QString password;
    QString from;
    QString to;
    QString subject;
    QString body;
    QStringList commands;
    int commandIndex;
    bool waitingForResponse;
    QByteArray buffer;
    int state;
};

#endif // SMTPCLIENT_H

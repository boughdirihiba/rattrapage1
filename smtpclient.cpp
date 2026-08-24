#include "smtpclient.h"
#include <QDebug>

SmtpClient::SmtpClient(const QString &host, int port, const QString &user, const QString &pass, QObject *parent)
    : QObject(parent), host(host), port(port), user(user), password(pass)
{
    socket = new QSslSocket(this);
    connect(socket, &QSslSocket::connected, this, &SmtpClient::onConnected);
    connect(socket, &QSslSocket::readyRead, this, &SmtpClient::onReadyRead);
    connect(socket, &QSslSocket::encrypted, this, &SmtpClient::onEncrypted);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::error), this, &SmtpClient::onError);
}

void SmtpClient::sendMail(const QString &from, const QString &to, const QString &subject, const QString &body)
{
    this->from = from;
    this->to = to;
    this->subject = subject;
    this->body = body;
    commandIndex = 0;
    state = 0;
    waitingForResponse = false;
    buffer.clear();

    commands.clear();
    commands << "EHLO localhost";
    commands << "AUTH LOGIN";
    commands << QByteArray(user.toUtf8()).toBase64();
    commands << QByteArray(password.toUtf8()).toBase64();
    commands << "MAIL FROM:<" + from + ">";
    commands << "RCPT TO:<" + to + ">";
    commands << "DATA";
    commands << "QUIT";

    socket->connectToHostEncrypted(host, port);
}

void SmtpClient::onConnected() {}
void SmtpClient::onEncrypted() { nextState(); }

void SmtpClient::onReadyRead()
{
    buffer.append(socket->readAll());
    if (buffer.contains("\r\n")) {
        QString response = QString::fromUtf8(buffer);
        buffer.clear();
        parseResponse(response);
    }
}

void SmtpClient::parseResponse(const QString &resp)
{
    int code = resp.left(3).toInt();
    if (code == 235 || code == 250 || code == 334 || code == 354) {
        waitingForResponse = false;
        nextState();
        return;
    }
    if (code >= 500) {
        emit finished(false, "SMTP Error: " + resp.trimmed());
        socket->disconnectFromHost();
        return;
    }
}

void SmtpClient::nextState()
{
    if (waitingForResponse) return;
    if (commandIndex >= commands.size()) {
        emit finished(true);
        socket->disconnectFromHost();
        return;
    }
    QString cmd = commands[commandIndex++];
    if (cmd == "DATA") {
        QString email = "From: " + from + "\r\n"
                                          "To: " + to + "\r\n"
                               "Subject: " + subject + "\r\n"
                                    "Content-Type: text/plain; charset=utf-8\r\n"
                                    "\r\n" + body + "\r\n.\r\n";
        socket->write(email.toUtf8());
    } else {
        socket->write((cmd + "\r\n").toUtf8());
    }
    if (cmd != "QUIT" && !cmd.startsWith("EHLO") && !cmd.startsWith("MAIL") && !cmd.startsWith("RCPT") && cmd != "DATA") {
        waitingForResponse = true;
    }
}

void SmtpClient::onError(QAbstractSocket::SocketError error)
{
    emit finished(false, socket->errorString());
    socket->disconnectFromHost();
}

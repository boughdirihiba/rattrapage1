#include "connexion.h"
#include <QDebug>
#include <QSqlQuery>

Connection::Connection()
{
    // On utilise QODBC (pas ODBC)
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection::~Connection()
{
    if (db.isOpen()) db.close();
}

Connection& Connection::getInstance()
{
    static Connection instance;
    return instance;
}

bool Connection::connecter()
{
    // Chaîne de connexion directe à Oracle XE
    QString dsn = "DRIVER={Oracle in XE};DBQ=localhost:1521/XE;UID=hiba;PWD=Hiba2026;";
    db.setDatabaseName(dsn);

    if (!db.open()) {
        qDebug() << "Erreur de connexion :" << db.lastError().text();
        return false;
    }

    // Petite requête pour prouver que c'est bien Oracle
    QSqlQuery query("SELECT * FROM V$VERSION WHERE ROWNUM = 1", db);
    if (query.next()) {
        qDebug() << "Version Oracle :" << query.value(0).toString();
    }

    qDebug() << "✅ Connexion à Oracle réussie !";
    return true;
}

void Connection::deconnecter()
{
    if (db.isOpen()) db.close();
}

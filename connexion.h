#ifndef CONNEXION_H
#define CONNEXION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class Connection
{
public:
    static Connection& getInstance();
    bool connecter();
    void deconnecter();
    QSqlDatabase& getDatabase() { return db; }

private:
    Connection();
    ~Connection();
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    QSqlDatabase db;
};

#endif // CONNEXION_H

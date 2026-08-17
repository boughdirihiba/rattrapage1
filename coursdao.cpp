#include "coursdao.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool CoursDAO::ajouter(const Cours& c) {
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("INSERT INTO COURS (NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR) "
                  "VALUES (:nom, :duree, :dateDebut, :dateFin, :formateur)");

    query.bindValue(":nom", c.getNom());
    query.bindValue(":duree", c.getDuree());
    query.bindValue(":dateDebut", c.getDateDebut());
    query.bindValue(":dateFin", c.getDateFin());
    query.bindValue(":formateur", c.getFormateur());

    if (!query.exec()) {
        qDebug() << "Erreur SQL (Ajout Cours) :" << query.lastError().text();
        return false;
    }
    return true;
}

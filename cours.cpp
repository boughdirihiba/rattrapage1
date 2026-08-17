#include "cours.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// ===== CONSTRUCTEURS =====
Cours::Cours() {}

Cours::Cours(int id, QString nom, int duree, QDate dateDebut, QDate dateFin, QString formateur)
    : id(id), nom(nom), duree(duree), dateDebut(dateDebut), dateFin(dateFin), formateur(formateur) {}

// ===== GETTERS =====
int Cours::getId() const { return id; }
QString Cours::getNom() const { return nom; }
int Cours::getDuree() const { return duree; }
QDate Cours::getDateDebut() const { return dateDebut; }
QDate Cours::getDateFin() const { return dateFin; }
QString Cours::getFormateur() const { return formateur; }

// ===== SETTERS =====
void Cours::setId(int id) { this->id = id; }
void Cours::setNom(const QString &nom) { this->nom = nom; }
void Cours::setDuree(int duree) { this->duree = duree; }
void Cours::setDateDebut(const QDate &dateDebut) { this->dateDebut = dateDebut; }
void Cours::setDateFin(const QDate &dateFin) { this->dateFin = dateFin; }
void Cours::setFormateur(const QString &formateur) { this->formateur = formateur; }

// ====================================================
// CRUD
// ====================================================

bool Cours::ajouter() {
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("INSERT INTO COURS (ID, NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR) "
                  "VALUES (:id, :nom, :duree, :debut, :fin, :formateur)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":duree", duree);
    query.bindValue(":debut", dateDebut);
    query.bindValue(":fin", dateFin);
    query.bindValue(":formateur", formateur);

    if (!query.exec()) {
        qDebug() << "Erreur ajout cours : " << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Cours::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT ID, NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR FROM COURS");
    if (!query.exec()) {
        qDebug() << "Erreur affichage cours : " << query.lastError().text();
    }
    model->setQuery(query);
    return model;
}

bool Cours::supprimer(int id) {
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("DELETE FROM COURS WHERE ID = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Erreur suppression cours : " << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

bool Cours::modifier() {
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("UPDATE COURS SET NOM=:nom, DUREE=:duree, DATE_DEBUT=:debut, "
                  "DATE_FIN=:fin, FORMATEUR=:formateur WHERE ID=:id");
    query.bindValue(":nom", nom);
    query.bindValue(":duree", duree);
    query.bindValue(":debut", dateDebut);
    query.bindValue(":fin", dateFin);
    query.bindValue(":formateur", formateur);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur modification cours : " << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

// ====================================================
// MÉTIERS
// ====================================================

QSqlQueryModel* Cours::rechercher(const QString &motCle) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT ID, NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR FROM COURS "
                  "WHERE UPPER(NOM) LIKE UPPER(:mot) OR UPPER(FORMATEUR) LIKE UPPER(:mot)");
    query.bindValue(":mot", "%" + motCle + "%");
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Cours::trierParDuree(bool ascendant) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(Connection::getInstance().getDatabase());
    QString ordre = ascendant ? "ASC" : "DESC";
    query.prepare("SELECT ID, NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR FROM COURS "
                  "ORDER BY DUREE " + ordre);
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Cours::filtrerParFormateur(const QString &formateur) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT ID, NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR FROM COURS "
                  "WHERE UPPER(FORMATEUR) LIKE UPPER(:formateur)");
    query.bindValue(":formateur", "%" + formateur + "%");
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Cours::statistiquesDureeMoyenne() {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT 'Durée moyenne' AS INDICATEUR, AVG(DUREE) AS VALEUR FROM COURS");
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Cours::coursEntreDates(const QDate &debut, const QDate &fin) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT ID, NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR FROM COURS "
                  "WHERE DATE_DEBUT >= :debut AND DATE_FIN <= :fin");
    query.bindValue(":debut", debut);
    query.bindValue(":fin", fin);
    query.exec();
    model->setQuery(query);
    return model;
}

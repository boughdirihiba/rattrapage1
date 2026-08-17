#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class Cours
{
private:
    int id;
    QString nom;
    int duree;
    QDate dateDebut;
    QDate dateFin;
    QString formateur;

public:
    Cours();
    Cours(int id, QString nom, int duree, QDate dateDebut, QDate dateFin, QString formateur);

    // Getters
    int getId() const;
    QString getNom() const;
    int getDuree() const;
    QDate getDateDebut() const;
    QDate getDateFin() const;
    QString getFormateur() const;

    // Setters
    void setId(int id);
    void setNom(const QString &nom);
    void setDuree(int duree);
    void setDateDebut(const QDate &dateDebut);
    void setDateFin(const QDate &dateFin);
    void setFormateur(const QString &formateur);

    // ----- CRUD -----
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier();

    // ----- Métiers -----
    QSqlQueryModel* rechercher(const QString &motCle);
    QSqlQueryModel* trierParDuree(bool ascendant);
    QSqlQueryModel* filtrerParFormateur(const QString &formateur);
    QSqlQueryModel* statistiquesDureeMoyenne();
    QSqlQueryModel* coursEntreDates(const QDate &debut, const QDate &fin);
};

#endif // COURS_H

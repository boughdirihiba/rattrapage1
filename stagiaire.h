#ifndef STAGIAIRE_H
#define STAGIAIRE_H

#include <QString>
#include <QDate>

class Stagiaire {
public:
    Stagiaire();

    // Getters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getEmail() const;
    QString getTelephone() const;
    QDate getDateInscription() const;
    QString getNiveau() const;

    // Setters
    void setId(int newId);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setEmail(const QString &email);
    void setTelephone(const QString &telephone);
    void setDateInscription(const QDate &date);
    void setNiveau(const QString &niveau);

private:
    int id;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    QDate dateInscription;
    QString niveau;
};

#endif // STAGIAIRE_H

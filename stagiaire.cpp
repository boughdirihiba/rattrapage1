#include "stagiaire.h"

// Constructeur
Stagiaire::Stagiaire() : id(0) {}

// --- Getters ---
int Stagiaire::getId() const { return id; }
QString Stagiaire::getNom() const { return nom; }
QString Stagiaire::getPrenom() const { return prenom; }
QString Stagiaire::getEmail() const { return email; }
QString Stagiaire::getTelephone() const { return telephone; }
QDate Stagiaire::getDateInscription() const { return dateInscription; }
QString Stagiaire::getNiveau() const { return niveau; }

// --- Setters ---
void Stagiaire::setId(int newId) { id = newId; }
void Stagiaire::setNom(const QString &n) { nom = n; }
void Stagiaire::setPrenom(const QString &p) { prenom = p; }
void Stagiaire::setEmail(const QString &e) { email = e; }
void Stagiaire::setTelephone(const QString &t) { telephone = t; }
void Stagiaire::setDateInscription(const QDate &d) { dateInscription = d; }
void Stagiaire::setNiveau(const QString &n) { niveau = n; }

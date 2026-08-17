#include "courswidget.h"
#include "ui_courswidget.h"
#include "connexion.h"
#include <QMessageBox>
#include <QDate>
#include <QSqlError>

CoursWidget::CoursWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoursWidget)
{
    ui->setupUi(this);

    // Modèle SQL
    model = new QSqlTableModel(this, Connection::getInstance().getDatabase());
    model->setTable("COURS");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableViewCours->setModel(model);

    if (!model->select()) {
        QMessageBox::critical(this, "Erreur", "Erreur chargement table COURS : " + model->lastError().text());
    } else {
        ui->tableViewCours->resizeColumnsToContents();
        ui->tableViewCours->hideColumn(0); // cache l'ID
    }

    // Dates par défaut
    ui->dateDebut->setDate(QDate::currentDate());
    ui->dateFin->setDate(QDate::currentDate().addDays(30));

    // Connexion unique : bouton Ajouter
    connect(ui->btnAjouter, &QPushButton::clicked, this, &CoursWidget::on_btnAjouter_clicked);

    afficherCours();
}

CoursWidget::~CoursWidget()
{
    delete ui;
}

void CoursWidget::afficherCours()
{
    model->select();
    ui->tableViewCours->resizeColumnsToContents();
}

void CoursWidget::viderChamps()
{
    ui->lineEditNom->clear();
    ui->lineEditDuree->clear();
    ui->lineEditFormateur->clear();
}

// =============================================
// SLOT AJOUTER (UNIQUEMENT)
// =============================================
void CoursWidget::on_btnAjouter_clicked()
{
    QString nom = ui->lineEditNom->text().trimmed();
    QString dureeStr = ui->lineEditDuree->text().trimmed();
    QString formateur = ui->lineEditFormateur->text().trimmed();
    QDate dateDebut = ui->dateDebut->date();
    QDate dateFin = ui->dateFin->date();

    // Validations
    if (nom.isEmpty() || dureeStr.isEmpty() || formateur.isEmpty()) {
        QMessageBox::warning(this, "Champs vides", "Tous les champs doivent être remplis !");
        return;
    }

    bool ok;
    int duree = dureeStr.toInt(&ok);
    if (!ok || duree <= 0) {
        QMessageBox::warning(this, "Durée invalide", "La durée doit être un nombre entier positif !");
        return;
    }

    if (dateDebut > dateFin) {
        QMessageBox::warning(this, "Dates invalides", "La date de début doit être avant la date de fin !");
        return;
    }

    // Ajout dans la base via le modèle
    int row = model->rowCount();
    model->insertRow(row);
    // Ordre des colonnes : 0=ID, 1=NOM, 2=DUREE, 3=DATE_DEBUT, 4=DATE_FIN, 5=FORMATEUR
    model->setData(model->index(row, 1), nom);
    model->setData(model->index(row, 2), duree);
    model->setData(model->index(row, 3), dateDebut);
    model->setData(model->index(row, 4), dateFin);
    model->setData(model->index(row, 5), formateur);

    if (!model->submitAll()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout : " + model->lastError().text());
        model->revertAll();
    } else {
        QMessageBox::information(this, "Succès", "Cours ajouté !");
        viderChamps();
        afficherCours();
    }
}

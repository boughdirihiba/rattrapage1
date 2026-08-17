#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAjouter_clicked();
    void on_btnModifier_clicked();
    void on_btnSupprimer_clicked();
    void on_btnRechercher_clicked();
    void on_btnAscendant_clicked();
    void on_btnDescendant_clicked();
    void on_btnExporterPDF_clicked();
    void on_btnStatistiques_clicked();

    // Cours
    void on_btnAjouterCours_clicked();
    void on_btnSupprimerCours_clicked();
    void on_btnExporterPDFCours_clicked();
    void on_btnStatistiquesCours_clicked();
    void on_btnRechercherCours_clicked();
    void on_btnTriCours_clicked();
    void on_btnAscendantCours_clicked();
    void on_btnDescendantCours_clicked();
    void on_btnCours_3_clicked();
    void on_btnStagiaires_4_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QSqlTableModel *modelCours;

    void afficherStagiaires();
    void viderChamps();
    void trier(Qt::SortOrder ordre);
    void trierCours(Qt::SortOrder ordre);    // Déclaration uniquement !
};
#endif // MAINWINDOW_H

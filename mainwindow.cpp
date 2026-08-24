#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connexion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QRegularExpression>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QLocale>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QTextDocument>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QHeaderView>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QSqlQuery>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QRegularExpression>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QRegularExpression>
#include <QFrame>
#include <QProcess>
#include <QTemporaryFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>

// =============================================
// DELEGATE pour afficher l'ID en entier (sans notation scientifique)
// =============================================

class StatusDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;
        QString statut = index.data().toString();
        if (statut == "ACTIF") opt.backgroundBrush = QColor(144, 238, 144);
        else if (statut == "TERMINE") opt.backgroundBrush = QColor(255, 182, 193);
        else if (statut == "ANNULE") opt.backgroundBrush = QColor(211, 211, 211);
        QStyledItemDelegate::paint(painter, opt, index);
    }
};

// =============================================
// CONSTRUCTEUR
// =============================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- MODÈLE SQL POUR LES STAGIAIRES ---
    model = new QSqlTableModel(this, Connection::getInstance().getDatabase());
    model->setTable("STAGIAIRE");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableView->setModel(model);

    // Appliquer le delegate sur la colonne ID (colonne 0)


    model->select();
    ui->tableView->resizeColumnsToContents();

    // --- CONNEXIONS CRUD STAGIAIRES ---
    connect(ui->btnAjouter, &QPushButton::clicked, this, &MainWindow::on_btnAjouter_clicked);
    connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::on_btnModifier_clicked);
    connect(ui->btnSupprimer_2, &QPushButton::clicked, this, &MainWindow::on_btnSupprimer_clicked);

    // --- CONNEXIONS RECHERCHE & TRI ---
    connect(ui->btnRechercher, &QPushButton::clicked, this, &MainWindow::on_btnRechercher_clicked);
    connect(ui->btnAscendant, &QPushButton::clicked, this, &MainWindow::on_btnAscendant_clicked);
    connect(ui->btnDescendant, &QPushButton::clicked, this, &MainWindow::on_btnDescendant_clicked);

    // --- CONNEXIONS PDF & STATISTIQUES ---
    connect(ui->btnExporterPDF, &QPushButton::clicked, this, &MainWindow::on_btnExporterPDF_clicked);
    connect(ui->btnStatistiques, &QPushButton::clicked, this, &MainWindow::on_btnStatistiques_clicked);
    connect(ui->btnReclamation, &QPushButton::clicked, this, &MainWindow::on_btnReclamation_clicked);
    connect(ui->btnContacterFormateur, &QPushButton::clicked, this, &MainWindow::on_btnContacterFormateur_clicked);
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &MainWindow::on_calendarWidget_clicked);
    connect(ui->btnFeedback, &QPushButton::clicked, this, &MainWindow::on_btnFeedback_clicked);
    connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::on_btnModifier_clicked);
    connect(ui->btnModifierCours, &QPushButton::clicked, this, &MainWindow::on_btnModifierCours_clicked);




    // Date par défaut (Stagiaires)
    ui->dateEditInscription_2->setDate(QDate::currentDate());

    // ============================================
    // 🟢 CE QUE VOUS DEVEZ AJOUTER EN PLUS DESSOUS :
    // ============================================

    // --- 1. MODÈLE SQL POUR LES COURS ---
    modelCours = new QSqlTableModel(this, Connection::getInstance().getDatabase());
    modelCours->setTable("COURS");
    modelCours->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableViewCours->setModel(modelCours);
    modelCours->select();
    ui->tableViewCours->resizeColumnsToContents();


    // --- 2. DÉMARRER SUR LA PAGE STAGIAIRE (Page 0) ---
    // Pour être sûr que l'application commence sur les stagiaires au lancement
    ui->stackedWidget->setCurrentIndex(0);
    // Appliquer les couleurs sur la colonne STATUT (colonne 6)

}

// =============================================
// DESTRUCTEUR
// =============================================
MainWindow::~MainWindow()
{
    delete ui;
}

// =============================================
// MÉTHODES PRIVÉES
// =============================================
void MainWindow::afficherStagiaires()
{
    model->select();
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::viderChamps()
{
    ui->lineEditID->clear();
    ui->lineEditNom->clear();
    ui->lineEditPrenom->clear();
    ui->lineEditEmail_2->clear();
    ui->lineEditTelephone->clear();
    ui->comboBoxNiveau->setCurrentIndex(0);
    ui->dateEditInscription_2->setDate(QDate::currentDate());
}

// =============================================
// AJOUTER
// =============================================
void MainWindow::on_btnAjouter_clicked()
{
    QString idStr = ui->lineEditID->text().trimmed();
    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID.");
        return;
    }
    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être un nombre positif.");
        return;
    }

    QSqlQuery check(Connection::getInstance().getDatabase());
    check.prepare("SELECT ID FROM STAGIAIRE WHERE ID = :id");
    check.bindValue(":id", id);
    check.exec();
    if (check.next()) {
        QMessageBox::warning(this, "Erreur", "Cet ID existe déjà.");
        return;
    }

    if (ui->lineEditNom->text().isEmpty() || ui->lineEditPrenom->text().isEmpty() ||
        ui->lineEditEmail_2->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom, prénom et email sont obligatoires.");
        return;
    }

    QRegularExpression regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!regex.match(ui->lineEditEmail_2->text()).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Adresse email invalide.");
        return;
    }

    QString nom = ui->lineEditNom->text().trimmed();
    QString prenom = ui->lineEditPrenom->text().trimmed();
    QString email = ui->lineEditEmail_2->text().trimmed();
    QString telephone = ui->lineEditTelephone->text().trimmed();
    QDate dateIns = ui->dateEditInscription_2->date();
    QString niveau = ui->comboBoxNiveau->currentText();

    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("INSERT INTO STAGIAIRE (ID, NOM, PRENOM, EMAIL, TELEPHONE, DATE_INSCRIPTION, NIVEAU) "
                  "VALUES (:id, :nom, :prenom, :email, :tel, :dateIns, :niveau)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":dateIns", dateIns);
    query.bindValue(":niveau", niveau);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Stagiaire ajouté !");
        afficherStagiaires();
        viderChamps();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout : " + query.lastError().text());
    }
}

// =============================================
// MODIFIER
// =============================================
void MainWindow::on_btnModifier_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un stagiaire.");
        return;
    }

    int id = model->data(model->index(row, 0)).toInt();
    QString nom = ui->lineEditNom->text().trimmed();
    QString prenom = ui->lineEditPrenom->text().trimmed();
    QString email = ui->lineEditEmail_2->text().trimmed();
    QString telephone = ui->lineEditTelephone->text().trimmed();
    QDate dateIns = ui->dateEditInscription_2->date();
    QString niveau = ui->comboBoxNiveau->currentText();

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom, prénom et email sont obligatoires.");
        return;
    }

    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("UPDATE STAGIAIRE SET NOM=:nom, PRENOM=:prenom, EMAIL=:email, "
                  "TELEPHONE=:tel, DATE_INSCRIPTION=:dateIns, NIVEAU=:niveau "
                  "WHERE ID=:id");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":dateIns", dateIns);
    query.bindValue(":niveau", niveau);
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(this, "Succès", "Stagiaire modifié !");
            afficherStagiaires();
            viderChamps();
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune modification.");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification : " + query.lastError().text());
    }
}

// =============================================
// SUPPRIMER
// =============================================
void MainWindow::on_btnSupprimer_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un stagiaire.");
        return;
    }

    int id = model->data(model->index(row, 0)).toInt();
    qDebug() << "ID à supprimer :" << id;

    if (QMessageBox::question(this, "Confirmation",
                              "Supprimer le stagiaire ID " + QString::number(id) + " ?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("DELETE FROM STAGIAIRE WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Échec de la suppression : " + query.lastError().text());
        return;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Erreur", "Aucune ligne supprimée (ID inexistant).");
        return;
    }

    QMessageBox::information(this, "Succès", "Stagiaire supprimé !");
    model->select();
    viderChamps();
}

// =============================================
// RECHERCHE
// =============================================
void MainWindow::on_btnRechercher_clicked()
{
    QString texte = ui->lineEditRecherche->text().trimmed();
    QString critere = ui->comboBoxRecherche->currentText().toUpper();

    if (texte.isEmpty()) {
        model->setFilter("");
        model->select();
        return;
    }

    QString filtre;
    if (critere == "ID") {
        bool ok;
        int id = texte.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "L'ID doit être un nombre.");
            return;
        }
        filtre = QString("ID = %1").arg(id);
    } else if (critere == "NOM") {
        filtre = "UPPER(NOM) LIKE UPPER('%" + texte + "%')";
    } else if (critere == "PRENOM") {
        filtre = "UPPER(PRENOM) LIKE UPPER('%" + texte + "%')";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère inconnu.");
        return;
    }

    model->setFilter(filtre);
    model->select();
}

// =============================================
// TRI
// =============================================
void MainWindow::on_btnAscendant_clicked()
{
    trier(Qt::AscendingOrder);
}

void MainWindow::on_btnDescendant_clicked()
{
    trier(Qt::DescendingOrder);
}

void MainWindow::trier(Qt::SortOrder ordre)
{
    QString critere = ui->comboBoxTri->currentText().toUpper();

    QString colonne;
    if (critere == "ID") {
        colonne = "ID";
    } else if (critere == "NOM") {
        colonne = "NOM";
    } else if (critere == "PRENOM") {
        colonne = "PRENOM";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri inconnu.");
        return;
    }

    int colIndex = model->fieldIndex(colonne);
    if (colIndex >= 0) {
        model->setSort(colIndex, ordre);
        model->select();
        ui->tableView->resizeColumnsToContents();
    }
}

// =============================================
// EXPORTER EN PDF - VERSION XXL (MAX)
// =============================================
void MainWindow::on_btnExporterPDF_clicked()
{
    // 1. Obtenir le chemin du Bureau
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString defaultFileName = desktopPath + "/liste_stagiaires_" + QDate::currentDate().toString("yyyyMMdd") + ".pdf";

    // 2. Demander où enregistrer
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer le PDF",
                                                    defaultFileName,
                                                    "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    // 3. Créer le PDF en PAYSAGE
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setPageMargins(QMarginsF(10, 10, 10, 10));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    // ==========================================
    // 4. HTML avec tout en GRAND
    // ==========================================
    QString html = "<html>"
                   "<head>"
                   "<style>"
                   "body { "
                   "   font-family: Arial, sans-serif; "
                   "   margin: 0px; "
                   "   padding: 0px; "
                   "   width: 100%; "
                   "}"
                   ".title { "
                   "   color: #1A2B4C; "
                   "   font-size: 48pt; "
                   "   font-weight: bold; "
                   "   text-align: center; "
                   "   margin-bottom: 0px; "
                   "   letter-spacing: 3px; "
                   "}"
                   ".subtitle { "
                   "   color: #2C4A7C; "
                   "   font-size: 22pt; "
                   "   font-weight: bold; "
                   "   text-align: center; "
                   "   margin-top: 0px; "
                   "   margin-bottom: 5px; "
                   "}"
                   "hr { "
                   "   border: 3px solid #1A2B4C; "
                   "   margin: 5px 0 10px 0; "
                   "}"
                   ".info-container { "
                   "   display: flex; "
                   "   justify-content: space-between; "
                   "   font-size: 13pt; "
                   "   color: #333333; "
                   "   margin: 5px 0 10px 0; "
                   "   padding: 0 5px; "
                   "}"
                   ".info-left { text-align: left; }"
                   ".info-right { text-align: right; }"
                   "table { "
                   "   width: 100%; "
                   "   border-collapse: collapse; "
                   "   margin-top: 5px; "
                   "   font-size: 14pt; "
                   "   table-layout: fixed; "
                   "}"
                   "th { "
                   "   background-color: #1A2B4C; "
                   "   color: white; "
                   "   padding: 15px 10px; "
                   "   text-align: center; "
                   "   font-weight: bold; "
                   "   border: 2px solid #1A2B4C; "
                   "   font-size: 16pt; "
                   "   word-wrap: break-word; "
                   "}"
                   "td { "
                   "   padding: 15px 10px; "
                   "   border: 2px solid #CCCCCC; "
                   "   text-align: left; "
                   "   font-size: 14pt; "
                   "   word-wrap: break-word; "
                   "}"
                   ".col-id { width: 8%; }"
                   ".col-nom { width: 14%; }"
                   ".col-prenom { width: 14%; }"
                   ".col-email { width: 22%; }"
                   ".col-tel { width: 12%; }"
                   ".col-date { width: 12%; }"
                   ".col-niveau { width: 12%; }"
                   ".row-even { background-color: #FFFFFF; }"
                   ".row-odd { background-color: #F2F2F2; }"
                   ".footer { "
                   "   margin-top: 20px; "
                   "   font-size: 11pt; "
                   "   color: #2C4A7C; "
                   "   text-align: center; "
                   "   font-style: italic; "
                   "   border-top: 2px solid #1A2B4C; "
                   "   padding-top: 10px; "
                   "}"
                   ".page { "
                   "   text-align: right; "
                   "   font-size: 10pt; "
                   "   color: #666666; "
                   "   margin-top: 5px; "
                   "}"
                   "</style>"
                   "</head>"
                   "<body>";

    // --- TITRE ---
    html += "<div class='title'>🏛️ SmartForma</div>";
    html += "<div class='subtitle'>📋 Indicateur des stagiaires</div>";
    html += "<hr>";

    // --- INFOS ---
    html += "<div class='info-container'>";
    html += "   <div class='info-left'>📅 Généré le : " + QDate::currentDate().toString("dd/MM/yyyy") + "</div>";
    html += "   <div class='info-right'>👥 Total : " + QString::number(model->rowCount()) + " stagiaires</div>";
    html += "</div>";

    // --- TABLEAU ---
    html += "<table>";

    // En-têtes
    html += "<tr>";
    for (int col = 0; col < model->columnCount(); ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        QString colClass;
        if (header.contains("ID", Qt::CaseInsensitive)) colClass = "col-id";
        else if (header.contains("NOM", Qt::CaseInsensitive) && !header.contains("PRENOM", Qt::CaseInsensitive)) colClass = "col-nom";
        else if (header.contains("PRENOM", Qt::CaseInsensitive)) colClass = "col-prenom";
        else if (header.contains("EMAIL", Qt::CaseInsensitive)) colClass = "col-email";
        else if (header.contains("TELEPHONE", Qt::CaseInsensitive)) colClass = "col-tel";
        else if (header.contains("DATE", Qt::CaseInsensitive)) colClass = "col-date";
        else if (header.contains("NIVEAU", Qt::CaseInsensitive)) colClass = "col-niveau";
        else colClass = "col-nom";

        html += "<th class='" + colClass + "'>" + header + "</th>";
    }
    html += "</tr>";

    // Données
    for (int row = 0; row < model->rowCount(); ++row) {
        QString rowClass = (row % 2 == 0) ? "row-even" : "row-odd";
        html += "<tr class='" + rowClass + "'>";

        for (int col = 0; col < model->columnCount(); ++col) {
            QString value = model->data(model->index(row, col)).toString();

            // Formatage ID
            if (col == 0) {
                bool ok;
                long long id = value.toLongLong(&ok);
                if (ok) value = QString::number(id);
            }

            // Formatage DATE
            if (col == 5) {
                if (value.contains('T')) {
                    value = value.left(value.indexOf('T'));
                }
                QDate date = QDate::fromString(value, "yyyy-MM-dd");
                if (date.isValid()) {
                    value = date.toString("dd/MM/yyyy");
                }
            }

            html += "<td>" + value + "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";

    // --- PIED DE PAGE ---
    html += "<div class='footer'>© SmartForma - Gestion du Centre de Formation</div>";
    html += "<div class='page'>Page 1</div>";

    html += "</body></html>";

    // ==========================================
    // 5. DESSINER LE PDF
    // ==========================================
    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(QSizeF(pdfWriter.width(), pdfWriter.height()));

    int pageCount = doc.pageCount();
    for (int i = 0; i < pageCount; ++i) {
        if (i > 0) {
            pdfWriter.newPage();
        }
        painter.save();
        painter.translate(0, 0);
        doc.drawContents(&painter, QRectF(0, 0, pdfWriter.width(), pdfWriter.height()));
        painter.restore();
    }

    painter.end();

    // 6. Ouvrir le PDF automatiquement
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

    // 7. Message de succès
    QMessageBox::information(this, "Succès",
                             "✅ PDF généré avec succès !\n\n"
                             "📄 Fichier : " + fileName + "\n\n"
                                              "🏛️ SmartForma - Centre de Formation");
}
// =============================================
// STATISTIQUES PAR NIVEAU (camembert)
// =============================================
void MainWindow::on_btnStatistiques_clicked()
{
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT NIVEAU, COUNT(*) AS NB FROM STAGIAIRE GROUP BY NIVEAU");
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les statistiques : " + query.lastError().text());
        return;
    }

    QList<QPair<QString, int>> data;
    int total = 0;
    while (query.next()) {
        QString niveau = query.value(0).toString();
        int nombre = query.value(1).toInt();
        data.append({niveau, nombre});
        total += nombre;
    }

    if (data.isEmpty()) {
        QMessageBox::information(this, "Info", "Aucun stagiaire dans la base.");
        return;
    }

    QPieSeries *series = new QPieSeries();
    QString resultat = "📊 Répartition par niveau (pourcentage) :\n\n";
    for (const auto &d : data) {
        int pourcentage = (d.second * 100) / total;
        QPieSlice *slice = series->append(d.first, d.second);
        slice->setLabel(QString("%1\n%2%").arg(d.first).arg(pourcentage));
        slice->setLabelVisible(true);
        resultat += d.first + " : " + QString::number(pourcentage) + "% (" + QString::number(d.second) + ")\n";
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des stagiaires par niveau (pourcentages)");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);

    if (!series->slices().isEmpty()) {
        series->slices().first()->setExploded(true);
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques - SmartForma");
    dialog->resize(700, 600);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QLabel *label = new QLabel(resultat);
    label->setStyleSheet("font-size: 14px; padding: 10px; background-color: #F5F5F5; border-radius: 5px;");
    layout->addWidget(label);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(400);
    layout->addWidget(chartView);

    dialog->setLayout(layout);
    dialog->exec();
}
void MainWindow::on_btnCours_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_btnAjouterCours_clicked()
{
    QString idStr = ui->lineEditCoursID->text().trimmed();
    QString nom = ui->lineEditCoursNom->text().trimmed();
    QString duree = ui->lineEditCoursDuree->text().trimmed();
    QDate dateDebut = ui->dateEditCoursDebut->date();
    QDate dateFin = ui->dateEditCoursFin->date();
    QString formateur = ui->lineEditCoursFormateur->text().trimmed();


    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID pour le cours.");
        return;
    }
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom du cours est obligatoire.");
        return;
    }

    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("INSERT INTO COURS (ID, NOM, DUREE, DATE_DEBUT, DATE_FIN, FORMATEUR) "
                  "VALUES (:id, :nom, :duree, :dateDebut, :dateFin, :formateur)");
    query.bindValue(":id", idStr.toInt());
    query.bindValue(":nom", nom);
    query.bindValue(":duree", duree.remove(QRegularExpression("[^0-9]")).toInt());
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);
    query.bindValue(":formateur", formateur);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Cours ajouté avec succès !");
        modelCours->select();
        ui->tableViewCours->resizeColumnsToContents();

        ui->lineEditCoursID->clear();
        ui->lineEditCoursNom->clear();
        ui->lineEditCoursDuree->clear();
        ui->lineEditCoursFormateur->clear();
        ui->dateEditCoursDebut->setDate(QDate::currentDate());
        ui->dateEditCoursFin->setDate(QDate::currentDate());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout : " + query.lastError().text());
    }
}


void MainWindow::on_btnStagiaires_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnSupprimerCours_clicked()
{
    int row = ui->tableViewCours->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un cours dans le tableau.");
        return;
    }

    int id = modelCours->data(modelCours->index(row, 0)).toInt();

    if (QMessageBox::question(this, "Confirmation", "Supprimer le cours ID " + QString::number(id) + " ?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("DELETE FROM COURS WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Cours supprimé !");
        modelCours->select(); // Rafraîchir le tableau
        ui->tableViewCours->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression : " + query.lastError().text());
    }
}
void MainWindow::on_btnExporterPDFCours_clicked()
{
    // 1. Chemin du Bureau
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString defaultFileName = desktopPath + "/liste_cours_" + QDate::currentDate().toString("yyyyMMdd") + ".pdf";

    // 2. Demander où enregistrer
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", defaultFileName, "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    // 3. Créer le PDF en PAYSAGE
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setPageMargins(QMarginsF(10, 10, 10, 10));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    // ==========================================
    // CSS CORRIGÉ : "SmartMarket", Titre centré, Tableau centré et LARGE (100%)
    // ==========================================
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; width: 100%; margin: 0; padding: 0; text-align: center; }"
                   ".title { color: #003399; font-size: 60pt; font-weight: 900; letter-spacing: 4px; margin-bottom: 5px; text-align: center; }"
                   ".subtitle { color: #1A2B4C; font-size: 28pt; font-weight: bold; margin-top: 0px; text-align: center; }"
                   "hr { border: 4px solid #003399; margin: 15px 0 20px 0; }"
                   ".info-container { display: flex; justify-content: center; gap: 50px; font-size: 16pt; color: #333; margin-bottom: 15px; }"
                   "table { width: 100%; table-layout: fixed; border-collapse: collapse; margin: 0 auto; font-size: 18pt; }"
                   "th { background-color: #003399; color: white; padding: 20px 10px; border: 3px solid #003399; font-size: 18pt; text-align: center; }"
                   "td { padding: 18px 10px; border: 2px solid #CCCCCC; text-align: center; font-size: 18pt; }"
                   ".row-even { background-color: #FFFFFF; }"
                   ".row-odd { background-color: #F2F9FF; }"
                   ".footer { margin-top: 30px; font-size: 14pt; color: #003399; text-align: center; border-top: 3px solid #003399; padding-top: 15px; font-weight: bold; }"
                   "</style></head><body>";

    // --- TITRE (SmartMarket) ---
    html += "<div class='title'>🏛️ SmartMarket</div>";
    html += "<div class='subtitle'>📋 Liste des Cours</div><hr>";

    // --- INFOS (Centrées) ---
    html += "<div class='info-container'>";
    html += "<div>📅 Généré le : " + QDate::currentDate().toString("dd/MM/yyyy") + "</div>";
    html += "<div>📚 Total : " + QString::number(modelCours->rowCount()) + " cours</div>";
    html += "</div>";

    // --- TABLEAU LARGE ---
    html += "<table><tr>";
    QStringList headers = {"ID", "NOM", "DUREE", "DATE_DEBUT", "DATE_FIN", "FORMATEUR"};
    for (const QString &h : headers) {
        html += "<th>" + h + "</th>";
    }
    html += "</tr>";

    // --- DONNÉES ---
    for (int row = 0; row < modelCours->rowCount(); ++row) {
        QString rowClass = (row % 2 == 0) ? "row-even" : "row-odd";
        html += "<tr class='" + rowClass + "'>";
        for (int col = 0; col < 6; ++col) {
            QString value = modelCours->data(modelCours->index(row, col)).toString();

            // ✅ CORRECTION DES DATES (Enlève le "T00:00:00.000")
            if (col == 3 || col == 4) {
                if (value.contains('T')) {
                    value = value.split('T').at(0); // On garde juste la partie avant le T
                }
                QDate date = QDate::fromString(value, "yyyy-MM-dd");
                if (date.isValid()) {
                    value = date.toString("dd/MM/yyyy");
                }
            }
            html += "<td>" + value + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    // --- PIED DE PAGE ---
    html += "<div class='footer'>© SmartMarket - Gestion du Centre de Formation</div>";
    html += "</body></html>";

    // 5. Dessiner le PDF
    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(QSizeF(pdfWriter.width(), pdfWriter.height()));

    int pageCount = doc.pageCount();
    for (int i = 0; i < pageCount; ++i) {
        if (i > 0) pdfWriter.newPage();
        painter.save();
        doc.drawContents(&painter, QRectF(0, 0, pdfWriter.width(), pdfWriter.height()));
        painter.restore();
    }
    painter.end();

    // 6. Ouvrir le PDF
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

    QMessageBox::information(this, "Succès", "✅ PDF des cours généré avec succès !");
}
void MainWindow::on_btnStatistiquesCours_clicked()
{
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT NOM, COUNT(*) AS NB FROM COURS GROUP BY NOM");
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les statistiques des cours : " + query.lastError().text());
        return;
    }

    QList<QPair<QString, int>> data;
    int total = 0;
    while (query.next()) {
        QString nom = query.value(0).toString();
        int nb = query.value(1).toInt();
        data.append({nom, nb});
        total += nb;
    }

    if (data.isEmpty()) {
        QMessageBox::information(this, "Info", "Aucun cours dans la base.");
        return;
    }

    QPieSeries *series = new QPieSeries();
    QString resultat = "📊 Répartition des cours par nom (pourcentage) :\n\n";
    for (const auto &d : data) {
        int pourcentage = (d.second * 100) / total;
        QPieSlice *slice = series->append(d.first, d.second);
        slice->setLabel(QString("%1\n%2%").arg(d.first).arg(pourcentage));
        slice->setLabelVisible(true);
        resultat += d.first + " : " + QString::number(pourcentage) + "% (" + QString::number(d.second) + ")\n";
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des cours par nom (pourcentages)");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);

    if (!series->slices().isEmpty()) {
        series->slices().first()->setExploded(true);
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques Cours - SmartForma");
    dialog->resize(700, 600);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QLabel *label = new QLabel(resultat);
    label->setStyleSheet("font-size: 14px; padding: 10px; background-color: #F5F5F5; border-radius: 5px;");
    layout->addWidget(label);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(400);
    layout->addWidget(chartView);

    dialog->setLayout(layout);
    dialog->exec();
}
void MainWindow::on_btnRechercherCours_clicked()
{
    QString texte = ui->lineEditRechercheCours->text().trimmed();
    QString critere = ui->comboBoxRechercheCours->currentText().toUpper();

    // Si le champ de recherche est vide, on affiche tout
    if (texte.isEmpty()) {
        modelCours->setFilter("");
        modelCours->select();
        return;
    }

    QString filtre;
    if (critere == "ID") {
        bool ok;
        int id = texte.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "L'ID doit être un nombre.");
            return;
        }
        filtre = QString("ID = %1").arg(id);
    }
    else if (critere == "NOM" || critere == "FORMATEUR") {
        filtre = "UPPER(" + critere + ") LIKE UPPER('%" + texte + "%')";
    }
    else {
        QMessageBox::warning(this, "Erreur", "Critère de recherche inconnu.");
        return;
    }

    modelCours->setFilter(filtre);
    modelCours->select();
}
void MainWindow::on_btnModifierCours_clicked()
{
    // LIGNE 1 : Vérifier qu’une ligne est sélectionnée dans le tableau
    int row = ui->tableViewCours->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un cours.");
        return;
    }

    // LIGNE 2 : Récupérer l’ID du cours sélectionné
    int id = modelCours->data(modelCours->index(row, 0)).toInt();

    // LIGNE 3 : Récupérer les nouvelles valeurs depuis le formulaire
    QString nom = ui->lineEditCoursNom->text().trimmed();
    QString duree = ui->lineEditCoursDuree->text().trimmed();
    QDate dateDebut = ui->dateEditCoursDebut->date();
    QDate dateFin = ui->dateEditCoursFin->date();
    QString formateur = ui->lineEditCoursFormateur->text().trimmed();

    // LIGNE 4 : (optionnel) si tu as la colonne STATUT, décommente :
    // QString statut = ui->comboBoxCoursStatut->currentText();

    // LIGNE 5 : Vérifier que le nom n’est pas vide
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom du cours est obligatoire.");
        return;
    }

    // LIGNE 6 : Construire la requête UPDATE
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("UPDATE COURS SET NOM=:nom, DUREE=:duree, DATE_DEBUT=:dateDebut, "
                  "DATE_FIN=:dateFin, FORMATEUR=:formateur "
                  // si statut : ", STATUT=:statut "
                  "WHERE ID=:id");

    // LIGNE 7 : Lier les valeurs
    query.bindValue(":nom", nom);
    query.bindValue(":duree", duree.remove(QRegularExpression("[^0-9]")).toInt());
    query.bindValue(":dateDebut", dateDebut);
    query.bindValue(":dateFin", dateFin);
    query.bindValue(":formateur", formateur);
    // si statut : query.bindValue(":statut", statut);
    query.bindValue(":id", id);

    // LIGNE 8 : Exécuter la requête
    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            QMessageBox::information(this, "Succès", "Cours modifié !");
            modelCours->select();                       // Rafraîchir le tableau
            ui->tableViewCours->resizeColumnsToContents();
            viderChampsCours();                         // Vider le formulaire
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune modification.");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification : " + query.lastError().text());
    }
}
// =============================================
// FONCTION DE TRI POUR LES COURS
// =============================================
void MainWindow::trierCours(Qt::SortOrder ordre)
{
    QString critere = ui->comboBoxTriCours->currentText().toUpper();

    QString colonne;
    if (critere == "ID") {
        colonne = "ID";
    } else if (critere == "NOM") {
        colonne = "NOM";
    } else if (critere == "DUREE") {
        colonne = "DUREE";
    } else if (critere == "DATE_DEBUT") {
        colonne = "DATE_DEBUT";
    } else if (critere == "FORMATEUR") {
        colonne = "FORMATEUR";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri inconnu.");
        return;
    }

    int colIndex = modelCours->fieldIndex(colonne);
    if (colIndex >= 0) {
        modelCours->setSort(colIndex, ordre);
        modelCours->select();
        ui->tableViewCours->resizeColumnsToContents();
    }
}

// =============================================
// LES 3 BOUTONS DE TRI
// =============================================
void MainWindow::on_btnTriCours_clicked()
{
    // Le bouton "Tri" du milieu trie par ordre croissant (A -> Z)
    trierCours(Qt::AscendingOrder);
}

void MainWindow::on_btnAscendantCours_clicked()
{
    trierCours(Qt::AscendingOrder);
}

void MainWindow::on_btnDescendantCours_clicked()
{
    trierCours(Qt::DescendingOrder);
}
void MainWindow::on_btnReclamation_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un stagiaire.");
        return;
    }

    int idStagiaire = model->data(model->index(row, 0)).toInt();
    QString nom = model->data(model->index(row, 1)).toString();
    QString prenom = model->data(model->index(row, 2)).toString();

    // Dialogue
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📝 Soumettre une réclamation");
    dialog->setFixedSize(700, 650);
    dialog->setStyleSheet(
        "QDialog { background-color: #f5f7fa; border-radius: 12px; }"
        "QLabel { font-family: 'Segoe UI', sans-serif; }"
        "QLineEdit, QTextEdit { padding: 8px; border: 1px solid #d0d7de; border-radius: 6px; background-color: white; }"
        "QPushButton { padding: 8px 20px; border-radius: 6px; font-weight: bold; }"
        "QTableView { border: 1px solid #d0d7de; border-radius: 6px; background-color: white; }"
        "QHeaderView::section { background-color: #1A2B4C; color: white; padding: 6px; }"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // Titre
    QLabel *titleLabel = new QLabel("📝 Nouvelle réclamation");
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #1A2B4C;");
    mainLayout->addWidget(titleLabel);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #1A2B4C; height: 2px;");
    mainLayout->addWidget(line);

    // Stagiaire
    QLabel *infoStagiaire = new QLabel("👤 " + nom + " " + prenom + " (ID " + QString::number(idStagiaire) + ")");
    infoStagiaire->setStyleSheet("font-size: 12pt; font-weight: bold; color: #2c3e50;");
    mainLayout->addWidget(infoStagiaire);

    // Sujet
    QLabel *labelSujet = new QLabel("Sujet :");
    labelSujet->setStyleSheet("font-weight: bold; font-size: 11pt;");
    mainLayout->addWidget(labelSujet);

    QLineEdit *lineEditSujet = new QLineEdit();
    lineEditSujet->setPlaceholderText("Titre de la réclamation");
    mainLayout->addWidget(lineEditSujet);

    // Description
    QLabel *labelDescription = new QLabel("Description :");
    labelDescription->setStyleSheet("font-weight: bold; font-size: 11pt;");
    mainLayout->addWidget(labelDescription);

    QTextEdit *textEditDescription = new QTextEdit();
    textEditDescription->setPlaceholderText("Décrivez votre réclamation en détail...");
    textEditDescription->setMaximumHeight(120);
    mainLayout->addWidget(textEditDescription);

    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    QPushButton *btnAnnuler = new QPushButton("❌ Annuler");
    btnAnnuler->setStyleSheet("background-color: #e74c3c; color: white;");
    QPushButton *btnSoumettre = new QPushButton("✅ Soumettre");
    btnSoumettre->setStyleSheet("background-color: #1A2B4C; color: white;");
    btnLayout->addWidget(btnAnnuler);
    btnLayout->addWidget(btnSoumettre);
    mainLayout->addLayout(btnLayout);

    // Séparateur
    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("background-color: #b0b0b0; height: 1px;");
    mainLayout->addWidget(line2);

    // Historique des réclamations
    QLabel *histLabel = new QLabel("📜 Historique des réclamations");
    histLabel->setStyleSheet("font-weight: bold; font-size: 12pt; color: #1A2B4C;");
    mainLayout->addWidget(histLabel);

    QTableView *tableViewHist = new QTableView();
    tableViewHist->setAlternatingRowColors(true);
    tableViewHist->setStyleSheet("alternate-background-color: #f8f9fa;");

    QSqlTableModel *modelHist = new QSqlTableModel(dialog, Connection::getInstance().getDatabase());
    modelHist->setTable("RECLAMATION");
    modelHist->setFilter("ID_STAGIAIRE = " + QString::number(idStagiaire));
    modelHist->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelHist->select();
    tableViewHist->setModel(modelHist);
    tableViewHist->resizeColumnsToContents();

    // Renommer les en-têtes
    modelHist->setHeaderData(0, Qt::Horizontal, "ID");
    modelHist->setHeaderData(2, Qt::Horizontal, "Sujet");
    modelHist->setHeaderData(3, Qt::Horizontal, "Description");
    modelHist->setHeaderData(4, Qt::Horizontal, "Date");
    modelHist->setHeaderData(5, Qt::Horizontal, "Statut");
    modelHist->setHeaderData(6, Qt::Horizontal, "Réponse");
    tableViewHist->setColumnHidden(1, true); // Masquer ID_STAGIAIRE

    mainLayout->addWidget(tableViewHist);

    // Connexions
    connect(btnAnnuler, &QPushButton::clicked, dialog, &QDialog::reject);

    connect(btnSoumettre, &QPushButton::clicked, [=]() {
        QString sujet = lineEditSujet->text().trimmed();
        QString description = textEditDescription->toPlainText().trimmed();

        if (sujet.isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Le sujet est obligatoire.");
            return;
        }

        QSqlQuery insert(Connection::getInstance().getDatabase());
        insert.prepare("INSERT INTO RECLAMATION (ID, ID_STAGIAIRE, SUJET, DESCRIPTION, DATE_RECLAMATION, STATUT, REPONSE) "
                       "VALUES (SEQ_RECLAMATION.NEXTVAL, :idStag, :sujet, :desc, SYSDATE, 'EN_ATTENTE', NULL)");
        insert.bindValue(":idStag", idStagiaire);
        insert.bindValue(":sujet", sujet);
        insert.bindValue(":desc", description);

        if (insert.exec()) {
            QMessageBox::information(dialog, "Succès", "✅ Réclamation soumise !");
            modelHist->select();
            tableViewHist->resizeColumnsToContents();
            lineEditSujet->clear();
            textEditDescription->clear();
        } else {
            QMessageBox::critical(dialog, "Erreur", "❌ Échec : " + insert.lastError().text());
        }
    });

    dialog->exec();
    delete dialog;
}
#include <QDesktopServices>
#include <QUrl>
#include <QRegularExpression>

void MainWindow::envoyerEmailBrevo(const QString &destinataire,
                                   const QString &sujet,
                                   const QString &message)
{
    // Identifiants Brevo
    QString apiKey = "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";
    QString expediteur = "boughdirihiba@gmail.com";

    // --- Construction du JSON (méthode compatible) ---
    QJsonObject json;

    // Expéditeur
    QJsonObject sender;
    sender["email"] = expediteur;
    sender["name"] = "SmartForma";
    json["sender"] = sender;

    // Destinataire
    QJsonArray toArray;
    QJsonObject to;
    to["email"] = destinataire;
    toArray.append(to);
    json["to"] = toArray;

    // Sujet et contenu
    json["subject"] = sujet;
    json["htmlContent"] = "<p>" + message + "</p>";
    json["textContent"] = message;

    QByteArray jsonData = QJsonDocument(json).toJson();

    // --- Envoi de la requête HTTP ---
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://api.brevo.com/v3/smtp/email"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("api-key", apiKey.toUtf8());

    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Succès", "✅ Email envoyé avec succès !");
        } else {
            QString error = reply->readAll();
            QMessageBox::critical(this, "Erreur", "❌ Échec : " + error);
        }
        reply->deleteLater();
        manager->deleteLater();
    });

    manager->post(request, jsonData);

}
void MainWindow::on_btnContacterFormateur_clicked()
{
    // Vérifier qu’un stagiaire est sélectionné
    int row = ui->tableView->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un stagiaire.");
        return;
    }

    QString nomStagiaire = model->data(model->index(row, 1)).toString();
    QString prenomStagiaire = model->data(model->index(row, 2)).toString();

    // Créer la fenêtre de dialogue
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📩 Contacter un formateur");
    dialog->resize(500, 450);
    dialog->setStyleSheet("QDialog { background-color: #f5f7fa; border-radius: 12px; }");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(12);
    layout->setContentsMargins(25, 25, 25, 25);

    // Titre
    QLabel *title = new QLabel("📩 Envoyer un message à un formateur");
    title->setStyleSheet("font-size: 16pt; font-weight: bold; color: #1A2B4C;");
    layout->addWidget(title);

    layout->addWidget(new QLabel("👤 " + nomStagiaire + " " + prenomStagiaire));

    layout->addWidget(new QLabel("Email du formateur :"));
    QLineEdit *lineEditEmail = new QLineEdit();
    lineEditEmail->setPlaceholderText("exemple@formateur.com");
    layout->addWidget(lineEditEmail);

    layout->addWidget(new QLabel("Sujet :"));
    QLineEdit *lineEditSujet = new QLineEdit();
    lineEditSujet->setPlaceholderText("Objet du message");
    layout->addWidget(lineEditSujet);

    layout->addWidget(new QLabel("Message :"));
    QTextEdit *textEditMessage = new QTextEdit();
    textEditMessage->setPlaceholderText("Écrivez votre message ici...");
    textEditMessage->setMaximumHeight(150);
    layout->addWidget(textEditMessage);

    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnAnnuler = new QPushButton("❌ Annuler");
    btnAnnuler->setStyleSheet("background-color: #e74c3c; color: white;");
    QPushButton *btnEnvoyer = new QPushButton("✅ Envoyer");
    btnEnvoyer->setStyleSheet("background-color: #1A2B4C; color: white;");
    btnLayout->addWidget(btnAnnuler);
    btnLayout->addWidget(btnEnvoyer);
    layout->addLayout(btnLayout);

    // Connexions
    connect(btnAnnuler, &QPushButton::clicked, dialog, &QDialog::reject);

    connect(btnEnvoyer, &QPushButton::clicked, [=]() {
        QString emailFormateur = lineEditEmail->text().trimmed();
        QString sujet = lineEditSujet->text().trimmed();
        QString message = textEditMessage->toPlainText().trimmed();

        if (emailFormateur.isEmpty() || sujet.isEmpty() || message.isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Tous les champs sont obligatoires.");
            return;
        }

        QString corps = "De : " + nomStagiaire + " " + prenomStagiaire + "\n\n" + message;

        // === Envoi via Brevo ===
        envoyerEmailBrevo(emailFormateur, sujet, corps);
    });

    dialog->exec();
    delete dialog;
}
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    // Vider la liste
    ui->listWidgetCoursJour->clear();

    // Parcourir tous les cours
    QStringList coursDuJour;
    for (int row = 0; row < modelCours->rowCount(); ++row) {
        QDate dateDebut = modelCours->data(modelCours->index(row, 3)).toDate(); // colonne DATE_DEBUT
        QDate dateFin   = modelCours->data(modelCours->index(row, 4)).toDate(); // colonne DATE_FIN

        if (date >= dateDebut && date <= dateFin) {
            QString nom = modelCours->data(modelCours->index(row, 1)).toString(); // colonne NOM
            coursDuJour << nom;
        }
    }

    // Afficher le résultat
    if (coursDuJour.isEmpty()) {
        ui->listWidgetCoursJour->addItem("📅 Aucun cours ce jour.");
    } else {
        ui->listWidgetCoursJour->addItem("📚 Cours du " + date.toString("dd/MM/yyyy") + " :");
        ui->listWidgetCoursJour->addItems(coursDuJour);
    }
}
void MainWindow::on_btnFeedback_clicked()
{
    // 1. Vérifier qu’un cours est sélectionné
    int row = ui->tableViewCours->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un cours.");
        return;
    }

    int idCours = modelCours->data(modelCours->index(row, 0)).toInt();
    QString nomCours = modelCours->data(modelCours->index(row, 1)).toString();

    // 2. Créer la fenêtre de saisie
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("💬 Feedback anonyme - " + nomCours);
    dialog->resize(400, 250);
    dialog->setStyleSheet("QDialog { background-color: #f5f7fa; border-radius: 12px; }");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->setSpacing(12);
    layout->setContentsMargins(25, 25, 25, 25);

    layout->addWidget(new QLabel("Votre avis sur le cours (anonyme) :"));

    QTextEdit *textAvis = new QTextEdit();
    textAvis->setPlaceholderText("Écrivez votre feedback ici...");
    textAvis->setMaximumHeight(150);
    layout->addWidget(textAvis);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnAnnuler = new QPushButton("Annuler");
    btnAnnuler->setStyleSheet("background-color: #e74c3c; color: white;");
    QPushButton *btnEnvoyer = new QPushButton("Envoyer");
    btnEnvoyer->setStyleSheet("background-color: #1A2B4C; color: white;");
    btnLayout->addWidget(btnAnnuler);
    btnLayout->addWidget(btnEnvoyer);
    layout->addLayout(btnLayout);

    connect(btnAnnuler, &QPushButton::clicked, dialog, &QDialog::reject);

    connect(btnEnvoyer, &QPushButton::clicked, [=]() {
        QString avis = textAvis->toPlainText().trimmed();

        if (avis.isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Veuillez écrire un avis.");
            return;
        }

        QSqlQuery insert(Connection::getInstance().getDatabase());
        insert.prepare("INSERT INTO FEEDBACK (ID, ID_COURS, AVIS) "
                       "VALUES (SEQ_FEEDBACK.NEXTVAL, :idCours, :avis)");
        insert.bindValue(":idCours", idCours);
        insert.bindValue(":avis", avis);

        if (insert.exec()) {
            QMessageBox::information(dialog, "Succès", "✅ Feedback envoyé (anonyme) !");
            dialog->accept();

            // Rafraîchir la liste des feedbacks
            chargerFeedback(idCours);
        } else {
            QMessageBox::critical(dialog, "Erreur", "❌ Échec : " + insert.lastError().text());
        }
    });

    dialog->exec();
    delete dialog;
}
void MainWindow::chargerFeedback(int idCours)
{
    ui->listWidgetFeedback->clear();

    // 1. Récupérer le nom du cours
    QSqlQuery coursQuery(Connection::getInstance().getDatabase());
    coursQuery.prepare("SELECT NOM FROM COURS WHERE ID = :idCours");
    coursQuery.bindValue(":idCours", idCours);
    QString nomCours = "Cours inconnu";
    if (coursQuery.exec() && coursQuery.next()) {
        nomCours = coursQuery.value(0).toString();
    }

    // 2. Ajouter un titre avec le nom du cours
    ui->listWidgetFeedback->addItem("📚 Feedbacks pour : " + nomCours);
    ui->listWidgetFeedback->addItem(""); // ligne vide pour séparer

    // 3. Récupérer les feedbacks
    QSqlQuery query(Connection::getInstance().getDatabase());
    query.prepare("SELECT AVIS, DATE_AVIS FROM FEEDBACK WHERE ID_COURS = :idCours ORDER BY DATE_AVIS DESC");
    query.bindValue(":idCours", idCours);
    query.exec();

    int count = 0;
    while (query.next()) {
        QString avis = query.value(0).toString();
        QDate date = query.value(1).toDate();
        ui->listWidgetFeedback->addItem("📝 " + avis + "  (" + date.toString("dd/MM/yyyy") + ")");
        count++;
    }

    if (count == 0) {
        ui->listWidgetFeedback->addItem("Aucun feedback pour ce cours.");
    }
}
void MainWindow::on_tableViewCours_clicked(const QModelIndex &index)
{
    int row = index.row();

    // Remplir le formulaire (sans le statut)
    ui->lineEditCoursID->setText(modelCours->data(modelCours->index(row, 0)).toString());
    ui->lineEditCoursNom->setText(modelCours->data(modelCours->index(row, 1)).toString());
    ui->lineEditCoursDuree->setText(modelCours->data(modelCours->index(row, 2)).toString());
    ui->dateEditCoursDebut->setDate(modelCours->data(modelCours->index(row, 3)).toDate());
    ui->dateEditCoursFin->setDate(modelCours->data(modelCours->index(row, 4)).toDate());
    ui->lineEditCoursFormateur->setText(modelCours->data(modelCours->index(row, 5)).toString());

    // ⚠️ On ne touche pas au statut (comboBoxCoursStatut n'existe pas)
    // ui->comboBoxCoursStatut->setCurrentText(...);  // ← Commentée

    // ✅ Charge les feedbacks pour ce cours
    chargerFeedback(modelCours->data(modelCours->index(row, 0)).toInt());  // ← bien .toInt()
}
void MainWindow::viderChampsCours()
{
    ui->lineEditCoursID->clear();
    ui->lineEditCoursNom->clear();
    ui->lineEditCoursDuree->clear();
    ui->dateEditCoursDebut->setDate(QDate::currentDate());
    ui->dateEditCoursFin->setDate(QDate::currentDate().addDays(30));
    ui->lineEditCoursFormateur->clear();
    // ui->comboBoxCoursStatut->setCurrentIndex(0);  // si tu as la comboBox
}

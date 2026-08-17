#include "mainwindow.h"
#include "connexion.h" // J'ajoute l'include ici
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // On appelle la méthode connecter()
    Connection &c = Connection::getInstance();
    if (!c.connecter()) {
        QMessageBox::critical(nullptr, "Erreur Critique", "Impossible de se connecter à la base de données !");
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}

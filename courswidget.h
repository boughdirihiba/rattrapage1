#ifndef COURSWIDGET_H
#define COURSWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class CoursWidget;
}

class CoursWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CoursWidget(QWidget *parent = nullptr);
    ~CoursWidget();

private slots:
    void on_btnAjouter_clicked();

private:
    Ui::CoursWidget *ui;
    QSqlTableModel *model;

    void afficherCours();
    void viderChamps();
};

#endif // COURSWIDGET_H

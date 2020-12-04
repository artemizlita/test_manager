#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QtWidgets>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int tasks_count = 0, tests_count = 0, current_test = 1;
    QString contest_name, task_name;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotAction();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void load_contest(QString);

    void import_task(QString, QString);

    void change_condition(QString);

private:
    Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_H

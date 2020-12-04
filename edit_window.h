#ifndef EDIT_WINDOW_H
#define EDIT_WINDOW_H

#include <QDialog>
#include <QtWidgets>
#include <QtGui>

namespace Ui {
class edit_window;
}

class edit_window : public QDialog
{
    Q_OBJECT

public:
    explicit edit_window(QWidget *parent = 0);
    ~edit_window();

    void initialize();

private:
    int tests_count = 0;
    QString contest_name, task_name;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_tabWidget_2_tabBarClicked(int index);

signals:
    void signal_condition(QString);

private:
    Ui::edit_window *ui;
};

#endif // EDIT_WINDOW_H

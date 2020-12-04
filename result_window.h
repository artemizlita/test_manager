#ifndef RESULT_WINDOW_H
#define RESULT_WINDOW_H

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class result_window;
}

class result_window : public QDialog
{
    Q_OBJECT

public:
    explicit result_window(QWidget *parent = 0);
    ~result_window();

    void initialize();
    void getAdress(QString adress);
    void getComboBox(int num);

    int tests_count = 0;
    QString contest_name, task_name, adress, Cplus_compiler_adress, Python_compiler_adress;
    int pl;

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::result_window *ui;
};

#endif // RESULT_WINDOW_H

#ifndef LOAD_WINDOW_H
#define LOAD_WINDOW_H

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class load_window;
}

class load_window : public QDialog
{
    Q_OBJECT

public:
    bool task_import;

    void import(bool task_import);

    void initialize();

    explicit load_window(QWidget *parent = 0);
    ~load_window();

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

signals:
    void signal_load(QString);

    void signal_import(QString, QString);

private:
    Ui::load_window *ui;
};

#endif // LOAD_WINDOW_H

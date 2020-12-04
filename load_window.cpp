#include "load_window.h"
#include "ui_load_window.h"

load_window::load_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::load_window)
{
    ui->setupUi(this);
}

void load_window::import(bool task_import_from_main) {
    task_import = task_import_from_main;
}

void load_window::initialize() {
    setFixedSize(430, 360);
    ui->lineEdit->setReadOnly(1);

    QStringList contests = QDir(qApp->applicationDirPath() + "/contests").entryList(QDir::Dirs);
    contests.removeAt(0);
    contests.removeAt(0);
    for (int i = 0; i < contests.size(); ++i) {
        ui->listWidget->addItem(contests[i]);
    }
    if (task_import) {
        ui->pushButton->setText("Импортировать задачу");
    } else {
        ui->pushButton->setText("Загрузить состязание");
    }

    if (contests.size() > 0) {
        ui->listWidget->setCurrentRow(0);
        ui->lineEdit->setText(ui->listWidget->item(0)->text());
        QStringList tasks = QDir(qApp->applicationDirPath() + "/contests/" + ui->listWidget->item(0)->text()).entryList(QDir::Dirs);
        tasks.removeAt(0);
        tasks.removeAt(0);
        for (int i = 0; i < tasks.size(); ++i) {
            ui->listWidget_2->addItem(tasks[i]);
            if (tasks.size() > 0) {
                ui->listWidget_2->setCurrentRow(0);
            }
        }
    }
}

load_window::~load_window()
{
    delete ui;
}

void load_window::on_listWidget_clicked(const QModelIndex &index)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление",
                              "Вы действительно хотите удалить " + ui->listWidget->item(index.row())->text() + "?",
                              QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QDir(qApp->applicationDirPath() + "/contests/" + ui->listWidget->item(index.row())->text()).removeRecursively();

            delete ui->listWidget->takeItem(index.row());
        } else if (reply == QMessageBox::No) {
            ui->listWidget->setCurrentRow(index.row());
        }
    } else {
        QStringList tasks = QDir(qApp->applicationDirPath() + "/contests/" + ui->listWidget->item(index.row())->text()).entryList(QDir::Dirs);
        tasks.removeAt(0);
        tasks.removeAt(0);
        ui->lineEdit->setText(ui->listWidget->item(index.row())->text());
        ui->listWidget_2->clear();
        for (int i = 0; i < tasks.size(); ++i) {
            ui->listWidget_2->addItem(tasks[i]);
            if (tasks.size() > 0) {
                ui->listWidget_2->setCurrentRow(0);
            }
        }
    }
}

void load_window::on_pushButton_clicked()
{
    QString contest = ui->listWidget->currentItem()->text();
    if (task_import) {
        QString task = ui->listWidget_2->currentItem()->text();
        emit signal_import(contest, task);
    } else {
        emit signal_load(contest);
    }
}

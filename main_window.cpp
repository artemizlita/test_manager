#include "main_window.h"
#include "ui_main_window.h"
#include "edit_window.h"
#include "result_window.h"
#include "load_window.h"
#include "ui_load_window.h"
#include "options_window.h"
#include "ui_options_window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(1);
    ui->textEdit->setReadOnly(1);

    connect(ui->action_contest_load, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->action_contest_new, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->action_contest_save, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->action_contest_exit, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->action_options_compilers, SIGNAL(triggered()), this, SLOT(slotAction()));
    connect(ui->action_options_help, SIGNAL(triggered()), this, SLOT(slotAction()));

    qDebug()<<qApp->applicationDirPath();

    //QStringList contest = QDir("data").entryList(QDir::Dirs);
    QStringList contest = QDir(qApp->applicationDirPath() + "/data").entryList(QDir::Dirs);

    //for (int i = 0; i < contest.size(); ++i) {
    //    qDebug()<<contest[i];
    //}

    contest_name = contest[2];
    setWindowTitle(contest_name);
    setFixedSize(1020, 530);

    QStringList dirs = QDir(qApp->applicationDirPath() + "/data/" + contest_name).entryList(QDir::Dirs);
    dirs.removeAt(0);
    dirs.removeAt(0);

    for (int i = 0; i < dirs.size(); ++i) {
        ++tasks_count;
        ui->listWidget->addItem(dirs[i]);
    }

    if (dirs.size() > 0) {
        ui->listWidget->setCurrentRow(0);
        task_name = ui->listWidget->item(0)->text();

        QFile file (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + task_name + ".txt");
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        ui->textEdit->setText(stream.readAll());
        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAction() {
    if (sender() == ui->action_contest_load) {
        load_window *load;
        load = new load_window;
        connect(load, SIGNAL(signal_load(QString)), this, SLOT(load_contest(QString)));
        load->setWindowTitle("Загрузить состязание");
        load->import(0);
        load->initialize();
        load->exec();
    }
    if (sender() == ui->action_contest_new) {
        bool ok;
        QString contest = QInputDialog::getText(this, tr("Новое состязание"),
                                tr("Введите название состязания:"), QLineEdit::Normal,
                                "", &ok);

        QStringList dirs = QDir(qApp->applicationDirPath() + "/contests").entryList(QDir::Dirs);
        dirs.removeAt(0);
        dirs.removeAt(0);

        bool there_is = 0;
        for (int i = 0; i < dirs.size(); ++i) {
            if (dirs[i] == contest) {
                there_is = 1;
            }
        }

        bool true_name = 1;
        for (int i = 0; i < contest.size(); ++i) {
            if (!(contest[i].isLetterOrNumber() || contest[i] == '_')) {
                true_name = 0;
            }
            QChar check = contest.at(i);
            if (check.unicode() >= 1040 && check.unicode() <= 1103 ||
                check.unicode() == 1025 || check.unicode() == 1105 ) {
                true_name = 0;
            }
        }

        if (ok && !contest.isEmpty() && !there_is && true_name) {
            setWindowTitle(contest);
            QDir("data/" + contest_name).removeRecursively();
            ui->listWidget->clear();
            ui->textEdit->clear();
            contest_name = contest;
            QDir().mkdir("data/" + contest_name);
        } else if (ok) {
            QMessageBox::about(this,"Неправильный формат ввода",
                               "Название состязания должно состоять из латинских букв, цифр и символа '_'");
        }
    }
    if (sender() == ui->action_contest_save) {
        QDir(qApp->applicationDirPath() + "/contests/" + contest_name).removeRecursively();
        QDir().mkdir(qApp->applicationDirPath() + "/contests/" + contest_name);
        for (int i = 0; i < ui->listWidget->count(); ++i) {
            QString task_name = ui->listWidget->item(i)->text();

            QDir().mkdir(qApp->applicationDirPath() + "/contests/" + contest_name + "/" + task_name);
            QStringList tests = QDir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name).entryList(QDir::Dirs);
            tests.removeAt(0);
            tests.removeAt(0);
            QFile().copy(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + task_name + ".txt",
                         qApp->applicationDirPath() + "/contests/" + contest_name + "/" + task_name + "/" + task_name + ".txt");

            for (int j = 0; j < tests.size(); ++j) {
                QDir().mkdir(qApp->applicationDirPath() + "/contests/" + contest_name + "/" + task_name + "/" + tests[j]);
                QFile().copy(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + tests[j] + "/in.txt",
                             qApp->applicationDirPath() + "/contests/" + contest_name + "/" + task_name + "/" + tests[j] + "/in.txt");
                QFile().copy(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + tests[j] + "/res.txt",
                             qApp->applicationDirPath() + "/contests/" + contest_name + "/" + task_name + "/" + tests[j] + "/res.txt");
            }
        }
        QMessageBox::about(this,"Сохранение",
                           "Состязание успешно сохранено!");
    }
    if (sender() == ui->action_contest_exit) {
        qApp->quit();
    }
    if (sender() == ui->action_options_compilers) {
        options_window options;
        options.setWindowTitle("Адреса компиляторов (через /!)");
        options.exec();
    }
    if (sender() == ui->action_options_help) {
        QProcess* proc_help = new QProcess(this);
        proc_help->start("notepad " + qApp->applicationDirPath() + "/help.txt");
    }
}

void MainWindow::on_pushButton_clicked()
{
    bool ok;
    QString task_name = QInputDialog::getText(this, tr("Новая задача"),
                            tr("Введите название задачи:"), QLineEdit::Normal,
                            "", &ok);

    bool there_is = 0;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if (task_name == ui->listWidget->item(i)->text()) {
            there_is = 1;
        }
    }

    bool true_name = 1;
    for (int i = 0; i < task_name.size(); ++i) {
        if (!(task_name[i].isLetterOrNumber() || task_name[i] == '_')) {
            true_name = 0;
        }
        QChar check = task_name.at(i);
        if (check.unicode() >= 1040 && check.unicode() <= 1103 ||
            check.unicode() == 1025 || check.unicode() == 1105 ) {
            true_name = 0;
        }
    }

    if (ok && !task_name.isEmpty() && !there_is && true_name) {
        QListWidgetItem* item = new QListWidgetItem(task_name);
        ui->listWidget->addItem(item);
        tasks_count++;
        ui->listWidget->setCurrentRow(tasks_count - 1);

        QDir().mkdir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name);
        ui->comboBox->addItem(task_name);

        QFile file (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + task_name + ".txt");
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << "";
        file.close();

        edit_window *edit;
        edit = new edit_window;
        connect(edit, SIGNAL(signal_condition(QString)), this, SLOT(change_condition(QString)));
        edit->setWindowTitle(task_name);
        edit->initialize();
        edit->exec();
    } else if (ok) {
        QMessageBox::about(this,"Неправильный формат ввода",
                           "Название задачи должно состоять из латинских букв, цифр и символа '_'");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    //up
}

void MainWindow::on_pushButton_3_clicked()
{
    //down
}

void MainWindow::on_pushButton_4_clicked()
{
    load_window *load;
    load = new load_window;
    connect(load, SIGNAL(signal_import(QString, QString)), this, SLOT(import_task(QString, QString)));
    load->setWindowTitle("Загрузить состязание");
    load->import(1);
    load->initialize();
    load->exec();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString fName = QFileDialog::getOpenFileName(this, tr("Open File"), "./"/*, tr("matr(*.cpp)")*/);
    ui->textEdit_2->setText(fName);
}

void MainWindow::on_pushButton_6_clicked()
{
    QString name_cpp = ui->textEdit_2->toPlainText();

    if (name_cpp.size() > 4) {
        if (name_cpp.right(4) == ".cpp" or name_cpp.right(3) == ".py") {
            result_window result;
            result.setWindowTitle(ui->listWidget->currentItem()->text());
            result.getAdress(ui->textEdit_2->toPlainText());
            result.getComboBox(ui->comboBox->currentIndex());
            result.initialize();
            result.exec();
        }
    }
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление",
                              "Вы действительно хотите удалить " + ui->listWidget->item(index.row())->text() + "?",
                              QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            --tasks_count;

            QDir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + ui->listWidget->item(index.row())->text()).removeRecursively();
            ui->comboBox->removeItem(ui->listWidget->currentRow());
            delete ui->listWidget->takeItem(index.row());
        } else if (reply == QMessageBox::No) {
            ui->listWidget->setCurrentRow(index.row());
        }
    } else {
        ui->textEdit->clear();
        task_name = ui->listWidget->item(index.row())->text();

        QFile file (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + task_name + ".txt");
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        ui->textEdit->setText(stream.readAll());
        file.close();

        QStringList tests = QDir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name).entryList(QDir::Dirs);
        tests.removeAt(0);
        tests.removeAt(0);
        tests_count = tests.size();
    }
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    edit_window *edit;
    edit = new edit_window;
    connect(edit, SIGNAL(signal_condition(QString)), this, SLOT(change_condition(QString)));
    edit->setWindowTitle(ui->listWidget->item(index.row())->text());
    edit->initialize();
    edit->exec();
}

void MainWindow::load_contest(QString contest) {
    QDir(qApp->applicationDirPath() + "/data/" + contest_name).removeRecursively();
    contest_name = contest;
    setWindowTitle(contest_name);
    QDir().mkdir(qApp->applicationDirPath() + "/data/" + contest);

    QStringList dirs = QDir(qApp->applicationDirPath() + "/contests/" + contest).entryList(QDir::Dirs);
    dirs.removeAt(0);
    dirs.removeAt(0);

    tasks_count = 0;
    tests_count = 0;
    current_test = 1;

    ui->listWidget->clear();
    ui->textEdit->clear();

    for (int i = 0; i < dirs.size(); ++i) {
        QDir().mkdir(qApp->applicationDirPath() + "/data/" + contest + "/" + dirs[i]);
        ++tasks_count;
        ui->listWidget->addItem(dirs[i]);

        QFile().copy(qApp->applicationDirPath() + "/contests/" + contest + "/" + dirs[i] + "/" + dirs[i] + ".txt",
                     qApp->applicationDirPath() + "/data/" + contest + "/" + dirs[i] + "/" + dirs[i] + ".txt");

        QStringList tests = QDir(qApp->applicationDirPath() + "/contests/" + contest + "/" + dirs[i]).entryList(QDir::Dirs);
        tests.removeAt(0);
        tests.removeAt(0);

        for (int j = 0; j < tests.size(); ++j) {
            QDir().mkdir(qApp->applicationDirPath() + "/data/" + contest + "/" + dirs[i] + "/" + tests[j]);
            QFile().copy(qApp->applicationDirPath() + "/contests/" + contest + "/" + dirs[i] + "/" + tests[j] + "/in.txt",
                         qApp->applicationDirPath() + "/data/" + contest + "/" + dirs[i] + "/" + tests[j] + "/in.txt");
            QFile().copy(qApp->applicationDirPath() + "/contests/" + contest + "/" + dirs[i] + "/" + tests[j] + "/res.txt",
                         qApp->applicationDirPath() + "/data/" + contest + "/" + dirs[i] + "/" + tests[j] + "/res.txt");
        }
    }
}

void MainWindow::import_task(QString contest, QString task) {
    QDir().mkdir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task);

    QStringList dirs = QDir(qApp->applicationDirPath() + "/contests/" + contest + "/" + task).entryList(QDir::Dirs);
    dirs.removeAt(0);
    dirs.removeAt(0);
    ++tasks_count;
    ui->listWidget->addItem(task);
    ui->comboBox->addItem(task);

    QFile().copy(qApp->applicationDirPath() + "/contests/" + contest + "/" + task + "/" + task + ".txt",
                 qApp->applicationDirPath() + "/data/" + contest_name + "/" + task + "/" + task + ".txt");

    QStringList tests = QDir(qApp->applicationDirPath() + "/contests/" + contest + "/" + task).entryList(QDir::Dirs);
    tests.removeAt(0);
    tests.removeAt(0);

    for (int i = 0; i < tests.size(); ++i) {
        QDir().mkdir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task + "/" + tests[i]);
        QFile().copy(qApp->applicationDirPath() + "/contests/" + contest + "/" + task + "/" + tests[i] + "/in.txt",
                     qApp->applicationDirPath() + "/data/" + contest_name + "/" + task + "/" + tests[i] + "/in.txt");
        QFile().copy(qApp->applicationDirPath() + "/contests/" + contest + "/" + task + "/" + tests[i] + "/res.txt",
                     qApp->applicationDirPath() + "/data/" + contest_name + "/" + task + "/" + tests[i] + "/res.txt");
    }
}

void MainWindow::change_condition(QString condition) {
    ui->textEdit->setText(condition);
}

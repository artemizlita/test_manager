#include "edit_window.h"
#include "ui_edit_window.h"

edit_window::edit_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_window)
{
    ui->setupUi(this);
}

void edit_window::initialize() {
    setFixedSize(820, 520);
    ui->textEdit_2->setReadOnly(1);
    ui->textEdit_3->setReadOnly(1);

    QStringList contest = QDir(qApp->applicationDirPath() + "/data").entryList(QDir::Dirs);
    contest_name = contest[2];

    task_name = windowTitle();

    QStringList dirs = QDir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name).entryList(QDir::Dirs);
    dirs.removeAt(0);
    dirs.removeAt(0);
    tests_count = dirs.size();

    for (int i = 0; i < dirs.size(); ++i) {
        ui->listWidget->addItem(dirs[i]);
    }

    ui->textEdit->clear();
    QFile file (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + task_name + ".txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    ui->textEdit->setText(stream.readAll());
    file.close();

    if (dirs.size() > 0) {
        ui->listWidget->setCurrentRow(0);

        QString name = ui->listWidget->item(0)->text();

        int string_count = 0;
        QFile file1 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/in.txt");
        file1.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream1(&file1);
        //ui->textEdit_2->setText(stream1.readAll());
        while (string_count < 100 && !stream1.atEnd()) {
            ui->textEdit_2->append(stream1.readLine());
            ++string_count;
        }
        if (string_count >= 100) {
            ui->textEdit_2->append("...");
        }
        file1.close();

        string_count = 0;
        QFile file2 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/res.txt");
        file2.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream2(&file2);
        //ui->textEdit_3->setText(stream2.readAll());
        while (string_count < 100 && !stream2.atEnd()) {
            ui->textEdit_3->append(stream2.readLine());
            ++ string_count;
        }
        if (string_count >= 100) {
            ui->textEdit_3->append("...");
        }
        file2.close();
    }
}

edit_window::~edit_window()
{
    delete ui;
}

void edit_window::on_pushButton_clicked()
{
    QFile file (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + task_name + ".txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();

    emit signal_condition(ui->textEdit->toPlainText());
}

void edit_window::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
    QFile file (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + task_name + ".txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    ui->textEdit->setText(stream.readAll());
    file.close();
}

void edit_window::on_pushButton_3_clicked()
{
    bool ok;
    QString test_name = QInputDialog::getText(this, tr("Новый тест"),
                            tr("Введите название теста:"), QLineEdit::Normal,
                            "", &ok);

    bool there_is = 0;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        if (test_name == ui->listWidget->item(i)->text()) {
            there_is = 1;
        }
    }

    bool true_name = 1;
    for (int i = 0; i < test_name.size(); ++i) {
        if (!(test_name[i].isLetterOrNumber() || test_name[i] == '_')) {
            true_name = 0;
        }
        QChar check = test_name.at(i);
        if (check.unicode() >= 1040 && check.unicode() <= 1103 ||
            check.unicode() == 1025 || check.unicode() == 1105 ) {
            true_name = 0;
        }
    }

    if (ok && !test_name.isEmpty() && !there_is && true_name) {
        ui->textEdit_2->clear();
        ui->textEdit_3->clear();

        QStringList contest = QDir(qApp->applicationDirPath() + "/data").entryList(QDir::Dirs);
        contest_name = contest[2];

        ++tests_count;

        QListWidgetItem* item = new QListWidgetItem(test_name);
        ui->listWidget->addItem(item);

        QDir().mkdir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name);

        QFile file1 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/in.txt");
        file1.open(QIODevice::WriteOnly);
        QTextStream out1(&file1);
        out1 << "";
        file1.close();

        QFile file2 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/res.txt");
        file2.open(QIODevice::WriteOnly);
        QTextStream out2(&file2);
        out2 << "";
        file2.close();

        ui->listWidget->setCurrentRow(tests_count - 1);
    } else if (ok) {
        QMessageBox::about(this,"Неправильный формат ввода",
                           "Название теста должно состоять из латинских букв, цифр и символа '_'");
    }
}

void edit_window::on_pushButton_4_clicked()
{
    //up
}

void edit_window::on_pushButton_5_clicked()
{
    //down
}

void edit_window::on_pushButton_6_clicked()
{
    ui->tabWidget_2->setCurrentIndex(0);
    QString name = ui->listWidget->currentItem()->text();
    QProcess* proc = new QProcess(this);
    proc->start("notepad " + qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/in.txt");
    ui->pushButton_8->setEnabled(1);
}

void edit_window::on_pushButton_7_clicked()
{
    ui->tabWidget_2->setCurrentIndex(0);
    QString name = ui->listWidget->currentItem()->text();
    QProcess* proc = new QProcess(this);
    proc->start("notepad " + qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/res.txt");
    ui->pushButton_8->setEnabled(1);
}

void edit_window::on_pushButton_8_clicked()
{
    /*if (tests_count > 0) {
        QString name = ui->listWidget->currentItem()->text();

        QFile file1 ("./debug/data/" + contest_name + "/" + task_name + "/" + name + "/in.txt");
        file1.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream1(&file1);
        ui->textEdit_2->setText(stream1.readAll());
        file1.close();

        QFile file2 ("./debug/data/" + contest_name + "/" + task_name + "/" + name + "/res.txt");
        file2.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream2(&file2);
        ui->textEdit_3->setText(stream2.readAll());
        file2.close();

        ui->pushButton_8->setEnabled(0);
    }*/
}

void edit_window::on_listWidget_clicked(const QModelIndex &index)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удаление",
                              "Вы действительно хотите удалить " + ui->listWidget->item(index.row())->text() + "?",
                              QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QDir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + ui->listWidget->item(index.row())->text()).removeRecursively();

            delete ui->listWidget->takeItem(index.row());
            --tests_count;
        } else if (reply == QMessageBox::No) {
            ui->listWidget->setCurrentRow(index.row());
        }
    } else {
        QString name = ui->listWidget->item(index.row())->text();

        ui->textEdit_2->clear();
        ui->textEdit_3->clear();

        QFile file1 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/in.txt");
        file1.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream1(&file1);
        //ui->textEdit_2->setText(stream1.readAll());
        int string_count = 0;
        while (string_count < 100 && !stream1.atEnd()) {
            ui->textEdit_2->append(stream1.readLine());
            ++string_count;
        }
        if (string_count >= 100) {
            ui->textEdit_2->append("...");
        }
        file1.close();

        QFile file2 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/res.txt");
        file2.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream2(&file2);
        //ui->textEdit_3->setText(stream2.readAll());
        string_count = 0;
        while (string_count < 100 && !stream2.atEnd()) {
            ui->textEdit_3->append(stream2.readLine());
            ++string_count;
        }
        if (string_count >= 100) {
            ui->textEdit_3->append("...");
        }
        file2.close();

        ui->pushButton_8->setEnabled(0);
    }
}

void edit_window::on_tabWidget_2_tabBarClicked(int index)
{
    QString name = ui->listWidget->currentItem()->text();
    if (index == 1) {
        ui->textEdit_2->clear();
        QFile file1 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/in.txt");
        file1.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream1(&file1);
        //ui->textEdit_2->setText(stream1.readAll());
        int string_count = 0;
        while (string_count < 100 && !stream1.atEnd()) {
            ui->textEdit_2->append(stream1.readLine());
            ++string_count;
        }
        if (string_count >= 100) {
            ui->textEdit_2->append("...");
        }
        file1.close();
    } else if (index == 2) {
        ui->textEdit_3->clear();
        QFile file2 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + name + "/res.txt");
        file2.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream2(&file2);
        //ui->textEdit_3->setText(stream2.readAll());
        int string_count = 0;
        while (string_count < 100 && !stream2.atEnd()) {
            ui->textEdit_3->append(stream2.readLine());
            ++string_count;
        }
        if (string_count >= 100) {
            ui->textEdit_3->append("...");
        }
        file2.close();
    }
}

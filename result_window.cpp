#include "result_window.h"
#include "ui_result_window.h"

result_window::result_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::result_window)
{
    ui->setupUi(this);
    QFile file (qApp->applicationDirPath() + "/compilers.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    Cplus_compiler_adress = stream.readLine();
    Python_compiler_adress = stream.readLine();
    file.close();
}

void result_window::getAdress(QString adress_from_main) {
    adress = adress_from_main;
}

void result_window::getComboBox(int num) {
    pl = num;
}

void result_window::initialize() {
    setFixedSize(630, 600);
    ui->textEdit->setReadOnly(1);
    ui->textEdit_2->setReadOnly(1);
    ui->textEdit_3->setReadOnly(1);
    ui->lineEdit->setReadOnly(1);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 14pt; }");
    ui->tableWidget->setColumnWidth(0, 183);
    ui->tableWidget->setColumnWidth(1, 289);
    ui->tableWidget->setColumnWidth(2, 100);
    //ui->tableWidget->setColumnWidth(3, 100);

    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 14pt; }");
    ui->tableWidget_2->setColumnWidth(0, 200);
    ui->tableWidget_2->setColumnWidth(1, 289);
    ui->tableWidget_2->setColumnWidth(2, 100);
    //ui->tableWidget_2->setColumnWidth(3, 100);

    QStringList contest = QDir(qApp->applicationDirPath() + "/data").entryList(QDir::Dirs);
    contest_name = contest[2];

    task_name = windowTitle();

    QStringList tests = QDir(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name).entryList(QDir::Dirs);
    tests.removeAt(0);
    tests.removeAt(0);
    tests_count = tests.size();

    //qDebug()<<"./debug/data/" + contest_name + "/" + task_name;

    if (pl == 0) { //C++

        for (int i = 0; i < tests_count; ++i) {
            QString cmd;
            QString cd = "cd " + qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + tests[i];
            QString tilda = " & ";
            QString g_plus = "C:/Qt/Qt5.3.1/Tools/mingw482_32/bin/g++ "; //g++
            QString param_o = " -o ";
            QString file_name = "prog";
            cmd = cd + tilda + g_plus + adress + param_o + file_name;
            cmd.replace("/", "\\");

            system(cmd.toStdString().c_str());

            QString exe = ".exe";
            cmd = cd + tilda + file_name + exe;
            cmd.replace("/", "\\");

            QTime start = QTime::currentTime();

            system(cmd.toStdString().c_str());

            int time = start.elapsed();

            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

            QTableWidgetItem *item0 = new QTableWidgetItem;
            item0->setText(tests[i]);
            ui->tableWidget->setItem(i, 0, item0);

            QTableWidgetItem *item1 = new QTableWidgetItem;
            if (!QFile(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + tests[i] + "/out.txt").exists()) {
                if (!QFile(qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + tests[i] + "/prog.exe").exists()) {
                    item1->setText("Ошибка компиляции");
                } else {
                    item1->setText("Неправильный формат");
                }
            } else {
                QFile file2 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + tests[i] + "/res.txt");
                file2.open(QFile::ReadOnly | QFile::Text);
                QTextStream stream2(&file2);

                QFile file3 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + tests[i] + "/out.txt");
                file3.open(QFile::ReadOnly | QFile::Text);
                QTextStream stream3(&file3);

                if (stream2.readAll() == stream3.readAll()) {
                    item1->setText("Успешно пройден");
                } else {
                    item1->setText("Неправильный ответ");
                }
            }
            ui->tableWidget->setItem(i, 1, item1);

            QTableWidgetItem *item2 = new QTableWidgetItem;
            item2->setText(QString::number(time) + " ms");
            ui->tableWidget->setItem(i, 2, item2);
        }

    } else { //Python3

        /*for (int i = 0; i < tests_count; ++i) {
            //QString myenv = "py -3.4 -m venv myenv";
            //system(myenv.toStdString().c_str());

            //qDebug()<<myenv;

            QString activate = "myenv\\Scripts\\activate.bat";
            system(activate.toStdString().c_str());

            qDebug()<<activate;

            //QString pyinstaller = "pip install pyinstaller";
            //system(pyinstaller.toStdString().c_str());

            //qDebug()<<pyinstaller;

            QString cd = "cd /D " + adress.replace("/", "\\");
            //cd.replace("/", "\\");
            system(cd.toStdString().c_str());

            qDebug()<<cd;

            QString file_name = adress;
            //qDebug()<<file_name;
            adress.replace("/", "\\");
            file_name = file_name.right(file_name.size() - file_name.lastIndexOf("\\") - 1);
            //qDebug()<<file_name;
            QString compile = "pyinstaller --onefile " + file_name;
            system(compile.toStdString().c_str());

            qDebug()<<compile;
        }*/

    }

    ui->lineEdit->setText("Тестирование завершено");
    ui->tableWidget->setCurrentCell(0, 0);
}

result_window::~result_window()
{
    delete ui;
}

void result_window::on_tableWidget_clicked(const QModelIndex &index)
{

}

void result_window::on_tableWidget_cellClicked(int row, int column)
{
    QString test_name = ui->tableWidget->item(row, 0)->text();
    ui->lineEdit->setText("Выбран " + test_name);

    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->textEdit_3->clear();

    QFile file1 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/in.txt");
    file1.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream1(&file1);
    int string_count = 0;
    while (string_count < 100 && !stream1.atEnd()) {
        ui->textEdit->append(stream1.readLine());
        ++string_count;
    }
    if (string_count >= 100) {
        ui->textEdit->append("...");
    }
    //ui->textEdit->setText(stream1.readAll());
    file1.close();

    string_count = 0;
    QFile file2 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/out.txt");
    file2.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream2(&file2);
    while (string_count < 100 && !stream2.atEnd()) {
        ui->textEdit_2->append(stream2.readLine());
        ++string_count;
    }
    if (string_count >= 100) {
        ui->textEdit_2->append("...");
    }
    //ui->textEdit_2->setText(stream2.readAll());
    file2.close();

    string_count = 0;
    QFile file3 (qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/res.txt");
    file3.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream3(&file3);
    while (string_count < 100 && !stream3.atEnd()) {
        ui->textEdit_3->append(stream3.readLine());
        ++string_count;
    }
    if (string_count >= 100) {
        ui->textEdit_3->append("...");
    }
    //ui->textEdit_3->setText(stream3.readAll());
    file3.close();

    for (int i = 0; i < 3; ++i) {
        ui->tableWidget_2->item(0, i)->setText(ui->tableWidget->item(row, i)->text());
    }
}

void result_window::on_pushButton_clicked()
{
    QString test_name = ui->tableWidget_2->item(0, 0)->text();
    QProcess* proc = new QProcess(this);
    proc->start("notepad " + qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/in.txt");
}

void result_window::on_pushButton_2_clicked()
{
    QString test_name = ui->tableWidget_2->item(0, 0)->text();
    QProcess* proc = new QProcess(this);
    proc->start("notepad " + qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/out.txt");
}

void result_window::on_pushButton_3_clicked()
{
    QString test_name = ui->tableWidget_2->item(0, 0)->text();
    QProcess* proc = new QProcess(this);
    proc->start("notepad " + qApp->applicationDirPath() + "/data/" + contest_name + "/" + task_name + "/" + test_name + "/res.txt");
}

#include "options_window.h"
#include "ui_options_window.h"

options_window::options_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::options_window)
{
    ui->setupUi(this);
    QFile file (qApp->applicationDirPath() + "/compilers.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    ui->textEdit->setText(stream.readLine());
    //ui->textEdit_2->setText(stream.readLine());
    file.close();
}

options_window::~options_window()
{
    delete ui;
}

void options_window::on_pushButton_clicked()
{

    QFile file (qApp->applicationDirPath() + "/compilers.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << ui->textEdit->toPlainText() << "\n";
    //out << ui->textEdit_2->toPlainText();
    file.close();
}

void options_window::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
    //ui->textEdit_2->clear();
    QFile file (qApp->applicationDirPath() + "/compilers.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    ui->textEdit->setText(stream.readLine());
    //ui->textEdit_2->setText(stream.readLine());
    file.close();
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_file_clicked()
{
    QFile *f = new QFile(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Data Files (*.json)")));
    if(f->fileName().isNull())
    {
        ui->textBrowser_name_file->setText("No file celected!");
    }
    else
    {
        QFileInfo fileInfo(f->fileName());
        ui->textBrowser_name_file->setText(fileInfo.fileName());
        string name = (f->fileName()).toStdString();

        if (!interfaceBundle(OPEN_FILE, &name, NULL, &dataVector))
        {
            ui->textBrowser_name_file->setText(f->errorString());
            qDebug() << f->errorString();
        }
    }
    delete f;
    print_json();
}

void MainWindow::print_json()
{
    QString text;
    for(string i : dataVector)
        text.append(QString::fromStdString(i) + "\n");
    ui->json_text->setText(text);
}

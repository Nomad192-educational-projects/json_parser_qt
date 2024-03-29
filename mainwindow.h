#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <vector>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_file_clicked();
    void on_pushButton_check_clicked();

private:
    Ui::MainWindow *ui;
    std::vector <std::string> dataVector;

    void print_json();
};
#endif // MAINWINDOW_H

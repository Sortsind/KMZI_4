#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:

    void on_push_input_clicked();

    void on_push_output_clicked();

    void on_push_key_clicked();

    void on_push_decode_clicked();

    void on_pushcaesarcode_clicked();

    void on_pushcaesardecode_clicked();

    void on_pushvicode_clicked();

    void on_pushvidecode_clicked();

    void ces_crypt(int index);

    void vij_crypt(int index);
};

#endif // MAINWINDOW_H

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

    void on_crypt_clicked();

    void on_decrypt_clicked();

    void crypt();

    void decrypt();
};

#endif // MAINWINDOW_H

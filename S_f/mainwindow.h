#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <vector>

using namespace std;

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

    void on_pushcode_clicked();

    void on_pushdecode_clicked();

    void on_push_input_clicked();

    void on_push_output_clicked();

    void on_push_outputF_clicked();

    void on_push_inputF_clicked();

    void StegCode();

    void StegDecode();

    int GetBytesPerAmpl(unsigned char BitsPerAmpl);

    int WriteOutFile(char* OutputFileName, vector<unsigned char> output);

    vector<unsigned char> readFile(char* FileName);

};

#endif // MAINWINDOW_H

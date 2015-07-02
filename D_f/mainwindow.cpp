#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

string input, output, keys, decode;
QString infile, outfile, keyfile, decfile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_crypt_clicked()
{
    crypt();
}

void MainWindow::on_decrypt_clicked()
{
    decrypt();
}

void MainWindow::on_push_input_clicked()
{
    infile = QFileDialog::getOpenFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    input = infile.toStdString();
    ui->label_5->setText(infile);
    ui->radioButton->setChecked(true);
}

void MainWindow::on_push_output_clicked()
{
    outfile = QFileDialog::getSaveFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    output = outfile.toStdString();
    ui->label_6->setText(outfile);
    ui->radioButton_2->setChecked(true);
}

void MainWindow::on_push_key_clicked()
{
    keyfile = QFileDialog::getOpenFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    keys = keyfile.toStdString();
    ui->label_7->setText(keyfile);
    ui->radioButton_3->setChecked(true);
}

void MainWindow::on_push_decode_clicked()
{
    decfile = QFileDialog::getSaveFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    decode = decfile.toStdString();
    ui->label_8->setText(decfile);
    ui->radioButton_4->setChecked(true);
}

void MainWindow::crypt()
{
    ifstream f(input.c_str());
    ofstream o(output.c_str());
    ifstream k(keys.c_str());

    if(!f.is_open())
    {
        QMessageBox::information(this,"Error!","Can't open input file!");
        return;
    }
    if(!k.is_open())
    {
        QMessageBox::information(this,"Error!","Can't open key file!");
        return;
    }
    if(!o.is_open())
    {
        QMessageBox::information(this,"Error!","Can't open output file!");
        return;
    }

    bool initial_key[64];
    bool** key = new bool*[16];
    for (int i = 0; i < 16; i++)
        key[i] = new bool[56];
    int amount;
    int amount_ = value_file(f);

    if ((amount_ % 8) == 0)
        amount = amount_ / 8;
    else
        amount = (amount_ / 8) + 1;

    bool** encrypted_data_L = new bool*[16];
    for (int i = 0; i < 16; i++)
        encrypted_data_L[i] = new bool[48];

    bool** encrypted_data_R = new bool*[16];
    for (int i = 0; i < 16; i++)
        encrypted_data_R[i] = new bool[48];

    bool** data_in = new bool*[amount];
    for (int i = 0; i < amount; i++)
        data_in[i] = new bool[64];

    entry_file(f, data_in, amount);
    enter_key(k, initial_key);

    first_iteration_key(initial_key);
    second_iteration_key(initial_key, key);
    third_iteration_key(key);

    pre_first_iteration(data_in, amount);

    for (int i = 0; i < amount; i++)
    {
        formation_R0_L0(encrypted_data_R[0], encrypted_data_L[0], data_in[i]);
        for (int j = 0; j < 15; j++)
        {
            first_iteration(encrypted_data_R[j], encrypted_data_L[j + 1]);
            second_iteration(encrypted_data_R[j], key[j + 1]);
            third_iteration(encrypted_data_R[j]);
            fourth_iteration(encrypted_data_R[j]);
            fifth_iteration(encrypted_data_R[j], encrypted_data_L[j], encrypted_data_R[j + 1]);
        }
        composition(encrypted_data_R[15], encrypted_data_L[15], data_in[i]);
        final_iteration(data_in[i]);
        output_file(data_in[i], o);
    }
    clear(data_in, encrypted_data_R, encrypted_data_L, key, amount);
    QMessageBox::information(this,"Success!","File is crypted!");
    f.close();
    o.close();
    k.close();
}

void MainWindow::decrypt()
{
    ifstream oo(output.c_str());
    ifstream k(keys.c_str());
    ofstream ff(decode.c_str());
    if(!oo.is_open())
    {
        QMessageBox::information(this,"Error!","Can't open input file!");
        return;
    }
    if(!k.is_open())
    {
        QMessageBox::information(this,"Error!","Can't open key file!");
        return;
    }
    if(!ff.is_open())
    {
        QMessageBox::information(this,"Error!","Can't open output file!");
        return;
    }

    bool initial_key[64];
    bool** key = new bool*[16];
    for (int i = 0; i < 16; i++)
        key[i] = new bool[56];
    int amount;
    int amount_ = value_file(oo);
    if ((amount_ % 8) == 0)
        amount = amount_ / 8;
    else
        amount = (amount_ / 8) + 1;

    bool** encrypted_data_L = new bool*[16];
    for (int i = 0; i < 16; i++)
        encrypted_data_L[i] = new bool[48];

    bool** encrypted_data_R = new bool*[16];
    for (int i = 0; i < 16; i++)
        encrypted_data_R[i] = new bool[48];

    bool** data_in = new bool*[amount];
    for (int i = 0; i < amount; i++)
        data_in[i] = new bool[64];

    entry_file(oo, data_in, amount);
    enter_key(k, initial_key);

    first_iteration_key(initial_key);
    second_iteration_key(initial_key, key);
    third_iteration_key(key);

    pre_first_iteration(data_in, amount);
    for (int i = 0; i < amount; i++)
    {
        formation_R15_L15(encrypted_data_R[15], encrypted_data_L[15], data_in[i]);
        for (int j = 15; j > 0; j--)
        {
            first_iteration(encrypted_data_L[j], encrypted_data_R[j - 1]);
            second_iteration(encrypted_data_L[j], key[j]);
            third_iteration(encrypted_data_L[j]);
            fourth_iteration(encrypted_data_L[j]);
            fifth_iteration_reverse(encrypted_data_R[j], encrypted_data_L[j], encrypted_data_L[j - 1]);
        }
        composition(encrypted_data_R[0], encrypted_data_L[0], data_in[i]);
        final_iteration(data_in[i]);
        output_file(data_in[i], ff);
    }
    clear(data_in, encrypted_data_R, encrypted_data_L, key, amount);
    QMessageBox::information(this,"Success!","File is decrypted!");
    ff.close();
    oo.close();
    k.close();
}

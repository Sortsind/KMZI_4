#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtGui>
#include <QTextStream>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;
string input, output, key, s, t, w, decode;
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

void MainWindow::on_pushcaesarcode_clicked()
{
    int index = 1;
    ces_crypt(index);
}

void MainWindow::on_pushcaesardecode_clicked()
{
    int index = 2;
    ces_crypt(index);
}

void MainWindow::on_pushvicode_clicked()
{
    int index = 1;
    vij_crypt(index);
}

void MainWindow::on_pushvidecode_clicked()
{
    int index = 2;
    vij_crypt(index);
}

void MainWindow::on_push_input_clicked()
{
    infile = QFileDialog::getOpenFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    input = infile.toStdString();
    ui->lab->setText(infile);
    ui->checkBox->setChecked(true);
    ifstream in(input.c_str());
    getline(in,s);
    in.close();
}

void MainWindow::on_push_output_clicked()
{
    outfile = QFileDialog::getSaveFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    output = outfile.toStdString();
    ui->labe->setText(outfile);
    ui->checkBox_2->setChecked(true);
}

void MainWindow::on_push_key_clicked()
{
    keyfile = QFileDialog::getOpenFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    key = keyfile.toStdString();
    ui->la->setText(keyfile);
    ui->checkBox_3->setChecked(true);
    ifstream ke(key.c_str());
    getline(ke,t);
    ke.close();
}

void MainWindow::on_push_decode_clicked()
{
    decfile = QFileDialog::getSaveFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    decode = decfile.toStdString();
    ui->label_6->setText(decfile);
    ui->checkBox_4->setChecked(true);
}

void MainWindow::ces_crypt(int index)
{
    QString strKey = ui->linekey->text();
    bool ok;
    int key = strKey.toInt(&ok);
    if (!ok)
    {
        QMessageBox::information(this,"Error!","Incorrect key. Enter a number and try again.");
        return;
    }

    char ch;
    string result = "";
    if (index == 1)
    {
        for (unsigned int i = 0; i < s.length(); i++)
        {
            ch = s[i];
            if (ch != EOF)
            {
                ch = (ch + key) % 256;
                result = result + ch;
            }
        }
        ofstream out(output.c_str());
        out << result;
        out.close();
        ui->pushcaesarcode->setEnabled(false);
        QMessageBox::information(this,"Success!","File is crypted!");
    }
    if (index == 2)
    {
        ifstream ou(output.c_str());
        getline(ou,w);
        ou.close();
        for (unsigned int i = 0; i < w.length(); i++)
        {
            ch = w[i];
            if (ch != EOF)
            {
                ch = (ch - key) % 256;
                result = result + ch;
            }
        }
        ofstream dec(decode.c_str());
        dec << result;
        dec.close();
        ui->pushcaesarcode->setEnabled(true);
        QMessageBox::information(this,"Success!","File is decrypted!");

        unsigned int check = 0;
        if(s.length() == result.length())
        {
            for(unsigned int i = 0; i < s.length(); i++)
            {
                if(s[i] == result[i])
                    check++;
            }
        }
        if(check == s.length())
            QMessageBox::information(this,"Success!","File after encoding and decoding completely restored.");
        else
            QMessageBox::information(this,"Error!","Something went wrong ...");
    }
}

void MainWindow::vij_crypt(int index)
{
    char ch;
    string strKey = "";
    string::iterator i = strKey.begin();
    for (unsigned int j = 0; j < t.length(); j++)
    {
        ch = t[j];
        if (ch != EOF)
        {
           strKey = strKey + ch;
        }
    }

    string result = "";
    i = strKey.begin();
    if (index == 1)
    {
        for (unsigned int j = 0; j < s.length(); j++)
        {
            ch = s[j];
            if (ch != EOF)
            {
                ch = (ch + *i) % 256;
                result = result + ch;
                if (i + 1 == strKey.end())
                    i = strKey.begin();
                else
                    i++;
             }
        }
        ofstream out(output.c_str());
        out << result;
        out.close();
        ui->pushvicode->setEnabled(false);
        QMessageBox::information(this,"Success!","File is crypted!");
    }
    if (index == 2)
    {
        ifstream ou(output.c_str());
        getline(ou,w);
        ou.close();
        for (unsigned int j = 0; j < w.length(); j++)
        {
            ch = w[j];
            if (ch != EOF)
            {
                ch = (ch - *i) % 256;
                result = result + ch;
                if (i + 1 == strKey.end())
                    i = strKey.begin();
                else
                    i++;
             }
        }

        ofstream dec(decode.c_str());
        dec << result;
        dec.close();
        ui->pushvicode->setEnabled(true);
        QMessageBox::information(this,"Success!","File is decrypted!");

        unsigned int check = 0;
        if(s.length() == result.length())
        {
            for(unsigned int i = 0; i < s.length(); i++)
            {
                if(s[i] == result[i])
                    check++;
            }
        }
        if(check == s.length())
            QMessageBox::information(this,"Success!","File after encoding and decoding completely restored.");
        else
            QMessageBox::information(this,"Error!","Something went wrong ...");
    }
}

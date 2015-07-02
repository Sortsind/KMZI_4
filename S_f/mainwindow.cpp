#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;
string input_, output_, decode, inputs;
QString infile, outfile, decfile, infiles;

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

void MainWindow::on_pushcode_clicked()
{
    StegCode();
}

void MainWindow::on_pushdecode_clicked()
{
    StegDecode();
}

void MainWindow::on_push_input_clicked()
{
    infile = QFileDialog::getOpenFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    input_ = infile.toStdString();
    ui->label_5->setText(infile);
    ui->checkBox->setChecked(true);
}

void MainWindow::on_push_inputF_clicked()
{
    infiles = QFileDialog::getOpenFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    inputs = infiles.toStdString();
    ui->label_6->setText(infiles);
    ui->checkBox_2->setChecked(true);
}

void MainWindow::on_push_output_clicked()
{
    outfile = QFileDialog::getSaveFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    output_ = outfile.toStdString();
    ui->label_7->setText(outfile);
    ui->checkBox_3->setChecked(true);
}

void MainWindow::on_push_outputF_clicked()
{
    decfile = QFileDialog::getSaveFileName(this,tr("Открыть файл"),"/Users/","Allfiles(*)");
    decode = decfile.toStdString();
    ui->label_8->setText(decfile);
    ui->checkBox_4->setChecked(true);
}

void MainWindow::StegCode()
{  
    string tmp;
    int lenIn, lenOut, lenCo;

    tmp = input_;
    lenCo = tmp.length();
    char ContainerFileName[250];
    for (int i = 0; i < lenCo; i++)
        ContainerFileName[i] = tmp[i];
    for (int i = lenCo; i < 250; i++)
        ContainerFileName[i] = NULL;

    tmp = inputs;
    lenIn = tmp.length();
    char InformationFileName[250];
    for (int i = 0; i < lenIn; i++)
        InformationFileName[i] = tmp[i];
    for (int i = lenIn; i < 250; i++)
        InformationFileName[i] = NULL;

    tmp = output_;
    lenOut = tmp.length();
    char OutputFileName[250];
    for (int i = 0; i < lenOut; i++)
        OutputFileName[i] = tmp[i];
    for (int i = lenOut; i < 250; i++)
        OutputFileName[i] = NULL;

    vector<unsigned char> inWav = readFile(ContainerFileName);
    vector<unsigned char> infFile = readFile(InformationFileName);

    if (infFile.empty())
    {
        QMessageBox::information(this,"Error!","Not exist information file!");
        return;
    }
    else
    if (inWav.size()<infFile.size() * 8 + 20 + 44)
    {
        QMessageBox::information(this,"Error!","Information file is big");
        return;
    }
    if (inWav[8] != 'W' || inWav[9] != 'A' || inWav[10] != 'V' || inWav[11] != 'E')
	{
        QMessageBox::information(this,"Error!","Enter container is not wav.");
        return;
	}
    vector<unsigned char> data;

    int fileSize = infFile.size();
    for (int i = 0; i < 16; i++)
    {
        data.push_back(fileSize % 2);
        fileSize /= 2;
    }

    for (unsigned int i = 0; i < infFile.size(); i++)
    {
        unsigned char b = infFile[i];
        for (int j = 0; j < 8; j++)
        {
            data.push_back(b % 2);
            b /= 2;
        }
    }

    int bytesPerAmpl = GetBytesPerAmpl(inWav[34]);

    for (unsigned int i = 0, wavIndex = 44; i < data.size(); i++, wavIndex += bytesPerAmpl)
    {
        if (inWav[wavIndex] % 2 == 1)
        {
            if (data[i] == 0)
                inWav[wavIndex]++;
        }
        else
        {
            if (data[i] == 1)
                inWav[wavIndex]++;
        }
    }
    WriteOutFile(OutputFileName, inWav);
    QMessageBox::information(this,"Success!","File is crypted!");
}

void MainWindow::StegDecode()
{
    string tmp;
    int lenOutC, lenOut;

    tmp = output_;
    lenOutC = tmp.length();
    char ContainerFileName[250];
    for (int i=0; i<lenOutC; i++)
        ContainerFileName[i] = tmp[i];
    for (int i = lenOutC; i < 250; i++)
        ContainerFileName[i] = NULL;

    tmp = decode;
    lenOut = tmp.length();
    char OutputFileName[250];
    for (int i=0; i<lenOutC; i++)
        OutputFileName[i] = tmp[i];
    for (int i = lenOutC; i < 250; i++)
        OutputFileName[i] = NULL;

    vector<unsigned char> inWav = readFile(ContainerFileName);

    int wavIndex = 44;
    int bytesPerAmpl = GetBytesPerAmpl(inWav[34]);
    if (inWav[8] != 'W' || inWav[9] != 'A' || inWav[10] != 'V' || inWav[11] != 'E')
	{
        QMessageBox::information(this,"Error!","Enter container is not wav.");
        return;
	}
    int fileSize = 0;
    int pow2 = 1;
    for (int i = 0; i < 16; i++)
    {
        fileSize += (inWav[wavIndex] % 2) * pow2;
        pow2 *= 2;
        wavIndex += bytesPerAmpl;
    }

    vector<unsigned char> output;
    for (int i = 0; i < fileSize; i++)
    {
        unsigned char b = 0;
        for (int j = 0, pow2 = 1; j < 8; j++, pow2 *= 2, wavIndex += bytesPerAmpl)
        {
            b += (inWav[wavIndex] % 2) * pow2;
        }
        output.push_back(b);
    }

    WriteOutFile(OutputFileName, output);
    QMessageBox::information(this,"Success!","File is decrypted!");
}

vector<unsigned char> MainWindow::readFile(char* fileName)
{
    vector<unsigned char> input;
    ifstream ifst(fileName, ios::binary);
    if (ifst.fail())
        return input;

    ifst.seekg(0, ios::end);
    int size = ifst.tellg();
    ifst.seekg(0, ios::beg);

    input.resize(size);
    ifst.read((char*)&input.front(), size);
    ifst.close();

    return input;
}

int MainWindow::GetBytesPerAmpl(unsigned char BitsPerAmpl)
{
    int BytesPerAmpl = 0;
    if (BitsPerAmpl == 8)
        BytesPerAmpl = 1;
    else
    if (BitsPerAmpl == 16)
        BytesPerAmpl = 2;
    else
    if (BitsPerAmpl == 24)
        BytesPerAmpl = 3;
    else
    if (BitsPerAmpl == 32)
        BytesPerAmpl = 4;
    else
        BytesPerAmpl = 1;
    return BytesPerAmpl;
}

int MainWindow::WriteOutFile(char* OutputFileName, vector<unsigned char> output)
{
    ofstream outStream(OutputFileName, ios::binary);
    outStream.write((char*)&output.front(), output.size());
    outStream.close();
    return 1;
}

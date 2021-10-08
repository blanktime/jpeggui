//jpeggui.h ������ͷ�ļ�
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_jpeggui.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <ActiveQt/QAxObject>
#include "generate.h"

class jpeggui : public QMainWindow
{
    Q_OBJECT

public:
    jpeggui(QWidget *parent = Q_NULLPTR);
    static QString filename;
    QPixmap pixmap;
    QPixmap fitpixmap;
    QImage* img;

private:
    Ui::jpegguiClass ui;
    generate* gen;
    QAxObject* application;
    QAxObject* workBooks;
    QAxObject* workBook;
    QAxObject* sheets;
    QAxObject* sheet;
    int rowCount;

public:
    void newCreateExl(const QString& fileName);  //�½�һ��Excel
    void setCellVal(int row, int column, const QString& val);    //��Excel��Ԫ����д������
    void saveExl(const QString& fileName);      //����Excel
    void closeExl(const QString& fileName);     //�ر�Excel
    void freeExl();                             //�ͷ�Excel

//�ۺ���
private slots:
    void openButton();
    void nextButton();
    void openDirButton();
};

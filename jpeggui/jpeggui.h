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
    QAxObject* m_pApplication;
    QAxObject* m_pWorkBooks;
    QAxObject* m_pWorkBook;
    QAxObject* m_pSheets;
    QAxObject* m_pSheet;
    int m_totalRowCnt;

public:
    void newCreateExl(const QString& fileName); //1.�½�һ��Excel
    void appendSheet(const QString& sheetName); //2.����1��Worksheet
    void setCellVal(int row, int column, const QString& val);    //3.��Excel��Ԫ����д������
    void saveExl(const QString& fileName);      //4.����Excel
    void closeExl(const QString& fileName);     //5.�ر�Excel
    void freeExl();                             //6.�ͷ�Excel
    void setSheetName(int itemIndex, const QString& sheetName); //7.����sheet����
    int  getRowCount(void);         //�õ�����
    void openFile(QString strFile); //��excel�ļ�
    void importExlToDbs(void);      //���뵽���ݿ�

//�ۺ���
private slots:
    void openButton();
    void nextButton();
    void openDirButton();
};

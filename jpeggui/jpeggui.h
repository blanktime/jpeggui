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
    void newCreateExl(const QString& fileName); //1.新建一个Excel
    void appendSheet(const QString& sheetName); //2.增加1个Worksheet
    void setCellVal(int row, int column, const QString& val);    //3.向Excel单元格中写入数据
    void saveExl(const QString& fileName);      //4.保存Excel
    void closeExl(const QString& fileName);     //5.关闭Excel
    void freeExl();                             //6.释放Excel
    void setSheetName(int itemIndex, const QString& sheetName); //7.设置sheet名称
    int  getRowCount(void);         //得到行数
    void openFile(QString strFile); //打开excel文件
    void importExlToDbs(void);      //导入到数据库

//槽函数
private slots:
    void openButton();
    void nextButton();
    void openDirButton();
};

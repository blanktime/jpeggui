//jpeggui.h 主窗口头文件
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
    void newCreateExl(const QString& fileName);  //新建一个Excel
    void setCellVal(int row, int column, const QString& val);    //向Excel单元格中写入数据
    void saveExl(const QString& fileName);      //保存Excel
    void closeExl(const QString& fileName);     //关闭Excel
    void freeExl();                             //释放Excel

//槽函数
private slots:
    void openButton();
    void nextButton();
    void openDirButton();
};

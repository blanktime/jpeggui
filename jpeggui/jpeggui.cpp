#include "jpeggui.h"
#include <QString>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>
#include <time.h>

extern "C" {
#include "jpeglib.h"
#include "libexif/exif-data.h"
}

jpeggui::jpeggui(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    application = NULL;
    workBooks = NULL;
    workBook = NULL;
    sheets = NULL;
    sheet = NULL;
}

//���ļ�
void jpeggui::openButton() {
    filename = QFileDialog::getOpenFileName(NULL, tr("open jpeg"), "", tr("Image (*.jpg *.JPG *.jpeg *.JPEG)"));
    if (filename.isEmpty())
    {
        return;
    }
    //��ʾ
    img = new QImage;
    if (!(img->load(filename)))      //����ͼ��
    {
        QMessageBox::information(this, tr("Error"), tr("can't open files!"));
        delete img;
        return;
    }
    pixmap = QPixmap::fromImage(*img);
    fitpixmap = pixmap.scaled(ui.showlabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // ����������
    ui.showlabel->setPixmap(fitpixmap);
    ui.showlabel->setAlignment(Qt::AlignCenter);
}

//��һ��
void jpeggui::nextButton() {
    if (pixmap.isNull()) {
        QMessageBox::warning(NULL, "warning", "Please select a picture!", QMessageBox::Yes, QMessageBox::Yes);
    }
    else {
        gen = new generate;
        gen->show();
        this->hide();
    }

}

//generate���淵�ذ�ť
void generate::backButton() {
    jpeggui* jgui = new jpeggui;
    jgui->show();
    this->hide();
}

void jpeggui::saveExl(const QString& fileName)
{
    if (workBook != NULL) {
        workBook->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(fileName));
    }
}

void jpeggui::closeExl(const QString& fileName)
{
    if (workBook != NULL) {
        workBook->dynamicCall("Close(const QString &)", QDir::toNativeSeparators(fileName));
    }
}

void jpeggui::freeExl()
{
    if (application != NULL) {
        application->dynamicCall("Quit()");
        delete application;
        application = NULL;
    }
}

void jpeggui::newCreateExl(const QString& fileName)
{
    application = new QAxObject(this);
    application->setControl("Excel.Application");//����Exl�ؼ�
    application->dynamicCall("SetVisible(bool)", false);//false����ʾ����
    application->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ��
    workBooks = application->querySubObject("Workbooks");

    QFile file(fileName);
    if (file.exists()) {
        qDebug("%s file exists\n", fileName.toStdString().c_str());
        workBook = workBooks->querySubObject("Open(const QString &)", fileName);
    }
    else {
        qDebug("%s file not exists\n", fileName.toStdString().c_str());
        workBooks->dynamicCall("Add");
        workBook = application->querySubObject("ActiveWorkBook");
    }
    sheets = workBook->querySubObject("Sheets");
    sheet = sheets->querySubObject("Item(int)", 1);
}


int jpeggui::getRowCount(void)
{
    QAxObject* range = sheet->querySubObject("UsedRange"); //��ȡ��sheet��ʹ�÷�Χ����
    QVariant var = range->dynamicCall("Value");
    delete range;
    QVariantList varRows = var.toList();  //�õ�����е���������
    if (varRows.isEmpty()) {
        rowCount = 0;
        qDebug("varRows is empty\n");
        return 0;
    }
    rowCount = varRows.size();
    qDebug("rowCount:%d\n", rowCount);
    return rowCount;
}

void jpeggui::setCellVal(int row, int column, const QString& value)
{
    row = (row < 1) ? 1 : row;
    column = (column < 1) ? 1 : column;
    QAxObject* pRange = sheet->querySubObject("Cells(int,int)", row, column);
    pRange->dynamicCall("Value", value);
}

struct my_error_mgr {
    struct jpeg_error_mgr pub;    
    jmp_buf setjmp_buffer;       
};

typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

//���ļ��н���ͼ��ͷ�ļ���Ϣ�Ա�
void jpeggui::openDirButton() {
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("choose a dir"), tr("./"));
    if (dirPath.isEmpty()) {
        return;
    }
    QDir dir(dirPath.toStdString().c_str());
    QStringList nameFilters;
    nameFilters << "*.jpg"<<"*jpeg";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Time | QDir::Reversed);

    QString excelPath = dirPath + "/out.xlsx";
    newCreateExl(excelPath);
    setCellVal(1, 1, QString("File Name"));
    setCellVal(1, 2, QString("is_Baseline"));
    setCellVal(1, 3, QString("Data Precision"));
    setCellVal(1, 4, QString("Height"));
    setCellVal(1, 5, QString("Width"));
    setCellVal(1, 6, QString("Nunber of Component"));
    setCellVal(1, 7, QString("JFIF Version"));
    setCellVal(1, 8, QString("Resolution Units"));
    setCellVal(1, 9, QString("X Resolution"));
    setCellVal(1, 10, QString("Y Resolution"));
    setCellVal(1, 11, QString("Size"));
    int curLine = getRowCount();
    //����jpeg�ļ�
    for (int i = 0; i < files.size(); ++i) {

        QString jpegPath = dirPath + "/" + files.at(i);
        JSAMPLE* image_buffer;       
        struct jpeg_decompress_struct cinfo;
        struct my_error_mgr jerr;    
        FILE* infile;                
        JSAMPARRAY buffer;           
        int row_stride;              

        if ((infile = fopen(jpegPath.toStdString().c_str(), "rb")) == NULL) {
            qDebug() << "can't open infile!\n";
            return ;
        }

        fseek(infile, 0, SEEK_END);
        unsigned int fsize = ftell(infile);
        fseek(infile, 0, SEEK_SET);

        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        if (setjmp(jerr.setjmp_buffer)) {
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            return ;
        }
        jpeg_create_decompress(&cinfo);        

        jpeg_stdio_src(&cinfo, infile);

        (void)jpeg_read_header(&cinfo, TRUE);   
        setCellVal(curLine + 1, 1, files.at(i));
        setCellVal(curLine + 1, 2, QString::number(cinfo.is_baseline));
        setCellVal(curLine + 1, 3, QString::number(cinfo.data_precision));
        setCellVal(curLine + 1, 4, QString::number(cinfo.image_height));
        setCellVal(curLine + 1, 5, QString::number(cinfo.image_width));
        setCellVal(curLine + 1, 6, QString::number(cinfo.num_components));
        setCellVal(curLine + 1, 7, QString::number(cinfo.JFIF_major_version + cinfo.JFIF_minor_version * 0.1));
        setCellVal(curLine + 1, 8, QString::number(cinfo.density_unit));
        setCellVal(curLine + 1, 9, QString::number(cinfo.X_density));
        setCellVal(curLine + 1, 10, QString::number(cinfo.Y_density));
        setCellVal(curLine + 1, 11, QString::number(fsize));
        curLine += 1;

        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

    }
    saveExl(excelPath);
    closeExl(excelPath);
    freeExl();
    QMessageBox::about(NULL, tr("successiful"), tr("Excel was generated successfully!"));  
}
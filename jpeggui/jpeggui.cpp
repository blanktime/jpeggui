#include "jpeggui.h"
#include <QString>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
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

//打开文件
void jpeggui::openButton() {
    filename = QFileDialog::getOpenFileName(NULL, tr("open jpeg"), "", tr("Image (*.jpg *.JPG *.jpeg *.JPEG)"));
    if (filename.isEmpty())
    {
        return;
    }
    //显示
    img = new QImage;
    if (!(img->load(filename)))      //加载图像
    {
        QMessageBox::information(this, tr("Error"), tr("can't open files!"));
        delete img;
        return;
    }
    pixmap = QPixmap::fromImage(*img);
    fitpixmap = pixmap.scaled(ui.showlabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui.showlabel->setPixmap(fitpixmap);
    ui.showlabel->setAlignment(Qt::AlignCenter);
}

//下一步
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

//generate界面返回按钮
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
    application->setControl("Excel.Application");//连接Exl控件
    application->dynamicCall("SetVisible(bool)", false);//false不显示窗体
    application->setProperty("DisplayAlerts", false);//不显示任何警告信息。
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
    QAxObject* range = sheet->querySubObject("UsedRange"); //获取该sheet的使用范围对象
    QVariant var = range->dynamicCall("Value");
    delete range;
    QVariantList varRows = var.toList();  //得到表格中的所有数据
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

//打开文件夹进行图像头文件信息对比
void jpeggui::openDirButton() {
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("choose a dir"), tr(" "));
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
    setCellVal(1, 6, QString("QF"));
    setCellVal(1, 7, QString("Nunber of Component"));
    setCellVal(1, 8, QString("JFIF Version"));
    setCellVal(1, 9, QString("Resolution Units"));
    setCellVal(1, 10, QString("X Resolution"));
    setCellVal(1, 11, QString("Y Resolution"));
    setCellVal(1, 12, QString("Size"));
    int curLine = getRowCount();

    static const unsigned int std_luminance_quant_tbl[64] = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109,103,77,
        24, 35, 55, 64, 81, 104,113,92,
        49, 64, 78, 87, 103,121,120,101,
        72, 92, 95, 98, 112,100,103,99
    };

    static const unsigned int std_chrominance_quant_tbl[64] = {
        17, 18, 24, 47, 99, 99, 99, 99,
        18, 21, 26, 66, 99, 99, 99, 99,
        24, 26, 56, 99, 99, 99, 99, 99,
        47, 66, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99
    };

    //遍历jpeg文件
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
        int tmpQuality = 0;
        int linearQuality = 0;
        const int averTimes = 5;
        int times = 0;
        int averQuality = 0;

        //量化表反推5次，取平均值
        for (int i = 0; i < DCTSIZE2; i++)
        {
            long temp = cinfo.quant_tbl_ptrs[0]->quantval[i];
            if (temp < 32767L && temp>0)
            {
                linearQuality = round((float)(temp * 100L - 50L) / std_luminance_quant_tbl[i]);
                if (linearQuality == 1) tmpQuality = 1;
                else if (linearQuality == 100) tmpQuality = 100;
                else if (linearQuality > 100)
                {
                    tmpQuality = round((float)5000 / linearQuality);
                }
                else
                {
                    tmpQuality = 100 - round((float)linearQuality / 2);
                }
                averQuality += tmpQuality;
                if (averTimes == ++times)
                {
                    averQuality /= averTimes;
                    break;
                }
            }
        }
        setCellVal(curLine + 1, 1, files.at(i));
        setCellVal(curLine + 1, 2, QString::number(cinfo.is_baseline));
        setCellVal(curLine + 1, 3, QString::number(cinfo.data_precision));
        setCellVal(curLine + 1, 4, QString::number(cinfo.image_height));
        setCellVal(curLine + 1, 5, QString::number(cinfo.image_width));
        setCellVal(curLine + 1, 6, QString::number(averQuality));
        setCellVal(curLine + 1, 7, QString::number(cinfo.num_components));
        setCellVal(curLine + 1, 8, QString::number(cinfo.JFIF_major_version + cinfo.JFIF_minor_version * 0.1));
        setCellVal(curLine + 1, 9, QString::number(cinfo.density_unit));
        setCellVal(curLine + 1, 10, QString::number(cinfo.X_density));
        setCellVal(curLine + 1, 11, QString::number(cinfo.Y_density));
        setCellVal(curLine + 1, 12, QString::number(fsize));
        curLine += 1;

        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

    }
    saveExl(excelPath);
    closeExl(excelPath);
    freeExl();
    QMessageBox::about(NULL, tr("successiful"), tr("Excel was generated successfully!"));  
}
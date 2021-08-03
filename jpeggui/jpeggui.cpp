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
    m_pApplication = NULL;
    m_pWorkBooks = NULL;
    m_pWorkBook = NULL;
    m_pSheets = NULL;
    m_pSheet = NULL;
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
    if (m_pWorkBook != NULL) {
        m_pWorkBook->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(fileName));
    }
}

void jpeggui::closeExl(const QString& fileName)
{
    if (m_pWorkBook != NULL) {
        m_pWorkBook->dynamicCall("Close(const QString &)", QDir::toNativeSeparators(fileName));
    }
}

void jpeggui::freeExl()
{
    if (m_pApplication != NULL) {
        m_pApplication->dynamicCall("Quit()");
        delete m_pApplication;
        m_pApplication = NULL;
    }
}

void jpeggui::newCreateExl(const QString& fileName)
{
    m_pApplication = new QAxObject(this);
    m_pApplication->setControl("Excel.Application");//连接Exl控件
    m_pApplication->dynamicCall("SetVisible(bool)", false);//false不显示窗体
    m_pApplication->setProperty("DisplayAlerts", false);//不显示任何警告信息。
    m_pWorkBooks = m_pApplication->querySubObject("Workbooks");

    QFile file(fileName);
    if (file.exists()) {
        qDebug("%s file exists\n", fileName.toStdString().c_str());
        m_pWorkBook = m_pWorkBooks->querySubObject("Open(const QString &)", fileName);
    }
    else {
        qDebug("%s file not exists\n", fileName.toStdString().c_str());
        m_pWorkBooks->dynamicCall("Add");
        m_pWorkBook = m_pApplication->querySubObject("ActiveWorkBook");
    }
    m_pSheets = m_pWorkBook->querySubObject("Sheets");
    m_pSheet = m_pSheets->querySubObject("Item(int)", 1);
}

void jpeggui::appendSheet(const QString& sheetName)
{
    int nCount = m_pSheets->property("Count").toInt();
    QAxObject* pLastSheet = m_pSheets->querySubObject("Item(int)", nCount);
    m_pSheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
    m_pSheet = m_pSheets->querySubObject("Item(int)", nCount);
    pLastSheet->dynamicCall("Move(QVariant)", m_pSheet->asVariant());
    m_pSheet->setProperty("Name", sheetName);
}

int jpeggui::getRowCount(void)
{
    QAxObject* range = m_pSheet->querySubObject("UsedRange"); //获取该sheet的使用范围对象
    QVariant var = range->dynamicCall("Value");
    delete range;
    QVariantList varRows = var.toList();  //得到表格中的所有数据
    if (varRows.isEmpty()) {
        m_totalRowCnt = 0;
        qDebug("varRows is empty\n");
        return 0;
    }
    m_totalRowCnt = varRows.size();
    qDebug("rowCount:%d\n", m_totalRowCnt);
    return m_totalRowCnt;
}

void jpeggui::setCellVal(int row, int column, const QString& value)
{
    row = (row < 1) ? 1 : row;
    column = (column < 1) ? 1 : column;
    QAxObject* pRange = m_pSheet->querySubObject("Cells(int,int)", row, column);
    pRange->dynamicCall("Value", value);
}

void jpeggui::setSheetName(int itemIndex, const QString& sheetName)
{
    int nCount = m_pSheets->property("Count").toInt();
    if (itemIndex <= nCount) {
        QAxObject* pCurSheet = m_pSheets->querySubObject("Item(int)", itemIndex);
        pCurSheet->setProperty("Name", sheetName);
    }
}
void jpeggui::openFile(QString strFile)
{
    //1.获取Excel驱动，以及表格信息。
    QAxObject Exl("Excel.Application");

    Exl.setProperty("Visible", false); //不显示Exl界面，如果为true会看到启动的Exl界面

    m_pWorkBooks = Exl.querySubObject("WorkBooks");

    m_pWorkBooks->dynamicCall("Open (const QString&)", strFile);//打开指定文件

    m_pWorkBook = Exl.querySubObject("ActiveWorkBook");

    m_pSheets = m_pWorkBook->querySubObject("Sheets");//获取工作表

    int nSheetCount = m_pSheets->property("Count").toInt();  //获取工作表的数目
    qDebug("sheetCnt:%d\n", nSheetCount);

    //2.分区域获取Exl表格中的内容，最后一定要关闭Excel文件，不然该文件会一直处于“只读”状态。代码如下：
    if (nSheetCount > 0) {
        int nSection = 20, nSectionEnd = 0, nRowEnd = 20;
        QAxObject* pWorkSheet = m_pWorkBook->querySubObject("Sheets(int)", 1);//获取第一张表
        QVariantList params;
        QVariant varData;
        for (int i = 1; i <= nRowEnd; i += nSection) {
            nSectionEnd = i + nSection - 1;
            if (nSectionEnd > nRowEnd) {
                nSectionEnd = nRowEnd;
            }
            char cZimu = 1 + 64; //1-26分别代表A-Z，符合Excel表的列标识
            //Ai至Ai+nSectionEnd的数据,这里为了测试一次是读取20条
            params << QString("%1%2").arg(cZimu).arg(i) << QString("%1%2").arg(cZimu).arg(nSectionEnd);
            QAxObject* pCell = pWorkSheet->querySubObject("Range(QVariant,QVariant)", params);
            varData = pCell->dynamicCall("Value2()");
            //qDebug() << varData;
        }
        //3.测试文档已经读出结果如图，可以发现我们读出的数据包含了三层外衣。前两层都是QVariantList。
        //为了取出正确的数据，我们需要剥开这些外衣。
        QVariantList varList = varData.toList(); //解除第一层List
        if (varList.count() <= 0) {//防止内存溢出
            qDebug("无数据！");
            m_pWorkBooks->dynamicCall("Close()");
            return;
        }
        QVariantList varLstData = varList[0].toList();
        //解除第二层List
        if (varLstData.count() <= 0) {
            qDebug("无数据！");
            m_pWorkBooks->dynamicCall("Close()");
            return;
        }
        //判断数据类型，防止转化出错。
        if (QString(varLstData[0].typeName()) == "QString") {
            QString str = varLstData[0].toString(); //取出数据
            qDebug("字符串：%s", str);
        }

        if (QString(varLstData[0].typeName()) == "double") {
            double dData = varLstData[0].toDouble(); //取出数据
            qDebug("%0.1f", dData); //注意，int型读出来也是double，所以一定要注意转化
        }
    }
    m_pWorkBooks->dynamicCall("Close()");
    m_pWorkBook = NULL;
}

void jpeggui::importExlToDbs(void)
{
    QString strFilePathName = QFileDialog::getOpenFileName(this, QStringLiteral("选择Excel文件"), "", tr("Exel file(*.xls *.xlsx *.xlsm)"));
    if (strFilePathName.isNull()) {
        return;
    }
    QAxObject* Exl = new QAxObject(this);	//连接Exl控件
    if (Exl->setControl("Excel.Application")) {
    }
    else {
        Exl->setControl("ket.Application");  //连接Exl控件
    }
    Exl->setProperty("Visible", false);  //不显示窗体
    QAxObject* workbooks = Exl->querySubObject("WorkBooks");  //获取工作簿集合
    workbooks->dynamicCall("Open(const QString&)", strFilePathName); //打开打开已存在的工作簿
    QAxObject* workbook = Exl->querySubObject("ActiveWorkBook"); //获取当前工作簿
    QAxObject* sheets = workbook->querySubObject("Sheets");  //获取工作表集合，Sheets也可换用WorkSheets
    QAxObject* sheet = workbook->querySubObject("WorkSheets(int)", 1);//获取工作表集合的工作表1，即sheet1
    QAxObject* range = sheet->querySubObject("UsedRange"); //获取该sheet的使用范围对象
    QVariant var = range->dynamicCall("Value");
    delete range;
    QVariantList varRows = var.toList();  //得到表格中的所有数据
    if (varRows.isEmpty()) {
        return;
    }
    const int rowCount = varRows.size();
    //qDebug("rowCount:%d\n", rowCount);
    QStringList m_userid, m_card_id, m_action;
    for (int i = 1; i < rowCount; ++i) {
        QVariantList rowData = varRows[i].toList();
        m_userid << rowData[0].toString();
        m_card_id << rowData[1].toString();
        m_action << rowData[2].toString();
    }
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
    int CurLine = getRowCount();
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
        setCellVal(CurLine + 1, 1, files.at(i));
        setCellVal(CurLine + 1, 2, QString::number(cinfo.is_baseline));
        setCellVal(CurLine + 1, 3, QString::number(cinfo.data_precision));
        setCellVal(CurLine + 1, 4, QString::number(cinfo.image_height));
        setCellVal(CurLine + 1, 5, QString::number(cinfo.image_width));
        setCellVal(CurLine + 1, 6, QString::number(cinfo.num_components));
        setCellVal(CurLine + 1, 7, QString::number(cinfo.JFIF_major_version + cinfo.JFIF_minor_version * 0.1));
        setCellVal(CurLine + 1, 8, QString::number(cinfo.density_unit));
        setCellVal(CurLine + 1, 9, QString::number(cinfo.X_density));
        setCellVal(CurLine + 1, 10, QString::number(cinfo.Y_density));
        setCellVal(CurLine + 1, 11, QString::number(fsize));
        CurLine += 1;

        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

    }
    saveExl(excelPath);
    closeExl(excelPath);
    freeExl();
    QMessageBox::about(NULL, tr("successiful"), tr("Excel was generated successfully!"));  
}
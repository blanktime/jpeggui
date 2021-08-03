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
    m_pApplication->setControl("Excel.Application");//����Exl�ؼ�
    m_pApplication->dynamicCall("SetVisible(bool)", false);//false����ʾ����
    m_pApplication->setProperty("DisplayAlerts", false);//����ʾ�κξ�����Ϣ��
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
    QAxObject* range = m_pSheet->querySubObject("UsedRange"); //��ȡ��sheet��ʹ�÷�Χ����
    QVariant var = range->dynamicCall("Value");
    delete range;
    QVariantList varRows = var.toList();  //�õ�����е���������
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
    //1.��ȡExcel�������Լ������Ϣ��
    QAxObject Exl("Excel.Application");

    Exl.setProperty("Visible", false); //����ʾExl���棬���Ϊtrue�ῴ��������Exl����

    m_pWorkBooks = Exl.querySubObject("WorkBooks");

    m_pWorkBooks->dynamicCall("Open (const QString&)", strFile);//��ָ���ļ�

    m_pWorkBook = Exl.querySubObject("ActiveWorkBook");

    m_pSheets = m_pWorkBook->querySubObject("Sheets");//��ȡ������

    int nSheetCount = m_pSheets->property("Count").toInt();  //��ȡ���������Ŀ
    qDebug("sheetCnt:%d\n", nSheetCount);

    //2.�������ȡExl����е����ݣ����һ��Ҫ�ر�Excel�ļ�����Ȼ���ļ���һֱ���ڡ�ֻ����״̬���������£�
    if (nSheetCount > 0) {
        int nSection = 20, nSectionEnd = 0, nRowEnd = 20;
        QAxObject* pWorkSheet = m_pWorkBook->querySubObject("Sheets(int)", 1);//��ȡ��һ�ű�
        QVariantList params;
        QVariant varData;
        for (int i = 1; i <= nRowEnd; i += nSection) {
            nSectionEnd = i + nSection - 1;
            if (nSectionEnd > nRowEnd) {
                nSectionEnd = nRowEnd;
            }
            char cZimu = 1 + 64; //1-26�ֱ����A-Z������Excel����б�ʶ
            //Ai��Ai+nSectionEnd������,����Ϊ�˲���һ���Ƕ�ȡ20��
            params << QString("%1%2").arg(cZimu).arg(i) << QString("%1%2").arg(cZimu).arg(nSectionEnd);
            QAxObject* pCell = pWorkSheet->querySubObject("Range(QVariant,QVariant)", params);
            varData = pCell->dynamicCall("Value2()");
            //qDebug() << varData;
        }
        //3.�����ĵ��Ѿ����������ͼ�����Է������Ƕ��������ݰ������������¡�ǰ���㶼��QVariantList��
        //Ϊ��ȡ����ȷ�����ݣ�������Ҫ������Щ���¡�
        QVariantList varList = varData.toList(); //�����һ��List
        if (varList.count() <= 0) {//��ֹ�ڴ����
            qDebug("�����ݣ�");
            m_pWorkBooks->dynamicCall("Close()");
            return;
        }
        QVariantList varLstData = varList[0].toList();
        //����ڶ���List
        if (varLstData.count() <= 0) {
            qDebug("�����ݣ�");
            m_pWorkBooks->dynamicCall("Close()");
            return;
        }
        //�ж��������ͣ���ֹת������
        if (QString(varLstData[0].typeName()) == "QString") {
            QString str = varLstData[0].toString(); //ȡ������
            qDebug("�ַ�����%s", str);
        }

        if (QString(varLstData[0].typeName()) == "double") {
            double dData = varLstData[0].toDouble(); //ȡ������
            qDebug("%0.1f", dData); //ע�⣬int�Ͷ�����Ҳ��double������һ��Ҫע��ת��
        }
    }
    m_pWorkBooks->dynamicCall("Close()");
    m_pWorkBook = NULL;
}

void jpeggui::importExlToDbs(void)
{
    QString strFilePathName = QFileDialog::getOpenFileName(this, QStringLiteral("ѡ��Excel�ļ�"), "", tr("Exel file(*.xls *.xlsx *.xlsm)"));
    if (strFilePathName.isNull()) {
        return;
    }
    QAxObject* Exl = new QAxObject(this);	//����Exl�ؼ�
    if (Exl->setControl("Excel.Application")) {
    }
    else {
        Exl->setControl("ket.Application");  //����Exl�ؼ�
    }
    Exl->setProperty("Visible", false);  //����ʾ����
    QAxObject* workbooks = Exl->querySubObject("WorkBooks");  //��ȡ����������
    workbooks->dynamicCall("Open(const QString&)", strFilePathName); //�򿪴��Ѵ��ڵĹ�����
    QAxObject* workbook = Exl->querySubObject("ActiveWorkBook"); //��ȡ��ǰ������
    QAxObject* sheets = workbook->querySubObject("Sheets");  //��ȡ�������ϣ�SheetsҲ�ɻ���WorkSheets
    QAxObject* sheet = workbook->querySubObject("WorkSheets(int)", 1);//��ȡ�������ϵĹ�����1����sheet1
    QAxObject* range = sheet->querySubObject("UsedRange"); //��ȡ��sheet��ʹ�÷�Χ����
    QVariant var = range->dynamicCall("Value");
    delete range;
    QVariantList varRows = var.toList();  //�õ�����е���������
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
    int CurLine = getRowCount();
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
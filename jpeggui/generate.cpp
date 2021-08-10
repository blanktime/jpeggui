#define _CRT_SECURE_NO_WARNINGS

#include "generate.h"
#include "jpeggui.h"
#include <QJsonObject>  
#include <QJsonArray> 
#include <QJsonDocument> 
#include <QDir>

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>
#include <time.h>

#include "jpeglib.h"
#include "libexif/exif-data.h"


QString jpeggui::filename;

generate::generate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    model = new QStandardItemModel(this);
    labels = QObject::trUtf8("IFD,name,tag,default").simplified().split(",");//标签名
    model->setHorizontalHeaderLabels(labels);
    ui.tableView->setModel(model);
    ui.tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);//设置左对齐
    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//tableview表格自适应宽度
    //ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableView->setSelectionBehavior(QTableView::SelectRows);
}

generate::~generate()
{
}

//添加信息至tableview
void generate::addButton(){
    model = new QStandardItemModel(this);
    labels = QObject::trUtf8("IFD,name,tag,default").simplified().split(",");//标签名
    model->setHorizontalHeaderLabels(labels);
    ui.tableView->setModel(model);
    ui.tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);//设置左对齐
    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//tableview表格自适应宽度
    ui.tableView->setSelectionBehavior(QTableView::SelectRows);
    QList<QStandardItem*> list;

    int rowNum;
    //JFIF信息
    if (ui.checkBox_JFIF->isChecked()) {
        rowNum = model->rowCount();
        //JFIF标志
        list << new QStandardItem("JFIF") << new QStandardItem("color components") << new QStandardItem(" ") << new QStandardItem("3");
        model->insertRow(rowNum + 0, list);
        list.clear();
        //压缩类型
        list << new QStandardItem("JFIF") << new QStandardItem("Compression Type") << new QStandardItem(" ") << new QStandardItem("Baseline");
        model->insertRow(rowNum + 1, list);
        list.clear();
        //宽
        list << new QStandardItem("JFIF") << new QStandardItem("Width") << new QStandardItem(" ") << new QStandardItem("3000");
        model->insertRow(rowNum + 2, list);
        list.clear();
        //高
        list << new QStandardItem("JFIF") << new QStandardItem("Height") << new QStandardItem(" ") << new QStandardItem("4000");
        model->insertRow(rowNum + 3, list);
        list.clear();
        //版本 1.2
        list << new QStandardItem("JFIF") << new QStandardItem("Major Version") << new QStandardItem(" ") << new QStandardItem("1");
        model->insertRow(rowNum + 4, list);
        list.clear();
        list << new QStandardItem("JFIF") << new QStandardItem("Minor Version") << new QStandardItem(" ") << new QStandardItem("2");
        model->insertRow(rowNum + 5, list);
        list.clear();
        //单位 
        list << new QStandardItem("JFIF") << new QStandardItem("Units") << new QStandardItem(" ") << new QStandardItem("00");
        model->insertRow(rowNum + 6, list);
        list.clear();
        //水平像素密度
        list << new QStandardItem("JFIF") << new QStandardItem("Xdensity") << new QStandardItem(" ") << new QStandardItem("96");
        model->insertRow(rowNum + 7, list);
        list.clear();
        //垂直像素密度
        list << new QStandardItem("JFIF") << new QStandardItem("Ydensity") << new QStandardItem(" ") << new QStandardItem("96");
        model->insertRow(rowNum + 8, list);
        list.clear();
        //嵌入RGB缩略图水平像素值
        list << new QStandardItem("JFIF") << new QStandardItem("Thumbnail Horizontal Pixels") << new QStandardItem(" ") << new QStandardItem("0");
        model->insertRow(rowNum + 9, list);
        list.clear();
        //嵌入RGB缩略图垂直像素值
        list << new QStandardItem("JFIF") << new QStandardItem("Thumbnail Vertical  Pixels") << new QStandardItem(" ") << new QStandardItem("0");
        model->insertRow(rowNum + 10, list);
        list.clear();
        //质量因子
        list << new QStandardItem("JFIF") << new QStandardItem("Quality Factor") << new QStandardItem(" ") << new QStandardItem("90");
        model->insertRow(rowNum + 11, list);
        list.clear();
        ////亮度表
        //list << new QStandardItem("JFIF") << new QStandardItem("luminance Quantity Table") << new QStandardItem(" ") << new QStandardItem("16, 11, 10, 16, 24, 40, 51, 61,12, 12, 14, 19, 26, 58, 60, 55,14, 13, 16, 24, 40, 57, 69, 56,14, 17, 22, 29, 51, 87, 80, 62,18, 22, 37, 56, 68, 109, 103, 77,24, 35, 55, 64, 81, 104, 113, 92,49, 64, 78, 87, 103, 121, 120, 101,72, 92, 95, 98, 112, 100, 103, 99");
        //model->insertRow(rowNum + 12, list);
        //list.clear();
        ////色度表
        //list << new QStandardItem("JFIF") << new QStandardItem("chrominance Quantity Table") << new QStandardItem(" ") << new QStandardItem("17, 18, 24, 47, 99, 99, 99, 99,18, 21, 26, 66, 99, 99, 99, 99,24, 26, 56, 99, 99, 99, 99, 99,47, 66, 99, 99, 99, 99, 99, 99,99, 99, 99, 99, 99, 99, 99, 99,99, 99, 99, 99, 99, 99, 99, 99,99, 99, 99, 99, 99, 99, 99, 99,99, 99, 99, 99, 99, 99, 99, 99");
        //model->insertRow(rowNum + 13, list);
        //list.clear();
    }

    //IFD0信息
    if (ui.checkBox_IFD0->isChecked()) {
        rowNum = model->rowCount();
        //制造商
        list << new QStandardItem("IFD0") << new QStandardItem("Make") << new QStandardItem("010F") << new QStandardItem("Nikon");
        model->insertRow(rowNum + 0, list);
        list.clear();
        //型号
        list << new QStandardItem("IFD0") << new QStandardItem("Model") << new QStandardItem("0110") << new QStandardItem("ES105");
        model->insertRow(rowNum + 1, list);
        list.clear();
        //方向
        list << new QStandardItem("IFD0") << new QStandardItem("Orientation") << new QStandardItem("0112") << new QStandardItem("1");
        model->insertRow(rowNum + 2, list);
        list.clear();
        //水平分辨率
        list << new QStandardItem("IFD0") << new QStandardItem("XResolution") << new QStandardItem("011a") << new QStandardItem("72");
        model->insertRow(rowNum + 3, list);
        list.clear();
        //垂直分辨率
        list << new QStandardItem("IFD0") << new QStandardItem("YResolution") << new QStandardItem("011b") << new QStandardItem("72");
        model->insertRow(rowNum + 4, list);
        list.clear();
        //分辨率单位
        list << new QStandardItem("IFD0") << new QStandardItem("ResolutionUnit") << new QStandardItem("0128") << new QStandardItem("2");
        model->insertRow(rowNum + 5, list);
        list.clear();
        //软件
        list << new QStandardItem("IFD0") << new QStandardItem("Software") << new QStandardItem("0131") << new QStandardItem("Camera");
        model->insertRow(rowNum + 6, list);
        list.clear();
        //版权
        list << new QStandardItem("IFD0") << new QStandardItem("Copyright") << new QStandardItem("8298") << new QStandardItem("blank");
        model->insertRow(rowNum + 7, list);
        list.clear();
        //修改时间
        list << new QStandardItem("IFD0") << new QStandardItem("DateTime") << new QStandardItem("0132") << new QStandardItem("2001-01-01 12:00:00");
        model->insertRow(rowNum + 8, list);
        list.clear();
        //图像白点_fir
        list << new QStandardItem("IFD0") << new QStandardItem("WhitePoint_fir") << new QStandardItem("013e") << new QStandardItem("0.3127");
        model->insertRow(rowNum + 9, list);
        list.clear();
        //图像白点_mid
        list << new QStandardItem("IFD0") << new QStandardItem("WhitePoint_mid") << new QStandardItem("013e") << new QStandardItem("0.3290");
        model->insertRow(rowNum + 10, list);
        list.clear();
        //YCbCr转RGB格式的常量_fir
        list << new QStandardItem("IFD0") << new QStandardItem("YCbCrCoefficients_fir") << new QStandardItem("0211") << new QStandardItem("0.299");
        model->insertRow(rowNum + 11, list);
        list.clear();
        //YCbCr转RGB格式的常量_mid
        list << new QStandardItem("IFD0") << new QStandardItem("YCbCrCoefficients_mid") << new QStandardItem("0211") << new QStandardItem("0.587");
        model->insertRow(rowNum + 12, list);
        list.clear();
        //YCbCr转RGB格式的常量_las
        list << new QStandardItem("IFD0") << new QStandardItem("YCbCrCoefficients_las") << new QStandardItem("0211") << new QStandardItem("0.114");
        model->insertRow(rowNum + 13, list);
        list.clear();
        //YCbCr定位方法
        list << new QStandardItem("IFD0") << new QStandardItem("YCbCrPositioning") << new QStandardItem("0213") << new QStandardItem("2");
        model->insertRow(rowNum + 14, list);
        list.clear();
    }

    //SubIFD信息
    if (ui.checkBox_SubIFD->isChecked()) {
        rowNum = model->rowCount();
        //拍摄时间
        list << new QStandardItem("SubIFD") << new QStandardItem("DateTimeOriginal") << new QStandardItem("9003") << new QStandardItem("2001-01-01 12:00:00");
        model->insertRow(rowNum + 0, list);
        list.clear();
        //数字化时间
        list << new QStandardItem("SubIFD") << new QStandardItem("Date Time Digitized") << new QStandardItem("9004") << new QStandardItem("2001-01-01 12:00:00");
        model->insertRow(rowNum + 1, list);
        list.clear();
        //曝光时间
        list << new QStandardItem("SubIFD") << new QStandardItem("ExposureTime") << new QStandardItem("829a") << new QStandardItem("250");
        model->insertRow(rowNum + 2, list);
        list.clear();
        //光圈F值
        list << new QStandardItem("SubIFD") << new QStandardItem("FNumber") << new QStandardItem("829d") << new QStandardItem("4");
        model->insertRow(rowNum + 3, list);
        list.clear();
        //曝光程序
        list << new QStandardItem("SubIFD") << new QStandardItem("ExposureProgram") << new QStandardItem("8822") << new QStandardItem("2");
        model->insertRow(rowNum + 4, list);
        list.clear();
        //感光度
        list << new QStandardItem("SubIFD") << new QStandardItem("ISOSpeedRatings") << new QStandardItem("8827") << new QStandardItem("200");
        model->insertRow(rowNum + 5, list);
        list.clear();
        //快门速度
        list << new QStandardItem("SubIFD") << new QStandardItem("Shutter Speed Value") << new QStandardItem("9201") << new QStandardItem("256");
        model->insertRow(rowNum + 6, list);
        list.clear();
        //光圈
        list << new QStandardItem("SubIFD") << new QStandardItem("Aperture Value") << new QStandardItem("9202") << new QStandardItem("4");
        model->insertRow(rowNum + 7, list);
        list.clear();
        //亮度
        list << new QStandardItem("SubIFD") << new QStandardItem("BrightnessValue") << new QStandardItem("9203") << new QStandardItem("7");
        model->insertRow(rowNum + 8, list);
        list.clear();
        //曝光补偿
        list << new QStandardItem("SubIFD") << new QStandardItem("ExposureBiasValue") << new QStandardItem("9204") << new QStandardItem("0");
        model->insertRow(rowNum + 9, list);
        list.clear();
        //最大光圈
        list << new QStandardItem("SubIFD") << new QStandardItem("MaxApertureValue") << new QStandardItem("9205") << new QStandardItem("4");
        model->insertRow(rowNum + 10, list);
        list.clear();
        //主体距离
        list << new QStandardItem("SubIFD") << new QStandardItem("SubjectDistance") << new QStandardItem("9206") << new QStandardItem("0");
        model->insertRow(rowNum + 11, list);
        list.clear();
        //测光模式
        list << new QStandardItem("SubIFD") << new QStandardItem("MeteringMode") << new QStandardItem("9207") << new QStandardItem("2");
        model->insertRow(rowNum + 12, list);
        list.clear();
        //光源
        list << new QStandardItem("SubIFD") << new QStandardItem("LightSource") << new QStandardItem("9208") << new QStandardItem("3");
        model->insertRow(rowNum + 13, list);
        list.clear();
        //闪光灯
        list << new QStandardItem("SubIFD") << new QStandardItem("Flash") << new QStandardItem("9209") << new QStandardItem("0");
        model->insertRow(rowNum + 14, list);
        list.clear();
        //镜头焦距
        list << new QStandardItem("SubIFD") << new QStandardItem("FocalLength") << new QStandardItem("920a") << new QStandardItem("50");
        model->insertRow(rowNum + 15, list);
        list.clear();
        //色彩空间
        list << new QStandardItem("SubIFD") << new QStandardItem("ColorSpace") << new QStandardItem("a001") << new QStandardItem("1");
        model->insertRow(rowNum + 16, list);
        list.clear();
        //图像宽度
        list << new QStandardItem("SubIFD") << new QStandardItem("ExifImageWidth") << new QStandardItem("a002") << new QStandardItem("1024");
        model->insertRow(rowNum + 17, list);
        list.clear();
        //图像高度
        list << new QStandardItem("SubIFD") << new QStandardItem("ExifImageHeight") << new QStandardItem("a003") << new QStandardItem("1024");
        model->insertRow(rowNum + 18, list);
        list.clear();
        //焦平面水平分辨率
        list << new QStandardItem("SubIFD") << new QStandardItem("FocalPlaneXResolution") << new QStandardItem("a20e") << new QStandardItem("0");
        model->insertRow(rowNum + 19, list);
        list.clear();
        //焦平面垂直分辨率
        list << new QStandardItem("SubIFD") << new QStandardItem("FocalPlaneYResolution") << new QStandardItem("a20f") << new QStandardItem("0");
        model->insertRow(rowNum + 20, list);
        list.clear();
        //焦平面分辨率单位
        list << new QStandardItem("SubIFD") << new QStandardItem("FocalPlaneResolutionUnit") << new QStandardItem("a210") << new QStandardItem("2");
        model->insertRow(rowNum + 21, list);
        list.clear();
        //曝光指数
        list << new QStandardItem("SubIFD") << new QStandardItem("ExposureIndex") << new QStandardItem("a215") << new QStandardItem("0");
        model->insertRow(rowNum + 22, list);
        list.clear();
        //传感器类型
        list << new QStandardItem("SubIFD") << new QStandardItem("SensingMethod") << new QStandardItem("a217") << new QStandardItem("2");
        model->insertRow(rowNum + 23, list);
        list.clear();
        //用户注释
        list << new QStandardItem("SubIFD") << new QStandardItem("UserComment") << new QStandardItem("9286") << new QStandardItem("blank");
        model->insertRow(rowNum + 24, list);
        list.clear();
    }

    //GPSIFD信息
    if (ui.checkBox_GPSIFD->isChecked()) {
        rowNum = model->rowCount();
        //GPS版本 2.2.0.0
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSVersionID") << new QStandardItem("0000") << new QStandardItem("0x02,0x02,0x00,0x00");
        model->insertRow(rowNum + 0, list);
        list.clear();
        //GPS纬度参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLatitudeRef") << new QStandardItem("0001") << new QStandardItem("N");
        model->insertRow(rowNum + 1, list);
        list.clear();
        //GPS纬度fir
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLatitude_fir") << new QStandardItem("0002") << new QStandardItem("31");
        model->insertRow(rowNum + 2, list);
        list.clear();
        //GPS纬度mid
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLatitude_mid") << new QStandardItem("0002") << new QStandardItem("0");
        model->insertRow(rowNum + 3, list);
        list.clear();
        //GPS纬度las
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLatitude_las") << new QStandardItem("0002") << new QStandardItem("0");
        model->insertRow(rowNum + 4, list);
        list.clear();
        //GPS经度参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLongitudeRef") << new QStandardItem("0003") << new QStandardItem("E");
        model->insertRow(rowNum + 5, list);
        list.clear();
        //GPS经度fir
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLongitude_fir") << new QStandardItem("0004") << new QStandardItem("121");
        model->insertRow(rowNum + 6, list);
        list.clear();
        //GPS经度mid
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLongitude_mid") << new QStandardItem("0004") << new QStandardItem("0");
        model->insertRow(rowNum + 7, list);
        list.clear();
        //GPS经度las
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSLongitude_las") << new QStandardItem("0004") << new QStandardItem("0");
        model->insertRow(rowNum + 8, list);
        list.clear();
        //GPS高度参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSAltitudeRef") << new QStandardItem("0005") << new QStandardItem("0");
        model->insertRow(rowNum + 9, list);
        list.clear();
        //GPS高度
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSAltitude") << new QStandardItem("0006") << new QStandardItem("0");
        model->insertRow(rowNum + 10, list);
        list.clear();
        //GPS接收状态
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSStatus") << new QStandardItem("0009") << new QStandardItem("A");
        model->insertRow(rowNum + 11, list);
        list.clear();
        //GPS测量模式
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSMeasureMode") << new QStandardItem("000a") << new QStandardItem("3");
        model->insertRow(rowNum + 12, list);
        list.clear();
        //GPS测量精度
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDOP") << new QStandardItem("000b") << new QStandardItem("1");
        model->insertRow(rowNum + 13, list);
        list.clear();
        //GPS速度单位
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSSpeedRef") << new QStandardItem("000c") << new QStandardItem("K");
        model->insertRow(rowNum + 14, list);
        list.clear();
        //GPS速度
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSSpeed") << new QStandardItem("000d") << new QStandardItem("0");
        model->insertRow(rowNum + 15, list);
        list.clear();
        //GPS移动方向参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSTrackRef") << new QStandardItem("000e") << new QStandardItem("T");
        model->insertRow(rowNum + 16, list);
        list.clear();
        //GPS移动方向
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSTrack") << new QStandardItem("000f") << new QStandardItem("0");
        model->insertRow(rowNum + 17, list);
        list.clear();
        //GPS朝向参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSImgDirectionRef") << new QStandardItem("0010") << new QStandardItem("T");
        model->insertRow(rowNum + 18, list);
        list.clear();
        //GPS朝向
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSImgDirection") << new QStandardItem("0011") << new QStandardItem("0");
        model->insertRow(rowNum + 19, list);
        list.clear();
        //GPS基准坐标
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSMapDatum") << new QStandardItem("0012") << new QStandardItem("TOKYO");
        model->insertRow(rowNum + 20, list);
        list.clear();
        //GPS目的地纬度参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLatitudeRef") << new QStandardItem("0013") << new QStandardItem("N");
        model->insertRow(rowNum + 21, list);
        list.clear();
        //GPS目的地纬度fir
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLatitude_fir") << new QStandardItem("0014") << new QStandardItem("31");
        model->insertRow(rowNum + 22, list);
        list.clear();
        //GPS目的地纬度mid
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLatitude_mid") << new QStandardItem("0014") << new QStandardItem("0");
        model->insertRow(rowNum + 23, list);
        list.clear();
        //GPS目的地纬度las
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLatitude_las") << new QStandardItem("0014") << new QStandardItem("0");
        model->insertRow(rowNum + 24, list);
        list.clear();
        //GPS目的地经度参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLongitudeRef") << new QStandardItem("0015") << new QStandardItem("E");
        model->insertRow(rowNum + 25, list);
        list.clear();
        //GPS目的地经度fir
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLongitude_fir") << new QStandardItem("0016") << new QStandardItem("121");
        model->insertRow(rowNum + 26, list);
        list.clear();
        //GPS目的地经度mid
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLongitude_mid") << new QStandardItem("0016") << new QStandardItem("0");
        model->insertRow(rowNum + 27, list);
        list.clear();
        //GPS目的地经度las
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestLongitude_las") << new QStandardItem("0016") << new QStandardItem("0");
        model->insertRow(rowNum + 28, list);
        list.clear();
        //GPS目的地方位参考
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestBearingRef") << new QStandardItem("0017") << new QStandardItem("T");
        model->insertRow(rowNum + 29, list);
        list.clear();
        //GPS目的地方位
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestBearing") << new QStandardItem("0018") << new QStandardItem("0");
        model->insertRow(rowNum + 30, list);
        list.clear();
        //GPS目的地距离单位
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestDistanceRef") << new QStandardItem("0019") << new QStandardItem("K");
        model->insertRow(rowNum + 31, list);
        list.clear();
        //GPS目的地距离
        list << new QStandardItem("GPSIFD") << new QStandardItem("GPSDestDistance") << new QStandardItem("001a") << new QStandardItem("0");
        model->insertRow(rowNum + 32, list);
        list.clear();
    }

    //IOP信息
    if (ui.checkBox_IOP->isChecked()) {
        rowNum = model->rowCount();
        //可互通标准
        list << new QStandardItem("IOP") << new QStandardItem("InteroperabilityIndex") << new QStandardItem("0001") << new QStandardItem("R98");
        model->insertRow(rowNum + 0, list);
        list.clear();
        //相关图像格式
        list << new QStandardItem("IOP") << new QStandardItem("RelatedImageFileFormat") << new QStandardItem("1000") << new QStandardItem("Exif JPEG Ver. 2.1");
        model->insertRow(rowNum + 1, list);
        list.clear();
        //相关图像宽度
        list << new QStandardItem("IOP") << new QStandardItem("RelatedImageWidth") << new QStandardItem("1001") << new QStandardItem("0");
        model->insertRow(rowNum + 2, list);
        list.clear();
        //相关图像高度
        list << new QStandardItem("IOP") << new QStandardItem("RelatedImageLength") << new QStandardItem("1002") << new QStandardItem("0");
        model->insertRow(rowNum + 3, list);
        list.clear();
    }

    //IFD1信息
    if (ui.checkBox_IFD1->isChecked()) {
        rowNum = model->rowCount();
        //缩略图宽度
        list << new QStandardItem("IFD1") << new QStandardItem("ImageWidth") << new QStandardItem("0100") << new QStandardItem("0");
        model->insertRow(rowNum + 0, list);
        list.clear();
        //缩略图高度
        list << new QStandardItem("IFD1") << new QStandardItem("ImageLength") << new QStandardItem("0101") << new QStandardItem("0");
        model->insertRow(rowNum + 1, list);
        list.clear();
        //每像素比特位数目1
        list << new QStandardItem("IFD1") << new QStandardItem("BitsPerSample_1") << new QStandardItem("0102") << new QStandardItem("8");
        model->insertRow(rowNum + 2, list);
        list.clear();
        //每像素比特位数目2
        list << new QStandardItem("IFD1") << new QStandardItem("BitsPerSample_2") << new QStandardItem("0102") << new QStandardItem("8");
        model->insertRow(rowNum + 3, list);
        list.clear();
        //每像素比特位数目3
        list << new QStandardItem("IFD1") << new QStandardItem("BitsPerSample_3") << new QStandardItem("0102") << new QStandardItem("8");
        model->insertRow(rowNum + 4, list);
        list.clear();
        //压缩方案
        list << new QStandardItem("IFD1") << new QStandardItem("Compression") << new QStandardItem("0103") << new QStandardItem("6");
        model->insertRow(rowNum + 5, list);
        list.clear();
        //图像数据组件色彩空间
        list << new QStandardItem("IFD1") << new QStandardItem("PhotometricInterpretation") << new QStandardItem("0106") << new QStandardItem("6");
        model->insertRow(rowNum + 6, list);
        list.clear();
        //制造商
        list << new QStandardItem("IFD1") << new QStandardItem("Make") << new QStandardItem("010F") << new QStandardItem(" ");
        model->insertRow(rowNum + 7, list);
        list.clear();
        //型号
        list << new QStandardItem("IFD1") << new QStandardItem("Model") << new QStandardItem("0110") << new QStandardItem(" ");
        model->insertRow(rowNum + 8, list);
        list.clear();
        //方位
        list << new QStandardItem("IFD1") << new QStandardItem("Orientation") << new QStandardItem("0112") << new QStandardItem("1");
        model->insertRow(rowNum + 9, list);
        list.clear();
        //水平分辨率
        list << new QStandardItem("IFD1") << new QStandardItem("XResolution") << new QStandardItem("011a") << new QStandardItem("72");
        model->insertRow(rowNum + 10, list);
        list.clear();
        //垂直分辨率
        list << new QStandardItem("IFD1") << new QStandardItem("YResolution") << new QStandardItem("011b") << new QStandardItem("72");
        model->insertRow(rowNum + 11, list);
        list.clear();
        //分辨率单位
        list << new QStandardItem("IFD1") << new QStandardItem("ResolutionUnit") << new QStandardItem("0128") << new QStandardItem("1");
        model->insertRow(rowNum + 12, list);
        list.clear();
        //软件
        list << new QStandardItem("IFD1") << new QStandardItem("Software") << new QStandardItem("0131") << new QStandardItem(" ");
        model->insertRow(rowNum + 13, list);
        list.clear();
        //修改时间
        list << new QStandardItem("IFD1") << new QStandardItem("DateTime") << new QStandardItem("0132") << new QStandardItem("2001-01-01 12:00:00");
        model->insertRow(rowNum + 14, list);
        list.clear();
        //作者
        list << new QStandardItem("IFD1") << new QStandardItem("Artist") << new QStandardItem("013b") << new QStandardItem(" ");
        model->insertRow(rowNum + 15, list);
        list.clear();
        //YCbCr定位方法
        list << new QStandardItem("IFD1") << new QStandardItem("YCbCrPositioning") << new QStandardItem("0213") << new QStandardItem("1");
        model->insertRow(rowNum + 16, list);
        list.clear();
        //版权
        list << new QStandardItem("IFD1") << new QStandardItem("Copyright") << new QStandardItem("8928") << new QStandardItem(" ");
        model->insertRow(rowNum + 17, list);
        list.clear();
    }

    //设置代理
    ReadOnlyDelegate* readOnlyDelegate = new ReadOnlyDelegate();
    for (int i = 0; i < 3; i++) {//设置前三列只读
        ui.tableView->setItemDelegateForColumn(i, readOnlyDelegate); 
    }
}

//删除某一行信息
void generate::deleteButton()
{
    QModelIndex curIndex = ui.tableView->currentIndex();//获取模型索引
    if (curIndex.row() == model->rowCount() - 1) //最后一行
        model->removeRow(curIndex.row()); //删除最后一行
    else {
        model->removeRow(curIndex.row());//删除一行，并重新设置当前选择行
        ui.tableView->setCurrentIndex(curIndex);
    }
}

/***************************************************** JFIF *****************************************************************************/
struct my_error_mgr {
    struct jpeg_error_mgr pub;    /* “公共”区域 */
    jmp_buf setjmp_buffer;        /* 返回调用 */
};

typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

GLOBAL(int) rw_jpeg_JFIF(QJsonObject json, QString outfilepath) {
    JSAMPLE* image_buffer;       /* R,G,B数据数组 */
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;    /* 错误处理 */
    FILE* infile;                /* 源文件 */
    JSAMPARRAY buffer;           /* 输出行缓冲区 */
    int row_stride;              /* 输出缓冲区中的物理行宽度 */

    if ((infile = fopen((jpeggui::filename).toStdString().c_str(), "rb")) == NULL) {
        qDebug() << "can't open infile!\n";
        return 1;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return 1;
    }
    jpeg_create_decompress(&cinfo);         /* 初始化 */

    jpeg_stdio_src(&cinfo, infile);

    (void)jpeg_read_header(&cinfo, TRUE);   /* IJG将图像的缺省信息填充到cinfo结构中以便程序使用 */
    //cinfo.scale_num = 1;
    //cinfo.scale_denom = 1;
    //cinfo.out_color_space = JCS_RGB;       //灰度或者彩色
    jpeg_start_decompress(&cinfo);          /* 根据设定的解压缩参数进行图像解压缩操作 */
    row_stride = cinfo.output_width * cinfo.output_components;   /* 计算每行需要的空间大小，比如RGB图像就是宽度×3，灰度图就是宽度×1 */
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);     /* 使用了IJG的内存管理器来完成分配，分配每行数组大小 */

    /* 写入操作 */
    struct jpeg_compress_struct wcinfo;      /* JPEG对象 */
    struct jpeg_error_mgr wjerr;             /* 错误处理 */
    FILE* outfile;                           /* 目标文件 */
    JSAMPROW row_pointer[1];                 /* 指向JSAMPLE row[s] */
    int wrow_stride;                         /* 图像缓冲区中的物理行宽度 */

    wcinfo.err = jpeg_std_error(&wjerr);     /* 错误处理结构对象绑定在JPEG对象上 */

    jpeg_create_compress(&wcinfo);       /* 初始化 */
    if ((outfile = fopen(outfilepath.toStdString().c_str(), "wb")) == NULL) {
        qDebug() << "can't open outfile!\n";
        return 1;
    }

    jpeg_stdio_dest(&wcinfo, outfile);

    /************************************************** 参数设置 *********************************************************/
    int scale_factor = 50;

    /*unsigned int std_luminance_quant_tbl[64] = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109,103,77,
        24, 35, 55, 64, 81, 104,113,92,
        49, 64, 78, 87, 103,121,120,101,
        72, 92, 95, 98, 112,100,103,99
    };
    unsigned int std_chrominance_quant_tbl[64] = {
        17, 18, 24, 47, 99, 99, 99, 99,
        18, 21, 26, 66, 99, 99, 99, 99,
        24, 26, 56, 99, 99, 99, 99, 99,
        47, 66, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99
    };*/

    //赋值
    QJsonValue JFIFvalue = json.value("JFIF");
    QJsonObject obj = JFIFvalue.toObject();
    if (obj.contains("Width") && (obj.value("Width").toVariant().toInt() < cinfo.image_width)) {    //宽
        wcinfo.image_width = obj.value("Width").toVariant().toInt();
    }
    else {
        wcinfo.image_width = cinfo.image_width;  //宽度默认值
    }

    if (obj.contains("Height") && (obj.value("Height").toVariant().toInt() < cinfo.image_height)) {   //高
        wcinfo.image_height = obj.value("Height").toVariant().toInt();
    }
    else {
        wcinfo.image_height = cinfo.image_height;//高度默认值
    }

    if (obj.contains("color components")) {   //颜色通道数
        QJsonValue value = obj.value("color components");
        wcinfo.input_components = value.toVariant().toInt();    /*of color components per pixel */
        if (wcinfo.input_components == 1) {
            wcinfo.in_color_space = JCS_GRAYSCALE;     /* colorspace of input image */
        }
        else {
            wcinfo.in_color_space = JCS_RGB;
        }
    }
    else {
        wcinfo.input_components = 3;    //颜色通道数和色彩空间默认值
        wcinfo.in_color_space = JCS_RGB;
    }

    if (obj.contains("Quality Factor")) {   //质量因子
        QJsonValue value = obj.value("Quality Factor");
        scale_factor = value.toVariant().toInt();
    }
    //if (obj.contains("luminance Quantity Table")) {   //亮度表
    //    QJsonValue value = obj.value("luminance Quantity Table");
    //    unsigned int std_luminance_quant_tbl[64] = { value.toVariant().toInt() };
    //}
    //if (obj.contains("chrominance Quantity Table")) {   //色度表
    //    QJsonValue value = obj.value("chrominance Quantity Table");
    //    unsigned int std_chrominance_quant_tbl[64] = { value.toVariant().toInt() };
    //}     

    //指令集
    jpeg_set_defaults(&wcinfo);
    if (!obj.isEmpty()) {
        if (obj.contains("Major Version") && obj.contains("Minor Version")) {
            wcinfo.JFIF_major_version = obj.value("Major Version").toVariant().toInt();
            wcinfo.JFIF_minor_version = obj.value("Minor Version").toVariant().toInt();
        }
        if (obj.contains("Quality Factor")) {
            jpeg_set_quality(&wcinfo, scale_factor, FALSE);
        }
        /*if (obj.contains("luminance Quantity Table") && obj.contains("chrominance Quantity Table")) {
            jpeg_add_quant_table(&wcinfo, 0, std_luminance_quant_tbl, scale_factor, TRUE);
            jpeg_add_quant_table(&wcinfo, 1, std_chrominance_quant_tbl, scale_factor, TRUE);
        }
        else {
            jpeg_default_qtables(&wcinfo, FALSE);
        }*/
        if (obj.contains("Compression Type") && (obj.value("Compression Type").toString() == "Progressive")) {
            jpeg_simple_progression(&wcinfo);        //设置progressive类型
        }
    }
    else {
        wcinfo.write_JFIF_header = FALSE;
    }

    /*开始压缩*/
    jpeg_start_compress(&wcinfo, TRUE);     //开始压缩
    if (wcinfo.input_components == 3) { 
        wrow_stride = cinfo.image_width * 3;    /* JSAMPLEs per row in image_buffer */
    }
    else {
        wrow_stride = cinfo.image_width * 1;    /* JSAMPLEs per row in image_buffer */
    }

    while (cinfo.output_scanline < cinfo.output_height) {
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);     //解压出数据
        (void)jpeg_write_scanlines(&wcinfo, buffer, 1);   //压缩数据
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    jpeg_finish_compress(&wcinfo);
    jpeg_destroy_compress(&wcinfo);
    fclose(outfile);

    return 0;
}

/***************************************************** EXIF *****************************************************************************/

/* EXIF 头数据字节顺序 */
#define FILE_BYTE_ORDER_MOTOROLA EXIF_BYTE_ORDER_MOTOROLA
#define FILE_BYTE_ORDER_INTEL EXIF_BYTE_ORDER_INTEL

static const unsigned char GPSVersion[] = {0x02, 0x02, 0x00, 0x00};

/* EXIF_TAG_USER_COMMENT 特殊开头 */
#define ASCII_COMMENT "ASCII\0\0\0"

/* 获取EXIF中现有的标签，如果不存在，则创建一个 */
static ExifEntry* init_tag(ExifData* exif, ExifIfd ifd, ExifTag tag)
{
    ExifEntry* entry;
    /* 如果存在则返回一个标签 */
    if (!((entry = exif_content_get_entry(exif->ifd[ifd], tag)))) {
        /* 分配一个新 entry */
        entry = exif_entry_new();
        assert(entry != NULL); /* 捕获内存不足的情况 */
        entry->tag = tag; /* 在使用 exif_content_add_entry() 函数前必须设置标签 */

        /* 将 ExifEntry 附加到IFD上 */
        exif_content_add_entry(exif->ifd[ifd], entry);

        /* 为 entry 分配内存并初始化默认数据 */
        exif_entry_initialize(entry, tag);

        exif_entry_unref(entry);
    }
    return entry;
}

/* 在给定的IFD中，使用给定长度的数据字段创建一个全新的标记。当 exif_entry_initialize() 不能创建这种类型的标记本身，
   或者它创建的默认数据长度不是正确的长度时，就需要这样做。*/
static ExifEntry* create_tag(ExifData* exif, ExifIfd ifd, ExifTag tag, size_t len)
{
    void* buf;
    ExifEntry* entry;

    /* 创建一个内存分配器来管理这个 ExifEntry */
    ExifMem* mem = exif_mem_new_default();
    assert(mem != NULL); /* 捕获内存不足的情况 */

    /* 使用分配器创建一个新的 ExifEntry */
    entry = exif_entry_new_mem(mem);
    assert(entry != NULL);

    /* 分配内存来保存标签数据 */
    buf = exif_mem_alloc(mem, len);
    assert(buf != NULL);

    /* 填充 entry */
    entry->data = (unsigned char*)buf;
    entry->size = len;
    entry->tag = tag;
    entry->components = len;
    entry->format = EXIF_FORMAT_UNDEFINED;

    /* 将 ExifEntry 附加到IFD上 */
    exif_content_add_entry(exif->ifd[ifd], entry);

    /* ExifMem 和 ExifEntry 存储在其他地方 */
    exif_mem_unref(mem);
    exif_entry_unref(entry);

    return entry;
}

static int add_jpeg_Exif(QJsonObject json, unsigned char* pFrame, unsigned int nFrameLen, const char* pFilePath)
{
    int nRet = 0;
    FILE* pFile = NULL;
    QString str = NULL;
    ExifRational fir, mid, las;

    /* JPEG 图像数据部分的开始 */
    static const unsigned int image_data_offset = 20;

    /* 原始的 EXIF 头数据 */
    static const unsigned char exif_header[] = {
     0xff, 0xd8, 0xff, 0xe1
    };

    /* EXIF 头数据长度 */
    static const unsigned int exif_header_len = sizeof(exif_header);

    unsigned char* exif_data = NULL;
    unsigned int exif_data_len = 0;
    ExifEntry* entry = NULL;
    ExifData* exif = exif_data_new();
    if (!exif) {
        qDebug() << "out of menory!\n";
        return 1;
    }

    /* 设置图像选项 */
    exif_data_set_option(exif, EXIF_DATA_OPTION_FOLLOW_SPECIFICATION);
    exif_data_set_data_type(exif, EXIF_DATA_TYPE_COMPRESSED);
    exif_data_set_byte_order(exif, FILE_BYTE_ORDER_INTEL);

    /* 使用默认数据创建强制性的 EXIF 字段 */
    exif_data_fix(exif);

    //QJson对象
    QJsonValue IFD0value = json.value("IFD0");
    QJsonValue IFD1value = json.value("IFD1");
    QJsonValue SubIFDvalue = json.value("SubIFD");
    QJsonValue GPSIFDvalue = json.value("GPSIFD");
    QJsonValue IOPvalue = json.value("IOP");

    //获取当前时间 pDataTime，方便后续写入
    time_t t = time(NULL);
    struct tm stTime;
    localtime_s(&stTime, &t);
    char pDataTime[20] = { 0 };
    snprintf(pDataTime, sizeof(pDataTime), "%04d-%02d-%02d %02d:%02d:%02d", stTime.tm_year + 1900, stTime.tm_mon + 1, stTime.tm_mday, stTime.tm_hour, stTime.tm_min, stTime.tm_sec);
    /***********************************************GPS IFD**************************************************************/
    if (!GPSIFDvalue.toObject().isEmpty()) {
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_VERSION_ID, 4 * exif_format_get_size(EXIF_FORMAT_BYTE));
        entry->format = EXIF_FORMAT_BYTE;
        entry->components = 4;
        int i = 0;
        for (i = 0; i < 4; i++) {
            exif_set_sshort(entry->data + i, FILE_BYTE_ORDER_INTEL, GPSVersion[i]);
        }

        /* 上海北纬 30度40分~31度53分 */
        str = GPSIFDvalue.toObject().value("GPSLatitudeRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LATITUDE_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* 纬度 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LATITUDE, 3 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 3;
        fir.numerator = GPSIFDvalue.toObject().value("GPSLatitude_fir").toVariant().toInt(); /* 分子 */
        fir.denominator = 1; /* 分母 */
        mid.numerator = GPSIFDvalue.toObject().value("GPSLatitude_mid").toVariant().toInt();
        mid.denominator = 1;
        las.numerator = GPSIFDvalue.toObject().value("GPSLatitude_las").toVariant().toInt();
        las.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);
        exif_set_rational(entry->data + 8, FILE_BYTE_ORDER_INTEL, mid);
        exif_set_rational(entry->data + 16, FILE_BYTE_ORDER_INTEL, las);

        /* 上海东经 120度51分~122度12分*/
        str = GPSIFDvalue.toObject().value("GPSLongitudeRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LONGITUDE_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* 经度 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_LONGITUDE, 3 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 3;
        fir.numerator = GPSIFDvalue.toObject().value("GPSLongitude_fir").toVariant().toInt();
        fir.denominator = 1;
        mid.numerator = GPSIFDvalue.toObject().value("GPSLongitude_mid").toVariant().toInt();
        mid.denominator = 1;
        las.numerator = GPSIFDvalue.toObject().value("GPSLongitude_las").toVariant().toInt();
        las.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);
        exif_set_rational(entry->data + 8, FILE_BYTE_ORDER_INTEL, mid);
        exif_set_rational(entry->data + 16, FILE_BYTE_ORDER_INTEL, las);

        /* 海平面 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_ALTITUDE_REF, 1 * exif_format_get_size(EXIF_FORMAT_BYTE));
        entry->format = EXIF_FORMAT_BYTE;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, GPSIFDvalue.toObject().value("GPSAltitudeRef").toVariant().toInt());

        /* 高度 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_ALTITUDE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = GPSIFDvalue.toObject().value("GPSAltitude").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* GPS 卫星状态 */
        str = GPSIFDvalue.toObject().value("GPSStatus").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_STATUS, 1 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 1;
        memcpy(entry->data, str.toLatin1().data(), 1);

        /* GPS 卫星测量模式 */
        str = GPSIFDvalue.toObject().value("GPSMeasureMode").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_MEASURE_MODE, 1 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 1;
        memcpy(entry->data, str.toLatin1().data(), 1);

        /* GPS 数据精度 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DOP, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = GPSIFDvalue.toObject().value("GPSDOP").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* GPS 速度单位 */
        str = GPSIFDvalue.toObject().value("GPSSpeedRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_SPEED_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* GPS 速度值 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_SPEED, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = GPSIFDvalue.toObject().value("GPSSpeed").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* GPS 移动方位参考 */
        str = GPSIFDvalue.toObject().value("GPSTrackRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_TRACK_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);/* T--true direction，M--magnetic direction */

        /* GPS 移动方位 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_TRACK, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = GPSIFDvalue.toObject().value("GPSTrack").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* GPS 图像方位参考 */
        str = GPSIFDvalue.toObject().value("GPSImgDirectionRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* GPS 图像方位 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_IMG_DIRECTION, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = GPSIFDvalue.toObject().value("GPSImgDirection").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* GPS 地图基准数据 */
        str = GPSIFDvalue.toObject().value("GPSMapDatum").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_MAP_DATUM, 6 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 6;
        memcpy(entry->data, str.toLatin1().data(), 6);

        /* GPS 目标南北纬 */
        str = GPSIFDvalue.toObject().value("GPSDestLatitudeRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_LATITUDE_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* GPS 目标纬度 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_LATITUDE, 3 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 3;
        fir.numerator = GPSIFDvalue.toObject().value("GPSDestLatitude_fir").toVariant().toInt(); /* 分子 */
        fir.denominator = 1; /* 分母 */
        mid.numerator = GPSIFDvalue.toObject().value("GPSDestLatitude_mid").toVariant().toInt();
        mid.denominator = 1;
        las.numerator = GPSIFDvalue.toObject().value("GPSDestLatitude_las").toVariant().toInt();
        las.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);
        exif_set_rational(entry->data + 8, FILE_BYTE_ORDER_INTEL, mid);
        exif_set_rational(entry->data + 16, FILE_BYTE_ORDER_INTEL, las);

        /* GPS 目标东西经 */
        str = GPSIFDvalue.toObject().value("GPSDestLongitudeRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_LONGITUDE_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* GPS 目标经度 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_LONGITUDE, 3 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 3;
        fir.numerator = GPSIFDvalue.toObject().value("GPSDestLongitude_fir").toVariant().toInt(); /* 分子 */
        fir.denominator = 1; /* 分母 */
        mid.numerator = GPSIFDvalue.toObject().value("GPSDestLongitude_mid").toVariant().toInt();
        mid.denominator = 1;
        las.numerator = GPSIFDvalue.toObject().value("GPSDestLongitude_las").toVariant().toInt();
        las.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);
        exif_set_rational(entry->data + 8, FILE_BYTE_ORDER_INTEL, mid);
        exif_set_rational(entry->data + 16, FILE_BYTE_ORDER_INTEL, las);

        /* GPS 目标方位参考 */
        str = GPSIFDvalue.toObject().value("GPSDestBearingRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_BEARING_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* GPS 目标方位 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_BEARING, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = GPSIFDvalue.toObject().value("GPSDestBearing").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* GPS 目标距离参考 */
        str = GPSIFDvalue.toObject().value("GPSDestDistanceRef").toString();
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_DISTANCE_REF, 2 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 2;
        memcpy(entry->data, str.toLatin1().data(), 2);

        /* GPS 目标距离 */
        entry = create_tag(exif, EXIF_IFD_GPS, (ExifTag)EXIF_TAG_GPS_DEST_DISTANCE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = GPSIFDvalue.toObject().value("GPSDestDistance").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);
    }
    /***********************************************Sub IFD**************************************************************/
    if (!SubIFDvalue.toObject().isEmpty()) {
        /* 所有这些标记都是由 exif_data_fix() 用默认值创建的 */
        /* 将数据更改为该图像的正确值。*/
        entry = init_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_PIXEL_X_DIMENSION);
        exif_set_long(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ExifImageWidth").toVariant().toInt());

        entry = init_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_PIXEL_Y_DIMENSION);
        exif_set_long(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ExifImageHeight").toVariant().toInt());

        entry = init_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_COLOR_SPACE);
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, 1);

        /* 创建 EXIF_TAG_USER_COMMENT 标签。*/
        /* 必须以不同的方式处理这个标记，因为该标记不是由 exif_data_fix() 自动创建和分配的，
           也不能使用 exif_entry_initialize() 创建，所以必须在这里显式地分配。
        */
        QString str_user_comment = SubIFDvalue.toObject().value("UserComment").toString();
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_USER_COMMENT, sizeof(ASCII_COMMENT) + strlen(str_user_comment.toLatin1().data()) - 1);

        /* 为注释标签编写特殊的标头 */
        memcpy(entry->data, ASCII_COMMENT, sizeof(ASCII_COMMENT) - 1);

        /* 写入实际的注释文本，不带末尾的 NULL 字符 */
        memcpy(entry->data + 8, str_user_comment.toLatin1().data(), strlen(str_user_comment.toLatin1().data()));

        /* 创建一个 EXIF_TAG_SUBJECT_AREA 标签 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_SUBJECT_AREA, 4 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 4;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ExifImageWidth").toVariant().toInt() / 2);
        exif_set_short(entry->data + 2, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ExifImageHeight").toVariant().toInt() / 2);
        exif_set_short(entry->data + 4, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ExifImageWidth").toVariant().toInt());
        exif_set_short(entry->data + 6, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ExifImageHeight").toVariant().toInt());

        /* 拍摄时间 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL, 20 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 20;
        if (SubIFDvalue.toObject().value("DateTimeOriginal") == "") {
            memcpy(entry->data, pDataTime, sizeof(pDataTime));
        }
        else {
            str = SubIFDvalue.toObject().value("DateTimeOriginal").toString();
            memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));
        }

        /* 数字化时间 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_DIGITIZED, 20 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 20;
        if (SubIFDvalue.toObject().value("Date Time Digitized") == "") {
            memcpy(entry->data, pDataTime, sizeof(pDataTime));
        }
        else {
            str = SubIFDvalue.toObject().value("Date Time Digitized").toString();
            memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));
        }

        /* 曝光时间 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_EXPOSURE_TIME, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = 1;
        fir.denominator = SubIFDvalue.toObject().value("ExposureTime").toVariant().toInt();
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 光圈 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_FNUMBER, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("FNumber").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 曝光程序 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_EXPOSURE_PROGRAM, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ExposureProgram").toVariant().toInt());

        /* 感光度 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_ISO_SPEED_RATINGS, 2 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 2;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ISOSpeedRatings").toVariant().toInt());

        /* 快门速度 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_SHUTTER_SPEED_VALUE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = 1;
        fir.denominator = SubIFDvalue.toObject().value("Shutter Speed Value").toVariant().toInt();
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 光圈 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_APERTURE_VALUE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("Aperture Value").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 亮度 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_BRIGHTNESS_VALUE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("BrightnessValue").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 曝光补偿 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_EXPOSURE_BIAS_VALUE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("ExposureBiasValue").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 最大光圈 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_MAX_APERTURE_VALUE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("MaxApertureValue").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 主体距离 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_SUBJECT_DISTANCE, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("SubjectDistance").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 测光模式 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_METERING_MODE, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("MeteringMode").toVariant().toInt());

        /* 光源 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_LIGHT_SOURCE, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("LightSource").toVariant().toInt());

        /* 闪光灯 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_FLASH, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("Flash").toVariant().toInt());

        /* 镜头焦距 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_FOCAL_LENGTH, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("FocalLength").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 色彩空间 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_COLOR_SPACE, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("ColorSpace").toVariant().toInt());

        /* 焦平面水平分辨率 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_FOCAL_PLANE_X_RESOLUTION, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("FocalPlaneXResolution").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 焦平面垂直分辨率 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("FocalPlaneYResolution").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 焦平面分辨率单位 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("FocalPlaneResolutionUnit").toVariant().toInt());

        /* 曝光指数 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_EXPOSURE_INDEX, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = SubIFDvalue.toObject().value("ExposureIndex").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 传感器类型 */
        entry = create_tag(exif, EXIF_IFD_EXIF, EXIF_TAG_SENSING_METHOD, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, SubIFDvalue.toObject().value("SensingMethod").toVariant().toInt());
    }
    /*********************************************** IFD0 **************************************************************/
    if (!IFD0value.toObject().isEmpty()) {
        /* 制造商 */
        str = IFD0value.toObject().value("Make").toString();
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_MAKE, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 型号 */
        str = IFD0value.toObject().value("Model").toString();
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_MODEL, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 软件 */
        str = IFD0value.toObject().value("Software").toString();
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_SOFTWARE, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 版权信息 */
        str = IFD0value.toObject().value("Copyright").toString();
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_COPYRIGHT, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 方向 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_ORIENTATION, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD0value.toObject().value("Orientation").toVariant().toInt());

        /* 水平打印分辨率 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_X_RESOLUTION, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = IFD0value.toObject().value("XResolution").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 垂直打印分辨率 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_Y_RESOLUTION, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = IFD0value.toObject().value("YResolution").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 分辨率单位 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_RESOLUTION_UNIT, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD0value.toObject().value("ResolutionUnit").toVariant().toInt());

        /* 修改时间 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_DATE_TIME, 20 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 20;
        if (IFD0value.toObject().value("DateTime") == "") {
            memcpy(entry->data, pDataTime, sizeof(pDataTime));
        }
        else {
            str = IFD0value.toObject().value("DateTime").toString();
            memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));
        }

        /* 图像白点 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_WHITE_POINT, 2 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 2;
        fir.numerator = IFD0value.toObject().value("WhitePoint_fir").toVariant().toInt();
        fir.denominator = 1;
        mid.numerator = IFD0value.toObject().value("WhitePoint_mid").toVariant().toInt();
        mid.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);
        exif_set_rational(entry->data + 8, FILE_BYTE_ORDER_INTEL, mid);

        /* YCbCr转换常量 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_YCBCR_COEFFICIENTS, 3 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 3;
        fir.numerator = IFD0value.toObject().value("YCbCrCoefficients_fir").toVariant().toInt();
        fir.denominator = 1;
        mid.numerator = IFD0value.toObject().value("YCbCrCoefficients_mid").toVariant().toInt();
        mid.denominator = 1;
        las.numerator = IFD0value.toObject().value("YCbCrCoefficients_las").toVariant().toInt();
        las.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);
        exif_set_rational(entry->data + 8, FILE_BYTE_ORDER_INTEL, mid);
        exif_set_rational(entry->data + 16, FILE_BYTE_ORDER_INTEL, las);

        /* 色度采样点 */
        entry = create_tag(exif, EXIF_IFD_0, EXIF_TAG_YCBCR_POSITIONING, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD0value.toObject().value("YCbCrPositioning").toVariant().toInt());
    }
    /*********************************************** IOP **************************************************************/
    if (!IOPvalue.toObject().isEmpty()) {
        /* 可互通标准 */
        str = IOPvalue.toObject().value("InteroperabilityIndex").toString();
        entry = create_tag(exif, EXIF_IFD_INTEROPERABILITY, EXIF_TAG_INTEROPERABILITY_INDEX, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 相关图像文件格式 */
        str = IOPvalue.toObject().value("RelatedImageFileFormat").toString();
        entry = create_tag(exif, EXIF_IFD_INTEROPERABILITY, EXIF_TAG_RELATED_IMAGE_FILE_FORMAT, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 相关图像宽度 */
        entry = create_tag(exif, EXIF_IFD_INTEROPERABILITY, EXIF_TAG_RELATED_IMAGE_WIDTH, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IOPvalue.toObject().value("RelatedImageWidth").toVariant().toInt());

        /* 相关图像高度 */
        entry = create_tag(exif, EXIF_IFD_INTEROPERABILITY, EXIF_TAG_RELATED_IMAGE_LENGTH, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IOPvalue.toObject().value("RelatedImageLength").toVariant().toInt());
    } 
    /*********************************************** IFD1 **************************************************************/
    if (!IFD1value.toObject().isEmpty()) {
        /* 缩略图宽度 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_IMAGE_WIDTH, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("ImageWidth").toVariant().toInt());

        /* 缩略图高度 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_IMAGE_LENGTH, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("ImageLength").toVariant().toInt());

        /* 每像素比特位数目 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_BITS_PER_SAMPLE, 3 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 3;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("BitsPerSample_1").toVariant().toInt());
        exif_set_short(entry->data + 3, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("BitsPerSample_2").toVariant().toInt());
        exif_set_short(entry->data + 6, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("BitsPerSample_3").toVariant().toInt());

        /* 压缩方案 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_COMPRESSION, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("Compression").toVariant().toInt());

        /* 图像组件色彩空间 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_PHOTOMETRIC_INTERPRETATION, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("PhotometricInterpretation").toVariant().toInt());

        /* 制造商 */
        str = IFD1value.toObject().value("Make").toString();
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_MAKE, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 型号 */
        str = IFD1value.toObject().value("Model").toString();
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_MODEL, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 软件 */
        str = IFD1value.toObject().value("Software").toString();
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_SOFTWARE, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 作者 */
        str = IFD1value.toObject().value("Artist").toString();
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_ARTIST, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 版权信息 */
        str = IFD1value.toObject().value("Copyright").toString();
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_COPYRIGHT, strlen(str.toLatin1().data()) * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = strlen(str.toLatin1().data());
        memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));

        /* 方向 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_ORIENTATION, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("Orientation").toVariant().toInt());

        /* 水平分辨率 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_X_RESOLUTION, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = IFD1value.toObject().value("XResolution").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 垂直分辨率 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_Y_RESOLUTION, 1 * exif_format_get_size(EXIF_FORMAT_RATIONAL));
        entry->format = EXIF_FORMAT_RATIONAL;
        entry->components = 1;
        fir.numerator = IFD1value.toObject().value("YResolution").toVariant().toInt();
        fir.denominator = 1;
        exif_set_rational(entry->data, FILE_BYTE_ORDER_INTEL, fir);

        /* 分辨率单位 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_RESOLUTION_UNIT, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("ResolutionUnit").toVariant().toInt());

        /* 修改时间 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_DATE_TIME, 20 * exif_format_get_size(EXIF_FORMAT_ASCII));
        entry->format = EXIF_FORMAT_ASCII;
        entry->components = 20;
        if (IFD1value.toObject().value("DateTime") == "") {
            memcpy(entry->data, pDataTime, sizeof(pDataTime));
        }
        else {
            str = IFD1value.toObject().value("DateTime").toString();
            memcpy(entry->data, str.toLatin1().data(), strlen(str.toLatin1().data()));
        }

        /* YCbCr定位方法 */
        entry = create_tag(exif, EXIF_IFD_1, EXIF_TAG_YCBCR_POSITIONING, 1 * exif_format_get_size(EXIF_FORMAT_SHORT));
        entry->format = EXIF_FORMAT_SHORT;
        entry->components = 1;
        exif_set_short(entry->data, FILE_BYTE_ORDER_INTEL, IFD1value.toObject().value("YCbCrPositioning").toVariant().toInt());
    }
    
    /* 保存 EXIF 数据 */
    exif_data_save_data(exif, &exif_data, &exif_data_len);
    assert(exif_data != NULL);

    pFile = fopen(pFilePath, "wb+");
    if (!pFile) {
        nRet = -1;
        goto ERR_EXIT;
    }

    /* 写入 EXIF 头数据 */
    if (fwrite(exif_header, exif_header_len, 1, pFile) != 1) {
        nRet = -1;
        goto ERR_EXIT;
    }
    /* 按大端顺序写入 EXIF 块长度 */
    if (fputc((exif_data_len + 2) >> 8, pFile) < 0) {
        nRet = -1;
        goto ERR_EXIT;
    }
    if (fputc((exif_data_len + 2) & 0xff, pFile) < 0) {
        nRet = -1;
        goto ERR_EXIT;
    }
    /* 写入 EXIF 数据块 */
    if (fwrite(exif_data, exif_data_len, 1, pFile) != 1) {
        nRet = -1;
        goto ERR_EXIT;
    }
    /* 写入 JPEG 图像数据，跳过非 EXIF 头 */
    if (fwrite((pFrame + 2), (nFrameLen - 2), 1, pFile) != 1) {
        nRet = -1;
        goto ERR_EXIT;
    }

ERR_EXIT:
    if (pFile) {
        fclose(pFile);
    }
    //free(exif_data);
    exif_data_unref(exif);

    return nRet;
}


//生成json文件
void generate::generateButton()
{
    QStandardItem* aItem;
    int i, j;
    QJsonObject jfifObject;//构建Json对象 - JFIF
    QJsonObject ifd0Object;//构建Json对象 - IFD0
    QJsonObject ifd1Object;//构建Json对象 - IFD1
    QJsonObject subifdObject;//构建Json对象 - SubIFD
    QJsonObject iopObject;//构建Json对象 - IOP
    QJsonObject gpsObject;//构建Json对象 - GPS
    QJsonArray widthArray;//构建Json数组 - width
    QJsonArray heightArray;//构建Json数组 - height


    //遍历tableview
    for (i = 0; i < model->rowCount(); i++) {      
        for (j = 0; j < model->columnCount(); j++) {
            aItem = model->item(i, j);             //对应位置item  
            if (aItem->text() == "JFIF") {
                if (model->item(i, j + 1)->text() == "Width") {
                    QString data = model->item(i, j + 3)->text();
                    QStringList list = data.split(",");
                    widthArray = QJsonArray::fromStringList(list);
                    jfifObject.insert("Width", QJsonValue(widthArray));
                } 
                else if (model->item(i, j + 1)->text() == "Height") {
                    QString data = model->item(i, j + 3)->text();
                    QStringList list = data.split(",");
                    heightArray = QJsonArray::fromStringList(list);
                    jfifObject.insert("Height", QJsonValue(heightArray));
                }
                else {
                    jfifObject.insert(model->item(i, j + 1)->text(), model->item(i, j + 3)->text());
                }
                //jfifObject.insert(model->item(i, j + 1)->text(), model->item(i, j + 3)->text());
            }
            if (aItem->text() == "IFD0") {
                ifd0Object.insert(model->item(i, j + 1)->text(), model->item(i, j + 3)->text());
            }
            if (aItem->text() == "IFD1") {
                ifd1Object.insert(model->item(i, j + 1)->text(), model->item(i, j + 3)->text());
            }
            if (aItem->text() == "SubIFD") {
                subifdObject.insert(model->item(i, j + 1)->text(), model->item(i, j + 3)->text());
            }
            if (aItem->text() == "IOP") {
                iopObject.insert(model->item(i, j + 1)->text(), model->item(i, j + 3)->text());
            }
            if (aItem->text() == "GPSIFD") {
                gpsObject.insert(model->item(i, j + 1)->text(), model->item(i, j + 3)->text());
            }
        }
    }

    if (!model->item(0, 0)) {
        QMessageBox::warning(NULL,tr("Warning"),tr("Please add information in tableview!"), QMessageBox::Yes, QMessageBox::Yes);
    }
    else{
        //构建Json对象
        QJsonObject Json;
        Json.insert("JFIF", QJsonValue(jfifObject));

        //划分json
        QJsonValue JFIFvalue = Json.value("JFIF");
        QJsonObject obj = JFIFvalue.toObject();
        if (obj.contains("Width") && obj.contains("Height")) {
            QJsonArray widtharray = obj.value("Width").toArray();
            QJsonArray heightarray = obj.value("Height").toArray();

            int wSize = widtharray.size();
            int hSize = heightarray.size();

            for (int i = 0; i < wSize; ++i) {
                for (int j = 0; j < hSize; ++j) {
                    Sleep(10);
                    //复制新的一个Json文件
                    QJsonObject newJson = Json;
                    QJsonObject newobj = newJson.value("JFIF").toObject();

                    //更改对应值
                    newobj["Width"] = widtharray.at(i).toString();
                    newobj["Height"] = heightarray.at(j).toString();
                    //更新Json文件
                    newJson["JFIF"] = newobj;

                    newJson.insert("IFD0", QJsonValue(ifd0Object));
                    newJson.insert("IFD1", QJsonValue(ifd1Object));
                    newJson.insert("SubIFD", QJsonValue(subifdObject));
                    newJson.insert("GPSIFD", QJsonValue(gpsObject));
                    newJson.insert("IOP", QJsonValue(iopObject));

                    // 构建 Json 文档
                    QJsonDocument document;
                    document.setObject(newJson);
                    //保存为json文件
                    QFileInfo fileInfo(jpeggui::filename);
                    QString absFilePath = fileInfo.absolutePath() + "/" + QString::number(i + 1) + ".json";
                    QFile file(absFilePath);

                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                    {
                        QMessageBox::warning(NULL, tr("Warning"), tr("File error!"), QMessageBox::Yes, QMessageBox::Yes);
                    }
                    file.write(document.toJson());
                    file.close();
                }
            }
        }
        else {
            Json.insert("IFD0", QJsonValue(ifd0Object));
            Json.insert("IFD1", QJsonValue(ifd1Object));
            Json.insert("SubIFD", QJsonValue(subifdObject));
            Json.insert("GPSIFD", QJsonValue(gpsObject));
            Json.insert("IOP", QJsonValue(iopObject));

            // 构建 Json 文档
            QJsonDocument document;
            document.setObject(Json);
            //保存为json文件
            QFile file(jpeggui::filename + ".json");
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(NULL, "warning", "File error!", QMessageBox::Yes, QMessageBox::Yes);
            }
            file.write(document.toJson());
            file.close();
        }
        QMessageBox::about(NULL, tr("successful"), tr("Json was written successfully!"));
    }
}

//导入json文件生成图像
void generate::openButton()
{
    //获取json文件所在文件夹
    QString JsonDir = QFileDialog::getExistingDirectory(this, tr("choose Directory"),"./");
    if (JsonDir.isEmpty()) {
        return;
    }
    QDir dir(JsonDir.toStdString().c_str());
    QStringList nameFilters;
    nameFilters << "*.json";
    //列出文件夹下json文件列表（时间排序）
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Time | QDir::Reversed); 
    //遍历json文件
    for (int i = 0; i < files.size(); ++i) { 
        QString JsonPath = JsonDir + "/" + files.at(i);
        QFile JsonFile(JsonPath);
        if (!JsonFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Unable to load JSON file";
        }
        QByteArray JsonByteArray = JsonFile.readAll();

        //设置json读写错误
        QJsonParseError JsonError;
        QJsonDocument JsonDoc = QJsonDocument::fromJson(JsonByteArray, &JsonError);
        if (!JsonDoc.isNull() && (JsonError.error == QJsonParseError::NoError)) {  //没有报错则继续
            QJsonObject JsonObj = JsonDoc.object();

            QString srcJpegPath = JsonDir + "/" + QString::number(i + 1) + ".jpg";

            rw_jpeg_JFIF(JsonObj, srcJpegPath);

            if (!JsonObj.value("IFD0").toObject().isEmpty() || !JsonObj.value("IFD1").toObject().isEmpty() || !JsonObj.value("SubIFD").toObject().isEmpty() || !JsonObj.value("GPSIFD").toObject().isEmpty() || !JsonObj.value("IOP").toObject().isEmpty()) {
                FILE* f = fopen(srcJpegPath.toStdString().c_str(), "rb");
                if (f == NULL) {
                    qDebug() << "Error reading file!\n";
                }

                fseek(f, 0, SEEK_END);
                unsigned int Filesize = ftell(f);
                fseek(f, 0, SEEK_SET);

                unsigned char* buf = (unsigned char*)calloc(Filesize, sizeof(char));
                if (buf == NULL) {
                    qDebug() << "Calloc fails!\n";
                    fclose(f);
                }
                unsigned int a = 0;
                unsigned int pos = 0;
                unsigned char temp = 0;
                for (a = 0; a < Filesize - 1; a++) {
                    temp = fgetc(f);
                    if (EOF == temp)
                        break;
                    buf[pos++] = temp;
                }
                buf[pos] = 0;
                fclose(f);

                add_jpeg_Exif(JsonObj, buf, Filesize, srcJpegPath.toStdString().c_str());

                free(buf);
                buf = NULL;
            }
        }
    }
    QMessageBox::about(NULL, tr("successful"), tr("JPEG information was written successfully!"));
}


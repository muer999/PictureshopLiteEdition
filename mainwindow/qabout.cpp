#include "qabout.h"

#include <QFrame>
#include <QFont>
#include <QPalette>
#include <QDebug>

#define SAFE_FREE(p) {if(p != NULL) {delete p; p = NULL;}}

QAbout::QAbout(QWidget *parent) : QWidget(parent)
{
    /* init ui */
    initUiComponent();
}

QAbout::~QAbout()
{
    SAFE_FREE(titleLabel);
    SAFE_FREE(authorLabel);
    SAFE_FREE(infoLabel);
    SAFE_FREE(infoTextEdit);
    SAFE_FREE(exitBtn);
}

void QAbout::exitBtnClicked(void)
{
    this->close();
}

void QAbout::initUiComponent(void)
{
    int label_w = 720, label_h = 20;
    int text_w = 720, text_h = 350;
    int btn_w = 80, btn_h = 30;
    int btn_x = QABOUT_WIDGET_WIDTH - btn_w;
    int btn_y = QABOUT_WIDGET_HEIGHT - btn_h;

    titleLabel = new QLabel(this);
    titleLabel->setText(tr("Pictureshop Lite Edition for Windows(Version v1.4 2020.3.3)"));
    titleLabel->setGeometry(85, 10, label_w, label_h);

    QFont titleFont("Microsoft YaHei", 10, QFont::Bold);
    titleLabel->setFont(titleFont);

    authorLabel = new QLabel(this);
    authorLabel->setText(tr("Author: 陈小飞 曹文辉 邓斯琪 胡海勤 俞玮"));
    authorLabel->setGeometry(195, 35, label_w, label_h);

    infoLabel = new QLabel(this);
    infoLabel->setOpenExternalLinks(true);
    infoLabel->setText(tr("此程序是数字图像处理B课程的项目，是在Qt官方的QImageViewer程序的基础上做了大量的修改而成的。\n"
                          "QImageViewer项目的下载地址是[https://github.com/gitorup/QImageViewer]"));
    infoLabel->setWordWrap(true);
    infoLabel->setGeometry(25, 60, label_w, label_h*3);

    QString info;
    info.append("《数字图象处理B》课程项目实施方案\n"
                "上海大学 通信与信息工程学院 数字图像处理B课程组\n"
                "2019年11月\n"
                "1、诚信声明\n"
                "学生应团队合作、自主完成课程项目，保证实验数据真实可靠，保证不抄袭、不违规。\n"
                "2、项目组织\n"
                "1)课程项目以团队方式进行，5名学生为一组，自主确定组长一名，明确各个同学的分工与项目之间协同方式；\n"
                "2)课程项目题目由电子电工实验中心实验教学管理系统获取，该中心网址为：（http://202.121.199.224/teacher/），团队组织等相关信息在网上完成；\n"
                "3)本学期课程项目，由任课教师从附件二的5个项目中选一个；\n"
                "4)课外实验需数码相机（可用拍照手机代替）和电脑，由实验小组自备，本实验不涉及其他器材元件及设备。\n"
                "3、成绩比例\n"
                "1、课程项目占《数字图象处理》课程总成绩的20%；\n"
                "2、课程项目成绩由项目实施报告和项目成果二部分组成，项目报告占50%，项目成果占50%；\n"
                "3、项目实施报告包含三部分内容：项目的实施方案、项目的实施过程、项目的结果分析，三个部分各占33%；\n"
                "4、项目成果成绩由验收组验收考核进行评定，评定主要考虑四个方面内容：功能实现、性能指标（分割准确性、运行速度、人工干预度等）、工程规范（规范性、可靠性、操作便利性等）、理论水平；\n"
                "5、课程项目的团队成绩分配，强调团队合作精神，兼顾个人贡献。\n"
                "4、时间安排\n"
                "1、第一周在网上完成组队、选题并确认；\n"
                "2、第二周至第八周进行项目的实施；\n"
                "3、第五周提交完整的项目实施方案；\n"
                "4、第九周项目验收考核。\n\n\n");
    info.append("《数字图像处理》课程项目题目E\n"
                "简易图像处理软件系统的设计与开发\n"
                "一、概述\n"
                "图像处理软件是一类对图像进行各类编辑处理的软件，基本功能包括图像编辑、图像合成、校色调色及功能色效制作等。其中最著名的软件就是Adobe Photoshop，可广泛用于平面设计、广告摄影、影像创意、视觉创意等领域。本项目要求参考Photoshop的功能和界面，设计和开发一个简易的图像处理软件系统，实现基本的图像处理功能。\n"
                "二、 项目要求\n"
                "1、功能要求\n"
                "（1）图像基本操作：不同格式（大于3种）图像的读入与存盘、文字叠加、不同彩色空间的转换、图像的DCT及FFT变换等；\n"
                "（2）图像增强：包括直方图拉升（线性和非线性）、直方图均衡、平滑与锐化（采用不同的滤镜），美颜（加分项）；\n"
                "（3）图像恢复：几何操作（如旋转、缩放、投影校正等）、模糊恢复（如运动模糊消除等，加分项）；\n"
                "（4）图像合成（加分项）：实现换头、换背景、图像拼接等功能。\n"
                "2、实施要求\n"
                "（1）项目在自己的电脑上用软件完成。\n"
                "（2）整个项目的程序可以用Matlab编程实现各种功能，也可以采用C++、Python等其他语言，但要能在最后验收时实际运行演示，并提供不少于5幅的测试图像。\n"
                "（3）项目设计报告要将原理、结果（包括各步骤的图像）、分析等给出，并附上源代码。\n"
                "3、具体实施步骤\n"
                "（1）第一阶段\n"
                "方案设计：在广泛的资料查询的基础上，设计系统总体架构，包括采用的开发软件、开发系统、功能设计、界面设计等；\n"
                "（2）第二阶段\n"
                "实现图像基本操作、图像增强功能。\n"
                "（3）第三阶段\n"
                "实现图像恢复、图像合成功能。\n"
                "（4）第五阶段\n"
                "完成所有的设计与报告撰写，在期末进行演示、汇报和答辩。 \n"
                "三、评分依据\n"
                "1、功能实现：实现功能完整，符合要求；\n"
                "2、性能指标：各类图像处理的效果、运行速度等；\n"
                "3、工程规范：程序的规范性、可靠性、操作便利性等；\n"
                "4、理论知识：项目算法理论正确，分析过程清晰。\n"
                "四、验收思考题\n"
                "1、为了增强拍摄照片的效果，手机中采用了那些图像处理的手段？ \n"
                "2、Photoshop中图层、通道、滤镜的作用及背后的原理？\n"
                "参考文献：\n"
                "[1]肖彬. 计算机图像处理基础教程:Photoshop CS6. 中国电力出版社，2017.\n\n\n");
    info.append("                                     Statement\n");
    info.append("QImageViewer is a free and open source project. ");
    info.append("You can according to the published by the free software "
                "foundation GNU free distribution and/or modify it under the GPL; "
                "Using version 3 of the agreement, or any higher versions (according to your choice). "
                "Published the purpose is to hope it can useful to you, but we don't have any guarantee; "
                "Use it for any purpose of any direct or indirect consequences does not undertake any responsibility. "
                "Please see details of the GNU GPL.\n"
                "You should be at the same time of receiving the program received a copy of the GNU GPL. "
                "If not, see (http://www.gnu.org/licenses).");


    infoTextEdit = new QTextEdit(this);
    infoTextEdit->setText(info);
    infoTextEdit->setReadOnly(1);
    infoTextEdit->setGeometry(25, 130, text_w, text_h);

    QPalette palette;
    palette.setColor(QPalette::Text, Qt::red);
    infoTextEdit->setPalette(palette);
    QFont infoFont("Microsoft YaHei", 8, QFont::Cursive);
    infoTextEdit->setFont(infoFont);

    exitBtn = new QPushButton(this);
    exitBtn->setText(tr("OK"));
    exitBtn->setGeometry(btn_x - 10, btn_y - 5, btn_w, btn_h);
    connect(exitBtn, SIGNAL(clicked(bool)), this, SLOT(exitBtnClicked()));
}

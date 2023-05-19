#include "mainwindow.h"

#include <QWidget>
#include <QImage>
#include <QFileDialog>
#include <QPixmap>
#include <QAction>
#include <QMessageBox>
#include <QDebug>
#include <QScrollArea>
#include <QGridLayout>
#include <QErrorMessage>
#include <QApplication>

#include "qabout.h"
#include "qimageviewer.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    /* create imageViewer */
    imageViewer = new QImageViewer();

    /* init main window */
    initMainWindow();

    /* init ui */
    initUiComponent();

    /* init resource */
    initImageResource();

    connect(imageViewer,SIGNAL(imageChanged()),this,SLOT(imageHasBeenChanged()));
}

void MainWindow::initImageResource(void)
{
    imageLabel->clear();
    imageLabel->resize(QSize(200, 100));
    setWindowTitle(tr("Pictureshop Lite Edition v1.4 - Designed by CWH CXF DSQ HHQ YW 2020.3"));
}

void MainWindow::loadImageResource(void)
{
    imageLabel->setPixmap(imageViewer->pixmap);
    imageLabel->resize(imageViewer->size);
    setWindowTitle(QFileInfo(imageViewer->filename).fileName() + tr(" - Pictureshop Lite Edition v1.4 - Designed by CWH CXF DSQ HHQ YW 2020.3"));
}

void MainWindow::openActionTriggered(void)
{
    QString username = QDir::home().dirName();
    int ret = imageViewer->openImageFile(tr("Select image:"),
                                         "C://Users//"+username+"//Pictures",
                                         tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.tif)"));
    if (ret) {
        QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::closeActionTriggered(void)
{
    initImageResource();
    imageViewer->closeImageFile();
}
void MainWindow::saveActionTriggered(void)
{
    QMessageBox message(QMessageBox::Warning,
                        tr("Warning"),
                        tr("Are you sure want to cover the original image?"),
                        QMessageBox::Yes|QMessageBox::No,
                        nullptr);
    if (message.exec() == QMessageBox::No) {
        return ;
    }
    QString username = QDir::home().dirName();
    int ret = imageViewer->saveImageFile(tr("Select image:"),
                                         "C://Users//"+username+"//Pictures",
                                         tr("Images (*.jpg *.jpeg *.png *.bmp *.tif)"));
    if (ret) {
        QMessageBox::information(this, tr("Error"), tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}
void MainWindow::saveasActionTriggered(void)
{
    QString username = QDir::home().dirName();
    int ret = imageViewer->saveImageFileas(tr("Select image:"),
                                         "C://Users//"+username+"//Pictures",
                                         tr("Images (*.jpg *.jpeg *.png *.bmp *.tif)"));
    if (ret==-2)
    {
        QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
        return ;
    }
    else if (ret==-1)
    {
        QMessageBox::information(this,tr("Error"),tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}
void MainWindow::lastActionTriggered(void)
{
    int ret = imageViewer->last();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::nextActionTriggered(void)
{
    int ret = imageViewer->next();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::reloadActionTriggered(void)
{
    int ret = imageViewer->reloadImageResource();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    loadImageResource();
}

void MainWindow::toLeftActionTriggered(void)
{
    int ret = imageViewer->spinToLeft();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }
}

void MainWindow::toRightActionTriggered(void)
{
    int ret = imageViewer->spinToRight();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }
}

void MainWindow::toEnlargeActionTriggered(void)
{
    int ret = imageViewer->zoomIn();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }
}

void MainWindow::toLessenActionTriggered(void)
{
    int ret = imageViewer->zoomOut();
    if (ret) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }
}

void MainWindow::deleteActionTriggered(void)
{
    if (!QFile(imageViewer->filename).exists()) {
        QMessageBox::information(this,
                                 tr("Error"),
                                 tr("Open a image, please!"));
        return ;
    }

    QMessageBox message(QMessageBox::Warning,
                        tr("Warning"),
                        tr("Are you sure want to delete this image?"),
                        QMessageBox::Yes|QMessageBox::No,
                        nullptr);
    if (message.exec() == QMessageBox::No) {
        return ;
    }

    int ret = imageViewer->delImageFile();
    if (ret) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Delete a image failed!"));
        return ;
    }

    initImageResource();
}
void MainWindow::imageHasBeenChanged(void)
{
    loadImageResource();
}

void MainWindow::addTextActionTriggered(void)
{
    hideDockComponent();
    addTextDockWindow->show();
}

void MainWindow::convertColorSpaceActionTriggered(void)
{
    hideDockComponent();
    convertColorSpaceDockWindow->show();
}

void MainWindow::showDCTActionTriggered(void)
{
    hideDockComponent();
    showDCTDockWindow->show();
    imageViewer->showDCTWidget->caculateDCTandShow();
}

void MainWindow::showFFTActionTriggered(void)
{
    hideDockComponent();
    showFFTDockWindow->show();
    imageViewer->showFFTWidget->caculateFFTandShow();
}


void MainWindow::histPullActionTriggered(void)
{
    hideDockComponent();
    histPullDockWindow->show();
    HistogramDockWindow->show();
}

void MainWindow::histEqualizationActionTriggered(void)
{
    hideDockComponent();
    histEqualizationDockWindow->show();
    HistogramDockWindow->show();
}

void MainWindow::smoothingActionTriggered(void)
{
    hideDockComponent();
    smoothingDockWindow->show();
}

void MainWindow::sharpenActionTriggered(void)
{
    hideDockComponent();
    sharpenDockWindow->show();
}

void MainWindow::beautyActionTriggered(void)
{
    hideDockComponent();
    beautyDockWindow->show();
}


void MainWindow::rotateActionTriggered(void)
{
    hideDockComponent();
    rotateDockWindow->show();
}

void MainWindow::resizeActionTriggered(void)
{
    hideDockComponent();
    resizeDockWindow->show();
}

void MainWindow::geometricCalibrationActionTriggered(void)
{
    hideDockComponent();
    geometricCalibrationDockWindow->show();
}

void MainWindow::fuzzyRecoveryActionTriggered(void)
{
    hideDockComponent();
    fuzzyRecoveryDockWindow->show();
}


void MainWindow::changeHeadActionTriggered(void)
{
    hideDockComponent();
    changeHeadDockWindow->show();
}

void MainWindow::changeBackgroundActionTriggered(void)
{
    hideDockComponent();
    changeBackgroundDockWindow->show();
}

void MainWindow::stitchingActionTriggered(void)
{
    hideDockComponent();
    stitchingDockWindow->show();
}

void MainWindow::setQImageViewerWidget(void)
{
    /* label show image */
    imageLabel = new ImageShow_QLabel();

    imageScrollArea = new QScrollArea();
    imageScrollArea->setAlignment(Qt::AlignCenter);
    imageScrollArea->setFrameShape(QFrame::NoFrame);
    imageScrollArea->setWidget(imageLabel);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(imageScrollArea, 0, 0);
    centralWidget->setLayout(mainLayout);

    connect(imageLabel,SIGNAL(ImageShowLabel_mousePressed(QPoint)),this,SLOT(imageLabelMousePressed(QPoint)));
    connect(imageLabel,SIGNAL(ImageShowLabel_mouseMoved(QPoint)),this,SLOT(imageLabelMouseMoved(QPoint)));
    connect(imageLabel,SIGNAL(ImageShowLabel_toEnlarge()),this,SLOT(imageLabeltoEnlarge()));
    connect(imageLabel,SIGNAL(ImageShowLabel_toLessen()),this,SLOT(imageLabeltoLessen()));
}

void MainWindow::setWindowComponet(void)
{
    openAction = new QAction(tr("Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an image."));
    openAction->setIcon(QIcon(":/images/open2.png"));

    closeAction = new QAction(tr("Close"), this);
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close an image."));
    closeAction->setIcon(QIcon(":/images/close2.png"));

    saveAction = new QAction(tr("Save"), this);
    saveAction->setStatusTip(tr("To save an image."));
    saveAction->setIcon(QIcon(":/images/save4.png"));

    saveasAction = new QAction(tr("Save As"), this);
    saveasAction->setStatusTip(tr("To save an image."));
    saveasAction->setIcon(QIcon(":/images/saveas4.png"));

    lastAction = new QAction(tr("Last"), this);
    lastAction->setStatusTip(tr("Last image."));
    lastAction->setIcon(QIcon(":/images/left2.png"));

    reloadAction = new QAction(tr("Reload"), this);
    reloadAction->setStatusTip(tr("Reload image."));
    reloadAction->setIcon(QIcon(":/images/reloadAction.png"));

    nextAction = new QAction(tr("Next"), this);
    nextAction->setStatusTip(tr("Next image"));
    nextAction->setIcon(QIcon(":/images/right2.png"));

    toLeftAction = new QAction(tr("Left Spin"), this);
    toLeftAction->setStatusTip(tr("To Left."));
    toLeftAction->setIcon(QIcon(":/images/leftspin.png"));

    toRightAction = new QAction(tr("Right Spin"), this);
    toRightAction->setStatusTip(tr("To Right."));
    toRightAction->setIcon(QIcon(":/images/rightspin.png"));

    toEnlargeAction = new QAction(tr("Enlarge"), this);
    toEnlargeAction->setStatusTip(tr("To Enlarge."));
    toEnlargeAction->setIcon(QIcon(":/images/zoomin.png"));

    toLessenAction = new QAction(tr("Lessen"), this);
    toLessenAction->setStatusTip(tr("To Lessen."));
    toLessenAction->setIcon(QIcon(":/images/zoomout.png"));

    deleteAction = new QAction(tr("Delete"), this);
    deleteAction->setStatusTip(tr("Delete a image"));
    deleteAction->setIcon(QIcon(":/images/clear.png"));
    deleteAction->setShortcut(QKeySequence::Delete);

    addTextAction = new QAction(tr("Add Text"), this);
    addTextAction->setStatusTip(tr("Add Text on image."));
    addTextAction->setIcon(QIcon(":/images/addTextAction.png"));

    convertColorSpaceAction = new QAction(tr("Convert Color Space"), this);
    convertColorSpaceAction->setStatusTip(tr("To Convert Color Space."));
    convertColorSpaceAction->setIcon(QIcon(":/images/convertColorSpaceAction.png"));

    showDCTAction = new QAction(tr("Show DCT Result"), this);
    showDCTAction->setStatusTip(tr("To Display the Result of Discrete Cosine Transformation."));
    showDCTAction->setIcon(QIcon(":/images/showDCTAction.png"));

    showFFTAction = new QAction(tr("Show FFT Result"), this);
    showFFTAction->setStatusTip(tr("To Display the Result of Fast Fourier Transformation."));
    showFFTAction->setIcon(QIcon(":/images/showFFTAction.png"));


    histPullAction = new QAction(tr("Histogram Pull"), this);
    histPullAction->setStatusTip(tr("To Perform Linear Transformation."));
    histPullAction->setIcon(QIcon(":/images/histPullAction.png"));

    histEqualizationAction = new QAction(tr("Histogram Equalization"), this);
    histEqualizationAction->setStatusTip(tr("To Make Histgraph Equalization."));
    histEqualizationAction->setIcon(QIcon(":/images/histEqualizationAction.png"));

    smoothingAction = new QAction(tr("Smoothing"), this);
    smoothingAction->setStatusTip(tr("To Blur the Image."));
    smoothingAction->setIcon(QIcon(":/images/smoothingAction.png"));

    sharpenAction = new QAction(tr("Sharpen"), this);
    sharpenAction->setStatusTip(tr("To Sharpen the Image."));
    sharpenAction->setIcon(QIcon(":/images/sharpenAction.png"));

    beautyAction = new QAction(tr("Beauty"), this);
    beautyAction->setStatusTip(tr("To Beauty one's skin."));
    beautyAction->setIcon(QIcon(":/images/beautyAction.png"));

    rotateAction = new QAction(tr("Rotate"), this);
    rotateAction->setStatusTip(tr("To Rotate the Image."));
    rotateAction->setIcon(QIcon(":/images/rotateAction.png"));

    resizeAction = new QAction(tr("Resize"), this);
    resizeAction->setStatusTip(tr("To Resize the Image."));
    resizeAction->setIcon(QIcon(":/images/resizeAction.png"));

    geometricCalibrationAction = new QAction(tr("Geometric Calibration"), this);
    geometricCalibrationAction->setStatusTip(tr("To Perform Geometric Calibration."));
    geometricCalibrationAction->setIcon(QIcon(":/images/geometricCalibrationAction.png"));

    fuzzyRecoveryAction = new QAction(tr("Motion Blur Recovery"), this);
    fuzzyRecoveryAction->setStatusTip(tr("To Perform Motion Blur Recoveration."));
    fuzzyRecoveryAction->setIcon(QIcon(":/images/fuzzyRecoveryAction.png"));

    changeHeadAction = new QAction(tr("Change Head"), this);
    changeHeadAction->setStatusTip(tr("To Change one's head."));
    changeHeadAction->setIcon(QIcon(":/images/changeHeadAction.png"));

    changeBackgroundAction = new QAction(tr("Change Background"), this);
    changeBackgroundAction->setStatusTip(tr("To Change the Background of the image."));
    changeBackgroundAction->setIcon(QIcon(":/images/changeBackgroundAction.png"));

    stitchingAction = new QAction(tr("Stitching"), this);
    stitchingAction->setStatusTip(tr("To stitch two images."));
    stitchingAction->setIcon(QIcon(":/images/stitchingAction2.png"));

    QAction *exitAction = new QAction(tr("Exit"), this);
    exitAction->setStatusTip(tr("Exit"));
    exitAction->setIcon(QIcon(":/images/quit.png"));
    exitAction->setShortcut(QKeySequence::Quit);

    QAction *aboutQt = new QAction(tr("About Qt"), this);
    aboutQt->setStatusTip(tr("About Qt"));
    aboutQt->setIcon(QIcon(":/images/Qt.png"));

    QAction *about = new QAction(tr("About Pictureshop Lite Edition"), this);
    about->setStatusTip(tr("About Pictureshop Lite Edition"));
    about->setIcon(QIcon(":/images/help3.png"));

    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(closeAction);
    fileMenu->addAction(reloadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveasAction);
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu *operationMenu = menuBar->addMenu(tr("View"));
    operationMenu->addAction(lastAction);
    operationMenu->addAction(nextAction);
    operationMenu->addSeparator();
    operationMenu->addAction(toLeftAction);
    operationMenu->addAction(toRightAction);
    operationMenu->addSeparator();
    operationMenu->addAction(toEnlargeAction);
    operationMenu->addAction(toLessenAction);

    QMenu *basicMenu = menuBar->addMenu(tr("Basic"));
    basicMenu->addAction(addTextAction);
    basicMenu->addSeparator();
    basicMenu->addAction(convertColorSpaceAction);
    basicMenu->addSeparator();
    basicMenu->addAction(showDCTAction);
    basicMenu->addAction(showFFTAction);

    QMenu *enhancementMenu = menuBar->addMenu(tr("Enhancement"));
    enhancementMenu->addAction(histPullAction);
    enhancementMenu->addAction(histEqualizationAction);
    enhancementMenu->addSeparator();
    enhancementMenu->addAction(smoothingAction);
    enhancementMenu->addAction(sharpenAction);
    enhancementMenu->addSeparator();
    enhancementMenu->addAction(beautyAction);

    QMenu *recoveryMenu = menuBar->addMenu(tr("Recovery"));
    recoveryMenu->addAction(rotateAction);
    recoveryMenu->addAction(resizeAction);
    recoveryMenu->addSeparator();
    recoveryMenu->addAction(geometricCalibrationAction);
    recoveryMenu->addSeparator();
    recoveryMenu->addAction(fuzzyRecoveryAction);

    QMenu *synthesisMenu = menuBar->addMenu(tr("Synthesis"));
    synthesisMenu->addAction(changeHeadAction);
    synthesisMenu->addSeparator();
    synthesisMenu->addAction(changeBackgroundAction);
    synthesisMenu->addSeparator();
    synthesisMenu->addAction(stitchingAction);


    QMenu *helpMenu = menuBar->addMenu(tr("Help"));
    helpMenu->addAction(aboutQt);
    helpMenu->addAction(about);

    toolBar->addAction(openAction);
    toolBar->addAction(closeAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(saveasAction);
    toolBar->addAction(lastAction);
    toolBar->addAction(nextAction);
    toolBar->addAction(reloadAction);
    toolBar->addAction(toLeftAction);
    toolBar->addAction(toRightAction);
    toolBar->addAction(toEnlargeAction);
    toolBar->addAction(toLessenAction);
    toolBar->addAction(deleteAction);
    toolBar->addAction(about);

    basictoolBar->addAction(addTextAction);
    basictoolBar->addAction(convertColorSpaceAction);
    basictoolBar->addAction(showDCTAction);
    basictoolBar->addAction(showFFTAction);

    enhancementtoolBar->addAction(histPullAction);
    enhancementtoolBar->addAction(histEqualizationAction);
    enhancementtoolBar->addAction(smoothingAction);
    enhancementtoolBar->addAction(sharpenAction);
    enhancementtoolBar->addAction(beautyAction);

    recoverytoolBar->addAction(rotateAction);
    recoverytoolBar->addAction(resizeAction);
    recoverytoolBar->addAction(geometricCalibrationAction);
    recoverytoolBar->addAction(fuzzyRecoveryAction);

    synthesistoolBar->addAction(changeHeadAction);
    synthesistoolBar->addAction(changeBackgroundAction);
    synthesistoolBar->addAction(stitchingAction);

    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openActionTriggered()));
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(closeActionTriggered()));
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(saveActionTriggered()));
    connect(saveasAction, SIGNAL(triggered(bool)), this, SLOT(saveasActionTriggered()));
    connect(lastAction, SIGNAL(triggered(bool)), this, SLOT(lastActionTriggered()));
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextActionTriggered()));
    connect(reloadAction, SIGNAL(triggered(bool)), this, SLOT(reloadActionTriggered()));
    connect(toLeftAction, SIGNAL(triggered(bool)), this, SLOT(toLeftActionTriggered()));
    connect(toRightAction, SIGNAL(triggered(bool)), this, SLOT(toRightActionTriggered()));
    connect(toEnlargeAction, SIGNAL(triggered(bool)), this, SLOT(toEnlargeActionTriggered()));
    connect(toLessenAction, SIGNAL(triggered(bool)), this, SLOT(toLessenActionTriggered()));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteActionTriggered()));

    connect(about, SIGNAL(triggered(bool)), this, SLOT(aboutTriggered()));
    connect(aboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQtTriggered()));
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    connect(addTextAction, SIGNAL(triggered(bool)), this, SLOT(addTextActionTriggered()));
    connect(convertColorSpaceAction, SIGNAL(triggered(bool)), this, SLOT(convertColorSpaceActionTriggered()));
    connect(showDCTAction, SIGNAL(triggered(bool)), this, SLOT(showDCTActionTriggered()));
    connect(showFFTAction, SIGNAL(triggered(bool)), this, SLOT(showFFTActionTriggered()));

    connect(histPullAction, SIGNAL(triggered(bool)), this, SLOT(histPullActionTriggered()));
    connect(histEqualizationAction, SIGNAL(triggered(bool)), this, SLOT(histEqualizationActionTriggered()));
    connect(smoothingAction, SIGNAL(triggered(bool)), this, SLOT(smoothingActionTriggered()));
    connect(sharpenAction, SIGNAL(triggered(bool)), this, SLOT(sharpenActionTriggered()));
    connect(beautyAction, SIGNAL(triggered(bool)), this, SLOT(beautyActionTriggered()));

    connect(rotateAction, SIGNAL(triggered(bool)), this, SLOT(rotateActionTriggered()));
    connect(resizeAction, SIGNAL(triggered(bool)), this, SLOT(resizeActionTriggered()));
    connect(geometricCalibrationAction, SIGNAL(triggered(bool)), this, SLOT(geometricCalibrationActionTriggered()));
    connect(fuzzyRecoveryAction, SIGNAL(triggered(bool)), this, SLOT(fuzzyRecoveryActionTriggered()));

    connect(changeHeadAction, SIGNAL(triggered(bool)), this, SLOT(changeHeadActionTriggered()));
    connect(changeBackgroundAction, SIGNAL(triggered(bool)), this, SLOT(changeBackgroundActionTriggered()));
    connect(stitchingAction, SIGNAL(triggered(bool)), this, SLOT(stitchingActionTriggered()));
}

void MainWindow::hideDockComponent(void)
{
    addTextDockWindow->hide();
    convertColorSpaceDockWindow->hide();
    showDCTDockWindow->hide();
    showFFTDockWindow->hide();

    histPullDockWindow->hide();
    histEqualizationDockWindow->hide();
    smoothingDockWindow->hide();
    sharpenDockWindow->hide();
    beautyDockWindow->hide();

    rotateDockWindow->hide();
    resizeDockWindow->hide();
    geometricCalibrationDockWindow->hide();
    fuzzyRecoveryDockWindow->hide();

    changeHeadDockWindow->hide();
    changeBackgroundDockWindow->hide();
    stitchingDockWindow->hide();
}

void MainWindow::setupDockWindow(QDockWidget*& DockWindowName, QString DockWindowTitle, QWidget* WidgetName)
{
    DockWindowName = new QDockWidget(DockWindowTitle,this);
    DockWindowName->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetClosable);
    DockWindowName->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);
    DockWindowName->setMinimumWidth(400);
    DockWindowName->setMinimumHeight(400);
    DockWindowName->setMaximumHeight(500);
    this->addDockWidget(Qt::RightDockWidgetArea,DockWindowName);
    DockWindowName->setWidget(WidgetName);
    DockWindowName->resize(400,500);
}

void MainWindow::setDockComponent(void)
{
    setupDockWindow(addTextDockWindow, tr("Add Text"), imageViewer->addTextWidget);
    setupDockWindow(convertColorSpaceDockWindow, tr("Convert Color Space"), imageViewer->convertColorSpaceWidget);
    setupDockWindow(showDCTDockWindow, tr("DCT"), imageViewer->showDCTWidget);
    setupDockWindow(showFFTDockWindow, tr("FFT"), imageViewer->showFFTWidget);
    setupDockWindow(histPullDockWindow, tr("Histogram Pull"), imageViewer->histPullWidget);
    setupDockWindow(histEqualizationDockWindow, tr("Histogram Equalization"), imageViewer->histEqualizationWidget);
    setupDockWindow(smoothingDockWindow, tr("Smoothing"), imageViewer->smoothingWidget);
    setupDockWindow(sharpenDockWindow, tr("Sharpen"), imageViewer->sharpenWidget);
    setupDockWindow(beautyDockWindow, tr("Beauty"), imageViewer->beautyWidget);
    setupDockWindow(rotateDockWindow, tr("Rotate"), imageViewer->rotateWidget);
    setupDockWindow(resizeDockWindow, tr("Resize"), imageViewer->resizeWidget);
    setupDockWindow(geometricCalibrationDockWindow, tr("Geometric Calibration"), imageViewer->geometricCalibrationWidget);
    setupDockWindow(fuzzyRecoveryDockWindow, tr("Motion Blur Recovery"), imageViewer->fuzzyRecoveryWidget);
    setupDockWindow(changeHeadDockWindow, tr("Change Head"), imageViewer->changeHeadWidget);
    setupDockWindow(changeBackgroundDockWindow, tr("Change Background"), imageViewer->changeBackgroundWidget);
    setupDockWindow(stitchingDockWindow, tr("Stitching"), imageViewer->stitchingWidget);

    hideDockComponent();

    setupDockWindow(HistogramDockWindow, tr("Histogram"), imageViewer->HistogramWidget);
    HistogramDockWindow->setMinimumHeight(280);
    HistogramDockWindow->show();
}

void MainWindow::aboutQtTriggered(void)
{
    qApp->aboutQt();
}

void MainWindow::aboutTriggered(void)
{
    aboutWidget.setWindowTitle("Help Infomation");
    aboutWidget.setWindowIcon(QIcon(":/images/help2.png"));
    aboutWidget.setFixedSize(QABOUT_WIDGET_WIDTH, QABOUT_WIDGET_HEIGHT);

    aboutWidget.show();
}

void MainWindow::initUiComponent(void)
{
    /* set menu and toolbar */
    setWindowComponet();

    /* set dock components */
    setDockComponent();

    /* image show widget */
    setQImageViewerWidget();
}

void MainWindow::initMainWindow(void)
{
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    toolBar = new QToolBar(this);
    addToolBar(toolBar);

    basictoolBar = new QToolBar(this);
    addToolBar(basictoolBar);

    enhancementtoolBar = new QToolBar(this);
    addToolBar(enhancementtoolBar);

    recoverytoolBar = new QToolBar(this);
    addToolBar(recoverytoolBar);

    synthesistoolBar = new QToolBar(this);
    addToolBar(synthesistoolBar);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    statusLabel = new QLabel(this);
    statusLabel->setMaximumWidth(250);
    statusLabel->setMinimumWidth(100);
    statusBar->addWidget(statusLabel);
}

void MainWindow::imageLabelMousePressed(QPoint mousePosition)
{
    QPoint pixelPosition=imageViewer->caculatePixelPosition(mousePosition);
    imageViewer->MousePressPosition=pixelPosition;
    imageViewer->deliverPixelPosition();
    if(imageViewer->addTextWidget->setMouseGetPoint->isChecked())
    {
        QString text(QString::number(pixelPosition.x()));
        text.append(",");
        text.append(QString::number(pixelPosition.y()));
        imageViewer->addTextWidget->MouseGetPointEdit->setText(text);
    }
    if(imageViewer->geometricCalibrationWidget->LeftTopPointCheckBox->isChecked())
    {
        QString text(QString::number(pixelPosition.x()));
        text.append(",");
        text.append(QString::number(pixelPosition.y()));
        imageViewer->geometricCalibrationWidget->LeftTopPointEdit->setText(text);
    }
    else if(imageViewer->geometricCalibrationWidget->RightTopPointCheckBox->isChecked())
    {
        QString text(QString::number(pixelPosition.x()));
        text.append(",");
        text.append(QString::number(pixelPosition.y()));
        imageViewer->geometricCalibrationWidget->RightTopPointEdit->setText(text);
    }
    else if(imageViewer->geometricCalibrationWidget->LeftBottomPointCheckBox->isChecked())
    {
        QString text(QString::number(pixelPosition.x()));
        text.append(",");
        text.append(QString::number(pixelPosition.y()));
        imageViewer->geometricCalibrationWidget->LeftBottomPointEdit->setText(text);
    }
    else if(imageViewer->geometricCalibrationWidget->RightBottomPointCheckBox->isChecked())
    {
        QString text(QString::number(pixelPosition.x()));
        text.append(",");
        text.append(QString::number(pixelPosition.y()));
        imageViewer->geometricCalibrationWidget->RightBottomPointEdit->setText(text);
    }
}
void MainWindow::imageLabelMouseMoved(QPoint mousePosition)
{
    QPoint pixelPosition=imageViewer->caculatePixelPosition(mousePosition);
    statusLabel->setText(QString("pixel:(")+QString::number(pixelPosition.x())+QString(",")+QString::number(pixelPosition.y())+QString(")"));
}
void MainWindow::imageLabeltoEnlarge(void)
{
    imageScrollArea->setEnabled(false);
    toEnlargeActionTriggered();
    imageScrollArea->setEnabled(true);
}
void MainWindow::imageLabeltoLessen(void)
{
    imageScrollArea->setEnabled(false);
    toLessenActionTriggered();
    imageScrollArea->setEnabled(true);
}

ImageShow_QLabel::ImageShow_QLabel(QWidget *parent):QLabel(parent)
{
    setMouseTracking(true);
}
void ImageShow_QLabel::mousePressEvent(QMouseEvent *ev)
{
    emit ImageShowLabel_mousePressed(QPoint(ev->x(),ev->y()));
}
void ImageShow_QLabel::mouseMoveEvent(QMouseEvent *ev)
{
    emit ImageShowLabel_mouseMoved(QPoint(ev->x(),ev->y()));
}

void ImageShow_QLabel::keyPressEvent(QKeyEvent *)
{

}
void ImageShow_QLabel::keyReleaseEvent(QKeyEvent *)
{

}
void ImageShow_QLabel::wheelEvent(QWheelEvent *ev)
{
    if(ev->modifiers() == Qt::ControlModifier)
    {
        if(ev->delta() > 0)
        {
            emit ImageShowLabel_toEnlarge();
        }
        else
        {
            emit ImageShowLabel_toLessen();
        }
    }
}

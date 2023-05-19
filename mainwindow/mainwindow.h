#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QFileInfoList>
#include <QDockWidget>
#include <QMouseEvent>
#include <QScrollArea>

#include "qabout.h"
#include "qimageviewer.h"

class ImageShow_QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

private:
    QMenuBar *menuBar;
    QToolBar *toolBar;
    QToolBar *basictoolBar;
    QToolBar *enhancementtoolBar;
    QToolBar *recoverytoolBar;
    QToolBar *synthesistoolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;
    QLabel *statusLabel;
    ImageShow_QLabel *imageLabel;
    QScrollArea *imageScrollArea;
    QAbout aboutWidget;

    /* new class */
    QImageViewer *imageViewer;

    void initMainWindow(void);
    void initUiComponent(void);
    void setQImageViewerWidget(void);
    void setWindowComponet(void);
    void setDockComponent(void);
    void setupDockWindow(QDockWidget*& DockWindowName, QString DockWindowTitle, QWidget* WidgetName);
    void hideDockComponent(void);
    void initImageResource(void);
    void loadImageResource(void);


    QAction *openAction;
    QAction *closeAction;
    QAction *saveAction;
    QAction *saveasAction;
    QAction *lastAction;
    QAction *nextAction;
    QAction *reloadAction;
    QAction *toLeftAction;
    QAction *toRightAction;
    QAction *toEnlargeAction;
    QAction *toLessenAction;
    QAction *deleteAction;

    QAction *addTextAction;
    QAction *convertColorSpaceAction;
    QAction *showDCTAction;
    QAction *showFFTAction;

    QAction *histPullAction;
    QAction *histEqualizationAction;
    QAction *smoothingAction;
    QAction *sharpenAction;
    QAction *beautyAction;

    QAction *rotateAction;
    QAction *resizeAction;
    QAction *geometricCalibrationAction;
    QAction *fuzzyRecoveryAction;

    QAction *changeHeadAction;
    QAction *changeBackgroundAction;
    QAction *stitchingAction;


    QDockWidget *addTextDockWindow;
    QDockWidget *convertColorSpaceDockWindow;
    QDockWidget *showDCTDockWindow;
    QDockWidget *showFFTDockWindow;

    QDockWidget *histPullDockWindow;
    QDockWidget *histEqualizationDockWindow;
    QDockWidget *smoothingDockWindow;
    QDockWidget *sharpenDockWindow;
    QDockWidget *beautyDockWindow;

    QDockWidget *rotateDockWindow;
    QDockWidget *resizeDockWindow;
    QDockWidget *geometricCalibrationDockWindow;
    QDockWidget *fuzzyRecoveryDockWindow;

    QDockWidget *changeHeadDockWindow;
    QDockWidget *changeBackgroundDockWindow;
    QDockWidget *stitchingDockWindow;

    QDockWidget *HistogramDockWindow;


public slots:
    void imageHasBeenChanged(void);

    void openActionTriggered(void);
    void closeActionTriggered(void);
    void saveActionTriggered(void);
    void saveasActionTriggered(void);
    void lastActionTriggered(void);
    void nextActionTriggered(void);
    void reloadActionTriggered(void);
    void toLeftActionTriggered(void);
    void toRightActionTriggered(void);
    void toEnlargeActionTriggered(void);
    void toLessenActionTriggered(void);
    void deleteActionTriggered(void);
    void aboutTriggered(void);
    void aboutQtTriggered(void);

    void addTextActionTriggered(void);
    void convertColorSpaceActionTriggered(void);
    void showDCTActionTriggered(void);
    void showFFTActionTriggered(void);

    void histPullActionTriggered(void);
    void histEqualizationActionTriggered(void);
    void smoothingActionTriggered(void);
    void sharpenActionTriggered(void);
    void beautyActionTriggered(void);

    void rotateActionTriggered(void);
    void resizeActionTriggered(void);
    void geometricCalibrationActionTriggered(void);
    void fuzzyRecoveryActionTriggered(void);

    void changeHeadActionTriggered(void);
    void changeBackgroundActionTriggered(void);
    void stitchingActionTriggered(void);

    void imageLabelMousePressed(QPoint);
    void imageLabelMouseMoved(QPoint);
    void imageLabeltoEnlarge(void);
    void imageLabeltoLessen(void);
};

class ImageShow_QLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageShow_QLabel(QWidget *parent = nullptr);
    //bool eventAccept;
private:

protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseMoveEvent(QMouseEvent *ev) override;
    virtual void keyPressEvent(QKeyEvent *ev) override;
    virtual void keyReleaseEvent(QKeyEvent *ev) override;
    virtual void wheelEvent(QWheelEvent *ev) override;
signals:
    void ImageShowLabel_mousePressed(QPoint);
    void ImageShowLabel_mouseMoved(QPoint);
    void ImageShowLabel_toEnlarge(void);
    void ImageShowLabel_toLessen(void);
};

#endif // MAINWINDOW_H

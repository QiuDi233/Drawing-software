#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "figure.h"
#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QColorDialog>
#include <QFileDialog>
#include <QPainter>
#include <QBrush>
#include <QString>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);		// 绘制事件
    void mousePressEvent(QMouseEvent *event);	// 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);	// 鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);	// 鼠标松开事件
    void resizeEvent(QResizeEvent *event);		// 窗口调整事件

private slots:
    // 程序按钮对应的槽函数，切换程序状态
    //文件处理
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    //图形
    void on_actionPoint_triggered();
    void on_actionDottedPoint_triggered();
    void on_actionBrush_triggered();
    void on_actionLine_triggered();
    void on_actionDottedLine_triggered();
    void on_actionTriangle_triggered();
    void on_actionSquare_triggered();
    void on_actionRectangle_triggered();
    void on_actionCircle_triggered();
    void on_actionEllipse_triggered();
    void on_actionPolygon_triggered();
    void on_actionCurve_triggered();
    void on_actionCube_triggered();
    void on_actionPyramid_triggered();
    //移动
    void on_actionTranslate_triggered();
    void on_actionRotate_triggered();
    //颜色
    void on_actionPalette_triggered();
    void on_actionGradient_triggered();
    void on_actionDropper_triggered();
    void on_actionFill_triggered();
    //模式
    void on_actionSolid_triggered();
    void on_actionDotted_triggered();
    //某些其他操作
    void on_actionEraser_triggered();
    void on_actionDrawback_triggered();
    void on_actionDelete_triggered();

    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionClip_triggered();
    void on_actionPoint_Bigger_triggered();
    void on_actionPoint_Smaller_triggered();
    void on_actionAbout_VCLPaiting_triggered();

    void on_actionEarthquake_triggered();
    void on_actionHurricane_triggered();

private:
    Ui::MainWindow *ui;
    enum State {Point,DottedPoint,Brush,Line, DottedLine,Triangle, Square,
                Rectangle, Circle, Ellipse, Polygon, DottedPolygon,Curve,Cube,Pyramid,
                Translate, Rotate, Clip,Dropper,Fill,
                ZoomIn, ZoomOut,
                Eraser,Drawback,Delete,
                Earthquake,Hurricane} state;	// 程序处于的功能中
    QVector<QPoint> points;			// 鼠标点击的各个点
    QList<Figure *> figures;	    //画了的图形
    Figure *figure;			        // 当前操作的图形
    QImage image;					// 背景图
    QPen pen;						// 点的参数
    bool gradient;
    int mode;                       //0代表实线 1代表虚线
    QPainter painter;				// 画笔，用于绘制单个点
    QString s;               //打开文件
};

#endif // MAINWINDOW_H

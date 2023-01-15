#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<cstdlib>
#include<ctime>


#define mouse_dx 60//鼠标校准
#define mouse_dy 52
#define solid 0
#define dotted 1

extern QSet<QPoint>pointset;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    state(Point),
    gradient(false),
    figure(nullptr),
    mode(solid),
    pen(Qt::black,2)//第一个参数为画刷颜色 第二个参数是宽度
{
    ui->setupUi(this);
    // 状态栏对象只能有一个
}

MainWindow::~MainWindow()
{
    foreach (Figure *p, figures)
        delete p;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    image.fill(Qt::white);
    painter.begin(&image);

    if(state==Delete){
        foreach (Figure *p, figures){
            figures.removeAll(p);
            delete p;
        }
    }

    if (!s.isEmpty())
    {
        QImage temp(s);
        painter.drawImage(QPoint((image.width() - temp.width()) / 2, (image.height() - temp.height()) / 2),temp);
    }
    foreach (Figure *p, figures)
    {
        if(!gradient){
            painter.setPen(p->pen());
            painter.drawPoints(p->points());
        }
        else{
            QVector<QPoint>v;
            srand(time(0));
            int a=rand()%256;
            for(int i=0;i<(p->points().size());++i){
                v.push_back(p->points()[i]);
                int width=p->pen().width();
                QColor color;
                color.setRed((a+i)%256);
                color.setGreen((a+i)%256);
                color.setBlue((a+i)%256);
                painter.setPen({color,width});
                painter.drawPoints(v);
                v.clear();
            }
        }
    }
    if (state == Polygon || state == Curve||state==DottedPolygon)//选择画多边形或曲线时会出现已画的点
    {
        painter.setBrush(QBrush(pen.color()));
        foreach (QPoint p, points)
            painter.drawEllipse(p, 2, 2);
    }
    if (state == Clip && figure)
    {
        painter.setPen(figure->pen());
        painter.drawPoints(figure->points());
    }
    painter.end();
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= mouse_dx;
    pos.ry() -= mouse_dy;
    points.append(pos);//记录鼠标位置参数
    QPen pen1=pen;
    pen1.setWidth(pen.width()+8);//刷子的宽度


    switch (state)
    {
    case Point:
        figure = new Figure(pen, Figure::Point,
        {pos});
        figures.append(figure);
        break;
    case DottedPoint:
        figure = new Figure(pen, Figure::DottedPoint,
        {pos});
        figures.append(figure);
        break;
    case Brush:
        //用很宽的pen1初始化
        figure = new Figure(pen1, Figure::Brush,
        {pos});
        figures.append(figure);
        break;
    case Line:
        if(mode==solid){
            figure = new Figure(pen, Figure::Line,
            {pos, pos});
            figures.append(figure);
        }
        if(mode==dotted){
            figure = new Figure(pen, Figure::DottedLine,
            {pos, pos});
            figures.append(figure);
        }
        break;

    case Triangle:
        if(mode==solid){
            figure = new Figure(pen, Figure::Polygon,
            {pos, pos, pos});
            figures.append(figure);
        }
        if(mode==dotted){
            figure = new Figure(pen, Figure::DottedPolygon,
            {pos, pos, pos});
            figures.append(figure);
        }
        break;
    case Square:
        if(mode==solid){
            figure = new Figure(pen, Figure::Polygon,
            {pos, pos, pos, pos});
            figures.append(figure);
        }
        if(mode==dotted){
            figure = new Figure(pen, Figure::DottedPolygon,
            {pos, pos, pos, pos});
            figures.append(figure);
        }
        break;
    case Rectangle:
        if(mode==solid){
            figure = new Figure(pen, Figure::Polygon,
            {pos, pos, pos, pos});
            figures.append(figure);
        }
        if(mode==dotted){
            figure = new Figure(pen, Figure::DottedPolygon,
            {pos, pos, pos, pos});
            figures.append(figure);
        }
        break;
    case Cube:
        figure = new Figure(pen, Figure::Cube,
        {pos, pos});
        figures.append(figure);
        break;
    case Pyramid:
        figure = new Figure(pen, Figure::Pyramid,
        {pos, pos});
        figures.append(figure);
        break;
    case Circle:
        figure = new Figure(pen, Figure::Circle,
        {pos, QPoint(0, 0)});
        figures.append(figure);
        break;
    case Ellipse:
        figure = new Figure(pen, Figure::Ellipse,
        {pos, QPoint(0, 0)});
        figures.append(figure);
        break;
    case Delete:
    case Polygon:
    case DottedPolygon:
    case Curve:
        break;
    case Clip:
        figure = new Figure(QPen(Qt::black, 1), Figure::Polygon,
        {pos, pos, pos, pos});
        break;
    case Translate:
    case Rotate:
    case Fill:
    case ZoomIn:
    case ZoomOut:
    case Eraser:
    case Earthquake:
    case Hurricane:
    case Dropper:
        //找出鼠标点击处是哪个图形
        figure = nullptr;
        foreach (Figure *p, figures)
            if (p->isOn(pos))//鼠标点击的地方在这个图上
            {
                figure = p;
                break;
            }
        break;
    case Drawback:
        int n=figures.size();
        figure = figures.at(n-1);   //选定要删的是最后一个
        break;
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= mouse_dx;
    pos.ry() -= mouse_dy;
    QVector<QPoint> args;

    QPoint before;
    QPoint now;
    if(figure)
    {
        before = points[0] - figure->center();
        now = pos - figure->center();
    }
    qreal product = before.x() * now.y() - before.y() * now.x();
    qreal before_norm = qSqrt(qreal(before.x() * before.x() + before.y() * before.y()));
    qreal now_norm = qSqrt(qreal(now.x() * now.x() + now.y() * now.y()));

    switch (state)
    {
    case Point:
        points.append(pos);
        figure->setArgs(points);
        break;
    case DottedPoint:
        points.append(pos);
        figure->setArgs(points);
        break;
    case Brush:
        points.append(pos);
        figure->setArgs(points);
        break;
    case Line:
        figure->setArgs({points[0], pos});
        break;
    case DottedLine:
        figure->setArgs({points[0], pos});
        break;
    case Triangle:
        figure->setArgs({pos,
                            {(points[0].x() + pos.x()) / 2, points[0].y()},
                            {points[0].x(), pos.y()}});
        break;
    case Square://边长 (pos.x-points[0].x+pos.y-points[0].y)/2
        figure->setArgs({points[0],
                            {points[0].x(), (pos.x()-points[0].x()+pos.y()-points[0].y())/2+points[0].y()},
                            {(pos.x()-points[0].x()+pos.y()-points[0].y())/2 +points[0].x(),(pos.x()-points[0].x()+pos.y()-points[0].y())/2+points[0].y()},
                            {(pos.x()-points[0].x()+pos.y()-points[0].y())/2 +points[0].x(), points[0].y()}});
        break;
    case Rectangle:
        figure->setArgs({points[0],
                            {points[0].x(), pos.y()},
                            pos,
                            {pos.x(), points[0].y()}});
        break;
    case Circle:
        if (qAbs(pos.x() - points[0].x()) < qAbs(pos.y() - points[0].y()))
            if (pos.ry() > points[0].y())
                pos.ry() = points[0].y() + qAbs(pos.rx() - points[0].x());
            else
                pos.ry() = points[0].y() - qAbs(pos.rx() - points[0].x());
        else
            if (pos.rx() > points[0].x())
                pos.rx() = points[0].x() + qAbs(pos.ry() - points[0].y());
            else
                pos.rx() = points[0].x() - qAbs(pos.ry() - points[0].y());
        figure->setArgs({(pos + points[0]) / 2,
                            (pos - points[0]) / 2});
        break;
    case Ellipse:
        figure->setArgs({(pos + points[0]) / 2,
                            (pos - points[0]) / 2});
        break;
    case Cube:
        figure->setArgs({points[0], pos});
        break;
    case Pyramid:
        figure->setArgs({points[0], pos});
        break;
    case Delete:
    case Polygon:
    case DottedPolygon:
    case Curve:
    case ZoomIn:
    case ZoomOut:
    case Eraser:
    case Drawback:
    case Dropper:
        break;
    case Translate:
        if (!figure)
            break;
        args = figure->translate(pos - points[0]);
        figure->setPoints(args);
        break;
    case Fill:
        break;
    case Clip:
        figure->setArgs({points[0],
                            {points[0].x(), pos.y()},
                            pos,
                            {pos.x(), points[0].y()}});
        foreach (Figure *p, figures)
        {
            args = figure->args();
            args = p->clip(args[0], args[2]);
            p->setPoints(args);
        }
        break;
    case Rotate:
        if (!figure)
            break;

        args = figure->rotate(qAsin(product / (before_norm*now_norm)));
        figure->setPoints(args);

        break;
    case Earthquake:
        if (!figure)
            break;
        points[0]=pos;
        args = figure->earthquake(qAsin(product /(before_norm*now_norm)));
        figure->setPoints(args);

        break;
    case Hurricane:
        if (!figure)
            break;

        args = figure->hurricane(qAsin(product / (before_norm*now_norm)));
        figure->setArgs(args);

        break;
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    pos.rx() -= mouse_dx;
    pos.ry() -= mouse_dy;
    QVector<QPoint> args;

    QPoint before;
    QPoint now;
    if(figure)
    {
        before = points[0] - figure->center();
        now = pos - figure->center();
    }
    qreal product = before.x() * now.y() - before.y() * now.x();
    qreal before_norm = qSqrt(qreal(before.x() * before.x() + before.y() * before.y()));
    qreal now_norm = qSqrt(qreal(now.x() * now.x() + now.y() * now.y()));


    switch (state)
    {
    case Point:
        points.append(pos);
        figure->setArgs(points);
        break;
    case DottedPoint:
        points.append(pos);
        figure->setArgs(points);
        break;
    case Brush:
        points.append(pos);
        figure->setArgs(points);
        break;
    case Line:
        figure->setArgs({points[0], pos});
        break;
    case DottedLine:
        figure->setArgs({points[0], pos});
        break;
    case Triangle:
        figure->setArgs({pos,
                            {(points[0].x() + pos.x()) / 2, points[0].y()},
                            {points[0].x(), pos.y()}});
        break;
    case Square:
        figure->setArgs({points[0],
                            {points[0].x(), (pos.x()-points[0].x()+pos.y()-points[0].y())/2+points[0].y()},
                            {(pos.x()-points[0].x()+pos.y()-points[0].y())/2 +points[0].x(),(pos.x()-points[0].x()+pos.y()-points[0].y())/2+points[0].y()},
                            {(pos.x()-points[0].x()+pos.y()-points[0].y())/2 +points[0].x(), points[0].y()}});
        break;
    case Cube:
        figure->setArgs({points[0], pos});
        break;
    case Pyramid:
        figure->setArgs({points[0], pos});
        break;
    case Rectangle:
        figure->setArgs({points[0],
                            {points[0].x(), pos.y()},
                            pos,
                            {pos.x(), points[0].y()}});
        break;
    case Circle:
        if (qAbs(pos.x() - points[0].x()) < qAbs(pos.y() - points[0].y()))
            if (pos.ry() > points[0].y())
                pos.ry() = points[0].y() + qAbs(pos.rx() - points[0].x());
            else
                pos.ry() = points[0].y() - qAbs(pos.rx() - points[0].x());
        else
            if (pos.rx() > points[0].x())
                pos.rx() = points[0].x() + qAbs(pos.ry() - points[0].y());
            else
                pos.rx() = points[0].x() - qAbs(pos.ry() - points[0].y());
        figure->setArgs({(pos + points[0]) / 2,
                            (pos - points[0]) / 2});
        break;
    case Ellipse:
        figure->setArgs({(pos + points[0]) / 2,
                            (pos - points[0]) / 2});
        break;
    case Polygon:
        figure->setArgs(points);
        break;
    case DottedPolygon:
        figure->setArgs(points);
        break;
    case Curve:
        figure->setArgs(points);
        break;
    case Translate:
        if (!figure)
            break;
        args = figure->translate(pos - points[0]);
        figure->setArgs(args);
        break;
    case Fill:
        if(!figure)
            break;
        /*for(int i=0;i<figure->points().size();++i){
            pointset.insert(points[i]);
        }
        figure->fill(figure->center());*/
        break;
    case Clip:
        figure->setArgs({points[0],
                            {points[0].x(), pos.y()},
                            pos,
                            {pos.x(), points[0].y()}});

        foreach (Figure *p, figures)
        {
            args = figure->args();
            args = p->clip(args[0], args[2]);
            p->setArgs(args);
        }
        delete figure;
        figure = nullptr;
        break;
    case ZoomIn:
        if (!figure)
            break;
        args = figure->scale(1.2);
        figure->setArgs(args);
        break;
    case ZoomOut:
        if (!figure)
            break;
        args = figure->scale(0.8);
        figure->setArgs(args);
        break;
    case Eraser:
        figures.removeAll(figure);
        delete figure;
        break;
    case Drawback:
        figures.removeAll(figure);
        delete figure;
        break;
    case Delete:
        break;
    case Rotate:
        if (!figure)
            break;
        args = figure->rotate(qAsin(product /(before_norm*now_norm)));
        figure->setArgs(args);
        break;
    case Earthquake:
        if (!figure)
            break;

        args = figure->earthquake(qAsin(product / (before_norm*now_norm)));
        figure->setArgs(args);
        break;
    case Hurricane:
        if (!figure)
            break;

        args = figure->hurricane(qAsin(product /(before_norm*now_norm)));
        figure->setArgs(args);
        break;
    case Dropper:
        if(!figure)
            pen.setColor(Qt::white);
        else
            pen.setColor(figure->pen().color());
    }
    update();
    if (state != Curve && state != Polygon &&state!=DottedPolygon)
        points.clear();
}

void MainWindow::resizeEvent(QResizeEvent *event)//改变大小
{
    Q_UNUSED(event)
    ui->label->resize(ui->centralWidget->size() - QSize(30, 30));
    image = QImage(ui->label->size(), QImage::Format_RGB32);
}

void MainWindow::on_actionOpen_triggered()
{
    s = (QFileDialog::getOpenFileName(this, QString(), QString(), "Image Files(*.bmp *.jpg *.png)"));
    ui->statusBar->showMessage(tr("打开文件"));
}

void MainWindow::on_actionSave_triggered()
{
    image.save(QFileDialog::getSaveFileName(this, QString(), QString(), "Image Files(*.bmp *.jpg *.png)"));
    ui->statusBar->showMessage(tr("保存文件"));
}

void MainWindow::on_actionPoint_triggered()
{
    state = Point;
    points.clear();
    ui->statusBar->showMessage(tr("自由绘画中"));
}

void MainWindow::on_actionDottedPoint_triggered()
{
    state = DottedPoint;
    points.clear();
    ui->statusBar->showMessage(tr("自由虚线绘制中"));
}

void MainWindow::on_actionBrush_triggered()
{
    state = Brush;
    points.clear();
    ui->statusBar->showMessage(tr("刷子使用中"));
}

void MainWindow::on_actionLine_triggered()
{
    state = Line;
    points.clear();
    ui->statusBar->showMessage(tr("直线绘制中"));
}

void MainWindow::on_actionDottedLine_triggered()
{
    state = DottedLine;
    points.clear();
    ui->statusBar->showMessage(tr("虚线绘制中"));
}

void MainWindow::on_actionTriangle_triggered()
{
    state = Triangle;
    points.clear();
    ui->statusBar->showMessage(tr("三角形绘制中"));
}


void MainWindow::on_actionSquare_triggered()
{
    state = Square;
    points.clear();
    ui->statusBar->showMessage(tr("正方形绘制中"));
}

void MainWindow::on_actionRectangle_triggered()
{
    state = Rectangle;
    points.clear();
    ui->statusBar->showMessage(tr("长方形绘制中"));
}

void MainWindow::on_actionCircle_triggered()
{
    state = Circle;
    points.clear();
    ui->statusBar->showMessage(tr("圆形绘制中"));
}

void MainWindow::on_actionEllipse_triggered()
{
    state = Ellipse;
    points.clear();
    ui->statusBar->showMessage(tr("椭圆绘制中"));
}

void MainWindow::on_actionPolygon_triggered()
{
    if(mode==solid){
        state = Polygon;
        points.clear();
        figure = new Figure(pen, Figure::Polygon, points);
        figures.append(figure);
        ui->statusBar->showMessage(tr("多边形绘制中"));
    }
    if(mode==dotted){
        state = DottedPolygon;
        points.clear();
        figure = new Figure(pen, Figure::DottedPolygon, points);
        figures.append(figure);
        ui->statusBar->showMessage(tr("虚线多边形绘制中"));
    }
}

void MainWindow::on_actionCurve_triggered()
{
    state = Curve;
    points.clear();
    //qDebug()<<"curve1"<<endl;
    figure = new Figure(pen, Figure::Curve, points);
    //qDebug()<<"curve2"<<endl;
    figures.append(figure);
    //qDebug()<<"curve3"<<endl;
    ui->statusBar->showMessage(tr("曲线绘制中"));
}

void MainWindow::on_actionCube_triggered()
{
    state = Cube;
    points.clear();
    ui->statusBar->showMessage(tr("长方体绘制中"));
}
void MainWindow::on_actionPyramid_triggered()
{
    state = Pyramid;
    points.clear();
    ui->statusBar->showMessage(tr("金字塔绘制中"));
}
void MainWindow::on_actionTranslate_triggered()
{
    state = Translate;
    points.clear();
    ui->statusBar->showMessage(tr("拖动图形进行移动"));
}

void MainWindow::on_actionFill_triggered()
{
    state = Fill;
    points.clear();
    ui->statusBar->showMessage(tr("点击图形进行颜色填充"));
}

void MainWindow::on_actionRotate_triggered()
{
    state = Rotate;
    points.clear();
    ui->statusBar->showMessage(tr("拖动图形进行旋转"));
}

void MainWindow::on_actionEarthquake_triggered()
{
    state = Earthquake;
    points.clear();
    ui->statusBar->showMessage(tr("选择图形给其一点小小的地震震撼"));
}

void MainWindow::on_actionHurricane_triggered()
{
    state = Hurricane;
    points.clear();
    ui->statusBar->showMessage(tr("选择图形给其一点小小的飓风震撼"));
}

void MainWindow::on_actionPalette_triggered()
{
    pen.setColor(QColorDialog::getColor(pen.color(), this));
    ui->statusBar->showMessage(tr("颜色选择中"));
}
void MainWindow::on_actionDropper_triggered()
{
    state=Dropper;
    ui->statusBar->showMessage(tr("颜色提取中"));
}
void MainWindow::on_actionGradient_triggered()
{
    gradient=1-gradient;
    ui->statusBar->showMessage(tr("渐变效果"));
}

void MainWindow::on_actionEraser_triggered()
{
    state = Eraser;
    points.clear();
    ui->statusBar->showMessage(tr("图形擦除中"));
}

void MainWindow::on_actionDrawback_triggered()
{
    state = Drawback;
    points.clear();
    ui->statusBar->showMessage(tr("点击空白处进行图形撤回"));
}

void MainWindow::on_actionDelete_triggered()
{
    state = Delete;
    points.clear();
    ui->statusBar->showMessage(tr("画布清除"));
}

void MainWindow::on_actionZoomIn_triggered()
{
    state = ZoomIn;
    points.clear();
    ui->statusBar->showMessage(tr("图形放大"));
}

void MainWindow::on_actionZoomOut_triggered()
{
    state = ZoomOut;
    points.clear();
    ui->statusBar->showMessage(tr("图形缩小"));
}

void MainWindow::on_actionClip_triggered()
{
    state = Clip;
    points.clear();
    ui->statusBar->showMessage(tr("图形裁剪中"));
}

void MainWindow::on_actionPoint_Bigger_triggered()
{
    pen.setWidth(pen.width() + 1);
    ui->statusBar->showMessage(tr("画笔变大"));
}

void MainWindow::on_actionPoint_Smaller_triggered()
{
    if (pen.width() > 1)
        pen.setWidth(pen.width() - 1);
    ui->statusBar->showMessage(tr("画笔变小"));
}
void MainWindow::on_actionSolid_triggered()
{
    mode=solid;
    ui->statusBar->showMessage(tr("切换为实线"));
}
void MainWindow::on_actionDotted_triggered()
{
    mode=dotted;
    ui->statusBar->showMessage(tr("切换为虚线"));
}
void MainWindow::on_actionAbout_VCLPaiting_triggered()
{
    QDesktopServices::openUrl(QUrl("https://docs.qq.com/doc/DSnBnSWFKT2hXSFFr"));
}

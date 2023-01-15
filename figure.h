#ifndef FIGURE_H
#define FIGURE_H

#include <QPen>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QtMath>
#include <QtAlgorithms>
#include <QDebug>
#include <functional>
class Figure
{
public:
    Figure();
    ~Figure();
    enum Type { Point,DottedPoint,Brush,Line,DottedLine,
                Polygon, DottedPolygon,Circle, Ellipse,
                Curve,Cube,Pyramid};
    Figure(QPen pen, Type type, QVector<QPoint> args);

    QPen pen() const;
    QPoint center() const;
    Type type() const;
    QVector<QPoint> args() const;
    QVector<QPoint> points() const;

    bool isOn(QPoint p);	// 判断图形是否有这个点
    void setArgs(QVector<QPoint> args);	// 设置args参数,设置点，设置中心
    void setPoints(QVector<QPoint> args);	// 设置points

    static QVector<QPoint> drawPoint(QVector<QPoint> args);		// 绘制点 即自由绘画
    static QVector<QPoint> drawDottedPoint(QVector<QPoint> args);		// 自由画虚线
    static QVector<QPoint> drawBrush(QVector<QPoint> args);		// 刷子
    static QVector<QPoint> drawLine(QVector<QPoint> args);		// 绘制直线
    static QVector<QPoint> drawDottedLine(QVector<QPoint> args);//绘制虚线
    static QVector<QPoint> drawPolygon(QVector<QPoint> args);	// 绘制多边形
    static QVector<QPoint> drawDottedPolygon(QVector<QPoint> args);	// 绘制虚线多边形
    static QVector<QPoint> drawCircle(QVector<QPoint> args);	// 绘制圆形
    static QVector<QPoint> drawEllipse(QVector<QPoint> args);	// 绘制椭圆
    static QVector<QPoint> drawCurve(QVector<QPoint> args);		// 绘制曲线
    static QVector<QPoint> drawCube(QVector<QPoint> args);		// 绘制立方体
    static QVector<QPoint> drawPyramid(QVector<QPoint> args);		// 绘制立方体

    QVector<QPoint> translate(QPoint pos);		// 平移
    QVector<QPoint> rotate(qreal r);			// 旋转
    QVector<QPoint> scale(qreal t);				// 缩放
    QVector<QPoint> clip(QPoint lt, QPoint rb);	// 裁剪
    void fill(QPoint q);

    QVector<QPoint> earthquake(qreal r);
    QVector<QPoint> hurricane(qreal r);
private:
    QPen _pen;	// 构成图形的点的颜色和大小
    Type _type;	// 类型（直线、多边形、圆形、椭圆、曲线）
    QPoint _center;	// 图形的中心，旋转和缩放会用到
    QVector<QPoint> _args;	// 图形的各个参数
    QVector<QPoint> _points;	// 组成图形的点集
};

#endif // FIGURE_H

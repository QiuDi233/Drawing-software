#include "Figure.h"
#include "utils.h"
#define solid 0
#define dotted 1
Figure::Figure()
{

}

Figure::~Figure(){

}

Figure::Figure(QPen pen, Figure::Type type, QVector<QPoint> args)
    :_pen(pen), _type(type)
{

    setArgs(args);
}

QPen Figure::pen() const
{
    return _pen;
}

bool Figure::isOn(QPoint pos)//用于判断这个位置在不在某图形上，对图形进行平移旋转等操作时会用到
{
    foreach (QPoint p, _points)
    {
        QPoint d = p - pos;
        if (d.x() * d.x() + d.y() * d.y() < 40)
            return true;
    }
    return false;
}

QPoint Figure::center() const
{
    return _center;
}

Figure::Type Figure::type() const
{
    return _type;
}

QVector<QPoint> Figure::args() const
{
    return _args;
}

QVector<QPoint> Figure::points() const
{
    return _points;
}

/*-------------------------------设置参数和点--------------------------*/
void Figure::setArgs(QVector<QPoint> args){//设置点和中心点
    _args = args;
    setPoints(args);
    int xsum = 0;
    int ysum = 0;
    foreach (QPoint p, args)
    {
        xsum += p.x();
        ysum += p.y();
    }
    if(args.size()!=0)
        _center = QPoint(xsum / args.size(), ysum / args.size()) ;
}

void Figure::setPoints(QVector<QPoint> args)
{
    switch (_type)
    {
    case Point:
        _points = drawPoint(args); break;
    case DottedPoint:
        _points = drawDottedPoint(args); break;
    case Brush:
        _points = drawBrush(args); break;
    case Line:
        _points = drawLine(args); break;
    case DottedLine:
        _points = drawDottedLine(args); break;
    case Polygon:
        _points = drawPolygon(args); break;
    case DottedPolygon:
        _points = drawDottedPolygon(args); break;
    case Circle:
        _points = drawCircle(args); break;
    case Cube:
        _points = drawCube(args); break;
    case Pyramid:
        _points = drawPyramid(args); break;
    case Ellipse:
        _points = drawEllipse(args); break;
    case Curve:
        _points = drawCurve(args); break;
    }
}

/*---------------------------画图形的函数----------------------------------------*/
QVector<QPoint> Figure::drawPoint(QVector<QPoint> args)
{
    QVector<QPoint> points;

    int n=args.size();
    for(int i=0;i<n-1;++i){
        points.append(drawLine({args[i], args[i + 1]}));//和之后的那个点连成直线
    }

    return points;
}

QVector<QPoint> Figure::drawDottedPoint(QVector<QPoint> args)
{
    QVector<QPoint> points;
    points=args;
    return points;
}

QVector<QPoint> Figure::drawBrush(QVector<QPoint> args)
{
    QVector<QPoint> points;

    int n=args.size();
    for(int i=0;i<n-1;++i){
        points.append(drawLine({args[i], args[i + 1]}));//和之后的那个点连成直线
    }

    return points;
}
QVector<QPoint> Figure::drawLine(QVector<QPoint> args)
{
    //Bresenham画线算法
    int x1 = args[0].x();//参数是起始点和结束点
    int y1 = args[0].y();
    int x2 = args[1].x();
    int y2 = args[1].y();

    QVector<QPoint> points;

    if (x1 == x2) {//垂直
        size_t miny = y1 < y2 ? y1 : y2;
        size_t maxy = y1 < y2 ? y2 : y1;
        for (size_t y = miny; y <= maxy; ++y) {
           points.append(QPoint(x1, y));
        }
        return points;
    }
    float slope = ((float) (y2 - y1) /(x2 - x1)); //斜率
    if (slope >= 0 && slope < 1) {
        size_t X0 = x1 < x2 ? x1 : x2;//x0<x1
        size_t X1 = x1 < x2 ? x2 : x1;
        size_t Y0 = x1 < x2 ? y1 : y2;
        size_t Y1 = x1 < x2 ? y2 : y1;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (X1 - X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dy - dx;
        int F = dy - dx / 2;
        for (x = X0; x <= X1; ++x) {
            points.append(QPoint(x, y));
            if (F < 0)
                F += dy;
            else {
                y++;
                F += dydx;
            }
        }
        return points;
    }
    if (slope >= 1) {//把0到1之间的情况的x和y反过来
        size_t Y0 = y1 < y2 ? y1 : y2;//y1<y2
        size_t Y1 = y1 < y2 ? y2 : y1;
        size_t X0 = y1 < y2 ? x1 : x2;
        size_t X1 = y1 < y2 ? x2 : x1;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (X1 - X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dx - dy;
        int F = dx - dy / 2;
        for (y = Y0; y <= Y1; ++y) {
            points.append(QPoint(x, y));
            if (F < 0)
                F += dx;
            else {
                x++;
                F += dydx;
            }
        }
        return points;
    }
    if (slope <0 && slope > -1) {//x变成-x
        size_t X0 = x1 < x2 ? x2 : x1;//x0>x1
        size_t X1 = x1 < x2 ? x1 : x2;
        size_t Y0 = x1 < x2 ? y2 : y1;
        size_t Y1 = x1 < x2 ? y1 : y2;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (-X1 + X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dy - dx;
        int F = dy - dx / 2;
        for (x = X0; x >= X1; --x) {
            points.append(QPoint(x, y));
            if (F < 0)
                F += dy;
            else {
                y++;
                F += dydx;
            }
        }
        return points;
    }
    if (slope <=-1) {                            //把0到1之间的情况的x和y反过来 x变成-x
        size_t Y0 = y1 < y2 ? y1 : y2;//y1<=y2
        size_t Y1 = y1 < y2 ? y2 : y1;
        size_t X0 = y1 < y2 ? x1 : x2;
        size_t X1 = y1 < y2 ? x2 : x1;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (-X1 + X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dx - dy;
        int F = dx - dy / 2;
        for (y = Y0; y <= Y1; ++y) {
            points.append(QPoint(x, y));
            if (F < 0)
                F += dx;
            else {
                x--;
                F += dydx;
            }
        }
        return points;
    }
    return points;
}

QVector<QPoint> Figure::drawDottedLine(QVector<QPoint> args)
{
    //Bresenham画线算法
    int x1 = args[0].x();//参数是起始点和结束点
    int y1 = args[0].y();
    int x2 = args[1].x();
    int y2 = args[1].y();

    QVector<QPoint> points;

    if (x1 == x2) {//垂直
        size_t miny = y1 < y2 ? y1 : y2;
        size_t maxy = y1 < y2 ? y2 : y1;
        int cnt=0;
        for (size_t y = miny; y <= maxy; ++y) {
            if((cnt/10)%2==0){
                points.append(QPoint(x1, y));
            }
            cnt++;
        }
        return points;
    }
    float slope = ((float) (y2 - y1) /(x2 - x1)); //斜率
    int cnt=0;
    if (slope >= 0 && slope < 1) {
        size_t X0 = x1 < x2 ? x1 : x2;//x0<x1
        size_t X1 = x1 < x2 ? x2 : x1;
        size_t Y0 = x1 < x2 ? y1 : y2;
        size_t Y1 = x1 < x2 ? y2 : y1;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (X1 - X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dy - dx;
        int F = dy - dx / 2;
        for (x = X0; x <= X1; ++x) {
            if((cnt/10)%2==0){
                points.append(QPoint(x, y));
            }
            cnt++;
            if (F < 0)
                F += dy;
            else {
                y++;
                F += dydx;
            }
        }
        return points;
    }
    if (slope >= 1) {//把0到1之间的情况的x和y反过来
        size_t Y0 = y1 < y2 ? y1 : y2;//y1<y2
        size_t Y1 = y1 < y2 ? y2 : y1;
        size_t X0 = y1 < y2 ? x1 : x2;
        size_t X1 = y1 < y2 ? x2 : x1;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (X1 - X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dx - dy;
        int F = dx - dy / 2;
        for (y = Y0; y <= Y1; ++y) {
            if((cnt/10)%2==0){
                points.append(QPoint(x, y));
            }
            cnt++;
            if (F < 0)
                F += dx;
            else {
                x++;
                F += dydx;
            }
        }
        return points;
    }
    if (slope <0 && slope > -1) {//x变成-x
        size_t X0 = x1 < x2 ? x2 : x1;//x0>x1
        size_t X1 = x1 < x2 ? x1 : x2;
        size_t Y0 = x1 < x2 ? y2 : y1;
        size_t Y1 = x1 < x2 ? y1 : y2;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (-X1 + X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dy - dx;
        int F = dy - dx / 2;
        for (x = X0; x >= X1; --x) {
            if((cnt/10)%2==0){
                points.append(QPoint(x, y));
            }
            cnt++;
            if (F < 0)
                F += dy;
            else {
                y++;
                F += dydx;
            }
        }
        return points;
    }
    if (slope <=-1) {                            //把0到1之间的情况的x和y反过来 x变成-x
        size_t Y0 = y1 < y2 ? y1 : y2;//y1<=y2
        size_t Y1 = y1 < y2 ? y2 : y1;
        size_t X0 = y1 < y2 ? x1 : x2;
        size_t X1 = y1 < y2 ? x2 : x1;
        size_t x    = X0;
        size_t y    = Y0;
        int dx   = 2 * (-X1 + X0);
        int dy   = 2 * (Y1 - Y0);
        int dydx = dx - dy;
        int F = dx - dy / 2;
        for (y = Y0; y <= Y1; ++y) {
            if((cnt/10)%2==0){
                points.append(QPoint(x, y));
            }
            cnt++;
            if (F < 0)
                F += dx;
            else {
                x--;
                F += dydx;
            }
        }
        return points;
    }
    return points;
}

QVector<QPoint> Figure::drawPolygon(QVector<QPoint> args)//参数是依次点的点
{
    QVector<QPoint> points;
    int n = args.size();
    for (int i = 0; i < n; ++i)
    {
        if (i == n - 1)
            points.append(drawLine({args[i], args[0]}));//和第一个点连成线
        else
            points.append(drawLine({args[i], args[i + 1]}));//和之后的那个点连成直线
    }
    return points;
}

QVector<QPoint> Figure::drawDottedPolygon(QVector<QPoint> args)//参数是依次点的点
{
    QVector<QPoint> points;
    int n = args.size();
    for (int i = 0; i < n; ++i)
    {
        if (i == n - 1)
            points.append(drawDottedLine({args[i], args[0]}));//和第一个点连成线
        else
            points.append(drawDottedLine({args[i], args[i + 1]}));//和之后的那个点连成直线
    }
    return points;
}

QVector<QPoint> Figure::drawCube(QVector<QPoint> args)//参数是依次点的点
{
    QVector<QPoint> points;
    QPoint p1=args[0];
    QPoint p8=args[1];
    QPoint p2={p1.x()*1/4+p8.x()*3/4,p1.y()};
    QPoint p3={p1.x(),p8.y()*3/4+p1.y()*1/4};
    QPoint p4={p2.x(),p3.y()};
    QPoint p5={p1.x()*3/4+p8.x()*1/4,p1.y()*3/4+p8.y()*1/4};
    QPoint p6={p8.x(),p5.y()};
    QPoint p7={p5.x(),p8.y()};

    points.append(drawLine({p1,p2}));
    points.append(drawLine({p1,p3}));
    points.append(drawDottedLine({p3,p4}));
    points.append(drawDottedLine({p2,p4}));

    points.append(drawLine({p1,p5}));
    points.append(drawLine({p3,p7}));
    points.append(drawLine({p2,p6}));
    points.append(drawDottedLine({p4,p8}));

    points.append(drawLine({p5,p6}));
    points.append(drawLine({p5,p7}));
    points.append(drawLine({p6,p8}));
    points.append(drawLine({p7,p8}));
    //points.append(drawLine({p1,p8}));
    return points;
}

QVector<QPoint> Figure::drawPyramid(QVector<QPoint> args)//参数是依次点的点
{
    QVector<QPoint> points;
    QPoint p1=args[0];
    QPoint p2=args[1];
    QPoint p3={2*p1.x()-p2.x(),p2.y()};
    QPoint p4={p3.x()*3/2-p1.x()*1/2,p1.y()*2/5+p3.y()*3/5};

    points.append(drawLine({p1,p2}));
    points.append(drawLine({p1,p3}));
    points.append(drawLine({p2,p3}));

    points.append(drawLine({p1,p4}));
    points.append(drawLine({p3,p4}));
    points.append(drawDottedLine({p2,p4}));


    return points;
}

QVector<QPoint> Figure::drawCircle(QVector<QPoint> args)
{
    QVector<QPoint> points;
    int cx = args[0].x(), cy = args[0].y(), r = qMin(qAbs(args[1].x()), qAbs(args[1].y()));

    int p = 1 - r, x = 0, y = r;
    points.append(QPoint(cx + x, cy + y));
    points.append(QPoint(cx - x, cy + y));
    points.append(QPoint(cx - x, cy - y));
    points.append(QPoint(cx + x, cy - y));
    points.append(QPoint(cx + y, cy + x));
    points.append(QPoint(cx - y, cy + x));
    points.append(QPoint(cx - y, cy - x));
    points.append(QPoint(cx + y, cy - x));
    while (x < y)
    {
        if (p < 0) {
            p += 2 * x + 3;
        }
        else {
            y--;
            p += 2 * (x - y) + 5;
        }
        ++x;
        points.append(QPoint(cx + x, cy + y));
        points.append(QPoint(cx - x, cy + y));
        points.append(QPoint(cx - x, cy - y));
        points.append(QPoint(cx + x, cy - y));
        points.append(QPoint(cx + y, cy + x));
        points.append(QPoint(cx - y, cy + x));
        points.append(QPoint(cx - y, cy - x));
        points.append(QPoint(cx + y, cy - x));
    }
    return points;
}

QVector<QPoint> Figure::drawEllipse(QVector<QPoint> args)
{
    QVector<QPoint> points;
    int cx = args[0].x(), cy = args[0].y(), rx = qMax(qAbs(args[1].x()), 1), ry = qMax(qAbs(args[1].y()), 1);

    if (rx >= ry)
    {
        int rx2 = rx * rx, ry2 = ry * ry;
        int p = ry2 - rx2 * ry + rx2, x = 0, y = ry;
        points.append(QPoint(cx + x, cy + y));
        points.append(QPoint(cx - x, cy + y));
        points.append(QPoint(cx - x, cy - y));
        points.append(QPoint(cx + x, cy - y));
        while (ry2 * x <= rx2 * y)
        {
            if (p < 0)
                p += ry2 * (3 + 2 * x);
            else
            {
                p += ry2 * (3 + 2 * x) + rx2 * (2 - 2 * y);
                --y;
            }
            ++x;
            points.append(QPoint(cx + x, cy + y));
            points.append(QPoint(cx - x, cy + y));
            points.append(QPoint(cx - x, cy - y));
            points.append(QPoint(cx + x, cy - y));
        }
        p = int(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1.0) * (y - 1.0) - 1.0 * rx2 * ry2);
        while (y >= 0)
        {
            if (p < 0)
            {
                p += ry2 * (2 + 2 * x) + rx2 * (3 - 2 * y);
                ++x;
            }
            else
            {
                p += rx2 * (3 - 2 * y);
            }
            --y;
            points.append(QPoint(cx + x, cy + y));
            points.append(QPoint(cx - x, cy + y));
            points.append(QPoint(cx - x, cy - y));
            points.append(QPoint(cx + x, cy - y));
        }
    }
    else
    {
        qSwap(args[0].rx(), args[0].ry());
        qSwap(args[1].rx(), args[1].ry());
        points = drawEllipse(args);
        for (auto& p : points)
            qSwap(p.rx(), p.ry());
    }
    return points;
}



QVector<QPoint> Figure::drawCurve(QVector<QPoint> args)
{
    //贝塞尔曲线
    QVector<QPoint> points;
    int n = args.size();
    if (!n)
        return points;
    //t是[0,1]之间的那个比例的数
    float t=0;
    while(t<=1){
        float x=0;
        float y=0;
        for (int i = 0; i < n; ++i) {
            QPoint p = args[i];
            x += C(n - 1, i) * p.x() * pow((1 - t), n - 1 - i) * pow(t, i);
            y += C(n - 1, i) * p.y() * pow((1 - t), n - 1 - i) * pow(t, i);
        }
        QPoint curve{int(x),int(y)};
        points.append(curve);
        t+=1e-3;
    }
    return points;
}

QVector<QPoint> Figure::translate(QPoint pos)
{
    QVector<QPoint> args = _args;
    if (_type == Circle || _type == Ellipse)
        args[0] += pos;
    else{
        int n=args.size();
        for (int i = 0; i < n ; ++i)
            args[i] += pos;
        }
    return args;
}

QVector<QPoint> Figure::rotate(qreal r)
{
    QVector<QPoint> args = _args;
    qreal cosr = qCos(r), sinr = qSin(r);

    if (_type != Circle && _type != Ellipse)
        for (auto& arg : args)
        {
            int dx = arg.x() - _center.x();
            int dy = arg.y() - _center.y();
            arg.rx() = int(_center.x() + dx * cosr - dy * sinr);//利用成员函数rx()对其进行更改
            arg.ry() = int(_center.y() + dx * sinr + dy * cosr);
        }
    return args;
}

/*QSet<QPoint>pointset;
inline uint qHash(const QPoint &point)
{
   QString valueStr(QString::number(point.x()) + QString::number(point.y()));
   return valueStr.toUInt();
}*/
void Figure::fill(QPoint q)
{
    /*if(pointset.contains(q))
        return;
    pointset.insert(q);
    QPoint tmp;
    for(int i=-1;i<=1;++i){
        for(int j=-1;j<=1;++j){
            tmp.xp=q.x()+i;
            tmp.yp=q.y()+j;
            fill(tmp);
        }
    }
    _points.clear();
    foreach (const QPoint value, pointset)
        _points.push_back(value);
    return;*/
    return;
}

QVector<QPoint> Figure::earthquake(qreal r)
{
    QVector<QPoint> args = _args;
    qreal cosr = qCos(r), sinr = qSin(r);
    int dx, dy;
    if (_type != Circle && _type != Ellipse)
        for (auto& arg : args)
        {
            dx = arg.x() - _center.x();
            dy = arg.y() - _center.y();
            arg.rx() = int(_center.x() + dx * cosr - dy * sinr);//利用成员函数rx()对其进行更改
            arg.ry() = int(_center.y() + dx * sinr + dy * cosr);
        }
    setPoints(args);
    return args;
}
QVector<QPoint> Figure::hurricane(qreal r)
{
    QVector<QPoint> args = _args;
    qreal cosr = qCos(r), sinr = qSin(r);
    int dx, dy;
    if (_type != Circle && _type != Ellipse)
        for (auto& arg : args)
        {
            dx = arg.x() - _center.x();
            dy = arg.y() - _center.y();
            arg.rx() = int(_center.x() + dx * cosr - dy * sinr);//利用成员函数rx()对其进行更改
            arg.ry() = int(_center.y() + dx * sinr + dy * cosr);
        }
    return args;
}
QVector<QPoint> Figure::scale(qreal t)
{
    QVector<QPoint> args = _args;
    if (_type == Circle || _type == Ellipse)
        args[1] *= t;
    else
        for (auto& arg : args)
            arg = _center + (arg - _center) * t;
    return args;
}

uint8_t Code(int x,int y,int l,int r,int t,int b){
    uint8_t code = 0;
    if (y < t)
        code |= 1<<3;
    if (y > b)
        code |= 1<<2;
    if (x > r)
        code |= 1<<1;
    if (x < l)
        code |= 1;
    return code;
}
QVector<QPoint> Figure::clip(QPoint lt, QPoint rb)
{
    int l = qMin(lt.x(), rb.x());
    int r = qMax(lt.x(), rb.x());
    int t = qMin(lt.y(), rb.y());
    int b = qMax(lt.y(), rb.y());
    QVector<QPoint> args = _args;
    if (_type == Line)
    {
        int x = 0, y = 0;
        int x1 = args[0].x(), y1 = args[0].y();
        int x2 = args[1].x(), y2 = args[1].y();
        uint8_t code1 = Code(x1, y1,l,r,t,b);
        uint8_t code2 = Code(x2, y2,l,r,t,b);
        uint8_t code=0;
        while (code1 || code2)
        {
            if (code1 & code2)
                break;
            code = code1 ? code1 : code2;
            if (1 & code)
            {
                x = l;
                y = y1 + (y2 - y1) * (l - x1) / (x2 - x1);
            }
            else if (2 & code)
            {
                x = r;
                y = y1 + (y2 - y1) * (r - x1) / (x2 - x1);
            }
            else if (4 & code)
            {
                y = b;
                x = x1 + (x2 - x1) * (b - y1) / (y2 - y1);
            }
            else if (8 & code)
            {
                y = t;
                x = x1 + (x2 - x1) * (t - y1) / (y2 - y1);
            }
            if (code == code1)
            {
                x1 = x;
                y1 = y;
                code1 = Code(x1, y1,l,r,t,b);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = Code(x2, y2,l,r,t,b);
            }
        }
        args = {{x1, y1}, {x2, y2}};
    }
    return args;
}

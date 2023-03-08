#ifndef WIDGET_H
#define WIDGET_H
#include <windows.h>
#include <synchapi.h>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QWidget>
#include "WinUser.h"
#include <QDesktopWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct Mark{
    QString name;
    int x;
    int y;
    int rease;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *m_tcp;
    quint32 block;
    QByteArray array;
    Mark mark;
    QString str;
    int x;
    int screen_width;
    int screen_height;
};
#endif // WIDGET_H

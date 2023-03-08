#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPixmap>
#include <QWidget>
#include <QApplication>
#include <QScreen>
#include <windows.h>
#include <QGuiApplication>
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <QImage>
#include <QBuffer>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct Mark{
    QString name;//鼠标具体操作，如单击左键，双击左键
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
    void MouseActs(int x,int y,QString name,int z);
private slots:
    void on_btn_listen_clicked();

    void on_btn_share_clicked();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *m_s;
    QTcpSocket *m_tcp;
    QTimer *time;
    QScreen *screen;
    quint32 block_count;
    //quint8==unsigned char;quint16==unsigned short;quint32==unsigned int;quint64==unsigned long long int;
    Mark mark;
};
#endif // WIDGET_H

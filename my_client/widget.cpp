#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    setMouseTracking(true);
    x=460,mark.rease=0;
    ui->setupUi(this);
    ui->lineEdit_3->setText("未连接");
    array=0;
    m_tcp =new QTcpSocket(this);
    connect(m_tcp,&QTcpSocket::connected,this,[=]()
    {
        ui->lineEdit_3->setText("连接成功");
    });
    connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
    {

        QByteArray data=m_tcp->readAll();

        if(data.contains("w_h_w"))
        {
            QDataStream in(&data,QIODevice::ReadWrite);
            //in.setVersion(QDataStream::Qt_5_14);
            in>>screen_width;
            in>>screen_height;
            data.clear();
        }

        array.append(data);
        if(data.contains("THE end"))
        {
            ui->label->hide();
            ui->label_2->hide();
            ui->label_3->hide();
            ui->lineEdit->hide();
            ui->lineEdit_2->hide();
            ui->lineEdit_3->hide();
            ui->pushButton->hide();
            QByteArray byte;
            array.replace("THE end","");
            QDataStream in(&array,QIODevice::ReadWrite);
            //in.setVersion(QDataStream::Qt_5_14);
            this->setFixedSize(((double)screen_width/(double)screen_height)*x,x);
            ui->image->resize(this->rect().width(),this->rect().height());
            ui->image->setScaledContents(true);
            QPixmap pix;
            in.device()->seek(0);
            in>>block;
            in>>byte;
            QImage img;
            img.loadFromData(byte,"JPG");
            pix.convertFromImage(img);
            ui->image->setPixmap(pix);
            array.clear();
        }
    });

}

Widget::~Widget()
{
    delete ui;
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p_re = event->pos();
    mark.name="move";
    mark.x=p_re.x();
    mark.y=p_re.y();
    QByteArray p;
    QDataStream out(&p,QIODevice::ReadWrite);
    quint8 i;
    out<<(quint8)0;
    out<<mark.name;
    out<<mark.x;
    out<<mark.y;
    out<<mark.rease;
    out<<"end";
    out.device()->seek(0);
    out<<(quint8)(p.size()-sizeof(i));
    m_tcp->write(p);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
        QPoint p_re = event->pos();
        double width=ui->image->size().width();
        double height=ui->image->size().height();
        if (event->button() == Qt::LeftButton)
        {
            qDebug() << "left click";
            mark.name="leftclick";
            //str=mark.name;
            mark.x=(double)p_re.x()/width*(double)screen_width;
            mark.y=(double)p_re.y()/height*(double)screen_height;
            qDebug()<<mark.x<<" "<<mark.y<<endl;
        }

        else if (event->button() == Qt::RightButton)
        {
            qDebug() << "right click";
            mark.name="rightclick";
            //str=mark.name;
            mark.x=(double)p_re.x()/width*(double)screen_width;
            mark.y=(double)p_re.y()/height*(double)screen_height;
            qDebug()<<mark.x<<" "<<mark.y<<endl;
        }

        else if (event->button() == Qt::MidButton)
        {
            qDebug() << "mid click";
            mark.name="midclick";
            mark.x=(double)p_re.x()/width*(double)screen_width;
            mark.y=(double)p_re.y()/height*(double)screen_height;
            qDebug()<<mark.x<<" "<<mark.y<<endl;
        }
        QByteArray p;
        QDataStream out(&p,QIODevice::ReadWrite);
        quint8 i;
        out<<(quint8)0;
        out<<mark.name;
        out<<mark.x;
        out<<mark.y;
        out<<mark.rease;
        out<<"end";
        out.device()->seek(0);
        out<<(quint8)(p.size()-sizeof(i));
        m_tcp->write(p);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{

   /* mark.rease=1;
    QByteArray p;
    QDataStream out(&p,QIODevice::ReadWrite);
    quint8 i;
    mark.name=str;
    str="move";
    out<<(quint8)0;
    out<<mark.name;
    out<<mark.x;
    out<<mark.y;
    out<<mark.rease;
    out<<"end";
    out.device()->seek(0);
    out<<(quint8)(p.size()-sizeof(i));
    m_tcp->write(p);
    mark.rease=0;*/
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    double width=ui->image->size().width();
    double height=ui->image->size().height();
    QPoint p_re = event->pos();
    if (event->button() == Qt::LeftButton)
    {
            qDebug() << "left double click";
            mark.name="leftdoubleclick";
            mark.x=(double)p_re.x()/width*(double)screen_width;
            mark.y=(double)p_re.y()/height*(double)screen_height;
            qDebug()<<mark.x<<" "<<mark.y<<endl;
    }
    else if (event->button() == Qt::RightButton)
    {
            qDebug() << "right double click";
            mark.name="leftdoubleclick";
            mark.x=(double)p_re.x()/width*(double)screen_width;
            mark.y=(double)p_re.y()/height*(double)screen_height;
            qDebug()<<mark.x<<" "<<mark.y<<endl;
    }
    QByteArray p;
    QDataStream out(&p,QIODevice::ReadWrite);
    quint8 i;
    out<<(quint8)0;
    out<<mark.name;
    out<<mark.x;
    out<<mark.y;
    out<<mark.rease;
    out<<"end";
    out.device()->seek(0);
    out<<(quint8)(p.size()-sizeof(i));
    m_tcp->write(p);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    QPoint p_re=event->pos();
    if(event->delta()>0)
    {
        x=x+40;
    }
    else if(event->delta()<0)
    {
        x=x-40;
    }
}

void Widget::on_pushButton_clicked()
{
    QString ip=ui->lineEdit->text();
    int port=ui->lineEdit_2->text().toInt();
    m_tcp->connectToHost(QHostAddress(ip),port);

}

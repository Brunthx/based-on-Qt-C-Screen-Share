#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    time=new QTimer(this);
    ui->setupUi(this);
    m_s =new QTcpServer(this);
    QRect mRect;
    mRect = QGuiApplication::primaryScreen()->geometry();//获取设备屏幕大小
    qDebug()<<mRect.width()<<mRect.height();
    connect(m_s,&QTcpServer::newConnection,this,[=]()
    {
        //接收客户端发送的套接字
        m_tcp=m_s->nextPendingConnection();
        QByteArray p;
        QDataStream out(&p,QIODevice::ReadWrite);
        out<<(int)mRect.width();
        out<<(int)mRect.height();
        out<<"w_h_w";
        m_tcp->write(p);

        connect(m_tcp,&QTcpSocket::readyRead,this,[=]()
        {
            QByteArray data=m_tcp->readAll();
            QDataStream in(&data,QIODevice::ReadWrite);
            quint8 i;
            in>>i;
            if(data.size()<i)
            {
                data.clear();
            }
            else
            {
                if(data.contains("end"))
                {
                    in>>mark.name>>mark.x>>mark.y>>mark.rease;
                    data.clear();
                }
            }
            if(mark.x<=mRect.width()&&mark.x>=0&&mark.y<=mRect.height()&&mark.y>=0)
                MouseActs(mark.x, mark.y, mark.name,mark.rease);
            qDebug()<<mark.name<<mark.rease<<mark.x<<mark.y<<endl;
        });
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::MouseActs(int x, int y, QString name,int z)//鼠标活动
{
    SetCursorPos(x,y);//windows api
    if(name=="leftclick"){
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,x, y, 0, 0);
        mark.rease=0;

    }else if(name=="leftdoubleclick"){
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,x, y, 0, 0);

    }else if(name=="rightclick"){
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP,x, y, 0, 0);

    }else if(name=="rightdoubleclick"){
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN,x, y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP,x, y, 0, 0);
    }
}


void Widget::on_btn_listen_clicked()
{
    int port=ui->lineEdit->text().toInt();
    m_s->listen(QHostAddress::Any,port);
    ui->btn_listen->setDisabled(true);
}

void Widget::on_btn_share_clicked()
{
    ui->btn_share->setDisabled(true);
    time->start(35);
    screen = QGuiApplication::primaryScreen();
    connect(time,&QTimer::timeout,this,[=]{

        //如果第一个参数为0，那就是抓取以桌面上(x,y)为左上角点，宽为width，高为height的矩形。
        QPixmap pix=screen->grabWindow(0);

        QByteArray data;
        QByteArray array;
        QDataStream put(&data,QIODevice::ReadWrite);
        //put.setVersion(QDataStream::Qt_5_14);
        put<<(quint32)45;
        QBuffer buf(&array);
        pix.save(&buf,"jpg");
        put<<array;
        put<<"THE end";
        put.device()->seek(0);
        block_count=(quint32)(data.size()-sizeof (block_count));
        put<<(quint32)(data.size()-sizeof (block_count));
        m_tcp->write(data);
    });
}

void Widget::on_pushButton_clicked()
{

    {
        void* keyBoard = nullptr;
        bool m_b = Wow64DisableWow64FsRedirection(&keyBoard);
        QString boardExe = "C:/Windows/System32/osk.exe";
        QString params = "";
        ShellExecute(nullptr, L"open", (LPCWSTR)boardExe.utf16(), (LPCWSTR)params.utf16(), nullptr, SW_SHOWNORMAL);
            if (m_b)
            {
                Wow64RevertWow64FsRedirection(keyBoard);
            }
    }
}

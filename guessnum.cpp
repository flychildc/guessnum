#include "guessnum.h"
#include "ui_guessnum.h"
#include <QDebug>
#include <QTime>
#include <QMessageBox>

guessnum::guessnum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::guessnum)
{
    ui->setupUi(this);
    this->setWindowTitle(tr(u8"猜数字游戏"));
    //设置栈部件显示第一页
    ui->stackedWidget->setCurrentWidget(ui->page_mainui);

    //初始化数据
    //失败动图
    losemovie.setFileName(":/new/Image/Image/over.gif");
    //losemovie.setFileName(":/new/Image/Image/Luffy.png");
    ui->label_lose->setMovie(&losemovie);
    ui->label_lose->setScaledContents(true);//让动画自动适应标签大小
    //成功动图
    winmovie.setFileName(":/new/Image/Image/win.gif");
    ui->label_win->setMovie(&winmovie);
    ui->label_win->setScaledContents(true);

    //将游戏界面的数字按键和数字处理槽函数连接起来
    connect(ui->pushButton,   &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_10,&QPushButton::clicked, this, &guessnum::dealnum);
    connect(ui->pushButton_11,&QPushButton::clicked, this, &guessnum::dealnum);
}

guessnum::~guessnum()
{
    delete ui;
}

//进入游戏
void guessnum::on_pushButton_In_clicked()
{
    //切换到猜数字界面
    //ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setCurrentWidget(ui->page_gameui);

    //获取下拉框的内容，由于返回值是QString，将其转换为int类型
    gametime = ui->comboBox->currentText().toInt();
    qDebug() << gametime << "s";

    int num;
    //产生随机数种子
    //以0时0分0秒到当前时间的秒数为一个随机数种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //产生一个1000~9999的数
    while((num = qrand()%10000) < 999);
    randstr = QString::number(num);
    qDebug() << "randstr =" << randstr;

    //设置进度条
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(gametime);
    ui->progressBar->setValue(gametime);

    //开启游戏定时器
    gametimerID = 0;
    gametimerID = startTimer(1000);//定时1秒钟

    //清空所猜的数以及显示区
    ui->textEdit->clear();
    resultstr.clear();
}

//退出游戏
void guessnum::on_pushButton_Out_clicked()
{
    this->close();
}

void guessnum::timerEvent(QTimerEvent *event)
{
    //处理gametimerID定时器
    if(event->timerId() == gametimerID)
    {
        gametime--;
        ui->progressBar->setValue(gametime);

        if(gametime == 0)
        {
            killTimer(gametimerID);//关闭定时器
            //进行失败的相关操作
            QMessageBox::information(this,tr(u8"失败"),tr(u8"时间到了!!!"));
            //如果图片显示不出来，需要把编译好的文件夹删除重新编译就能显示出来了，这可能C++的函数模板两次编译机制有关
            losemovie.start();//开启失败动画
            ui->stackedWidget->setCurrentWidget(ui->page_lose);
            //定时器开启之后程序不会停在这里，而是继续执行，到exec()函数进入事件循环，监测事件
            losetimerID = startTimer(3000);//将失败动画显示3秒钟
        }
    }
    else if(event->timerId() == losetimerID)//处理失败定时器
    {
        losemovie.stop();
        killTimer(losetimerID);
        ui->stackedWidget->setCurrentWidget(ui->page_mainui);//回到主窗口
    }
    else if(event->timerId() == wintimerID)
    {
        winmovie.stop();
        killTimer(wintimerID);
        ui->stackedWidget->setCurrentWidget(ui->page_mainui);
    }
}
//处理按下的数字
void guessnum::dealnum()
{
    //获取按钮的内容
    QObject *mysender = sender();//返回指向发送信号对象的指针
    //QPushButton *pb = (QPushButton*)mysender;
    QPushButton *pb = static_cast<QPushButton*>(mysender);//转换为按钮类型

    if(pb != NULL)
    {
        //获取按钮内容
        QString temp = pb->text();
        resultstr += temp;
        //resultstr.append(temp);

        //输入的数字首位不能为0
        if(resultstr.size() == 1 && resultstr == "0")
        {
            resultstr.clear();
        }
        if(resultstr.size() <= 4)
        {
            //将猜测的数字显示在界面上
            ui->textEdit->setText(resultstr);
            if(resultstr.size() == 4)
            {
                if(resultstr < randstr)
                {
                    ui->textEdit->append(tr(u8"数字小了！！！"));
                }
                else if(resultstr > randstr)
                {
                    ui->textEdit->append(tr(u8"数字大了！！！"));
                }
                else
                {
                    ui->textEdit->append(tr(u8"恭喜你猜对了！！！"));
                    killTimer(gametimerID);
                    QMessageBox::information(this,tr(u8"胜利"),tr(u8"恭喜你猜对了！！！"));
                    winmovie.start();
                    ui->stackedWidget->setCurrentWidget(ui->page_win);
                    wintimerID = startTimer(3000);//将胜利动画显示3秒钟
                }
                resultstr.clear();
            }
        }
    }
}

//处理退格按下
void guessnum::on_pushButton_6_clicked()
{
    if(resultstr.size() == 1)
    {
        resultstr.clear();
        ui->textEdit->clear();
    }
    else
    {
        resultstr.chop(1);
        ui->textEdit->setText(resultstr);
    }
}

//处理“提示”键按下
void guessnum::on_pushButton_12_clicked()
{
    resultstr.clear();
    QString str = tr(u8"随机数为：") + randstr;
    ui->textEdit->setText(str);
}

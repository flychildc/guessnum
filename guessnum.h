#ifndef GUESSNUM_H
#define GUESSNUM_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class guessnum;
}

class guessnum : public QWidget
{
    Q_OBJECT

public:
    explicit guessnum(QWidget *parent = 0);
    ~guessnum();

    //自定义槽函数
    void dealnum();

private slots:
    void on_pushButton_In_clicked();

    void on_pushButton_Out_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_12_clicked();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::guessnum *ui;

    int gametime;//游戏时间
    QString randstr;//随机数
    int gametimerID;//游戏定时器
    int wintimerID;//胜利定时器
    int losetimerID;//失败定时器
    //使用字符串能够在游戏界面按下数字键时直接以字符串的形式连接起来
    //比较的时候因为随机数和结果都是字符串形式可以直接进行比较
    QString resultstr;//猜测的数字

    QMovie winmovie;//胜利时动画
    QMovie losemovie;//失败时动画
};

#endif // GUESSNUM_H

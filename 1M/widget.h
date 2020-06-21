#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


void show1(double price,double year,double area,double in,double pre);
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool line(int flage); //判断输入框中是否输入数据，是否合理
    bool button(int flage);//判断还款方式按钮是否点击
    bool isnumber(double num);

private slots:
    void on_Cway_currentIndexChanged(int index);//实现combox条形框内不同的内容与不同界面的对应
    void paintEvent(QPaintEvent *event);
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H

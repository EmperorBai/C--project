#include "widget.h"
#include "ui_widget.h"
#include<cmath>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include<QListWidget>
#include<QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    //固定窗口大小及标题
    setFixedSize(828,642);
    setWindowTitle("房贷计算器");

    //清空重填
    connect(ui->btn_clear1,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"提示信息","您真的确认清空数据吗",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        {ui->ln_lilv1->clear();
            ui->ln_price->clear();
            ui->ln_area->clear();
            ui->show->clear();
        };
    });
    connect(ui->btn_clear2,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"提示信息","您真的确认清空数据吗",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        {ui->ln_total->clear();
        ui->ln_lilv2->clear();
        ui->show->clear();
        }
    });
    connect(ui->btn_clear3,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"提示信息","您真的确认清空数据吗",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        {ui->ln_loan1->clear();
        ui->ln_loan2->clear();
        ui->ln_interest1->clear();
        ui->ln_interest2->clear();
        ui->show->clear();
        }
    });

    //规定初始界面

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);

    //进行按钮与界面之间的连接
    connect(ui->btn_way1,&QRadioButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->btn_way2,&QRadioButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->btn_way3,&QRadioButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });

    //限制输入范围
    ui->ln_lilv1->setValidator(new QDoubleValidator(0,100,2,ui->ln_lilv1));
    ui->ln_lilv2->setValidator(new QDoubleValidator(0,100,2,ui->ln_lilv2));
    ui->ln_interest1->setValidator(new QDoubleValidator(0,100,2,ui->ln_interest1));
    ui->ln_interest2->setValidator(new QDoubleValidator(0,100,2,ui->ln_interest2));
    ui->ln_area->setValidator(new QIntValidator(0,1000000000,ui->ln_area));
    ui->ln_loan1->setValidator(new QDoubleValidator(0,1000000000,2,ui->ln_loan1));
    ui->ln_loan2->setValidator(new QDoubleValidator(0,1000000000,2,ui->ln_loan2));
    ui->ln_total->setValidator(new QDoubleValidator(0,1000000000,2,ui->ln_total));
    ui->ln_price->setValidator(new QDoubleValidator(0,1000000000,2,ui->ln_price));

    //按揭年数与成数范围
    for(double i=8;i>=2;i-=0.5)
    {
        ui->cob_per->addItem(QString::number(i),"成");
    }
    for(int i=25;i>=1;i--)
    {
        ui->cob_year1->addItem(QString::number(i),"年");
        ui->cob_year2->addItem(QString::number(i),"年");
        ui->ln_year3->addItem(QString::number(i),"年");
    }

    //根据面积和单价计算（商业与公积金）
    connect(ui->btn_xi1,&QRadioButton::clicked,[=](){
                                                         //等额本息
        connect(ui->btn_cal1,&QPushButton::clicked,[=](){
            double p=ui->ln_price->text().toDouble();            //单价
            double a=ui->ln_area->text().toDouble();             //面积
            double per=ui->cob_per->currentText().toDouble();      //成数
            double y=ui->cob_year1->currentText().toDouble()*12; //年数
            double in=ui->ln_lilv1->text().toDouble()/1200.0;     //利

            if(line(1)){
                     QMessageBox::critical(this,"警告","有信息未输入");                 

            }else{

                double benjin=p*a*per/10;
                double d=pow((1.0+in),y);
                double hejin=(benjin*in*d*y)/(d-1.0);
                double shoufu=a*p-benjin;
                double lixi=hejin-benjin;
                double month=hejin/y;
                ui->show->clear();
                QString str,str1,str2,str3,str4,str5,str6;


                 str1=QString("%1 %2 %3").arg("首付: ").arg(shoufu,0,'f',0).arg("元");
                 str2=QString("%1 %2 %3").arg("每月月供: ").arg(month,0,'f',0).arg("元");
                 str3=QString("%1 %2 %3").arg("贷款总额: ").arg(benjin,0,'f',0).arg("元");
                 str4=QString("%1 %2 %3").arg("支付利息: ").arg(lixi,0,'f',0).arg("元");
                 str6=QString("%1 %2 %3").arg("还款月数: ").arg(y).arg("个月");
                 str5=QString("%1 %2 %3").arg("还款总额: ").arg(hejin,0,'f',0).arg("元");

                ui->show->addItem(str1);
                ui->show->addItem(str2);
                ui->show->addItem(str3);
                ui->show->addItem(str4);
                ui->show->addItem(str5);
                ui->show->addItem(str6);

            };

        });
    });

    connect(ui->btn_jin1,&QRadioButton::clicked,[=]()
    {
        connect(ui->btn_cal1,&QPushButton::clicked,[=]()          //等额本金
        {
            double p=ui->ln_price->text().toDouble();//单价
            double a=ui->ln_area->text().toDouble();//面积
            double per=ui->cob_per->currentText().toDouble();//成数
            double y=ui->cob_year1->currentText().toDouble()*12;//年数换月
            double in=ui->ln_lilv1->text().toDouble()/1200.0;//利率

            if(line(1)){

                 QMessageBox::critical(this,"警告","有信息未输入");            
            }
            else{
            double benjin=p*a*per/10;//贷款本金
            double month=(benjin/y)+benjin*in;//首月月供
            double reduce=benjin/y*in;//每月月供递减额
            double lixi=((benjin/y+benjin*in)+benjin/y*(1+in))/2*y-benjin;
            double hejin=(2*month-(y-1)*reduce)*y/2.0;
            double shoufu=a*p-benjin;
            ui->show->clear();
            QString str1,str2,str3,str4,str5,str6,str7;



             str1=QString("%1 %2 %3").arg("首付: ").arg(shoufu,0,'f',0).arg("元");
             str2=QString("%1 %2 %3").arg("每月月供: ").arg(month,0,'f',0).arg("元");
             str3=QString("%1 %2 %3").arg("每月递减: ").arg(reduce,0,'f',0).arg("元");
             str4=QString("%1 %2 %3").arg("贷款总额: ").arg(benjin,0,'f',0).arg("元");
             str5=QString("%1 %2 %3").arg("支付利息: ").arg(lixi,0,'f',0).arg("元");
             str6=QString("%1 %2 %3").arg("还款总额: ").arg(hejin,0,'f',0).arg("元");
             str7=QString("%1 %2 %3").arg("还款月数: ").arg(y).arg("月");


            ui->show->addItem(str1);
            ui->show->addItem(str2);
            ui->show->addItem(str3);
            ui->show->addItem(str4);
            ui->show->addItem(str5);
            ui->show->addItem(str6);
            ui->show->addItem(str7);
            };
        });
    });

    //根据贷款总额计算（商业与公积金）
    connect(ui->btn_xi2,&QRadioButton::clicked,[=]()        //等额本息
    {
        connect(ui->btn_cal2,&QPushButton::clicked,[=]()
        {
        double benjin=ui->ln_total->text().toDouble()*10000;//贷款总额
        double y=ui->cob_year2->currentText().toDouble()*12;//年数换月
        double in=ui->ln_lilv2->text().toDouble()/1200.0;//月利率

        if(line(2)){
            QMessageBox::critical(this,"警告","有信息未输入");

        }
        else{
        double r=pow((1.0+in),y);
        double hejin=(benjin*in*r*y)/(r-1.0);
        double lixi=hejin-benjin;
        double month=(benjin*in*r)/(r-1.0);
        ui->show->clear();
        QString str,str1,str2,str3,str4,str5;

         str1=QString("%1 %2 %3").arg("每月月供: ").arg(month,0,'f',0).arg("元");
         str2=QString("%1 %2 %3").arg("贷款总额: ").arg(benjin,0,'f',0).arg("元");
         str3=QString("%1 %2 %3").arg("支付利息: ").arg(lixi,0,'f',0).arg("元");
         str4=QString("%1 %2 %3").arg("还款总额: ").arg(hejin,0,'f',0).arg("元");
         str5=QString("%1 %2 %3").arg("还款月数: ").arg(y).arg("个月");
        ui->show->addItem(str1);
        ui->show->addItem(str2);
        ui->show->addItem(str3);
        ui->show->addItem(str4);
        ui->show->addItem(str5);
        };

        });
    });

    connect(ui->btn_jin2,&QRadioButton::clicked,[=]()        //等额本金
    {
        connect(ui->btn_cal2,&QPushButton::clicked,[=]()
        {
            double benjin=ui->ln_total->text().toDouble()*10000;//贷款总额
            double y=ui->cob_year2->currentText().toDouble()*12;//年数换月
            double in=ui->ln_lilv2->text().toDouble()/1200.0;//月利率

            if(line(2)){
                QMessageBox::critical(this,"警告","有信息未输入");

            }
            else{
            double month=(benjin/y)+benjin*in;//首月月供
            double reduce=benjin/y*in;//每月月供递减额
            double lixi=((benjin/y+benjin*in)+benjin/y*(1+in))/2*y-benjin;
            double hejin=(2*month-(y-1)*reduce)*y/2.0;
            ui->show->clear();

            QString str,str1,str2,str3,str4,str5,str6;

             str1=QString("%1 %2 %3").arg("每月月供: ").arg(month,0,'f',0).arg("元");
             str6=QString("%1 %2 %3").arg("每月递减: ").arg(reduce,0,'f',0).arg("元");
             str2=QString("%1 %2 %3").arg("贷款总额: ").arg(benjin,0,'f',0).arg("元");
             str3=QString("%1 %2 %3").arg("支付利息: ").arg(lixi,0,'f',0).arg("元");
             str4=QString("%1 %2 %3").arg("还款总额: ").arg(hejin,0,'f',0).arg("元");
             str5=QString("%1 %2 %3").arg("还款月数: ").arg(y).arg("个月");
            ui->show->addItem(str1);
            ui->show->addItem(str6);
            ui->show->addItem(str2);
            ui->show->addItem(str3);
            ui->show->addItem(str4);
            ui->show->addItem(str5);
        };

        });
    });

    //组合型贷款

    connect(ui->btn_xi3,&QRadioButton::clicked,[=]()           //等额本息
    {
        connect(ui->btn_cal3,&QPushButton::clicked,[=]()
        {
        double benjin1=ui->ln_loan1->text().toDouble()*10000;//商业贷款额
        double in1=ui->ln_interest1->text().toDouble()/1200.0;//商业利率
        double benjin2=ui->ln_loan2->text().toDouble()*10000;//公积金贷款额
        double in2=ui->ln_interest2->text().toDouble()/1200.0;//公积金利率
        double y=ui->ln_year3->currentText().toDouble()*12;//年数换月

        if(line(3)){

             QMessageBox::critical(this,"警告","有信息未输入");

        }        else{
        double r1=pow((1.0+in1),y);
        double henjin1=(benjin1*in1*r1*y)/(r1-1.0);
        double lixi1=henjin1-benjin1;
        double month1=(benjin1*in1*r1)/(r1-1.0);
        double r2=pow((1.0+in2),y);
        double hejin2=(benjin2*in2*r2*y)/(r2-1.0);
        double lixi2=hejin2-benjin2;
        double month2=(benjin2*in2*r1)/(r2-1.0);
        double hejin=henjin1+hejin2;
        double benjin=benjin1+benjin2;
        double lixi=lixi1+lixi2;
        double month=month1+month2;
        ui->show->clear();
        QString str1,str2,str3,str4,str5;



         str1=QString("%1 %2 %3").arg("每月月供: ").arg(month,0,'f',0).arg("元");
         str2=QString("%1 %2 %3").arg("贷款总额: ").arg(benjin,0,'f',0).arg("元");
         str3=QString("%1 %2 %3").arg("支付利息: ").arg(lixi,0,'f',0).arg("元");
         str4=QString("%1 %2 %3").arg("还款总额: ").arg(hejin,0,'f',0).arg("元");
         str5=QString("%1 %2 %3").arg("还款月数: ").arg(y).arg("个月");

        ui->show->addItem(str1);
        ui->show->addItem(str2);
        ui->show->addItem(str3);
        ui->show->addItem(str4);
        ui->show->addItem(str5);
        };

        });
    });

    connect(ui->btn_jin3,&QRadioButton::clicked,[=]()           //等额本金
    {
        connect(ui->btn_cal3,&QPushButton::clicked,[=]()
        {
            double benjin1=ui->ln_loan1->text().toDouble()*10000;//商业贷款额
            double in1=ui->ln_interest1->text().toDouble()/1200.0;//商业利率
            double benjin2=ui->ln_loan2->text().toDouble()*10000;//公积金贷款额
            double in2=ui->ln_interest2->text().toDouble()/1200.0;//公积金利率
            double y=ui->ln_year3->currentText().toDouble()*12;//年数换月

            if(line(3)&&button(3)){

                 QMessageBox::critical(this,"警告","有信息未输入");

            }
            else{
            double month1=(benjin1/y)+benjin1*in1;//首月月供
            double reduce1=benjin1/y*in1;//每月月供递减额
            double lixi1=((benjin1/y+benjin1*in1)+benjin1/y*(1+in1))/2*y-benjin1;
            double hejin1=(2*month1-(y-1)*reduce1)*y/2.0;
            double month2=(benjin2/y)+benjin2*in2;//首月月供
            double reduce2=benjin2/y*in2;//每月月供递减额
            double lixi2=((benjin2/y+benjin2*in2)+benjin2/y*(1+in2))/2*y-benjin2;
            double hejin2=(2*month2-(y-1)*reduce2)*y/2.0;
            double month=month1+month2;//每月月供
            double reduce=reduce1+reduce2;
            double lixi=lixi1+lixi2;
            double hejin=hejin1+hejin2;
            double benjin=benjin1+benjin2;
            ui->show->clear();
            QString str1,str2,str3,str4,str5,str6;



             str1=QString("%1 %2 %3").arg("每月月供: ").arg(month,0,'f',0).arg("元");
             str6=QString("%1 %2 %3").arg("每月递减: ").arg(reduce,0,'f',0).arg("元");
             str2=QString("%1 %2 %3").arg("贷款总额: ").arg(benjin,0,'f',0).arg("元");
             str3=QString("%1 %2 %3").arg("支付利息: ").arg(lixi,0,'f',0).arg("元");
             str4=QString("%1 %2 %3").arg("还款总额: ").arg(hejin,0,'f',0).arg("元");
             str5=QString("%1 %2 %3").arg("还款月数: ").arg(y).arg("个月");

            ui->show->addItem(str1);
            ui->show->addItem(str6);
            ui->show->addItem(str2);
            ui->show->addItem(str3);
            ui->show->addItem(str4);
            ui->show->addItem(str5);
            };

        });
    });




}

Widget::~Widget()
{
    delete ui;
}

//界面美化
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap m;
    m.load(":/picture/text");
    painter.drawPixmap(0,0,this->width(),this->height(),m);
}

bool Widget :: line(int flage){
    if(flage == 1){
            if(ui->ln_price->text().isEmpty()||ui->ln_area->text().isEmpty()||ui->ln_lilv1->text().isEmpty())
                return true;
            else
                return false;
        }
    else  if(flage == 2){
            if(ui->ln_total->text().isEmpty()||ui->ln_lilv2->text().isEmpty())
                return true;
            else
                return false;
        }
    else if(flage == 3){
            if(ui->ln_loan1->text().isEmpty()||ui->ln_loan2->text().isEmpty()||ui->ln_interest1->text().isEmpty()||ui->ln_interest2->text().isEmpty())
                return true;
            else
                return false;
        }
};

bool Widget :: isnumber(double num){
    QString str = QString::number(num,'f',2);
    for(int i=0; i<str.size();i++){
        if((str.at(i)>'9')||(str.at(i)<'0'))
        {
            return true;
        }
    }
    return false;
}

bool Widget :: button(int flage){

        if(flage == 1){
                if(ui->btn_xi1->isChecked()||ui->btn_jin1->isChecked())
                    return false;
                else
                    return true;
            }
        else  if(flage == 2){
                if(ui->btn_xi2->isChecked()||ui->btn_jin2->isChecked())
                    return false;
                else
                    return true;
            }
        else if(flage == 3){
                if(ui->btn_xi3->isChecked()||ui->btn_jin3->isChecked())
                    return false;
                else
                    return true;
            }

};

void Widget :: on_Cway_currentIndexChanged(int index)
{
    if(index==0) ui->stackedWidget_2->setCurrentIndex(0);
    if(index==1) ui->stackedWidget_2->setCurrentIndex(1);
}

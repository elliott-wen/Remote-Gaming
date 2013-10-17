#include "cloudportal.h"
#include "ui_cloudportal.h"

CloudPortal::CloudPortal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloudPortal)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(ui->gamelistWidget,SIGNAL(currentRowChanged(int)),this,SLOT(onGameChanged(int)));
    connect(ui->playpushButton,SIGNAL(clicked()),this,SLOT(onGamePlay()));
}

CloudPortal::~CloudPortal()
{
    delete ui;
}
void CloudPortal::onGamePlay()
{
    if(ui->gamelistWidget->currentRow()==-1)
    {
        QMessageBox::warning(this,"Please choose a game","Please choose a game");
        return;
    }
    ui->playpushButton->setEnabled(false);
    QString qurl=QString("http://222.200.182.75:8090/play?quality=%1&programId=%2").arg(ui->qualitycomboBox->currentIndex()+1).arg(ui->gamelistWidget->currentRow()+1);
    manager->get(QNetworkRequest(QUrl(qurl)));
    qDebug()<<qurl;
}

void CloudPortal::onGameChanged(int row)
{

    if(row==0)
    {
        ui->gameTextEdit->setPlainText("Dota is a funny game");
        QPixmap pixmap(":/img/dota.jpg");
        ui->imglabel_2->setPixmap(pixmap);
    }
    else if(row==1)
    {
        ui->gameTextEdit->setPlainText("Directx Example");
        QPixmap pixmap(":/img/directx.jpg");
        ui->imglabel_2->setPixmap(pixmap);
    }
}
void CloudPortal::replyFinished(QNetworkReply *reply)  //当回复结束后
{

     QJsonDocument doc=QJsonDocument::fromJson(reply->readAll());
     QJsonObject obj=doc.object();
     bool successful=obj["successful"].toBool();
     if(successful)
     {
         QProcess proc;
         QStringList arguments;
         arguments << "-a"<<obj["serverIp"].toString()<<"-p"<<QString("%1").arg(obj["port"].toDouble())<<"-q"<<QString("%1").arg(ui->qualitycomboBox->currentIndex()+1);
         qDebug()<<arguments;
         proc.start("client.exe", arguments);
         if (!proc.waitForStarted())
         {
            QMessageBox::warning(this,"Error","Fail to open client core");
         }
         else
         {
            this->hide();
            proc.closeWriteChannel();
            proc.waitForFinished(-1);
            int exitcode=proc.exitCode();
            this->show();
            qDebug()<<exitcode;
            QMessageBox::about(this,"Debug",QString("%1").arg(exitcode));
            if(exitcode==TUNNELBROKENERROR)
            {
                QMessageBox::warning(this,"Error","Lost connection with server");

            }
            else if(exitcode==SHUTDOWNSIGNAL)
            {
                  QMessageBox::about(this,"Finish","Game application closed properly");
            }
            else if(exitcode==0)
            {

            }
            else
            {
                QMessageBox::warning(this,"Error","Error when starting gaming engine");
            }
         }


     }
     else
     {
         QMessageBox::warning(this,"Error","Cloud is not ready yet");
     }
    ui->playpushButton->setEnabled(true);
    reply->deleteLater();   //最后要释放reply对象
}

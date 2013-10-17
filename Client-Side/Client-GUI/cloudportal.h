#ifndef CLOUDPORTAL_H
#define CLOUDPORTAL_H

#include <QDialog>
#include <QtNetwork>
#include <QJsonDocument>
#include <QMessageBox>
#define INITVIDEOERROR -10
#define INITAUDIOERROR -11
#define INITNETWORKERROR -12
#define INITDECODERERROR -13
#define INITCONTROLERERROR -14
#define TUNNELBROKENERROR -20
#define SHUTDOWNSIGNAL -21
#define TUNNELINITERROR -22
namespace Ui {
class CloudPortal;
}

class CloudPortal : public QDialog
{
    Q_OBJECT
    
public:
    explicit CloudPortal(QWidget *parent = 0);
    ~CloudPortal();
    
private:
    Ui::CloudPortal *ui;
    QNetworkAccessManager *manager;

private slots:
    void onGameChanged(int);
    void onGamePlay();
    void replyFinished(QNetworkReply *);

};

#endif // CLOUDPORTAL_H

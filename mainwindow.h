#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
     void TableWidgetDisplay(QNetworkReply * reply) ;
     void getWholeList(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager ;
};

#endif // MAINWINDOW_H

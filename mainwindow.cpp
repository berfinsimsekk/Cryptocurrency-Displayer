#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QRegExp>
#include <QFile>
#include <QStringList>

int numberOfLines=0;
QStringList coinName; // tells the coinName index by index. For example, coinName[0] gives name of the coin at 0th row.
QTableWidget *table;
int row=0;
QString WholeList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this) ;

    QString path=qgetenv("MYCRYPTOCONVERT"); // reads the environment variable

    QNetworkAccessManager* manager2=new QNetworkAccessManager(this);
     connect(manager2, SIGNAL(finished(QNetworkReply*)),this, SLOT(getWholeList(QNetworkReply*)));
    manager2->get(QNetworkRequest(QUrl("https://api.coingecko.com/api/v3/coins/list")));

    // reads the input file to know numberOfLines in the file.

       QFile inputFile(path);
       if (inputFile.open(QIODevice::ReadOnly))
       {
          QTextStream in(&inputFile);
          while (!in.atEnd())
          {
             QString line = in.readLine(); // the get current line.

             QString pattern = "\"symbol\":\""+line+"\",\"name\":\"(.*)\"";
                       QString pattern2 = "\"symbol\":\"(.*)\",\"name\":\""  +line+  "\"";

                       QRegExp rx(pattern);
                       if ( rx.indexIn(WholeList, 0) != -1 ) {
                           line=rx.cap(2);
                       }
                       QRegExp rx2(pattern2);
                       if( rx2.indexIn(WholeList,0)!=-1){
                           line=rx2.cap(2);
                       }


             coinName.append(line); // add the name of the coin into string array
             numberOfLines++; // increase the numberOfLines by 1
          }
          QString line = in.readLine(); // the get current line.
          coinName.append(line); // add the name of the coin into string array
          numberOfLines++; // increase the numberOfLines by 1

       }



       inputFile.close();


       table = new QTableWidget(this); // create the table
       table->setColumnCount(4); // number of columns is always 4

       // put the labels USD, EUR, GDP
       QStringList first;
       first << " "<<"USD"<<"EUR"<<"GBP";
       table->setHorizontalHeaderLabels(first);
       table->verticalHeader()->hide();
       this->setCentralWidget(table);


 connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(TableWidgetDisplay(QNetworkReply*)));
 manager->get(QNetworkRequest(QUrl("https://api.coingecko.com/api/v3/simple/price?ids="+coinName[0]+"&vs_currencies=usd,eur,gbp")));

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getWholeList(QNetworkReply *reply){
    WholeList=(QString) reply->readAll();
}


// every time this slot is triggered, a new row will be created.
void MainWindow::TableWidgetDisplay(QNetworkReply *reply){

    // read the data fetched from the web site
    QString data = (QString) reply->readAll();

    // use pattern matching to extract the rate
   QRegExp rx("\"usd\":(\\d+.?\\d*),\"eur\":(\\d+.?\\d*),\"gbp\":(\\d+.?\\d*)..");

        QTableWidgetItem *item;
        QTableWidgetItem *coin;
        coin = new QTableWidgetItem;

        table->insertRow(row);

        // put the very first cell name of the coin
        coin->setText(coinName[row]);
        table->setItem(row,0,coin);

        for(int column=1; column<4;column++){
            item =new QTableWidgetItem; // create a new cell

            int pos=0; // tells at which index to start to read data.
            // Since pos is 0, it will search starting from 0th index.

            if ( rx.indexIn(data, pos) != -1 ) {
                // given the data, try to match it with rx pattern, starting searching from pos index.
                // if it finds it successfully, rx.cap(1) gives USD, rx.cap(2) gives EUR, rx.cap(3) gives GBP

               item->setText(rx.cap(column)); // sets correspoding numerical value of USD,EUR or GDP value.
            }

            else{
                item->setText("Error"); // pattern could not be found.
            }

            table->setItem(row,column,item); // adds the table correspoding numerical value of USD,EUR or GDP value

        }



    row++;
    if(row<coinName.size()-1) manager->get(QNetworkRequest(QUrl("https://api.coingecko.com/api/v3/simple/price?ids="+coinName[row]+"&vs_currencies=usd,eur,gbp")));
}

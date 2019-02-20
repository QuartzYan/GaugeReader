#include "src/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  _gr = new GaugeReader();

  ui->graphicsView->setScene(new QGraphicsScene(this));
  ui->graphicsView->scene()->addItem(&pixmap);
  ui->graphicsView_2->setScene(new QGraphicsScene(this));
  ui->graphicsView_2->scene()->addItem(&pixmap_2);
  ui->graphicsView_3->setScene(new QGraphicsScene(this));
  ui->graphicsView_3->scene()->addItem(&pixmap_3);

  ui->lcdNumber->setMode(QLCDNumber::Dec);
  ui->lcdNumber->setStyleSheet("border: 1px solid green; color: green; background: silver;");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  _gr->readTemplateImage("11.jpg");
  v1showImage(_gr->templateQImage);
}

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{
  _gr->readSrcImage("111.jpg");
  v2showImage(_gr->srcQImage);
}

void MainWindow::on_pushButton_4_clicked()
{

}

void MainWindow::v1showImage(QImage img)
{
  if(img.isNull())
  {
    qDebug() << "image is null!!";
  }
  else
  {
    pixmap.setPixmap( QPixmap::fromImage(img.rgbSwapped()));
    //ui->graphicsView->resize(qimg.width() + 10, qimg.height() + 10);
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
  }
}

void MainWindow::v2showImage(QImage img)
{
  if(img.isNull())
  {
    qDebug() << "image is null!!";
  }
  else
  {
    pixmap_2.setPixmap( QPixmap::fromImage(img.rgbSwapped()));
    //ui->graphicsView->resize(qimg.width() + 10, qimg.height() + 10);
    ui->graphicsView_2->fitInView(&pixmap_2, Qt::KeepAspectRatio);
  }
}

void MainWindow::v3showImage(QImage img)
{
  if(img.isNull())
  {
    qDebug() << "v3 image is null!!";
  }
  else
  {
    pixmap_3.setPixmap( QPixmap::fromImage(img.rgbSwapped()));
    //ui->graphicsView->resize(qimg.width() + 10, qimg.height() + 10);
    ui->graphicsView_3->fitInView(&pixmap_3, Qt::KeepAspectRatio);
  }
}

void MainWindow::on_matchButton_clicked()
{
  _gr->matchImage();
  v3showImage(_gr->showQImage);
}

void MainWindow::on_adjustButton_clicked()
{
  _gr->adjustImage();
  v3showImage(_gr->showQImage);
}

void MainWindow::on_filterButton_clicked()
{
  _gr->filterImage(ui->filterParam_1->value(), ui->filterParam_2->value(), ui->filterParam_3->value());
  v3showImage(_gr->showQImage);
}

void MainWindow::on_cannyButton_clicked()
{
  _gr->cannyImage(ui->cannyParam_1->value(), ui->cannyParam_2->value(), ui->cannyParam_3->value());
  v3showImage(_gr->showQImage);
}

void MainWindow::on_circleButton_clicked()
{
  _gr->circleImage(ui->circleParam_1->value(), ui->circleParam_2->value(), ui->circleParam_3->value());
  v3showImage(_gr->showQImage);
}

void MainWindow::on_lineButton_clicked()
{
  _gr->lineImage(ui->lineParam_1->value(), ui->lineParam_2->value(), ui->lineParam_3->value());
  v3showImage(_gr->showQImage);

  ui->lcdNumber->display(_gr->getResult());
}

void MainWindow::on_matchParam_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->matchParamLabel->setText(str);
}

void MainWindow::on_adjustParam_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->adjustParamLabel->setText(str);
}

void MainWindow::on_filterParam_1_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->filterParamLabel_1->setText(str);
}

void MainWindow::on_filterParam_2_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->filterParamLabel_2->setText(str);
}

void MainWindow::on_filterParam_3_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->filterParamLabel_3->setText(str);
}

void MainWindow::on_cannyParam_1_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->cannyParamLabel_1->setText(str);
}

void MainWindow::on_cannyParam_2_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->cannyParamLabel_2->setText(str);
}

void MainWindow::on_cannyParam_3_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->cannyParamLabel_3->setText(str);
}

void MainWindow::on_circleParam_1_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->circleParamLabel_1->setText(str);
}

void MainWindow::on_circleParam_2_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->circleParamLabel_2->setText(str);
}

void MainWindow::on_circleParam_3_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->circleParamLabel_3->setText(str);
}

void MainWindow::on_lineParam_1_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->lineParamLabel_1->setText(str);
}

void MainWindow::on_lineParam_2_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->lineParamLabel_2->setText(str);
}

void MainWindow::on_lineParam_3_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->lineParamLabel_3->setText(str);
}

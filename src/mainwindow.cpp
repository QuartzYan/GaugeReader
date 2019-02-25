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
  QString filePath = QFileDialog::getOpenFileName(this, "Select template Image",
                                                  ".",
                                                  "JPEG Files(*.jpg);;PNG Files(*.png)",
                                                  0,
                                                  0);
  _gr->readTemplateImage(filePath);
  v1showImage(_gr->templateQImage);
}

void MainWindow::on_pushButton_2_clicked()//save config
{
  QString filePath = QFileDialog::getSaveFileName(this, "Save Config Files",
                                                  ".",
                                                  "CONFIG Files(*.ini)",
                                                  0,
                                                  0);
  QSettings *configIniWrite = new QSettings(filePath, QSettings::IniFormat);
  configIniWrite->setValue("gauge/startAngle", ui->lineEdit->text());
  configIniWrite->setValue("gauge/endAngle", ui->lineEdit_2->text());
  configIniWrite->setValue("gauge/maxVel", ui->lineEdit_3->text());
  configIniWrite->setValue("gauge/minvel", ui->lineEdit_4->text());
  configIniWrite->setValue("gauge/resolution", ui->lineEdit_5->text());
  configIniWrite->setValue("gauge/unit", ui->lineEdit_6->text());

  configIniWrite->setValue("image/filterPamer1", ui->filterParam_1->value());
  configIniWrite->setValue("image/filterPamer2", ui->filterParam_2->value());
  configIniWrite->setValue("image/filterPamer3", ui->filterParam_3->value());
  configIniWrite->setValue("image/thresholdParam", ui->thresholdParam->value());
  configIniWrite->setValue("image/cannyPamer1", ui->cannyParam_1->value());
  configIniWrite->setValue("image/cannyPamer2", ui->cannyParam_2->value());
  configIniWrite->setValue("image/cannyPamer3", ui->cannyParam_3->value());
  configIniWrite->setValue("image/circlePamer1", ui->circleParam_1->value());
  configIniWrite->setValue("image/circlePamer2", ui->circleParam_2->value());
  configIniWrite->setValue("image/circlePamer3", ui->circleParam_3->value());
  configIniWrite->setValue("image/linePamer1", ui->lineParam_1->value());
  configIniWrite->setValue("image/linePamer2", ui->lineParam_2->value());
  configIniWrite->setValue("image/linePamer3", ui->lineParam_3->value());

  //configIniWrite->setValue("IP", ui->lineEdit_2->text());
  delete configIniWrite;
}

void MainWindow::on_pushButton_5_clicked()//load config
{
  QString filePath = QFileDialog::getOpenFileName(this, "Select Config Files",
                                                  ".",
                                                  "CONFIG Files(*.ini)",
                                                  0,
                                                  0);
  QSettings *configIniRead = new QSettings(filePath, QSettings::IniFormat);
  ui->lineEdit->setText(configIniRead->value("gauge/startAngle").toString());
  ui->lineEdit_2->setText(configIniRead->value("gauge/endAngle").toString());
  ui->lineEdit_3->setText(configIniRead->value("gauge/maxVel").toString());
  ui->lineEdit_4->setText(configIniRead->value("gauge/minvel").toString());
  ui->lineEdit_5->setText(configIniRead->value("gauge/resolution").toString());
  ui->lineEdit_6->setText(configIniRead->value("gauge/unit").toString());

  ui->filterParam_1->setValue(configIniRead->value("image/filterPamer1").toInt());
  ui->filterParam_2->setValue(configIniRead->value("image/filterPamer2").toInt());
  ui->filterParam_3->setValue(configIniRead->value("image/filterPamer3").toInt());
  ui->thresholdParam->setValue(configIniRead->value("image/thresholdParam").toInt());
  ui->cannyParam_1->setValue(configIniRead->value("image/cannyPamer1").toInt());
  ui->cannyParam_2->setValue(configIniRead->value("image/cannyPamer2").toInt());
  ui->cannyParam_3->setValue(configIniRead->value("image/cannyPamer3").toInt());
  ui->circleParam_1->setValue(configIniRead->value("image/circlePamer1").toInt());
  ui->circleParam_2->setValue(configIniRead->value("image/circlePamer2").toInt());
  ui->circleParam_3->setValue(configIniRead->value("image/circlePamer3").toInt());
  ui->lineParam_1->setValue(configIniRead->value("image/linePamer1").toInt());
  ui->lineParam_2->setValue(configIniRead->value("image/linePamer2").toInt());
  ui->lineParam_3->setValue(configIniRead->value("image/linePamer3").toInt());

  delete configIniRead;
}

void MainWindow::on_pushButton_3_clicked()
{
  QString filePath = QFileDialog::getOpenFileName(this, "Select Src Image",
                                                  ".",
                                                  "JPEG Files(*.jpg);;PNG Files(*.png)",
                                                  0,
                                                  0);
  _gr->readSrcImage(filePath);
  v2showImage(_gr->srcQImage);
}

void MainWindow::on_pushButton_4_clicked()
{

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
  _gr->filterImage(ui->filterParam_1->value(), ui->filterParam_2->value(), ui->filterParam_3->value(), ui->thresholdParam->value());
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

  float d = (ui->lineEdit_3->text().toFloat() - ui->lineEdit_4->text().toFloat())/(ui->lineEdit_2->text().toFloat() - ui->lineEdit->text().toFloat());
  float a = _gr->getResult() - ui->lineEdit->text().toFloat();

  ui->lcdNumber->display(a * d + ui->lineEdit_4->text().toFloat());
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


void MainWindow::on_thresholdParam_valueChanged(int value)
{
  QString str = QString("%1").arg(value);
  ui->thresholdLabel->setText(str);
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

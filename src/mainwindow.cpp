#include "src/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->graphicsView->setScene(new QGraphicsScene(this));
  ui->graphicsView->scene()->addItem(&pixmap);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  cv::Mat templateImage = cv::imread("11.jpg");
  //判断图像是否加载成功
  if(templateImage.empty())
    qDebug() << "模板图像加载失败!";
  else
    qDebug() << "模板图像加载成功!" ;

  cv::resize(templateImage, templateImage, cv::Size(), 0.2, 0.2);

  QImage qimg(templateImage.data, templateImage.cols, templateImage.rows, templateImage.step, QImage::Format_RGB888);
  pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()));
  //ui->graphicsView->resize(qimg.width() + 10, qimg.height() + 10);
  ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{
  cv::Mat srcImage = cv::imread("1.jpg");
  //判断图像是否加载成功
  if(srcImage.empty())
    qDebug() << "源图像加载失败!!";
  else
    qDebug() << "源图像加载成功!!";

  cv::resize(srcImage, srcImage, cv::Size(), 0.2, 0.2);

  QImage qimg(srcImage.data, srcImage.cols, srcImage.rows, srcImage.step, QImage::Format_RGB888);
  pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()));
  //ui->graphicsView->resize(qimg.width() + 10, qimg.height() + 10);
  ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::on_pushButton_4_clicked()
{

}

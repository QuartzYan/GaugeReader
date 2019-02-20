#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <opencv2/opencv.hpp>
#include "src/gaugereader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_4_clicked();

  void v1showImage(QImage img);
  void v2showImage(QImage img);
  void v3showImage(QImage img);

  void on_matchButton_clicked();
  void on_adjustButton_clicked();
  void on_filterButton_clicked();
  void on_cannyButton_clicked();
  void on_circleButton_clicked();
  void on_lineButton_clicked();

  void on_matchParam_valueChanged(int value);

  void on_adjustParam_valueChanged(int value);

  void on_filterParam_1_valueChanged(int value);

  void on_filterParam_2_valueChanged(int value);

  void on_filterParam_3_valueChanged(int value);

  void on_cannyParam_1_valueChanged(int value);

  void on_cannyParam_2_valueChanged(int value);

  void on_cannyParam_3_valueChanged(int value);

  void on_circleParam_1_valueChanged(int value);

  void on_circleParam_2_valueChanged(int value);

  void on_circleParam_3_valueChanged(int value);

  void on_lineParam_1_valueChanged(int value);

  void on_lineParam_2_valueChanged(int value);

  void on_lineParam_3_valueChanged(int value);

private:
  GaugeReader *_gr;
  Ui::MainWindow *ui;

  QGraphicsPixmapItem pixmap;
  QGraphicsPixmapItem pixmap_2;
  QGraphicsPixmapItem pixmap_3;
};

#endif // MAINWINDOW_H

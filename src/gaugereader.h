#ifndef GAUGEREADER_H
#define GAUGEREADER_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <vector>
#include <cmath>

class GaugeReader : public QObject
{
  Q_OBJECT

public:
  explicit GaugeReader(QObject *parent = nullptr);


  QImage srcQImage;
  QImage showQImage;
  QImage templateQImage;

  bool readSrcImage(QString fileName);
  bool readTemplateImage(QString fileName);

  void matchImage();
  void adjustImage();
  void filterImage(int param1, double param2, double param3, int param4);
  void cannyImage(double param1, double param2, int param3);
  void circleImage(int param1, double param2, double param3);
  void lineImage(double param1, double param2, double param3);
  float getResult();

  //void DialDetection();
  //void PointerDetection();
private:
  QImage cvMat2QImage(cv::Mat inputImage);
  float lineK(cv::Point line_point1, cv::Point line_point2);
  float lineB(cv::Point line_point1, cv::Point line_point2);
  float point2pointDis(cv::Point point1, cv::Point point2);
  float point2lineDis(cv::Point goal_point, cv::Point line_point1, cv::Point line_point2);
  cv::Point lineIntersectPoint(cv::Point line1_point1, cv::Point line1_point2, cv::Point line2_point1, cv::Point line2_point2);
  cv::Point lineIntersectPoint(float k1, float b1, float k2, float b2);

signals:

public slots:

private:
  cv::Mat srcImage;
  cv::Mat matchImg;
  cv::Mat adjustImg;
  cv::Mat filterImg;
  cv::Mat cannyImg;
  cv::Mat dilateImg;
  cv::Mat circleImg;
  cv::Mat lineImg;
  cv::Mat showImg;
  cv::Mat templateImage;
  cv::Point circlePoint;
  int circleRadiu;
  float result;
  cv::Point tipPoint;
};

#endif // GAUGEREADER_H

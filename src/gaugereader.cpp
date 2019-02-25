#include "gaugereader.h"
#include <QDebug>


GaugeReader::GaugeReader(QObject *parent) : QObject(parent)
{

}

void GaugeReader::matchImage()
{
  cv::Mat resultImage;

  //match
  cv::matchTemplate(srcImage, templateImage, resultImage, cv::TM_CCOEFF_NORMED);

  float minVal = 1;
  float maxVal = 0;
  cv::Point minLoc, maxLoc;

  //find best match
  int rows = resultImage.rows;
  int cols = resultImage.cols * resultImage.channels();
  for(int i = 0; i < rows; i++)
  {
    float* data = resultImage.ptr<float>(i);
    for(int j = 0; j < cols; j++)
    {
      if(data[j] > maxVal)
      {
        maxVal = data[j];
        maxLoc.x = j;
        maxLoc.y = i;
      }
      if(data[j] < minVal)
      {
        minVal = data[j];
        minLoc.x = i;
        minLoc.y = j;
      }
    }
  }

  //qDebug() << minVal << maxVal;
  //qDebug() << maxLoc.x << maxLoc.y ;

  //截取图像
  cv::Mat roi = srcImage(cv::Rect(maxLoc.x, maxLoc.y, templateImage.cols, templateImage.rows));
  matchImg = roi.clone();

  //show
  showQImage = cvMat2QImage(matchImg);
}

void GaugeReader::adjustImage()
{
  cv::Ptr<cv::Feature2D>featureDetector = cv::xfeatures2d::SURF::create(1600);

  //定义KeyPoint变量,特征点描述符
  std::vector<cv::KeyPoint>keyPoints1;
  std::vector<cv::KeyPoint>keyPoints2;
  cv::Mat description1;
  cv::Mat description2;

  //计算模板图的特征
  featureDetector->detect(templateImage, keyPoints1);
  featureDetector->compute(templateImage, keyPoints1, description1);
  //计算roi图的特征
  featureDetector->detect(matchImg, keyPoints2);
  featureDetector->compute(matchImg, keyPoints2, description2);

  //定义matcher
  cv::FlannBasedMatcher matcher;
  std::vector<cv::DMatch>matches;
  matcher.match(description2, description1, matches);

  double max_dist = 0; double min_dist = 100;
  for (int i = 0; i < matches.size(); i++)
  {
    double dist = matches[i].distance;
    if (dist < min_dist)
      min_dist = dist;
    if (dist > max_dist)
      max_dist = dist;
  }
  //cout << "最大距离：" << max_dist << endl;
  //cout << "最小距离：" << min_dist << endl;

  //筛选出较好的匹配点
  std::vector<cv::DMatch> good_matches;
  double dThreshold = 0.5;
  for(int i = 0; i < matches.size(); i++)
  {
    if (matches[i].distance < dThreshold * max_dist)
    {
      good_matches.push_back(matches[i]);
    }
  }

  //根据matches将特征点对齐,将坐标转换为float类型
  std::vector<cv::KeyPoint> R_keypoint01, R_keypoint02;
  for (int i = 0; i < good_matches.size(); i++)
  {
    R_keypoint01.push_back(keyPoints1[good_matches[i].trainIdx]);
    R_keypoint02.push_back(keyPoints2[good_matches[i].queryIdx]);
  }
  //坐标转换
  std::vector<cv::Point2f>p01, p02;
  for (int i = 0; i < good_matches.size(); i++)
  {
    p01.push_back(R_keypoint01[i].pt);
    p02.push_back(R_keypoint02[i].pt);
  }
  //查找坐标变换
  std::vector<uchar> RansacStatus;
  cv::Mat Fundamental = cv::findHomography(p02, p01, RansacStatus, CV_RANSAC);
  cv::warpPerspective(matchImg, adjustImg, Fundamental, cv::Size(matchImg.cols, matchImg.rows));

  //show
  showQImage = cvMat2QImage(adjustImg);
}

void GaugeReader::filterImage(int param1, double param2, double param3, int param4)
{
  cv::Mat gray;
  cv::cvtColor(adjustImg, gray, cv::COLOR_BGR2GRAY);
  cv::bilateralFilter(gray, filterImg, param1, param2, param3);

  cv::threshold(filterImg, filterImg, param4, 255, 0);//er zhi hua

  //show
  cv::cvtColor(filterImg, showImg, cv::COLOR_GRAY2BGR);
  showQImage = cvMat2QImage(showImg);
}

void GaugeReader::cannyImage(double param1, double param2, int param3)
{
  //filterImg = cv::imread("filter.jpg", 0);

  cv::Canny(filterImg, cannyImg, param1, param2, 3);

  //cv::imwrite("canny.jpg", cannyImg);

  if(param3 > 0)
  {
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(param3, param3));
    cv::dilate(cannyImg, dilateImg, element); //膨胀
    //cv::erode(cannyImg, dilateImg, element); //腐蚀
    //cv::imwrite("dilate.jpg", dilateImg);

    cv::cvtColor(dilateImg, showImg, cv::COLOR_GRAY2BGR);
    showQImage = cvMat2QImage(showImg);
  }
  else
  {
    //show
    cv::cvtColor(cannyImg, showImg, cv::COLOR_GRAY2BGR);
    showQImage = cvMat2QImage(showImg);
  }
}

void GaugeReader::circleImage(int param1, double param2, double param3)
{
  //cv::Mat dilateImg = cv::imread("dilate.jpg");
  circleImg = dilateImg.clone();
  cv::cvtColor(dilateImg, showImg, cv::COLOR_GRAY2BGR);

  std::vector<cv::Vec3f> circles;
  cv::HoughCircles(circleImg, circles, cv::HOUGH_GRADIENT, 1, param1, param2, param3, circleImg.rows/4, circleImg.rows/2);

  if(circles.size() > 0)
  {
    //show
    cv::Point centrt(0, 0);
    int r = 0;
    int num = 0;
    int xdown = circleImg.cols / 4;
    int xup = circleImg.cols / 4 * 3;
    int ydown = circleImg.rows / 4;
    int yup = circleImg.rows / 4 * 3;
    for (size_t i = 0; i < circles.size(); i++)
    {
      cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      if(((center.x > xdown)&&(center.x < xup))&&((center.y > ydown)&&(center.y < yup)))
      {
        //绘制圆心
        cv::circle(showImg, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
        //绘制圆轮廓
        //cv::circle(showImg, center, radius, cv::Scalar(155, 50, 255), 3, 8, 0);

        centrt.x += cvRound(circles[i][0]);
        centrt.y += cvRound(circles[i][1]);
        r += cvRound(circles[i][2]);
        num++;
      }
    }

    if(num > 0)
    {
      centrt.x = int(centrt.x / num);
      centrt.y = int(centrt.y / num);
      r = int(r / num);

      qDebug() << centrt.x << centrt.y << r << num;

      cv::circle(showImg, centrt, r, cv::Scalar(155, 50, 255), 3, 8, 0);
      cv::line(showImg, cv::Point(centrt.x, 0), cv::Point(centrt.x, showImg.rows), cv::Scalar(255, 255, 255), 8, 8, 0);
      cv::line(showImg, cv::Point(0, centrt.y), cv::Point(showImg.cols, centrt.y), cv::Scalar(255, 255, 255), 8, 8, 0);
      cv::circle(showImg, centrt, 3, cv::Scalar(0, 255, 0), -1, 8, 0);

      circlePoint = centrt;
      circleRadiu = r;
    }
    else
    {
      qDebug() << "no good circle!!!";
    }
  }
  else
  {
    qDebug() << "no circle!!!";
  }

  showQImage = cvMat2QImage(showImg);
}

void GaugeReader::lineImage(double param1, double param2, double param3)
{
  int d = int(circleRadiu * sin(3.1415926 / 4));
  cv::Mat roi = dilateImg(cv::Rect(circlePoint.x - d, circlePoint.y - d, 2*d, 2*d));
  lineImg = roi.clone();

  cv::cvtColor(lineImg, showImg, cv::COLOR_GRAY2BGR);
  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(lineImg, lines, 1, CV_PI / 180, param1, 0, 0);

  std::vector<cv::Vec4i> goodLines;
  if(lines.size() > 0)
  {
    for(size_t i = 0; i < lines.size(); i++)
    {
      cv::Vec4i l = lines[i];
      float ll = point2pointDis(cv::Point(l[0], l[1]), cv::Point(l[2], l[3]));
      float dd = point2lineDis(circlePoint, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]));

      cv::line(showImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);

      if(ll > param2 && dd < param3)
      {
        cv::line(showImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        goodLines.push_back(l);
      }
    }
    if(goodLines.size() > 0)
    {
      qDebug() << "line num:" << lines.size() << "\tgood line num:" << goodLines.size();

      float KMax = lineK(cv::Point(goodLines[0][0], goodLines[0][1]), cv::Point(goodLines[0][2], goodLines[0][3]));
      float KMin = KMax;
      for(size_t i = 0; i < goodLines.size(); i++)
      {
        cv::Vec4i l = goodLines[i];
        float K = lineK(cv::Point(l[0], l[1]), cv::Point(l[2], l[3]));
        if(K > KMax) KMax = K;
        if(K < KMin) KMin = K;
      }
      qDebug() << "KMax:" << KMax << "\t" << "KMin:" << KMin;
      std::vector<cv::Vec4i> lines1, lines2;
      float k_r = (KMax + KMin)/2;
      float K_l1 = 0.0; float B_l1 = 0.0;
      float K_l2 = 0.0; float B_l2 = 0.0;

      for(size_t i = 0; i < goodLines.size(); i++)
      {
        cv::Vec4i l = goodLines[i];
        float K = lineK(cv::Point(l[0], l[1]), cv::Point(l[2], l[3]));
        float B = lineB(cv::Point(l[0], l[1]), cv::Point(l[2], l[3]));
        if(K > k_r)
        {
          lines1.push_back(l);
          K_l1+=K;
          B_l1+=B;
          cv::line(showImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        }
        if(K < k_r)
        {
          lines2.push_back(l);
          K_l2+=K;
          B_l2+=B;
          cv::line(showImg, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
        }
      }
      if(lines1.size() > 0) { K_l1 = K_l1/lines1.size(); B_l1 = B_l1/lines1.size();}
      if(lines2.size() > 0) { K_l2 = K_l2/lines2.size(); B_l2 = B_l2/lines2.size();}

      float sK = (K_l1 + K_l2)/2;
      float sB = (B_l1 + B_l2)/2;

      cv::Point slp1 = cv::Point(0, sB);
      cv::Point slp2 = cv::Point(showImg.cols, (showImg.cols*sK+sB));
      cv::line(showImg, slp1, slp2, cv::Scalar(0, 255, 255), 5, cv::LINE_AA);

      result = atan(sK) * 57.29578;
      tipPoint = lineIntersectPoint(K_l1, B_l1, K_l2, B_l2);
      cv::circle(showImg, tipPoint, 10, cv::Scalar(249, 167, 203), -1, 8, 0);

      cv::imwrite("line.jpg",showImg);
    }
    else
    {
      qDebug() << "no good lines!!";
    }
  }
  else
  {
    qDebug() << "no lines!!";
  }


  //show
  showQImage = cvMat2QImage(showImg);
}

float GaugeReader::getResult()
{
  int d = int(circleRadiu * sin(3.1415926 / 4));
  tipPoint.x = tipPoint.x + d;
  tipPoint.y = tipPoint.y + d;
  if(tipPoint.x > circlePoint.x && tipPoint.y > circlePoint.y)
  {
    return 270 + abs(result);
  }
  else if(tipPoint.x > circlePoint.x && tipPoint.y < circlePoint.y)
  {
    return 270 - abs(result);
  }
  else if(tipPoint.x < circlePoint.x && tipPoint.y < circlePoint.y)
  {
    return 90 + abs(result);
  }
  else if(tipPoint.x < circlePoint.x && tipPoint.y > circlePoint.y)
  {
    return 90 - abs(result);
  }
}

QImage GaugeReader::cvMat2QImage(cv::Mat inputImage)
{
  QImage qimg;
  if(inputImage.empty())
  {
    qDebug() << "input image is null!";
  }
  else
  {
    qimg = QImage(inputImage.data, inputImage.cols, inputImage.rows, inputImage.step,
                  QImage::Format_RGB888);
  }
  return qimg;
}

bool GaugeReader::readSrcImage(QString fileName)
{
  srcImage = cv::imread(fileName.toStdString(), 1);
  if(srcImage.empty())
  {
    qDebug() << "read image faild!";
    return false;
  }
  else
  {
    srcQImage = cvMat2QImage(srcImage);
    return true;
  }
}

bool GaugeReader::readTemplateImage(QString fileName)
{
  templateImage = cv::imread(fileName.toStdString(), 1);
  if(templateImage.empty())
  {
    qDebug() << "read image faild!";
    return false;
  }
  else
  {
    templateQImage = cvMat2QImage(templateImage);
    return true;
  }
}

float GaugeReader::point2lineDis(cv::Point goal_point, cv::Point line_point1, cv::Point line_point2)
{
  float a = line_point2.y - line_point1.y;
  float b = line_point1.x - line_point2.x;
  float c = line_point2.x * line_point1.y - line_point1.x * line_point2.y;
  float dis = abs(a * goal_point.x + b * goal_point.y + c) / sqrt(a * a + b * b);
  return dis;
}

cv::Point GaugeReader::lineIntersectPoint(cv::Point line1_point1, cv::Point line1_point2, cv::Point line2_point1, cv::Point line2_point2)
{
  float a1 = line1_point2.y - line1_point1.y;
  float b1 = line1_point1.x - line1_point2.x;
  float c1 = line1_point2.x * line1_point1.y - line1_point1.x * line1_point2.y;

  float a2 = line2_point2.y - line2_point1.y;
  float b2 = line2_point1.x - line2_point2.x;
  float c2 = line2_point2.x * line2_point1.y - line2_point1.x * line2_point2.y;

  cv::Point intersectPoint;

  intersectPoint.y = ((c2/a2) - (c1/a1)) / ((b1/a1) - (b2/a2));
  intersectPoint.x = ((c2/b2) - (c1/b1)) / ((a1/b1) - (a2/b2));

  return intersectPoint;
}

cv::Point GaugeReader::lineIntersectPoint(float k1, float b1, float k2, float b2)
{
  float x = (b2 - b1)/(k1 - k2);
  float y = k1*x + b1;
  return cv::Point(x, y);
}

float GaugeReader::point2pointDis(cv::Point point1, cv::Point point2)
{
  return sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2));
}

float GaugeReader::lineK(cv::Point line_point1, cv::Point line_point2)
{
  float a = line_point2.y - line_point1.y;
  float b = line_point1.x - line_point2.x;
  float c = line_point2.x * line_point1.y - line_point1.x * line_point2.y;
  return -(a/b);
}

float GaugeReader::lineB(cv::Point line_point1, cv::Point line_point2)
{
  float a = line_point2.y - line_point1.y;
  float b = line_point1.x - line_point2.x;
  float c = line_point2.x * line_point1.y - line_point1.x * line_point2.y;
  return -(c/b);
}

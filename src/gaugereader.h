#ifndef GAUGEREADER_H
#define GAUGEREADER_H

#include <QObject>
#include <QImage>

class GaugeReader : public QObject
{
  Q_OBJECT

public:
  explicit GaugeReader(QObject *parent = nullptr);

  QImage srcImage;
  QImage roiImage;
  QImage resltImage;
  QImage templateImage;

  void matchImage();


signals:

public slots:

private:


};

#endif // GAUGEREADER_H

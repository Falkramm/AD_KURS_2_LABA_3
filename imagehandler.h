#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H
#include <QtWidgets>
#include <vector>
#include <utility>
#define GLOBAL_EPS 10
#define ADAPTIVE_D 5
#define ADAPTIVE_A (0.2)
class ImageHandler {
public:
  ImageHandler();
  //
  static QImage globalThresholdProcessing(QImage image);
  //NIBLEG
  static QImage adaptiveThresholdProcessing(QImage image,const int delta=ADAPTIVE_D);
  //
  static QImage linearContrast(QImage image);
};

#endif // IMAGEHANDLER_H

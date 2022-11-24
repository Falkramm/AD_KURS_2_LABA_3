#include "imagehandler.h"

ImageHandler::ImageHandler() {}

QImage ImageHandler::globalThresholdProcessing(QImage image) {

  int mini = INT_MAX;
  int maxi = INT_MIN;
  for (size_t i = 0; i < image.width(); ++i)
    for (size_t j = 0; j < image.height(); ++j) {
      int h, s, l;
      QColor pixel(image.pixelColor(i, j));
      pixel.getHsl(&h, &s, &l);
      mini = std::min(mini, l);
      maxi = std::max(maxi, l);
    }
  int threshold = (mini + maxi) / 2;
  int newThreshold = threshold;
  do {
    threshold = newThreshold;
    int miniL = INT_MAX, miniG = INT_MAX;
    int maxiL = INT_MIN, maxiG = INT_MIN;
    for (size_t i = 0; i < image.width(); ++i)
      for (size_t j = 0; j < image.height(); ++j) {
        int h, s, l;
        QColor pixel(image.pixelColor(i, j));
        pixel.getHsl(&h, &s, &l);
        if (l <= threshold) {
          miniL = std::min(miniL, l);
          maxiL = std::max(maxiL, l);
        } else {
          miniG = std::min(miniG, l);
          maxiG = std::max(maxiG, l);
        }
      }
    int uL = (miniL + maxiL) / 2, uG = (miniG + maxiG) / 2;
    newThreshold = (uL + uG) / 2;
  } while (std::abs(threshold - newThreshold) > GLOBAL_EPS);
  for (size_t i = 0; i < image.width(); ++i)
    for (size_t j = 0; j < image.height(); ++j) {
      int h, s, l;
      QColor pixel(image.pixelColor(i, j));
      pixel.getHsl(&h, &s, &l);
      if (l <= threshold)
        image.setPixelColor(i, j, Qt::white);
      else
        image.setPixelColor(i, j, Qt::black);
    }
  return image;
}

QImage ImageHandler::adaptiveThresholdProcessing(QImage image,
                                                 const int delta) {
  std::vector<std::vector<int>> thresholdArr(image.width(),
                                             std::vector<int>(image.height()));
  for (size_t i = 0; i < image.width(); ++i)
    for (size_t j = 0; j < image.height(); ++j) {
      int mini = INT_MAX;
      int maxi = INT_MIN;
      double lightSum = 0;
      double deltaLightSum = 0;
      QColor pixel(image.pixelColor(i, j));
      int h0, s0, light;
      pixel.getHsl(&h0, &s0, &light);
      for (int n = -delta; n <= delta; ++n)
        for (int m = -delta; m <= delta; ++m)
          if (i + n >= 0 && i + n < image.width() && j + m >= 0 &&
              j + m < image.height()) {
            int h, s, l;
            QColor pixel(image.pixelColor(i + n, j + m));
            pixel.getHsl(&h, &s, &l);
            mini = std::min(mini, l);
            maxi = std::max(maxi, l);
            lightSum += l;
            deltaLightSum += std::abs(l - light) * std::abs(l - light);
          }
      lightSum /= (2. * delta + 1) * (2. * delta + 1);
      deltaLightSum /= (2. * delta + 1) * (2. * delta + 1);
      thresholdArr[i][j] = lightSum + ADAPTIVE_A * std::sqrt(deltaLightSum);
    }
  for (size_t i = 0; i < image.width(); ++i)
    for (size_t j = 0; j < image.height(); ++j) {
      QColor pixel(image.pixelColor(i, j));
      int h, s, light;
      pixel.getHsl(&h, &s, &light);
      if (light > thresholdArr[i][j])
        image.setPixelColor(i, j, Qt::white);
      else
        image.setPixelColor(i, j, Qt::black);
    }
  return image;
}

QImage ImageHandler::linearContrast(QImage image) {
  int lMin = INT_MAX;
  int lMax = INT_MIN;

  for (size_t i = 0; i < image.width(); ++i)
    for (size_t j = 0; j < image.height(); ++j) {
      QColor pixel(image.pixel(i, j));
      int h, s, l;
      pixel.getHsl(&h, &s, &l);
      lMin = std::min(lMin, l);
      lMax = std::max(lMax, l);
    }
  for (size_t i = 0; i < image.width(); ++i)
    for (size_t j = 0; j < image.height(); ++j) {
      QColor pixel(image.pixel(i, j));
      int h, s, l;
      pixel.getHsl(&h, &s, &l);
      l=(l-lMin)*(255)/(lMax-lMin);
      //TODO set h and s zero if you need
      image.setPixelColor(i,j,QColor::fromHsl(h,s,l));
    }
  return image;
}

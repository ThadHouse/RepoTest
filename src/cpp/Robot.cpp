#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "networktables/NetworkTableInstance.h"
#include "networktables/NetworkTable.h"
#include "cscore.h"
#include "llvm/raw_ostream.h"
#include "support/timestamp.h"
#include <vector>

int main() {
  cs::UsbCamera usbCamera("Camera", 0);
  usbCamera.SetResolution(640, 480);
  usbCamera.SetFPS(30);
  cs::MjpegServer mainServer("MainServer", 1181);
  mainServer.SetSource(usbCamera);
  cs::CvSink sink("Sink");
  sink.SetSource(usbCamera);
  cs::MjpegServer cvStream("CvServer", 1182);

  cs::CvSource source("Source", cs::VideoMode::kMJPEG, 640, 480, 30);
  cvStream.SetSource(source);




  cv::Mat inputMat;
  cv::Mat hsvMat;
  cv::Mat notBlack;
  cv::Mat resize;
  std::vector<std::vector<cv::Point> > contours;

  while (true) {
    auto time = sink.GrabFrame(inputMat);
    if (time == 0) {
      continue;
    }
    auto now = wpi::Now();
    cv::cvtColor(inputMat, hsvMat, CV_BGR2HSV);
    auto hsv = wpi::Now();

    cv::resize(inputMat, resize, cv::Size(320, 240));
    auto size = wpi::Now();

    cv::inRange(hsvMat, cv::Scalar(0, 100, 100), cv::Scalar(180, 255, 255), notBlack);
    auto inRange = wpi::Now();


    cv::findContours(notBlack, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    auto contours = wpi::Now();

    source.PutFrame(notBlack);



    llvm::outs() << "HSV: "<< (hsv - now) << " : Resize: " << size - hsv << " : InRange: " << inRange - size << " : Contours: " << contours - inRange << "\n";
    llvm::outs().flush();



  }
}

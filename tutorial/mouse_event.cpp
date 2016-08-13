/* mouse_event.cpp */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct MouseInfo{
    int event;
    int  flags;
    unsigned int  x;
    unsigned int  y;
};

void on_mouse_callback(int event, int x, int y, int flags, void* param){
  MouseInfo *mInfo = (MouseInfo*)param;
  (*mInfo).x = x;
  (*mInfo).y = y;
  (*mInfo).event = event;
  (*mInfo).flags = flags;
}

int main(int argc, char* argv[]){
  Mat cambus(500, 700, CV_8UC3, Scalar::all(256));
  MouseInfo Information;
  imshow("mouse_example", cambus);
  int fromX=0, fromY=0, toX=0, toY=0;
  cvSetMouseCallback("mouse_example", on_mouse_callback,(MouseInfo*) &Information);
  while(1){
    Mat writing_cambus = cambus.clone();
    if(Information.event == EVENT_LBUTTONDOWN){
      fromX = Information.x;
      fromY = Information.y;
    } 
    else if(Information.flags == CV_EVENT_FLAG_LBUTTON){
      toX = Information.x;
      toY = Information.y;
      rectangle(writing_cambus, Point(fromX,fromY), Point(toX, toY), Scalar(0, 0, 256), 3, 4);
      imshow("mouse_example", writing_cambus);
    } 
    if(Information.event == EVENT_LBUTTONUP){
      toX = Information.x;
      toY = Information.y;
      rectangle(cambus, Point(fromX,fromY), Point(toX, toY), Scalar(0, 0, 256), 3, 4);
      imshow("mouse_example", cambus);
    }
    if(Information.flags != CV_EVENT_FLAG_LBUTTON){
      imshow("mouse_example", cambus);
    }
    if(waitKey(10) == 27) break;
  }
}

/* mat.cpp */
#include <iostream>
#include <random>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void end(){
  cout << "Enter any key, if you want to kill this program." << endl;
  waitKey(0);
  destroyAllWindows();
}

void draw_rect(Mat& cambus){
  random_device rnd_num;
  mt19937 mt(rnd_num());
  uniform_int_distribution<> rand256(0, 256);
  uniform_int_distribution<> randX(0, cambus.cols);
  uniform_int_distribution<> randY(0, cambus.rows);
  int fromX=randX(mt), fromY=randY(mt), toX=randX(mt), toY=randY(mt);
  if (fromX > toX) swap(fromX, toX);
  if (fromY > toY) swap(fromY, toY);
  int value, r, g ,b;
  value = rand256(mt);
  r = rand256(mt);
  g = rand256(mt);
  b = rand256(mt);
  for(int i = fromY; i < toY; i++){
    for(int j = fromX; j < toX; j++){
      if(cambus.channels() == 3){
        cambus.at<Vec3b>(i, j) = Vec3b(b,g,r);
        //cambus.at<Vec3b>(i, j)[0] = r;
        //cambus.at<Vec3b>(i, j)[1] = g;
        //cambus.at<Vec3b>(i, j)[2] = b;
      }else if(cambus.channels() == 1){
        cambus.at<unsigned char>(i, j) = value; 
      }
    }
  }
}

void sample0(){
  Mat original_image = imread("test.jpg", -1);
  Mat gray_image = imread("test.jpg", 0);
  Mat color_image = imread("test.jpg", 1);
  Mat color2gray, resize_image;

  cvtColor(color_image, color2gray,CV_RGB2GRAY);
  resize(color2gray, resize_image, cv::Size(320, 240));

  imshow("original", original_image);
  imshow("gray", gray_image);
  imshow("color", color_image);
  imshow("color2gray", color2gray);
  imshow("resize", resize_image);
  end();
}

void sample1(){
  Mat white(300, 600, CV_8UC3, Scalar::all(256));
  Mat black = Mat::zeros(300, 600, CV_8UC3); // Mat white(300, 600, CV_8UC3, Scalar::all(0));
  Mat blue(300, 600, CV_8UC3, Scalar(256, 0, 0));
  Mat green(300, 600, CV_8UC3, Scalar(0, 256, 0));
  Mat red(300, 600, CV_8UC3, Scalar(0, 0, 256));
  imshow("black", black);
  imshow("white", white);
  imshow("blue", blue);
  imshow("greeb", green);
  imshow("red", red);
  end();
}

void sample2(){
  Mat white(300, 600, CV_8UC1, Scalar(256));
  Mat gray(300, 600, CV_8UC1, Scalar(150));
  Mat black(300, 600, CV_8UC1, Scalar(0));
  imshow("white", white);
  imshow("gray", gray);
  imshow("black", black);
  end();
}

void sample3(){
  Mat white(600, 600, CV_8UC3, Scalar::all(256));
  random_device rnd_num;
  mt19937 mt(rnd_num());
  uniform_int_distribution<> rand256(0, 256);
  uniform_int_distribution<> rand600(0, 600);
  cout << "Press any key, if you want to kill this program." << endl;
  while(1){
  rectangle(white, Point(rand600(mt),rand600(mt)), Point(rand600(mt), rand600(mt)), Scalar(rand256(mt), rand256(mt), rand256(mt)), -1, CV_AA);
  imshow("random rectangle", white);
  if(waitKey(50) >= 0) break;
  }
}

void sample4(){
  Mat RGB(300, 600, CV_8UC3, Scalar::all(256));
  Mat Gray(300, 600, CV_8UC1, Scalar(256));
  while(1){
    draw_rect(RGB);
    draw_rect(Gray);
    imshow("RGB", RGB);
    imshow("Gray", Gray);
    if (waitKey(50) >= 0) break;
  }
}

int main(int argc, char* argv[]){
  if(argc != 2){
    cout << "usage: mat.out option(0~4)"<< endl;
    return 0;
  }
  int num = atoi(argv[1]);
  switch(num){
    case 0:
      sample0();
      break;
    case 1:
      sample1();
      break;
    case 2:
      sample2();
      break;
    case 3:
      sample3();
      break;
    case 4:
      sample4();
      break;

    defalut:
      cout << "no option ->" << num << endl;
      break;
  
  }
}

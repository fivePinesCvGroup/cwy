#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;
//自定义HSV
int hmin = 0, smin = 0, vmin = 255;
int hmax = 0, smax = 0, vmax = 255;

int main()
{
    VideoCapture cap(0);
    Mat img,img2,mask;
    Mat channel[3],a,b,c,d;
    int m=0; //计数
    //定义轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Rect boundRect; // 像素width * height   from  位置(x*y)

    while(true)
   {
        cap.read(img);
        //颜色处理************
        //t1 =1.9  t2=1.5  blue
        //t1 =2.2   t2=2.4 red
        split(img,channel);
        meanStdDev(channel[0],a,b);
        meanStdDev(channel[2],c,d);
        double t1=a.at<double>(0,0);
        double t2=c.at<double>(0,0);
        if(t1>t2){cout<<"blue"<<endl;}
        else{cout<<"red"<<endl;}
        //************
        resize(img,img,Size(500,500));
        Rect point[20];
        //前期处理
        cvtColor(img,img2,COLOR_BGR2HSV);
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(img2, lower, upper, mask);
        //imshow("1",img);
        //imshow("2",mask);
        //waitKey(1);
        findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);  //寻找轮廓
        // drawContours(img,contours,-1,Scalar(0,255,0),5);
        int index = 0;  //定义序号
        for (int i = 0; i < contours.size(); i++)
        {
               // cout<<contours.size()<<endl;
            boundRect = boundingRect(Mat(contours[i]));
            double t=boundRect.height / boundRect.width;//外接矩形  contours.size()=4
            if (t>1&&t<8 && boundRect.height > 50&& boundRect.height<200&& boundRect.width>10&&boundRect.width<100) //判断条件
            {
              //cout<<t<<endl;
              m=i;
              drawContours(img,contours,m,Scalar(0,255,0),5);
              //imshow("4",img);
              //waitKey(1);
              //cout<<boundRect.width<<endl;
              point[index] = boundRect;
              index++;  //index增加2次 寻找两个符合条件的轮廓
             }
        }

        int alike[2]; //定义数组 保存的是最接近的两个轮廓在数组中的index索引值
        int min = 10000; //min area
        //冒泡排序
        for (int i = 0; i < index-1; i++)
        {
            for (int j = i + 1; j < index; j++) {
                int value = abs(point[i].area() - point[j].area());
                if (value < min)
                {
                    min = value;
                    alike[0] = i;
                    alike[1] = j;
                }
            }
        }
        //关键步骤画出矩形
        Rect rectangle_1 = point[alike[0]];
        Rect rectangle_2 = point[alike[1]];
        if (rectangle_2.x == 0 || rectangle_1.x == 0) {
            throw "not enough points";
        }
        //cout<<rectangle_1.width<<endl;
        //cout<<rectangle_2.width<<endl;
        //cout<<rectangle_1.height<<endl;
        //cout<<rectangle_2.height<<endl;
        Point point1 = Point(rectangle_1.x + rectangle_1.width / 2, rectangle_1.y);
        Point point2 = Point(rectangle_1.x + rectangle_1.width / 2, rectangle_1.y + rectangle_1.height);
        Point point3 = Point(rectangle_2.x + rectangle_2.width / 2, rectangle_2.y);
        Point point4 = Point(rectangle_2.x + rectangle_2.width / 2, rectangle_2.y + rectangle_2.height);
        Point p[4] = { point1,point2,point4,point3 };//外接矩形的四个点的坐标
        //cout << p[0]<<p[1]<<p[2]<<p[3] << endl;
        //画出外接矩形
        for (int i = 0; i < 4; i++) {
            line(img, p[i%4], p[(i+1)%4], Scalar(0, 255, 0), 2);
        }
        //比例判别
        int m1 = abs(p[2].x-p[0].x);
        int m2 = abs(p[2].y-p[0].y);
        double t3=m1/m2;
        //cout<<t3<<endl;
        if(t3>2.5&&t3<5) {cout<<"大"<<endl;return 0;}
        if(t3<2.5&&t3>0.5) {cout<<"小"<<endl; return 0;}
        //大的是 2 3 4  小的 1  2
        imshow("video", img);
        if (waitKey(10) >= 0) {
            break;
        }
        //HSV调试
        /*namedWindow("Trackbars", (640, 200));
        createTrackbar("Hue Min", "Trackbars", &hmin, 179);
         createTrackbar("Hue Max", "Trackbars", &hmax, 179);
        createTrackbar("Sat Min", "Trackbars", &smin, 255);
        createTrackbar("Sat Max", "Trackbars", &smax, 255);
       createTrackbar("Val Min", "Trackbars", &vmin, 255);
         createTrackbar("Val Max", "Trackbars", &vmax, 255);*/
    }//for循环终止处
 }




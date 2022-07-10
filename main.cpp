#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin = 105, smin = 45, vmin = 45;
int hmax = 125, smax = 255, vmax = 255;

/*void getContours(Mat a, Mat b) {

    vector<vector<Point>> contours; //轮廓数据
    vector<Vec4i> hierarchy;

    findContours(a, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    drawContours(b, contours, -1, Scalar(255, 0, 255), 2); //绘制所有轮廓

}*/



int main()
{

    Mat a,img,mask;

    while(true){


        a=imread("/home/chenwanyi/图片/2.jpg");
        resize(a,a,Size(500,500));
        cvtColor(a,img,COLOR_BGR2HSV);

        //筛选蓝色，使其变白
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(img, lower, upper, mask);

        vector<vector<Point>> contours; //轮廓数据
        vector<Vec4i> hierarchy;

        findContours(mask,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
        drawContours(a,contours,-1,Scalar(0,255,0),5);

        int img_width = a.cols;
        int img_height = a.rows;
        Vec4f fitline;
        //拟合方法采用最小二乘法
        fitLine(contours[0],fitline,CV_DIST_L2,0,0.01,0.01);
        //求出直线上的两个点
            double k_line = fitline[1]/fitline[0];
            Point p1(0,k_line*(0 - fitline[2]) + fitline[3]);
            Point p2(img_width - 1,k_line*(img_width - 1 - fitline[2]) + fitline[3]);

            //显示拟合出的直线方程
            char text_equation[1024];
            sprintf(text_equation,"y-%.2f=%.2f(x-%.2f)",fitline[3],k_line,fitline[2]);
            putText(a,text_equation,Point(30,50),CV_FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,255),1,8);
            //显示拟合出的直线
            line(a,p1,p2,Scalar(0,0,255),2);
            imshow("原图+拟合结果", a);
            waitKey(1);


        //Vec4f line;
        //fitLine(contours,line,CV_DIST_L2,0,1e-2,1e-2);
        //cout<<line<<endl;
        //寻找轮廓
       // getContours(mask,a);

        //轮条滚动选择范围
        namedWindow("Trackbars", (640, 200));
        createTrackbar("Hue Min", "Trackbars", &hmin, 179);
        createTrackbar("Hue Max", "Trackbars", &hmax, 179);
        createTrackbar("Sat Min", "Trackbars", &smin, 255);
        createTrackbar("Sat Max", "Trackbars", &smax, 255);
        createTrackbar("Val Min", "Trackbars", &vmin, 255);
        createTrackbar("Val Max", "Trackbars", &vmax, 255);
        //显示图像
        imshow("1",a);

        waitKey(1);

    }
}

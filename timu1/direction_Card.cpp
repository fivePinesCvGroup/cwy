#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
//定义最佳分离颜色值
int hmin = 105, smin = 45, vmin = 45;
int hmax = 125, smax = 255, vmax = 255;
int  main()
{
   VideoCapture cap(0);
    Mat a,img,mask;
    while(true){
        //a=imread("/home/chenwanyi/图片/timu1.jpeg");
        cap.read(a);
        resize(a,a,Size(500,500));
        //分离颜色通道，蓝色为白
        cvtColor(a,img,COLOR_BGR2HSV);
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(img, lower, upper, mask);
        //细节预处理
        GaussianBlur(mask, mask, Size(5, 5), 3, 3);//高斯模糊
        threshold(mask, mask, 127, 255, THRESH_BINARY);//再次二值化使图像清晰
        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        erode(mask, mask,kernel);
        dilate(mask, mask, kernel);
        imshow("二值化图",mask);
        waitKey(1);
        //获得轮廓
        vector<vector<Point>> contours; //轮廓数据
        vector<Vec4i> hierarchy;
        findContours(mask,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
        // drawContours(a,contours,-1,Scalar(0,255,0),5);
        //定义变量
        int max1 = contourArea(contours[0],true);
        int max2 = contourArea(contours[0],true);
        int t,m=0;
        for(int i = 0;i<contours.size();i++)
        {
            //外接矩形宽高比
            vector<vector<Point>> conPoly(contours.size());
            vector<Rect> boundRect(contours.size());
            double area = contourArea(contours[i],true);
            //找轮廓的近似多边形或曲线
            double peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            //找每个近似曲线的最小上边界矩形
            boundRect[i] = boundingRect(conPoly[i]);
            //宽高比
            float aspRatio = (float)boundRect[i].width / boundRect[i].height;
            if (aspRatio > 0.5&& aspRatio<0.8&&area>5000&&area<20000)
            {

                    if(max1 < contourArea(contours[i]))
                    {
                        max1 =contourArea(contours[i]);
                        t = i;
                        //cout << aspRatio << endl;
                       // cout<<i<<endl;
                        //cout<<area<<endl;
                    }

             }
        }
        // cout<<t<<endl;
        drawContours(a,contours,t,Scalar(0,255,0),5);
        //拟合方法采用最小二乘法
        //获取行列
        int img_width = a.cols;
        int img_height = a.rows;
        Vec4f fitline;
        fitLine(contours[t],fitline,CV_DIST_L2,0,0.01,0.01);
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
        //轮条滚动选择范围
        /*namedWindow("Trackbars", (640, 200));
        createTrackbar("Hue Min", "Trackbars", &hmin, 179);
        createTrackbar("Hue Max", "Trackbars", &hmax, 179);
        createTrackbar("Sat Min", "Trackbars", &smin, 255);
        createTrackbar("Sat Max", "Trackbars", &smax, 255);
        createTrackbar("Val Min", "Trackbars", &vmin, 255);
        createTrackbar("Val Max", "Trackbars", &vmax, 255);*/
        //返回方向
        if(k_line<5 &&k_line>0){cout<<"左转"<<endl;}
        else if(k_line>-5&&k_line<0){cout<<"右转"<<endl;}
        else if(k_line>5||k_line<-5){cout<<"直行"<<endl;}
        //展示最终图像
        imshow("1",a);
        waitKey(1);         
    }
}

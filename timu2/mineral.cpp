#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;
//HSV基础数值定义
int hmin = 18, smin = 30, vmin = 50;//50 106
int hmax = 60, smax = 255, vmax = 255;

int main()
{
    int t=0;
    VideoCapture cap(0);
    Mat a,img,mask,img2,b;
    while(true){
        // a=imread("/home/chenwanyi/图片/ti2.jpeg");
        cap.read(a);
        //500*500图
        resize(a,a,Size(500,500));
        //筛选黄色，使其变白
        cvtColor(a,img,COLOR_BGR2HSV);
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(img, lower, upper, mask);
        //imshow("黑白图",mask);
        //waitKey(1);
        //细节预处理
        GaussianBlur(mask, mask, Size(5, 5), 3, 3);//高斯模糊
        threshold(mask, mask, 127, 255, THRESH_BINARY);//再次二值化使图像清晰
        Mat kernel = getStructuringElement(MORPH_RECT, Size(9,9));
        erode(mask, mask,kernel);
        dilate(mask, mask, kernel);
        imshow("黑白图",mask);
        waitKey(1);
        //过滤筛选
        vector<vector<Point>> contours; //轮廓定义
        vector<Vec4i> hierarchy;
        findContours(mask,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
        for(int j=0;j<1000;j++)
        {
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
            //严格筛选条件，不然不会显示
           // cout << aspRatio << endl;
            //cout<<area<<endl;
            if (aspRatio > 0.5&& aspRatio<1.2&&area>6000)
            {

                t=1;
                //cout << aspRatio << endl;
                drawContours(a,contours, i, Scalar(0,255, 0), 2);
                //斜率求R
                Vec4f fitline;
                fitLine(contours[i],fitline,CV_DIST_L2,0,0.01,0.01);
                //求出直线上的两个点
                double k_line = fitline[1]/fitline[0];
                cout<<k_line<<endl;
                cout<<area<<endl;
                cout << aspRatio << endl;
              //  cout<<k_line<<endl;
               // cout<<area<<endl;
               // cout << aspRatio << endl;
               // cout<<contours.size()<<endl;
                if(k_line>2&&k_line<1000&&aspRatio<0.9)
                {
                    cout<<k_line<<endl;
                    cout<<area<<endl;
                    cout << aspRatio << endl;
                    cout<<"R面"<<endl;
                    return 0;
                }
                else if (aspRatio>0.9&&k_line>-3&&k_line<3)
                {
                    cout<<k_line<<endl;
                    cout<<area<<endl;
                    cout << aspRatio << endl;
                    cout<<"二维码面"<<endl;
                    return 0;
                }
                imshow("2",a);
                waitKey(1);
             }

            }
            }//循环1000次
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
            //斜率求R
            Vec4f fitline;
            fitLine(contours[i],fitline,CV_DIST_L2,0,0.01,0.01);
            //求出直线上的两个点
            double k_line = fitline[1]/fitline[0];
            //&&k_line>0&&k_line<1
            if (aspRatio > 0.5&& aspRatio<1&&area>1000&&area<3000&&k_line>0&&k_line<2)
            {

                imshow("3",a);
                waitKey(1);
                cout<<k_line<<endl;
                drawContours(a,contours, i, Scalar(0,255, 0), 2);
                cout<<area<<endl;
                cout << aspRatio << endl;

                cout<<"空白面"<<endl;
                return 0;
            }
         }

          //轮条滚动选择HSV范围
       /* namedWindow("Trackbars", (640, 200));
        createTrackbar("Hue Min", "Trackbars", &hmin, 179);
        createTrackbar("Hue Max", "Trackbars", &hmax, 179);
        createTrackbar("Sat Min", "Trackbars", &smin, 255);
        createTrackbar("Sat Max", "Trackbars", &smax, 255);
        createTrackbar("Val Min", "Trackbars", &vmin, 255);
        createTrackbar("Val Max", "Trackbars", &vmax, 255);*/
    }
}

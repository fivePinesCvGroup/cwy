#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin = 14, smin = 50, vmin = 103;
int hmax = 60, smax = 255, vmax = 255;

int main()
{
   VideoCapture cap(0);
    Mat a,img,mask,img2;
    while(true){
       // a=imread("/home/chenwanyi/图片/timu2-2.jpeg");
        cap.read(a);
        //500*500图
        resize(a,a,Size(500,500));
        //筛选黄色，使其变白
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
        //imshow("黑白图",mask);
        //waitKey(1);
        //过滤筛选
        vector<vector<Point>> contours; //轮廓数据
        vector<Vec4i> hierarchy;
        findContours(mask,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
        for(int i = 0;i<contours.size();i++)
        {
            //外接矩形宽高比
            vector<vector<Point>> conPoly(contours.size());
            vector<Rect> boundRect(contours.size());
            double area = contourArea(contours[i],false);
            //找轮廓的近似多边形或曲线
            double peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            //找每个近似曲线的最小上边界矩形
            boundRect[i] = boundingRect(conPoly[i]);
            //宽高比
            float aspRatio = (float)boundRect[i].width / boundRect[i].height;
            if (aspRatio > 1&& aspRatio<2&&area>20000)
            {
                RotatedRect rect = minAreaRect(contours[i]);
                Point2f* touchVertices = new Point2f[4];
                rect.points(touchVertices);
                //cout<<area<<endl;
                // cout<<aspRatio<<endl;
                //  cout<<touchVertices[0]<<endl;
               // cout<<touchVertices[1]<<endl;
               //cout<<touchVertices[2]<<endl;
                //画出外接矩形方便观察
                for(int j=0;j<4;j++)
                {
                    line(a,touchVertices[j%4],touchVertices[(j+1)%4],Scalar(0,255,0));
                }
                imshow("原图",a);
                waitKey(1);
                //裁减a变为img2
                int t1=touchVertices[3].x-touchVertices[1].x;
                int t2=touchVertices[3].y-touchVertices[1].y;
                Rect roi(touchVertices[1].x, touchVertices[1].y,t1,t2);
                img2 = a(roi);
                imshow("1",img2);
                waitKey(1);
               // cout<<t1<<endl;
                //cout<<t2<<endl;
                Mat b;
                cvtColor(img2,b,COLOR_BGR2HSV);
                //筛选黄色，使其变白
                inRange(b, lower, upper, b);
                imshow("b",b);
                vector<vector<Point>> contours2; //轮廓数据
                vector<Vec4i> hierarchy2;
                findContours(b,contours2,hierarchy2,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
                //drawContours(img2,contours2, -1, Scalar(0,255, 0), 2);
                int sum=0;
                for(int i=0;i<contours2.size();i++)
                {
                     float length = arcLength(contours2[i], true);
                     sum=sum+length;

                }
                cout<<sum<<endl;
                //根据实体修改
                if(sum>2000&&sum<2500) cout<<"空白面"<<endl;
                else if(sum>2500&&sum<3000) cout<<"R面"<<endl;
                else if(sum>3000&&sum<5000) cout<<"二维码面"<<endl;
                imshow("img2",img2);
                waitKey(1);
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

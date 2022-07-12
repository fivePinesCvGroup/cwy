#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin = 18, smin = 50, vmin = 103;
int hmax = 60, smax = 255, vmax = 255;
//自定义过滤函数
int getContours(Mat a, Mat b) {
    vector<vector<Point>> contours; //轮廓数据
    vector<Vec4i> hierarchy;
    findContours(a, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //通过预处理的二值图像找到所有轮廓contours

    for (int i = 0; i < contours.size(); i++)
    {
        double area = contourArea(contours[i]); //计算每个轮廓区域

        vector<vector<Point>> conPoly(contours.size());
        vector<Rect> boundRect(contours.size());
        if(area>30000)//过滤噪声
        {
             cout << area << endl;
            //找轮廓的近似多边形或曲线
            double peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            //找每个近似曲线的最小上边界矩形
            boundRect[i] = boundingRect(conPoly[i]);

            int objCor = (int)conPoly[i].size();
            //宽高比
            float aspRatio = (float)boundRect[i].width / boundRect[i].height;
            cout << aspRatio << endl;

            if (aspRatio > 2.1&& aspRatio<2.9) //矿石宽高比范围判断
            {
                 //绘制滤除噪声后的所有轮廓
                drawContours(b, conPoly, i, Scalar(0,255, 0), 2);
                if(area>36000&&area<37000) return 1;  //R面
                else if (area>39000&&area<40000) return 2; //二维码面
                else if (area>43000&&area<45000) return 3; //空白面*/
            }
        }
     }
}

int main()
{
    Mat a,img,mask;
    while(true){
        a=imread("/home/chenwanyi/图片/timu2-3.jpeg");
        resize(a,a,Size(500,500));
        cvtColor(a,img,COLOR_BGR2HSV);
        //筛选黄色，使其变白
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(img, lower, upper, mask);
        GaussianBlur(mask,mask,Size(5,5),3,3);
        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        dilate(mask,mask, kernel);
        //过滤轮廓判断
        int t=getContours(mask,a);
        cout<<t<<endl;

        imshow("1",mask);

        //轮条滚动选择HSV范围
        namedWindow("Trackbars", (640, 200));
        createTrackbar("Hue Min", "Trackbars", &hmin, 179);
        createTrackbar("Hue Max", "Trackbars", &hmax, 179);
        createTrackbar("Sat Min", "Trackbars", &smin, 255);
        createTrackbar("Sat Max", "Trackbars", &smax, 255);
        createTrackbar("Val Min", "Trackbars", &vmin, 255);
        createTrackbar("Val Max", "Trackbars", &vmax, 255);
        //显示图像
        imshow("2",a);
        waitKey(1);
       /* if(t==1) return 1;  //R面
        if(t==2) return 2;  //二维码面
        if(t==3) return 3;  //空白面*/
  }
}

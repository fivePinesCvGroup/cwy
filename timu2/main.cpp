#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin = 18, smin = 50, vmin = 103;
int hmax = 60, smax = 255, vmax = 255;
int main()
{
    Mat a,img,mask;
    while(true){
        a=imread("/home/chenwanyi/图片/kuang1.jpeg");
        resize(a,a,Size(500,500));
        cvtColor(a,img,COLOR_BGR2HSV);

        //筛选黄色，使其变白
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(img, lower, upper, mask);
        imshow("1",mask);
        //获得轮廓
        vector<vector<Point>> contours; //轮廓数据
        vector<Vec4i> hierarchy;
        findContours(mask,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
        drawContours(a,contours,-1,Scalar(0,255,0),5);
        //计算长度
        double sum =0;
        for (int i = 0; i < contours.size(); ++i) {
                //double area = contourArea(contours[i]);
                double length = arcLength(contours[i], true);
                sum=sum+length;

         }
        //显示图像
        imshow("2",a);
        waitKey(1);
        //返回值
        cout<<sum<<endl;
        if(sum<8000&&sum>6000) return 1;  //R面
        else if (sum<10000&&sum>8000) return 2; //二维码面
        else if (sum<4000&&sum>3000) return 3; //空白面
  }
}

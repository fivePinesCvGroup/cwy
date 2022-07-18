#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

String fa1()
{
    String zhi;
    //定义最佳分离颜色值
    int hmin = 105, smin = 45, vmin = 45;
    int hmax = 125, smax = 255, vmax = 255;
        VideoCapture cap(0);
        Mat a,img,mask;   // a为最终展示图，img为颜色中间为过渡量，mask为二值化图
        while(true){
            cap.read(a);
             //a=imread("/home/chenwanyi/图片/1.JPG");
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
            //imshow("二值化图",mask);
            //waitKey(1);
            //获得轮廓
            vector<vector<Point>> contours; //轮廓数据
            vector<Vec4i> hierarchy;
            findContours(mask,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
            // drawContours(a,contours,-1,Scalar(0,255,0),5);
            //定义变量
            int max1 = contourArea(contours[0],true);
            int t=0;//t为最大面积轮廓
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
                if (aspRatio > 0.2&& aspRatio<0.8&&area>1000&&area<20000)
                {
                        //(aspRatio > 0.5&& aspRatio<0.8&&area>3000&&area<7000)
                        //0.6  0.7左转  0.3直行  3000
                        if(max1 < contourArea(contours[i]))
                        {
                            max1 =contourArea(contours[i]);
                            t = i;
                           // cout << aspRatio << endl;
                           // cout<<i<<endl;
                            //cout<<area<<endl;
                        }

                 }
            }
            // cout<<t<<endl;
            //drawContours(a,contours,t,Scalar(0,255,0),5);
            //拟合方法采用最小二乘法
            //获取行列
            int img_width = a.cols;
            int img_height = a.rows;
            Vec4f fitline;
            fitLine(contours[t],fitline,CV_DIST_L2,0,0.01,0.01);
            //求出直线上的两个点
            double k_line = fitline[1]/fitline[0];
           // Point p1(0,k_line*(0 - fitline[2]) + fitline[3]);
            //Point p2(img_width - 1,k_line*(img_width - 1 - fitline[2]) + fitline[3]);
            //显示拟合出的直线方程
            //char text_equation[1024];
            //sprintf(text_equation,"y-%.2f=%.2f(x-%.2f)",fitline[3],k_line,fitline[2]);
            //putText(a,text_equation,Point(30,50),CV_FONT_HERSHEY_COMPLEX,0.5,Scalar(0,0,255),1,8);
            //显示拟合出的直线
           // line(a,p1,p2,Scalar(0,0,255),2);
            //轮条滚动选择范围
           /* namedWindow("Trackbars", (640, 200));
            createTrackbar("Hue Min", "Trackbars", &hmin, 179);
            createTrackbar("Hue Max", "Trackbars", &hmax, 179);
            createTrackbar("Sat Min", "Trackbars", &smin, 255);
            createTrackbar("Sat Max", "Trackbars", &smax, 255);
            createTrackbar("Val Min", "Trackbars", &vmin, 255);
            createTrackbar("Val Max", "Trackbars", &vmax, 255);*/
            //返回方向
            if(k_line<5 &&k_line>0){zhi="左转";return zhi;}
            else if(k_line>-5&&k_line<0){zhi="右转";return zhi;}
            else if(k_line>5||k_line<-5){zhi="直行";return zhi;;}
            //展示最终图像
            //imshow("1",a);
            //waitKey(1);
       // }
}
}


String fa2()
{
    String zhi;
    int hmin = 18, smin = 30, vmin = 50;//50 106
    int hmax = 60, smax = 255, vmax = 255;

        int t1=0;
        int t2=0;
        int t3=0;
        VideoCapture cap(0);
        Mat a,img,mask,img2,b;
        while(true){
            cap.read(a);
            //a=imread("/home/chenwanyi/图片/timu2.jpeg");
            //500*500图
            resize(a,a,Size(500,500));
            //筛选黄色，使其变白
            cvtColor(a,img,COLOR_BGR2HSV);
            Scalar lower(hmin, smin, vmin);
            Scalar upper(hmax, smax, vmax);
            inRange(img, lower, upper, mask);
            //细节预处理
            GaussianBlur(mask, mask, Size(5, 5),3,3);//高斯模糊  5  3
            threshold(mask, mask, 127, 255, THRESH_BINARY);//再次二值化使图像清晰
            Mat kernel = getStructuringElement(MORPH_RECT, Size(4,8));
            erode(mask, mask,kernel);
            dilate(mask, mask, kernel);
            //imshow("黑白图",mask);
            //waitKey(1);
            //过滤筛选
            vector<vector<Point>> contours; //轮廓定义
            vector<Vec4i> hierarchy;
            findContours(mask,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point());
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
                //严格筛选条件，不然不会显示
                if (k_line>2&&k_line<50&&area>7000&&aspRatio > 0.5&& aspRatio<0.9)
                {
                        t1++;
                        //drawContours(a,contours, i, Scalar(0,255, 0), 2);
                        //imshow("R",a);
                        //waitKey(1);
                        //cout<<k_line<<endl;
                        //cout<<area<<endl;
                        //cout << aspRatio << endl;
                        //zhi="R面";
                        //return zhi;
                 }

             }
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

                if (area>8000&&aspRatio>0.9)
                {
                    ++t2;
                    //drawContours(a,contours, i, Scalar(0,255, 0), 2);
                    //imshow("er",a);
                    //waitKey(1);
                    //cout<<"二维码面"<<endl;
                    //return 0;
                }

                else if(area>1500&&area<3000&&aspRatio > 0.5&& aspRatio<1&&k_line<0&&k_line>-3)
                {
                    t3++;
                    //drawContours(a,contours, i, Scalar(0,255, 0), 2);
                    //imshow("bai",a);
                    //waitKey(1);
                    //cout<<k_line<<endl;
                    //cout<<area<<endl;
                    //cout << aspRatio << endl;
                    //cout<<"空白面"<<endl;
                    //return 0;

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
            //cout<<t2<<endl;
            if(t1>0) { zhi="R面";  return zhi;}
            if(t2>0) {zhi="二维码面";  return zhi;}
            if(t3>10) {zhi="空白面";  return zhi;}

        }
}



String fa3()
{
    String zhi;
    int x;//颜色判断
    //自定义HSV
    int hmin = 0, smin = 0, vmin = 255;
    int hmax = 0, smax = 0, vmax = 255;
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
           // img=imread("/home/chenwanyi/图片/timu3.png");
            //颜色处理************
            //t1 =1.9  t2=1.5  blue
            //t1 =2.2   t2=2.4 red
            split(img,channel);
            meanStdDev(channel[0],a,b);
            meanStdDev(channel[2],c,d);
            double t1=a.at<double>(0,0);
            double t2=c.at<double>(0,0);
            if(t1>t2){x=1;}//x=1 blue  x=2 red
            else{x=2;}
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
                try
                {
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
            catch (const char* msg)
            {
                cout << printf(msg) << endl;
            }
            }

            int alike[2]; //定义数组 保存的是最接近的两个轮廓在数组中的index索引值
            int min = 10000; //min area
            //冒泡排序  找出两个面积最近的就是灯条
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
            try
            {
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
            if(t3>2.5&&t3<5)
            {
                if(x==1) {zhi = "蓝色大装甲板";return zhi;}
                else if(x==2) {zhi = "红色大装甲板";return zhi;}
            }
            if(t3<2.5&&t3>0.5)
            {
                if(x==1) {zhi = "蓝色小装甲板";return zhi;}
                else if(x==2) {zhi = "红色小装甲板";return zhi;}
            }
        }
        catch (const char* msg)
        {
            cout << msg << endl;
            //continue;
        }
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


int main()
{
    int t=0; //计数
    String zhi; //记录返回值
    while(true)
    {
        if(t==0) cin>>t;
        //fa1
        if(t==1)
        {
            zhi=fa1();
            cout<<zhi<<endl;
            t--;
            cout<<t<<endl;
        }
        //fa2
        if(t==2)
        {
            zhi=fa2();
            cout<<zhi<<endl;
            t--;
            t--;
        }
        //fa3
        if(t==3)
        {
            zhi=fa3();
            cout<<zhi<<endl;          
            return 0;
        }
    }
}

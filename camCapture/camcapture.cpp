/************************************************************************
* Copyright(c) 2011  谢小军
* Brief: 视频捕捉Qt+OpenCV
* Version: 1.0
* Author: 谢小军
* Email: 346944475@qq.com
* Date:    2011/12/05
************************************************************************/
#include <QDebug>

#include "camcapture.h"

// 简单测试函数, 打开本地avi文件
int openAviFile()  
{  
    using namespace cv;
    //打开视频文件：其实就是建立一个VideoCapture结构  
    VideoCapture capture("E:\\ftp\\Qt_Opencv\\Debug\\1.avi");  
    //检测是否正常打开:成功打开时，isOpened返回ture  
    if(!capture.isOpened())  
        qDebug()<<"fail to open!";  
    //获取整个帧数  
    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);  
    qDebug()<<"整个视频共"<<totalFrameNumber<<"帧";  


    //设置开始帧()  
    long frameToStart = 0;  
    capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);  
    qDebug()<<"从第"<<frameToStart<<"帧开始读";  


    //设置结束帧  
    int frameToStop = 400;  

    if(frameToStop < frameToStart)  
    {  
        qDebug()<<"结束帧小于开始帧，程序错误，即将退出！";  
        return -1;  
    }  
    else  
    {  
        qDebug()<<"结束帧为：第"<<frameToStop<<"帧";  
    }  


    //获取帧率  
    double rate = capture.get(CV_CAP_PROP_FPS);  
    qDebug()<<"帧率为:"<<rate;  



    //定义一个用来控制读取视频循环结束的变量  
    bool stop = false;  
    //承载每一帧的图像  
    Mat frame;  
    //显示每一帧的窗口  
    namedWindow("Extracted frame");  
    //两帧间的间隔时间:  
    //int delay = 1000/rate;  
    int delay = 1000/rate;  


    //利用while循环读取帧  
    //currentFrame是在循环体中控制读取到指定的帧后循环结束的变量  
    long currentFrame = frameToStart;  


    //滤波器的核  
    int kernel_size = 3;  
    Mat kernel = Mat::ones(kernel_size,kernel_size,CV_32F)/(float)(kernel_size*kernel_size);  

    while(!stop)  
    {  
        //读取下一帧  
        if(!capture.read(frame))  
        {  
            qDebug()<<"读取视频失败";  
            return -1;    
        }  

        
        imshow("Extracted frame",frame);  

        //这里加滤波程序  
        filter2D(frame,frame,-1,kernel);  

        imshow("after filter",frame);  

        qDebug()<<"正在读取第"<<currentFrame<<"帧";  
        //waitKey(int delay=0)当delay ≤ 0时会永远等待；当delay>0时会等待delay毫秒  
        //当时间结束前没有按键按下时，返回值为-1；否则返回按键  


        int c = waitKey(delay);  
        //按下ESC或者到达指定的结束帧后退出读取视频  
        if((char) c == 27 || currentFrame > frameToStop)  
        {  
            stop = true;  
        }  
        //按下按键后会停留在当前帧，等待下一次按键  
        if( c >= 0)  
        {  
            waitKey(0);  
        }  
        currentFrame++;  

    }  
    //关闭视频文件  
    capture.release();  
    waitKey(0);  
    return 0;  
}  

// 测试代码, 使用摄像头 
void openVideoCapture()
{
    using namespace cv;
    VideoCapture cap(0);
    //设置摄像头
    cap.set( CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set( CV_CAP_PROP_FRAME_HEIGHT,480 );
    //确认是否成功打开摄像头
    if(!cap.isOpened()){
        qDebug() << "打开摄像头失败，退出";
        exit(-1);
    }
    namedWindow( "Capture",CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO );
    while (1)
    {
        Mat frame;
        cap>>frame;

        //各种处理

        imshow( "Capture",frame);
        if( waitKey(30)>=0 ) break;
    }
}

camCapture::camCapture(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    // 初始化处理，建立QImage和frame的关联，开启定时器
    //打开摄像头端口
    //方法1
    /*
    CvCapture* pCapture = NULL;
    if( !(pCapture = cvCaptureFromCAM(-1)))
    {
    fprintf(stderr, "Can not open camera.\n");
    return -2;
    }*/

    //openAviFile();

    //方法2
    capture.open(0);//打开摄像头端口
    if (capture.isOpened())//判断是否打开
    {
        capture >> frame;//往矩阵里存数据流
        if (!frame.empty())//判断是否取出流
        {
            //色彩空间转换 
            // 函数 cvCvtColor 将输入图像从一个色彩空间转换为另外一个色彩空间。函数忽略 IplImage 头中定义的
            // colorModel 和 channelSeq 域，所以输入图像的色彩空间应该正确指定 (包括通道的顺序，对RGB空间而言，
            // BGR意味着布局为 B0 G0 R0 B1 G1 R1 ... 层叠的 24-位格式，
            // 而 RGB 意味着布局为 R0 G0 B0 R1 G1 B1 ... 层叠的24-位格式.
            
            cv::cvtColor(frame, frame, CV_BGR2RGB);

            //垂直，水平或即垂直又水平翻转二维数组
            cv::flip(frame, frame, 1);

            //将矩阵转换成Qimage类型
            image = new QImage((const unsigned char*)(frame.data), 
                                frame.cols, 
                                frame.rows, 
                                QImage::Format_RGB888);

            //设置时间值：QTimer类提供了定时器信号和单触发定时器
            timer = new QTimer(this);
            //设置信号槽触发间隔
            timer->setInterval(30);
            //信号槽连接
            connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
            //开始一个msec毫秒定时的定时器。
            timer->start();
        }
    }
    ui.setupUi(this); //更新用户接口，细微变化
}

camCapture::~camCapture()
{
    delete image;//析构
    delete timer;//析构
}

void camCapture::paintEvent(QPaintEvent * e)
{
    // 更新图像
    QPainter painter(this);//绘制，在窗口部件上5
    painter.drawImage(QPoint(35, 100), *image);//在点p绘制图像i。int conversion_flags = 0,使用conversion_flags来指定你希望发生什么。 
    //QPoint(20, 30)：点的坐标
}

void camCapture::nextFrame()//槽函数
{
    // 更新数据
    capture >> frame;//将从摄像头获取的流读入到矩形框里
    if (!frame.empty())//判断矩形框类容不为空
    {
        cv::cvtColor(frame, frame, CV_BGR2RGB);//颜色转换
        cv::flip(frame, frame, 1);//垂直，水平或即垂直又水平翻转二维数组
        this->update();//更新数据
    }
}


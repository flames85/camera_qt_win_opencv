/************************************************************************
* Copyright(c) 2011  Yang Xian
* All rights reserved.
*
* File:    camcapture.h
* Brief: 视频捕捉Qt+OpenCV
* Version: 1.0
* Author: Yang Xian
* Email: xyang2011@sinano.ac.cn
* Date:    2011/12/05
* History:
************************************************************************/
#ifndef CAMCAPTURE_H
#define CAMCAPTURE_H

#include <QtGui/QMainWindow>
#include "ui_camcapture.h"
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtCore/QPoint>

#include "cv.h"
//#include <core.h>
#include <highgui.h>
//#include <imgproc.h>

class camCapture : public QMainWindow
{
    Q_OBJECT

public:
    camCapture(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~camCapture();
protected:
    void paintEvent(QPaintEvent * e);//画图事件：QT

private:
    Ui::camCaptureClass ui;//QT界面
    cv::Mat frame;//初始化矩阵的头(轻磅变量) 
    cv::VideoCapture capture;//摄像头端口
    QImage *image;//图片流
    QTimer *timer;//时间种子

private slots:
    void nextFrame();//信号槽函数
};

#endif 

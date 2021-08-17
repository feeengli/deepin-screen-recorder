/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     He Mingyang<hemingyang@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "scrollScreenshot.h"

#include <QDebug>
#include <QDateTime>
#include <X11/Xlibint.h>
#include <X11/extensions/XTest.h>

ScrollScreenshot::ScrollScreenshot(QObject *parent)  : QObject(parent)
{
    Q_UNUSED(parent);
    qRegisterMetaType<PixMergeThread::MergeErrorValue>("MergeErrorValue");

    m_mouseWheelTimer = new QTimer(this);
    connect(m_mouseWheelTimer, &QTimer::timeout, this, [=] {
        // 发送滚轮事件， 自动滚动
        static Display* m_display = XOpenDisplay(nullptr);
        XTestFakeButtonEvent(m_display, 5, 1, CurrentTime);
        XFlush(m_display);
        XTestFakeButtonEvent(m_display, 5, 0, CurrentTime);
        XFlush(m_display);
        // 滚动区域高度 200-300 取值2
        // 滚动区域高度 > 300  取值 3
        // 滚动区域高度 > 600  取值 5
        m_scrollCount++;
        if(m_scrollCount % m_shotFrequency == 0) {
            emit getOneImg();
        }
    });

    m_PixMerageThread = new PixMergeThread(this);
    connect(m_PixMerageThread, SIGNAL(updatePreviewImg(QImage)), this, SIGNAL(updatePreviewImg(QImage)));
    connect(m_PixMerageThread, SIGNAL(merageError(PixMergeThread::MergeErrorValue)), this, SLOT(merageImgState(PixMergeThread::MergeErrorValue)));
}


void ScrollScreenshot::addPixmap(const QPixmap &piximg)
{
    if(m_curStatus == Wait) {
        m_mouseWheelTimer->start(300);
        m_PixMerageThread->start();
        m_curStatus = Merging;
    }

    if(m_curStatus == Merging) {
        m_PixMerageThread->addShotImg(piximg);
    }
}

void ScrollScreenshot::changeState(const bool isStop)
{
    qDebug() << __FUNCTION__ << "====" <<isStop;
    // 暂停
    if (isStop && m_curStatus == Merging) {
        m_curStatus = Stop;
        m_mouseWheelTimer->stop();
    }

    // 开始
    if (!isStop && m_curStatus == Stop) {
        m_curStatus = Merging;
        m_mouseWheelTimer->start(300);
    }
}
QImage ScrollScreenshot::savePixmap()
{
    if (m_curStatus == ScrollStatus::Merging) {
        m_mouseWheelTimer->stop();
        emit getOneImg();
        m_PixMerageThread->stopTask();
        m_PixMerageThread->wait();
    }
    //QDateTime currentDate;
    //QString currentTime =  currentDate.currentDateTime().toString("yyyyMMddHHmmss");
    //QString saveFileName = QString("%1_%2_%3.%4").arg("S", "Long IMG", currentTime, "png");
    //(m_PicMerageThread->getMerageResult()).save(saveFileName);
    return m_PixMerageThread->getMerageResult();
}
// 获取两张图片变化区域
QRect ScrollScreenshot::getChangeArea(cv::Mat &img1, cv::Mat &img2)
{
    int minI = img1.rows;
    int minJ = img1.cols;
    int maxI = 0;
    int maxJ = 0;
    // 计算变化部分
    for(int i = 0; i < img1.rows; ++i) {
        for(int j = 0; j < img1.cols; ++j) {
            //if(img1.at<Vec3b>(i, j)[0] != img2.at<Vec3b>(i, j)[0] || img1.at<Vec3b>(i, j)[1] != img2.at<Vec3b>(i, j)[1] || img1.at<Vec3b>(i, j)[2] != img2.at<Vec3b>(i, j)[2]) {
            if(img1.at<cv::Vec3b>(i, j) != img2.at<cv::Vec3b>(i, j)){
                if( i < minI) minI = i;
                if( j < minJ) minJ = j;
                if( i > maxI) maxI = i;
                if( j > maxJ) maxJ = j;
            }
        }
    }
    return  QRect(minJ, minI, maxJ - minJ, maxI - minI);
}

void ScrollScreenshot::calcHeadHeight()
{

}


void ScrollScreenshot::merageImgState(PixMergeThread::MergeErrorValue state)
{
    qDebug() << "拼接状态值:" <<state;
    m_mouseWheelTimer->stop();
    if (state == PixMergeThread::MaxHeight) {
        m_curStatus = ScrollStatus::Mistake;
    }
    emit merageError(state);
}

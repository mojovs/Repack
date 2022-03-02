#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QFileInfo>
#include <QMessageBox>
#include <QErrorMessage>
#include <QThread>
#include <iostream>
#include "Repack.h"
#define MAX_TASK 4

Repack::Repack(QObject *parent) : QObject(parent)
{
    //创建线程
    fThread1 = new FileHandleThread();
    fThread2 = new FileHandleThread();
    fThread3 = new FileHandleThread();
    fThread4 = new FileHandleThread();

    //绑定函数
    //文件处理完成，发出信号
    connect(fThread1, SIGNAL(handleFinished(int)), this, SLOT(onThreadFinished(int)));
    connect(fThread2, SIGNAL(handleFinished(int)), this, SLOT(onThreadFinished(int)));
    connect(fThread3, SIGNAL(handleFinished(int)), this, SLOT(onThreadFinished(int)));
    connect(fThread4, SIGNAL(handleFinished(int)), this, SLOT(onThreadFinished(int)));
}
//析构
Repack::~Repack()
{
    //删除线程
    delete fThread1;
    delete fThread2;
    delete fThread3;
    delete fThread4;
}

QList<QUrl> Repack::fileUrls()
{
    return m_fileUrls;
}
/*---------------------- qml属性 ----------------------------*/
void Repack::setFileUrls(QList<QUrl> lists)
{
    m_fileUrls = lists;
    //同时设置 字符串路径
    if (lists.isEmpty()) return;
    m_filePaths.clear(); //清空
    for (int i = 0; i < lists.length(); i++)
    {
        QUrl url = lists.at(i);

        //从url中获取路径
        QString aPath = url.path();
        aPath.remove(0, 1); //截取
        m_filePaths.append(aPath);
    }
}

QList<QString> Repack::filePaths()
{
    return m_filePaths;
}

void Repack::setFilePaths(QList<QString> lists)
{
    m_filePaths = lists;
}

QString Repack::outputDir()
{
    return m_outputDir;
}
/*---------------------- ----------------------------*/
//执行打包命令
void Repack::start()
{
    if (m_filePaths.count() < 0 || m_filePaths.isEmpty())
    {
        qDebug() << "filePaths is NULL ";
        return;
    }
    //运行线程
    runThreadByTask();
}
void Repack::runThreadByTask()
{
    //一个循环，持续查询哪个线程不在工作，就给哪个线程分配文件处理任务
    int count = m_filePaths.count(); //总共多少个文件
    int n     = 0;
    while (n < count)
    {
        switch (m_taskNum)
        {
            //判断线程有没有在运行,空线程,而且还有文件没处理 就接任务
            case 4:
                if (!fThread4->isRunning() && n < count)
                {
                    //获取文件名
                    QString path  = m_filePaths.at(n);
                    QString nPath = newPath(path);
                    //给线程设置文件名
                    fThread4->setFilePath(path);
                    fThread4->setNewFilePath(nPath);
                    //给线程设置文件序号
                    fThread4->setFileNo(n);
                    //运行
                    fThread4->start();
                    n++;
                }
            case 3:
                if (!fThread3->isRunning() && n < count)
                {
                    //获取文件名
                    QString path  = m_filePaths.at(n);
                    QString nPath = newPath(path);
                    //给线程设置文件名
                    fThread3->setFilePath(path);
                    fThread3->setNewFilePath(nPath);
                    fThread3->setFileNo(n);
                    //运行
                    fThread3->start();
                    n++;
                }
            case 2:
                if (!fThread2->isRunning() && n < count)
                {
                    //获取文件名
                    QString path  = m_filePaths.at(n);
                    QString nPath = newPath(path);
                    //给线程设置文件名
                    fThread2->setFilePath(path);
                    fThread2->setNewFilePath(nPath);
                    fThread2->setFileNo(n);
                    //运行
                    fThread2->start();
                    n++;
                }
            case 1:
                if (!fThread1->isRunning() && n < count)
                {
                    //获取文件名
                    QString path  = m_filePaths.at(n);
                    QString nPath = newPath(path);
                    //给线程设置文件名
                    fThread1->setFilePath(path);
                    fThread1->setNewFilePath(nPath);
                    fThread1->setFileNo(n);
                    //运行
                    fThread1->start();
                    n++;
                }
                break;
        }

        //线程满了，睡眠
        QThread::msleep(50);
        std::cout << "+";
        std::cout << "处理完成\n";
    }
}

//创建乱码后的文件路径
QString Repack::newPath(QString path)
{
    QFileInfo fileInfo(path);
    if (!fileInfo.isFile())
    {
        qDebug() << "这个是个文件";
        return nullptr;
    }
    QString fileName;                       //输出的文件名
    QString baseName = fileInfo.baseName(); //文件名，不包括后缀
    //输出路径为空的话,路径设置为文件所在路径
    if (m_outputDir.isEmpty())
    {
        fileName = fileInfo.absolutePath() + "/" + baseName + "_." + fileInfo.suffix();
    }
    else
    {
        fileName = m_outputDir + baseName + "_." + fileInfo.suffix();
    }
    return fileName;
}
//线程处理完成
void Repack::onThreadFinished(int n)
{
    emit fileNoFinish(n);
}

void Repack::setOutputDir(QString path)
{
    if (path.isEmpty())
    {
        qDebug() << "Path is Empty";
        return;
    }
    //解析路径
    QFileInfo fileInfo(path);
    //确定路径是文件夹
    if (fileInfo.isDir())
    {
        //设置
        m_outputDir = path;
    }
    else
    {
        qDebug() << "Path is not a Dir";
        return;
    }
}

int Repack::taskNum()
{
    return m_taskNum;
}

void Repack::setTaskNum(int num)
{
    //最大四个任务
    if (num > 4)
    {
        m_taskNum = 4;
    }
    else if (num > 0 && num <= 4)
    {
        m_taskNum = num;
    }
    else
    {
        m_taskNum = 1; //默认一个任务线程
    }
}

#include "FileHandleThread.h"
#include <iostream>
#include <QDebug>
#include <QFileInfo>
#include <QDataStream>
#include <QSemaphore>
#include <QByteArray>
#pragma execution_character_set("utf-8")
#define BLOCK_SIZE 1024 * 1024 * 50
//读取文件路径
const QString &FileHandleThread::filePath() const
{
    return m_filePath;
}
//设置要处理文件路径
void FileHandleThread::setFilePath(const QString &newFilePath)
{
    m_filePath = newFilePath;
}
//新建文件路径
const QString &FileHandleThread::newFilePath() const
{
    return m_newFilePath;
}
//设置新建文件路径
void FileHandleThread::setNewFilePath(const QString &newNewFilePath)
{
    m_newFilePath = newNewFilePath;
}
//构造
int FileHandleThread::getFileNo() const
{
    return fileNo;
}

void FileHandleThread::setFileNo(int newFileNo)
{
    fileNo = newFileNo;
}

//构造函数
FileHandleThread::FileHandleThread(QObject *parent) : QThread(parent) {}
//析构
FileHandleThread::~FileHandleThread()
{
    delete[] buf; //删除缓冲池
}
//重载：线程运行
void FileHandleThread::run()
{
    buf = new char[BLOCK_SIZE];

    //打开文件
    QFile inFile(m_filePath);
    if (!inFile.open(QFile::ReadOnly))
    {
        std::cout << "打开文件失败" << m_filePath.toStdString();
        return;
    }
    //打开写入文件
    QFile outFile(m_newFilePath);
    if (!outFile.open(QFile::WriteOnly))
    {
        std::cout << "打开文件失败" << m_filePath.toStdString();
        return;
    }
    extern quint8 header[1536];
    QByteArray header_s = QByteArray::fromRawData((char *)header, 1536);
    //写入
    outFile.write(header_s); //写入数据

    //读出文件
    while (inFile.read(buf, sizeof(buf)) > 0)
    {
        if (outFile.write(buf, sizeof(buf)) < 0)
        {
            qDebug() << "文件写入失败";
        }
    }

    //关闭文件
    inFile.close();
    outFile.close();

    //发送处理完成信号
    emit handleFinished(fileNo);
}

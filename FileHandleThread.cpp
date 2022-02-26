#include "FileHandleThread.h"
#include <iostream>
#include <QDebug>
#include <QFileInfo>
#include <QDataStream>
#include <QSemaphore>
#include <QByteArray>
#pragma execution_character_set("utf-8")
#define BLOCK_SIZE 1024 * 1024 * 50

FileHandleThread::FileHandleThread(QObject *parent) : QThread(parent) {}

FileHandleThread::~FileHandleThread()
{
    delete[] buf; //删除缓冲池
}

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
    //发送拷贝完成的信号
    emit sig_copyFinished(); //拷贝完成
}

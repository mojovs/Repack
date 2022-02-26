#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QFileInfo>
#include <QMessageBox>
#include <QErrorMessage>
#include <QThread>
#include "Repack.h"
#define MAX_TASK 4

Repack::Repack(QObject *parent) : QObject(parent) {}

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
    for (int i = 0; i < m_filePaths.length(); i++)
    {
        //获取路径名
        QString path = m_filePaths.at(i);
        //创建读写进程
        fThread.m_filePath    = path;
        fThread.m_newFilePath = newPath(path);

        fThread.start();
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
    QString baseName = fileInfo.baseName(); //文件名，不包括后缀
    QString fileName = fileInfo.absolutePath() + "/" + baseName + "_." + fileInfo.suffix();

    return fileName;
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

#ifndef REPACK_H
#define REPACK_H

#include <QObject>
#include <QList>
#include <QUrl>
#include <QMap>
#include "FileHandleThread.h"
class Repack : public QObject
{
    Q_OBJECT
    //输入的地址url
    Q_PROPERTY(QList<QUrl> fileUrls READ fileUrls WRITE setFileUrls NOTIFY fileUrlsChanged)
    //实际的路径
    Q_PROPERTY(QList<QString> filePaths READ filePaths WRITE setFilePaths NOTIFY filePathsChanged)
    //输出路径地址
    Q_PROPERTY(QString outputDir READ outputDir WRITE setOutputDir NOTIFY outputDirChanged)
    //任务线程数
    Q_PROPERTY(int taskNum READ taskNum WRITE setTaskNum NOTIFY taskNumChanged)

  public:
    Repack(QObject *parent = 0);
    ~Repack();

    //属性设置部分
    QList<QUrl> fileUrls();
    void setFileUrls(QList<QUrl> lists);

    QList<QString> filePaths();
    void setFilePaths(QList<QString> lists);

    QString outputDir();
    void setOutputDir(QString path);

    int taskNum();
    void setTaskNum(int num);

    Q_INVOKABLE void start(); //开始执行重编码

    void runThreadByTask(); //根据任务数运行线程
  private:
    QList<QUrl> m_fileUrls;
    QList<QString> m_filePaths;
    QString m_outputDir;
    int m_taskNum = 2; //任务数
    int m_fileNo  = 0; //文件编号
    //四个处理线程
    FileHandleThread *fThread1 = nullptr;
    FileHandleThread *fThread2 = nullptr;
    FileHandleThread *fThread3 = nullptr;
    FileHandleThread *fThread4 = nullptr;

    QString newPath(QString path); //创建乱码后的路径

  signals:
    void fileUrlsChanged();
    void filePathsChanged();
    void outputDirChanged();
    void taskNumChanged();

    /* 处理完成
     * @n文件的编号，用来标志第几个文件已经完成了处理
     */
    void fileNoFinish(int n);

  public slots:
    void onThreadFinished(int n);
};

#endif // REPACK_H

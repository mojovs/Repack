#ifndef REPACK_H
#define REPACK_H

#include <QObject>
#include <QList>
#include <QUrl>
#include "FileHandleThread.h"
class Repack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QUrl> fileUrls READ fileUrls WRITE setFileUrls NOTIFY fileUrlsChanged)
    Q_PROPERTY(QList<QString> filePaths READ filePaths WRITE setFilePaths NOTIFY filePathsChanged)
    Q_PROPERTY(QString outputDir READ outputDir WRITE setOutputDir NOTIFY outputDirChanged)
  public:
    Repack(QObject *parent = 0);

    //属性设置部分
    QList<QUrl> fileUrls();
    void setFileUrls(QList<QUrl> lists);

    QList<QString> filePaths();
    void setFilePaths(QList<QString> lists);

    QString outputDir();
    void setOutputDir(QString path);

    FileHandleThread fThread;
    Q_INVOKABLE void start(); //开始执行重编码

  private:
    QList<QUrl> m_fileUrls;
    QList<QString> m_filePaths;
    QString m_outputDir;

    QString newPath(QString path); //创建乱码后的路径
  signals:
    void fileUrlsChanged();
    void filePathsChanged();
    void outputDirChanged();
};

#endif // REPACK_H

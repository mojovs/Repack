#ifndef FILEHANDLETHREAD_H
#define FILEHANDLETHREAD_H

#include <QThread>
#include <QString>
#include <QMutex>

class FileHandleThread : public QThread
{
    Q_OBJECT
  private:
    bool m_stop = false;

    char *buf = nullptr;   //缓冲池
    QString m_filePath;    //文件的路径
    QString m_newFilePath; //新建
    int fileNo = 0;        //文件序号
  public:
    //构造 析构
    FileHandleThread(QObject *parent = 0); //构造函数
    ~FileHandleThread();                   //析构

    //设置文件路径
    const QString &filePath() const;
    void setFilePath(const QString &newFilePath);
    //设置新创建文件路径
    const QString &newFilePath() const;
    void setNewFilePath(const QString &newNewFilePath);
    //设置文件号
    int getFileNo() const;
    void setFileNo(int newFileNo);

  protected:
    void run();
  signals:
    void handleFinished(int n);
};

#endif // FILEHANDLETHREAD_H

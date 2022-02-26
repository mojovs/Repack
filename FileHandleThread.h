#ifndef FILEHANDLETHREAD_H
#define FILEHANDLETHREAD_H

#include <QThread>
#include <QString>
#include <QMutex>

class FileHandleThread : public QThread
{
  private:
    bool m_stop = false;
    char *buf   = nullptr; //缓冲池

  public:
    QString m_filePath;
    QString m_newFilePath;
    FileHandleThread(QObject *parent = 0);
    ~FileHandleThread();

  protected:
    void run();
  signals:
    void sig_copyFinished();
};

#endif // FILEHANDLETHREAD_H

#include "logsystem.h"
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QThread>
#include <QDir>

static QtMessageHandler pMessageHandler = nullptr;

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QDateTime time = QDateTime::currentDateTime();
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss");
    //qrc:/ChatView.qml:386:29
    QString strMessage = QString("[%1] Thread ID: 0x%2 (%3:%4) %5:\n %6")\
                             .arg(strTime,QString::number((quint64)(QThread::currentThreadId()),16) ,context.file)\
                             .arg(context.line).arg(context.function, msg);

//    QString msgTpye("");
//    switch(type)
//    {
//    case QtDebugMsg:
//        msgTpye = QString("debug: ");
//        break;
//    case QtInfoMsg:
//        msgTpye = QString("info: ");
//        break;
//    case QtWarningMsg:
//        msgTpye = QString("warning: ");
//        break;
//    case QtCriticalMsg:
//        msgTpye = QString("critical: ");
//        break;
//    case QtFatalMsg:
//        msgTpye = QString("fatal: ");
//        break;
//    default:
//        msgTpye = QString("err: ");
//        break;
//    }

//    strMessage = msgTpye + strMessage;

    static QFile file;
    static QTextStream stream(&file);
    static QMutex mutex;
    QString path = QDir::currentPath();
    path += "/Logs/";
    QString fileName = QString("%1%2.txt").arg(path,QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    file.setFileName(fileName);
    stream.setCodec("UTF-8");

    // 加锁
    mutex.lock();

    // 输出信息至文件中（读写、追加形式）
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    stream << strMessage << "\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();

    //用系统原来的函数完成原来的功能. 比如输出到调试窗
    if(pMessageHandler)
    {
        pMessageHandler(type, context, strMessage);
    }
}

LogSystem::LogSystem(QObject *parent)
    : QObject{parent}
{
    pMessageHandler = qInstallMessageHandler(messageOutput);

    QString path = QDir::currentPath();
    path += "/Logs/";

    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkdir(path);
    }

    QDateTime tenDaysAgo = QDateTime::currentDateTime().addDays(-10);

    // 获取日志目录中的所有文件
    QStringList files = dir.entryList(QDir::Files);
    foreach (const QString& fileName, files) {
        QString filePath = path + QDir::separator() + fileName;

        // 解析文件名中的日期部分
        QString dateString = fileName.split(".").first();
        QDateTime fileDate = QDateTime::fromString(dateString, "yyyy-MM-dd");
        // 比较文件日期和10天前的日期
        if (fileDate < tenDaysAgo) {
            // 删除过期的日志文件
            QFile::remove(filePath);
        }
    }
}

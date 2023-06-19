/*!
 *@file QmlLog4Qml.cpp
 *@brief Qml写日志
 *@version 1.0
 *@section LICENSE Copyright (C) 2003-2103 CamelSoft Corporation
 *@author zhengtianzuo
*/
#include "QmlLog4Qml.h"
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QThread>
#include <QDir>

static QtMessageHandler gDefaultHandler = nullptr;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QDateTime time = QDateTime::currentDateTime();
    QString strTime = time.toString("yyyy-MM-dd hh:mm:ss");
    //qrc:/ChatView.qml:386:29
    QString strMessage = QString("[%1] Thread ID: 0x%2 (%3:%4) %5:\n %6")\
            .arg(strTime,QString::number((quint64)(QThread::currentThreadId()),16) ,context.file)\
            .arg(context.line).arg(context.function, msg);

    QString msgTpye("");
    switch(type)
    {
    case QtDebugMsg:
        msgTpye = QString("Debug");
        break;
    case QtInfoMsg:
        msgTpye = QString("Info");
        break;
    case QtWarningMsg:
        msgTpye = QString("Warning");
        break;
    case QtCriticalMsg:
        msgTpye = QString("Critical");
        break;
    case QtFatalMsg:
        msgTpye = QString("Fatal");
        break;
    default:
        msgTpye = QString("Err");
        break;
    }

//    strMessage = msgTpye + strMessage;

#if 1
    // 加锁
    static QMutex mutex;
    mutex.lock();

    // 输出信息至文件中（读写、追加形式）

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

    QString fileName = QString("%1%2.txt").arg(path,time.toString("yyyy-MM-dd"));
    QFile file(fileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
#endif
    //用系统原来的函数完成原来的功能. 比如输出到调试窗
    if(gDefaultHandler)
    {
        gDefaultHandler(type,context,strMessage);
    }

}


void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
//    static QMutex mutex;
//    mutex.lock();

    QString text;
    switch(static_cast<int>(type))
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString message = "";
    if (context.file != nullptr)
    {
        QString context_info = QString("(%1 %2):").arg(QString(context.file)).arg(context.line);
        QString current_date_time = QDateTime::currentDateTime().toString("MM-dd hh:mm:ss");
        QString current_date = QString("(%1)").arg(current_date_time);
        message = QString("%1 %2 %3 %4").arg(current_date,text,context_info,msg);
    }
    else
    {
        message = msg;
    }

//    QFile file("log.txt");
//    file.open(QIODevice::WriteOnly | QIODevice::Append);
//    QTextStream text_stream(&file);
//    text_stream << message << "\r\n";
//    file.flush();
//    file.close();

    qDebug() <<message;

//    mutex.unlock();
}


QmlLog4Qml::QmlLog4Qml(QObject *parent)
    : QObject{parent}
{
    gDefaultHandler = qInstallMessageHandler(myMessageOutput);
    //qrc:/ChatView.qml:386:29
//    gDefaultHandler = qInstallMessageHandler(outputMessage);
}

void QmlLog4Qml::qDebug_Info(int type, QString strInfo)
{
    QMessageLogContext context;
    context.file = nullptr;
    outputMessage(static_cast<QtMsgType>(type), context, strInfo);
}

#ifndef QMLLOG4QML_H
#define QMLLOG4QML_H

#include <QObject>

class QmlLog4Qml : public QObject
{
    Q_OBJECT
public:
    explicit QmlLog4Qml(QObject *parent = nullptr);
    Q_INVOKABLE void qDebug_Info(int type, QString strInfo);

signals:

};

#endif // QMLLOG4QML_H

#pragma once
#include "piqi_export.h"
#include <qnetworkreply.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT PiqiResponse : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QObject* Data READ getData)
    Q_PROPERTY(QString Response READ getResponse)
    Q_PROPERTY(int StatusCode READ getStatusCode)
    Q_PROPERTY(bool IsSuccessful READ getIsSuccessful)

    QNetworkReply* reply;
    QObject* obj;

    public:
        PiqiResponse(QObject* parent = nullptr);
        PiqiResponse(QObject* obj, QNetworkReply* reply);

        QObject* getData();
        QString getResponse();
        int getStatusCode();
        bool getIsSuccessful();
};

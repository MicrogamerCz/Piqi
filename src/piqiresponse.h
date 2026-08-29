#pragma once
#include "piqi_export.h"
#include <QNetworkReply>

class PIQI_EXPORT PiqiResponse : public QObject {
    Q_OBJECT

    Q_PROPERTY(QVariant data READ data)
    Q_PROPERTY(QString response READ response)
    Q_PROPERTY(int statusCode READ statusCode)
    Q_PROPERTY(bool isSuccessful READ isSuccessful)

  public:
    PiqiResponse(QObject *obj, const QNetworkReply &reply);

    QVariant data();
    QString response();
    int statusCode();
    bool isSuccessful();

  private:
    const QNetworkReply &reply;
    QVariant obj;
};

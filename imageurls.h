#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include "qepr.h"

class ImageUrls : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, squareMedium)
    QM_PROPERTY(QString, medium)
    QM_PROPERTY(QString, large)
    QM_PROPERTY(QString, original)
    QM_PROPERTY(QString, px16)
    QM_PROPERTY(QString, px50)
    QM_PROPERTY(QString, px170)

    public:
        ImageUrls(QObject* parent = nullptr) : QObject(parent) {};
        ImageUrls(QObject* parent, QJsonObject data) : QObject(parent)
        {
            if (data.contains("square_medium")) m_squareMedium = data["square_medium"].toString();
            if (data.contains("medium")) m_medium = data["medium"].toString();
            if (data.contains("large")) m_large = data["large"].toString();
            if (data.contains("original")) m_original = data["original"].toString();
            if (data.contains("px_16x16")) m_px16 = data["px_16x16"].toString();
            if (data.contains("px_50x50")) m_px50 = data["px_50x50"].toString();
            if (data.contains("px_170x170")) m_px170 = data["px_170x170"].toString();
        }
};

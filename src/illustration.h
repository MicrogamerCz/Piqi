#pragma once
#include "piqi_export.h"
#include "tag.h"
#include "user.h"
#include "work.h"
#include "workspace.h"
#include <QStringList>
#include <qcontainerfwd.h>

class PIQI_EXPORT Illustration : public Work
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, type)
    QM_PROPERTY(QStringList, tools)
    QM_PROPERTY(int, width)
    QM_PROPERTY(int, height)
    QM_PROPERTY(int, sanityLevel)
    QM_PROPERTY(WorkPrimitive *, series)
    QM_PROPERTY(QString, metaSinglePage)
    QM_PROPERTY(QList<ImageUrls *>, metaPages)
    QM_PROPERTY(int, illustAiType)
    QM_PROPERTY(int, illustBookType)
    // seasonal_effect_animation_urls
    // event_banners
    // request
    QM_PROPERTY(QStringList, restrictionAttributes)
    QM_PROPERTY(int, commentAccessControl)
    QM_PROPERTY(int, totalComments)

public:
    Illustration(QObject *parent = nullptr);
    Illustration(QObject *parent, QJsonObject data, QString accessToken = "", QString refreshToken = "");
    
};

#pragma once
#include "piqi_export.h"
#include "series.h"
#include "tag.h"
#include "user.h"
#include "work.h"

class PIQI_EXPORT Illustration : public Work
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, type)
    QM_PROPERTY(QStringList, tools)
    QM_PROPERTY(int, width)
    QM_PROPERTY(int, height)
    QM_PROPERTY(int, sanityLevel)
    QM_PROPERTY(Series *, series)
    QM_PROPERTY(QString, metaSinglePage)
    QM_PROPERTY(QList<ImageUrls *>, metaPages)
    QM_PROPERTY(int, illustAiType)
    QM_PROPERTY(int, illustBookType)

public:
    Illustration(QObject *parent = nullptr);
    Illustration(QObject *parent, QJsonObject data);
};

#pragma once
#include "imageurls.h"
#include "piqi_export.h"
#include "tag.h"
#include "user.h"
#include "work.h"
#include <qjsonobject.h>
#include <qlist.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT Novel : public Work
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(bool, isOriginal)
    QM_PROPERTY(int, textLength)
    // series
    QM_PROPERTY(int, totalComments)
    QM_PROPERTY(bool, isMypixivOnly)
    QM_PROPERTY(bool, isXRestricted)
    QM_PROPERTY(int, novelAiType)
    // request

    protected:
        const QString type() override;

    public:
        Novel(QObject* parent = nullptr);
        Novel(QObject* parent, QJsonObject data);
};

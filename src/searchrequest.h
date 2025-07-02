#pragma once
#include <QJsonObject>
#include <QtQmlIntegration>
#include <QAbstractListModel>
#include <qdatetime.h>
#include <qtmetamacros.h>
#include "piqi_export.h"
#include "qepr.h"
#include "tag.h"

// Purely a helper class as Pixiv search requests have quite a few parameters.
// Some optional, others not. This is the better solution imo
class PIQI_EXPORT SearchRequest : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    public:
        enum class SearchTarget {
            PartialTagsMatch = 0,
            ExactTagsMatch = 1,
            TitleAndDescription = 2
        };
        Q_ENUM(SearchTarget)

        QM_PROPERTY(QList<Tag*>, tags)
        QM_PROPERTY(SearchTarget, searchTarget) // SearchTarget should be used in practice
        QM_PROPERTY(bool, sortAscending) // false by default
        QM_PROPERTY(QDate*, start_date) // if this date is nullptr, it's set to current date
        QM_PROPERTY(QDate*, end_date) // if this date is nullptr, both date parameters are ignored in the request

        SearchRequest(QObject* parent);
        Q_SLOT void SetTags(QAbstractListModel* tags);
};

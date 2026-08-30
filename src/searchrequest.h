#pragma once
#include "illusts.h"
#include "novels.h"
#include "piqi_export.h"
#include <QAbstractListModel>
#include <QJsonObject>
#include <qtmetamacros.h>

// Purely a helper class as Pixiv search requests have quite a few parameters.
// Some optional, others not. This is the better solution imo
class PIQI_EXPORT SearchRequest : public QObject {
    Q_OBJECT
    QML_ELEMENT

  public:
    enum class SearchTarget {
        PartialTagsMatch = 0,
        ExactTagsMatch = 1,
        TitleAndDescription = 2
    };

    QM_PROPERTY(QList<Tag *>, tags)
    QM_PROPERTY(SearchTarget,
                searchTarget)        // SearchTarget should be used in practice
    QM_PROPERTY(bool, sortAscending) // false by default
    QM_PROPERTY(QDate *,
                start_date)        // if this date is nullptr, it's set to current date
    QM_PROPERTY(QDate *, end_date) // if this date is nullptr, both date
                                   // parameters are ignored in the request

  public:
    Q_ENUM(SearchTarget)

    Q_INVOKABLE SearchRequest(QObject *parent = nullptr);
    Q_INVOKABLE void setTags(QAbstractListModel *tags);

    Q_INVOKABLE QCoro::QmlTask search();
    QCoro::Task<SearchResults *> searchTask();

    Q_INVOKABLE QCoro::QmlTask searchNovels();
    QCoro::Task<NovelSearchResults *> searchNovelsTask();

    Q_INVOKABLE QCoro::QmlTask searchPopularPreview();
    QCoro::Task<Illusts *> searchPopularPreviewTask();

    Q_INVOKABLE QCoro::QmlTask searchNovelsPopularPreview();
    QCoro::Task<Novels *> searchNovelsPopularPreviewTask();
};

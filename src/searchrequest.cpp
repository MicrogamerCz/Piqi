#include "searchrequest.h"
#include "requestworker.h"

SearchRequest::SearchRequest(QObject *parent) : QObject(parent) {
    m_searchTarget = SearchTarget::PartialTagsMatch;
    m_sortAscending = false;
    m_start_date = m_end_date = nullptr;
};

void SearchRequest::setTags(QAbstractListModel *tags) {
    for (int i = 0; i < tags->rowCount(); i++) {
        auto item = tags->data(tags->index(i), Qt::DisplayRole).value<Tag *>();
        m_tags.append(item);
    }
}

QCoro::QmlTask SearchRequest::search() {
    return searchTask();
}
QCoro::Task<SearchResults *> SearchRequest::searchTask() {
    QUrl url("https://app-api.pixiv.net/v1/search/illust");
    QUrlQuery query;

    QString words;
    for (int i = 0; i < m_tags.length(); i++) {
        Tag *tag = m_tags[i];
        words += tag->m_name + " ";
    }
    words.removeLast();
    query.addQueryItem("word", words);

    switch (m_searchTarget) {
    case SearchRequest::SearchTarget::PartialTagsMatch: {
        query.addQueryItem("search_target", "partial_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::ExactTagsMatch: {
        query.addQueryItem("search_target", "exact_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::TitleAndDescription: {
        query.addQueryItem("search_target", "title_and_caption");
        break;
    }
    }

    if (m_end_date != nullptr) {
        if (m_start_date)
            query.addQueryItem("start_date", m_start_date->toString(Qt::DateFormat::ISODate));
        else
            query.addQueryItem("start_date", QDate::currentDate().toString(Qt::DateFormat::ISODate));

        query.addQueryItem("end_date", m_end_date->toString(Qt::DateFormat::ISODate));
    }

    if (m_sortAscending)
        query.addQueryItem("sort", "date_asc");
    else
        query.addQueryItem("sort", "date_desc");

    url.setQuery(query);

    return PiqiInternal::sendGet<SearchResults>(url);
}

QCoro::QmlTask SearchRequest::searchNovels() {
    return searchNovelsTask();
}
QCoro::Task<NovelSearchResults *> SearchRequest::searchNovelsTask() {
    QUrl url("https://app-api.pixiv.net/v1/search/novel");
    QUrlQuery query;

    QString words;
    for (int i = 0; i < m_tags.length(); i++) {
        Tag *tag = m_tags[i];
        words += tag->m_name + " ";
    }
    words.removeLast();
    query.addQueryItem("word", words);

    switch (m_searchTarget) {
    case SearchRequest::SearchTarget::PartialTagsMatch: {
        query.addQueryItem("search_target", "partial_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::ExactTagsMatch: {
        query.addQueryItem("search_target", "exact_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::TitleAndDescription: {
        query.addQueryItem("search_target", "title_and_caption");
        break;
    }
    }

    if (m_end_date != nullptr) {
        if (m_start_date)
            query.addQueryItem("start_date", m_start_date->toString(Qt::DateFormat::ISODate));
        else
            query.addQueryItem("start_date", QDate::currentDate().toString(Qt::DateFormat::ISODate));

        query.addQueryItem("end_date", m_end_date->toString(Qt::DateFormat::ISODate));
    }

    if (m_sortAscending)
        query.addQueryItem("sort", "date_asc");
    else
        query.addQueryItem("sort", "date_desc");

    url.setQuery(query);

    return PiqiInternal::sendGet<NovelSearchResults>(url);
}

QCoro::QmlTask SearchRequest::searchPopularPreview() {
    return searchPopularPreviewTask();
}
QCoro::Task<Illusts *> SearchRequest::searchPopularPreviewTask() {
    QUrl url("https://app-api.pixiv.net/v1/search/popular-preview/illust");
    QUrlQuery query;

    QString words;
    for (int i = 0; i < m_tags.length(); i++) {
        Tag *tag = m_tags[i];
        words += tag->m_name + " ";
    }
    words.removeLast();
    query.addQueryItem("word", words);

    switch (m_searchTarget) {
    case SearchRequest::SearchTarget::PartialTagsMatch: {
        query.addQueryItem("search_target", "partial_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::ExactTagsMatch: {
        query.addQueryItem("search_target", "exact_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::TitleAndDescription: {
        query.addQueryItem("search_target", "title_and_caption");
        break;
    }
    }

    if (m_end_date != nullptr) {
        if (m_start_date)
            query.addQueryItem("start_date", m_start_date->toString(Qt::DateFormat::ISODate));
        else
            query.addQueryItem("start_date", QDate::currentDate().toString(Qt::DateFormat::ISODate));

        query.addQueryItem("end_date", m_end_date->toString(Qt::DateFormat::ISODate));
    }

    url.setQuery(query);
    return PiqiInternal::sendGet<Illusts>(url);
}

QCoro::QmlTask SearchRequest::searchNovelsPopularPreview() {
    return searchNovelsPopularPreviewTask();
}
QCoro::Task<Novels *> SearchRequest::searchNovelsPopularPreviewTask() {
    QUrl url("https://app-api.pixiv.net/v1/search/popular-preview/novel");
    QUrlQuery query;

    QString words;
    for (int i = 0; i < m_tags.length(); i++) {
        Tag *tag = m_tags[i];
        words += tag->m_name + " ";
    }
    words.removeLast();
    query.addQueryItem("word", words);

    switch (m_searchTarget) {
    case SearchRequest::SearchTarget::PartialTagsMatch: {
        query.addQueryItem("search_target", "partial_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::ExactTagsMatch: {
        query.addQueryItem("search_target", "exact_match_for_tags");
        break;
    }
    case SearchRequest::SearchTarget::TitleAndDescription: {
        query.addQueryItem("search_target", "title_and_caption");
        break;
    }
    }

    if (m_end_date != nullptr) {
        if (m_start_date)
            query.addQueryItem("start_date", m_start_date->toString(Qt::DateFormat::ISODate));
        else
            query.addQueryItem("start_date", QDate::currentDate().toString(Qt::DateFormat::ISODate));

        query.addQueryItem("end_date", m_end_date->toString(Qt::DateFormat::ISODate));
    }

    url.setQuery(query);
    return PiqiInternal::sendGet<Novels>(url);
}

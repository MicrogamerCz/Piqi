#include "searchrequest.h"

SearchRequest::SearchRequest(QObject* parent) : QObject(parent) {
    m_searchTarget = SearchTarget::PartialTagsMatch;
    m_sortAscending = false;
    m_start_date = m_end_date = nullptr;
};

void SearchRequest::SetTags(QAbstractListModel* tags) {
    for (int i = 0; i < tags->rowCount(); i++) {
        auto item = tags->data(tags->index(i), Qt::DisplayRole).value<Tag*>();
        m_tags.append(item);
    }
}

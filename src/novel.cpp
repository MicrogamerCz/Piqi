#include "novel.h"
#include "imageurls.h"
#include "tag.h"
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qobject.h>

Novel::Novel(QObject* parent) : Work(parent) { }
Novel::Novel(QObject* parent, QJsonObject data) : Work(parent, data) {
    m_isOriginal = data["is_original"].toBool();
    m_textLength = data["text_length"].toInt();
    // series
    m_totalComments = data["total_comments"].toInt();
    m_isMypixivOnly = data["is_mypixiv_only"].toBool();
    m_isXRestricted = data["is_x_restricted"].toBool();
    m_novelAiType = data["novel_ai_type"].toInt();
    // request
}

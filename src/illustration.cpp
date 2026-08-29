#include "illustration.h"
#include "requestworker.h"
#include <qjsonobject.h>

Illustration::Illustration(QObject *parent) : Work(parent), m_series(nullptr) {
}
Illustration::Illustration(QObject *parent, QJsonObject data) : Work(parent), m_series(nullptr) {
    deserialize(data);
}
const QString Illustration::type() const {
    return "illust";
}

void Illustration::assignProperty(const QString &propertyName, const QJsonValue &data) {
    switch (properties.indexOf(propertyName)) {
    case 0:
        for (QJsonValue tool : data.toArray())
            m_tools.append(tool.toString());
        break;
    case 1:
        if (!data.isNull())
            m_series = new WorkPrimitive(this, data.toObject());
        // m_series = !data.isNull() ? new WorkPrimitive(this, data.toObject()) : nullptr;
        Q_EMIT seriesChanged();
        break;
    case 2: {
        const QJsonObject metaSinglePageObject = data.toObject();
        if (metaSinglePageObject.contains("original_image_url"))
            m_metaSinglePage = metaSinglePageObject["original_image_url"].toString();
        break;
    }
    case 3:
        for (QJsonValue metaPage : data.toArray())
            m_metaPages.append(new ImageUrls(this, metaPage.toObject()["image_urls"].toObject()));
        Q_EMIT metaPagesChanged();
        break;
    case 4:
        for (QJsonValue val : data.toArray())
            m_restrictionAttributes.append(val.toString());
        break;
    default:
        Work::assignProperty(propertyName, data);
    }
}

#include "imageurls.h"

ImageUrls::ImageUrls(QObject *parent) : QJObject(parent) {
}
ImageUrls::~ImageUrls() = default;

ImageUrls::ImageUrls(QObject *parent, QJsonObject data) : QJObject(parent) {
    deserialize(data);
}

void ImageUrls::assignProperty(const QString &propertyName, const QJsonValue &data) {
    if (propertyName == "px16x16") {
        m_px16 = data.toString();
        Q_EMIT px16Changed();
    } else if (propertyName == "px50x50") {
        m_px50 = data.toString();
        Q_EMIT px50Changed();
    } else if (propertyName == "px170x170") {
        m_px170 = data.toString();
        Q_EMIT px170Changed();
    } else
        QJObject::assignProperty(propertyName, data);
}

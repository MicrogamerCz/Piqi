#pragma once
#include "piqi_export.h"
#include "tag.h"
#include <QAbstractItemModel>

class PIQI_EXPORT Tags : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, nextUrl)
    QM_PROPERTY(QList<Tag *>, tags)

  public:
    Tags(QObject *parent = nullptr);
    Tags(QObject *parent, QJsonObject data);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_SLOT void Extend(Tags *nextTags);

    enum Roles {
        NameRole = Qt::UserRole + 1,
        TranslatedNameRole
    };

  private:
    const QHash<int, QByteArray> roles = {
        {Roles::NameRole, "name"},
        {Roles::TranslatedNameRole, "translatedName"},
    };
};

#pragma once
#include "imageurls.h"
#include "piqi_export.h"
#include "tag.h"
#include <qabstractitemmodel.h>
#include <qjsonobject.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class PIQI_EXPORT Tags : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, nextUrl)

    QList<Tag*> tags;

    public:
        Tags(QObject* parent = nullptr);
        Tags(QObject* parent, QJsonObject data);

        Q_SLOT void Extend(Tags* nextTags);

        int rowCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override;
};

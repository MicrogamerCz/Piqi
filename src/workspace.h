#pragma once
#include "piqi_export.h"
#include "qepr.h"
#include <qobject.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>
#include <qjsonobject.h>

class PIQI_EXPORT Workspace : public QObject {
    Q_OBJECT
    QML_ELEMENT

    QM_PROPERTY(QString, pc)
    QM_PROPERTY(QString, monitor)
    QM_PROPERTY(QString, tool)
    QM_PROPERTY(QString, scanner)
    QM_PROPERTY(QString, tablet)
    QM_PROPERTY(QString, mouse)
    QM_PROPERTY(QString, printer)
    QM_PROPERTY(QString, desktop)
    QM_PROPERTY(QString, music)
    QM_PROPERTY(QString, desk)
    QM_PROPERTY(QString, chair)
    QM_PROPERTY(QString, comment)

    public:
        Workspace(QObject* parent = nullptr);
        Workspace(QObject* parent, QJsonObject data);
};

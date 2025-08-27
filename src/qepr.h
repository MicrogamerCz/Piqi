#include <QObject>
#include <QtQmlIntegration>
#include <qtmetamacros.h>

#define QM_PROPERTY(type, name) \
    public: \
    Q_PROPERTY(type name MEMBER m_##name NOTIFY name##Changed) \
    type m_##name; \
    Q_SIGNAL void name##Changed();

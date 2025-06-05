// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 Joshua Goins <josh@redstrate.com>

#include "illusts.h"

int Illusts::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_illusts.count();
}

QVariant Illusts::data(const QModelIndex &index, int role) const
{
    const auto illustration = m_illusts[index.row()];
    if (role == CustomRoles::IllustRole) {
        return QVariant::fromValue(illustration);
    }
    return {};
}

QHash<int, QByteArray> Illusts::roleNames() const
{
    return {
        {CustomRoles::IllustRole, QByteArrayLiteral("illust")},
    };
}

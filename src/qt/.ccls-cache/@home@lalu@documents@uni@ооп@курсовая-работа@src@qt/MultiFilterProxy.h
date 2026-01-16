#pragma once

#include <QSortFilterProxyModel>
#include <QRegularExpression>

class MultiFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MultiFilterProxy(QObject* parent = nullptr)
        : QSortFilterProxyModel(parent)
    {
        setFilterCaseSensitivity(Qt::CaseInsensitive);
    }

    void setSearchText(const QString& text)
    {
        QRegularExpression re(QRegularExpression::escape(text),
                              QRegularExpression::CaseInsensitiveOption);
        m_re = re;
        invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override
    {
        if (!m_re.isValid() || m_re.pattern().isEmpty())
            return true;

        const int cols = sourceModel() ? sourceModel()->columnCount() : 0;
        for (int col = 0; col < cols; ++col)
        {
            QModelIndex idx = sourceModel()->index(sourceRow, col, sourceParent);
            const QString value = sourceModel()->data(idx, Qt::DisplayRole).toString();
            if (m_re.match(value).hasMatch())
                return true;
        }
        return false;
    }

private:
    QRegularExpression m_re;
};

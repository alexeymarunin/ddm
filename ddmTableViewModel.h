#ifndef DDMTABLEVIEWMODEL
#define DDMTABLEVIEWMODEL

#include <QAbstractTableModel>


class ddmTableViewModel: public QAbstractTableModel
{
public:
    ddmTableViewModel( QObject* parent = 0);

    QVariant data( const QModelIndex &index, int role ) const;

    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    int rowCount( const QModelIndex & index ) const;

    int columnCount( const QModelIndex & index ) const;

    Qt::ItemFlags flags( const QModelIndex& index) const;

private:
    int m_row;
    int m_col;

};

#endif // DDMTABLEVIEWMODEL


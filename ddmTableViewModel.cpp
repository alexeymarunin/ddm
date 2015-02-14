#include <QStandardItemModel>
#include <QSize>

#include "ddmTableViewmodel.h"


ddmTableViewModel::ddmTableViewModel( QObject *parent ) : QAbstractTableModel(parent)
{
    m_row = 3;
    m_col = 3;
}


QVariant ddmTableViewModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if (orientation != Qt::Horizontal)
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        switch ( section )
        {
           case 0: return "Колонка 1";
           case 1: return "Колонка 2";
           case 2: return "Колонка 3";
           default: return QVariant();
        }
    }

    return QVariant();
}


int ddmTableViewModel::columnCount( const QModelIndex& index ) const
{
    return m_col;
}


Qt::ItemFlags ddmTableViewModel::flags( const QModelIndex& index ) const
{
    return ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
}


int ddmTableViewModel::rowCount( const QModelIndex& index ) const
{
    return m_row;
}


QVariant ddmTableViewModel::data( const QModelIndex &index, int role ) const
{
    if ( role == Qt::DisplayRole && index.isValid() )
    {
        if ( index.isValid() )
            return QObject::tr( "Строка %1 Столбец %2" ).arg( index.row() ).arg( index.column() );
    }
    return QVariant();
}

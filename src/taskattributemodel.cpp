#include "taskattributemodel.h"

TableModel::TableModel(Task *pTask, QObject *parent) : QAbstractTableModel(parent)
{
		parentTask = pTask;
}

int TableModel::rowCount(const QModelIndex&) const
{
	return parentTask->attributeCount();
}

int TableModel::columnCount(const QModelIndex&) const
{
	return 1;
}
    
QVariant TableModel::data(const QModelIndex &index, int role) const
{
	int count;
	if (index.isValid())
	{
		count = index.row();
   		if (role == Qt::DisplayRole || role == Qt::EditRole)
   			return parentTask->getAttributeValue(parentTask->getAttribute(count));		
	}
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
    	parentTask->changeAttributeValue(parentTask->getAttribute(index.row()),value); // all editing done here
        return true;
    }
    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractTableModel::flags(index);
    if (index.isValid()) 
    	f |= Qt::ItemIsEditable;
    return f;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
	if (role != Qt::DisplayRole)
	    return QVariant();
	
	if (orientation == Qt::Horizontal)
	{
		if (section == 0)
	    	return "Value";
	    else 
	    	return QVariant();
	}
	else if (orientation == Qt::Vertical)
	{
		return parentTask->getAttribute(section);
	}
	return QVariant();	
 }
 
void TableModel::reload()
{
	reset();
}


#ifndef TASKATTRIBUTEMODEL_H
#define TASKATTRIBUTEMODEL_H

#include <QtGui>
#include "task.h"

class Task;

class TableModel : public QAbstractTableModel
{
	public:
	    TableModel(Task *pTask,QObject *parent = 0);
	    void reload();
	
	    int rowCount(const QModelIndex&) const;
	    int columnCount(const QModelIndex&) const;
	    
	    QVariant data(const QModelIndex &index, int role) const;
	   	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	    
	    bool setData(const QModelIndex &index, const QVariant &value, int role);
	
	    Qt::ItemFlags flags(const QModelIndex &index) const;
	
	private:
		Task *parentTask;
};

#endif

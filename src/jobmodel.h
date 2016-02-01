#ifndef JOBMODEL_H
#define JOBMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include "job.h"

#define NOSETS   8			// number of rows in the table
#define LINESIZE 4			// number of words per line (# of columns)

class Job;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class JobModel: public QAbstractTableModel {
	Q_OBJECT

private:
	int array[NOSETS][LINESIZE];			// where the data are stored

public:
	Job *curJob;	// link to parent job
	
	JobModel( QObject* parent=0 );	// must be reimplemented

	// rowCount and columnCount must be reimplemented 
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

	// data() must be reimplemented as well.  Returns the data at the specified index
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	// headerData allows us to put labels on the rows and columns
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	// setItem( r, c, d ) sets the contens of array[r][c] to d.  Emits a signal to 
	// force the view to update the display.
	int setItem( int row, int col, int datum );
};

//---------------------------------------------------------------------------
// cacheModel(): the main access function to the cache model.
//---------------------------------------------------------------------------
JobModel* cacheModel();

#endif

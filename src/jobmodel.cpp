/*-----------------------------------------------*/
/* _____         _    __  __           _      _  */
/*|_   _|_ _ ___| | _|  \/  | ___   __| | ___| | */
/*  | |/ _` / __| |/ / |\/| |/ _ \ / _` |/ _ \ | */
/*  | | (_| \__ \   <| |  | | (_) | (_| |  __/ | */
/*  |_|\__,_|___/_|\_\_|  |_|\___/ \__,_|\___|_| */
/*												 */
/*-----------------------------------------------*/
#include "jobmodel.h"

//---------------------------------------------------------------------------
// cacheModel: the main access function for the cache.  Returns a pointer
//			to the model and to the included data 
//---------------------------------------------------------------------------
JobModel* cacheModel() {
	static JobModel* p= new JobModel();
	return p;
}

//---------------------------------------------------------------------------
// constructor: initializes the contents of the array with a running counter
//---------------------------------------------------------------------------
JobModel::JobModel( QObject* parent ) : QAbstractTableModel(parent)
{
}

//---------------------------------------------------------------------------
// rowCount: returns the number of rows.
//---------------------------------------------------------------------------
int JobModel::rowCount(const QModelIndex &parent ) const {
	return curJob->taskCount();
}

//---------------------------------------------------------------------------
// columnCount: returns the number of columns
//---------------------------------------------------------------------------
int JobModel::columnCount(const QModelIndex &parent ) const {
	return 4;//curJob->attributeCount();
}

//---------------------------------------------------------------------------
// data: used by the view to get the item stored at a given index (which
//		 contains a row & column field)
//---------------------------------------------------------------------------
QVariant JobModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */ ) const {
     if (!index.isValid() || role != Qt::DisplayRole)
         return QVariant();
     //return curJob->taskAt(row()).
	switch(index.column())
	{
		case 0:
			return curJob->getTask(index.row())->getAttributeValue("Name");
		case 1:
			return curJob->getTask(index.row())->getAttributeValue("StartDate");
		case 2:
			return curJob->getTask(index.row())->getAttributeValue("EndDate");
		case 3:
			int timeSpent = curJob->getTask(index.row())->getAttributeValue("TimeSpent").toInt();
			QString timeTotal = QString::number(timeSpent/3600) + "hr " + QString::number((timeSpent%3600)/60) + "min " + QString::number((timeSpent%3600)%60) + "sec";
			return timeTotal;
	}
	 //return curJob->getTask(index.row())->getAttributeValue(curJob->getAttribute(index.column()));
     //return qGray(modelImage.pixel(index.column(), index.row()));
    return QVariant();
}

//---------------------------------------------------------------------------
// headerData: used to define the headers of the rows & columns
//---------------------------------------------------------------------------
QVariant JobModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal)
        	if (section == 0)
            	return "Name";
            else if (section == 1)
            	return "Start Date";
            else if (section == 2)
            	return "End Date";
            else if (section == 3)
            	return "Time Spent";
            else 
            	return QVariant();
        else
            return QString("Task %1").arg(section+1);
 }

										 
//---------------------------------------------------------------------------
// setItem: sets the item at (row, col) to the value in datum.
//			returns 1 if successful, 0 otherwise
//---------------------------------------------------------------------------
int JobModel::setItem( int row, int col, int datum ) {
	//--- validate the index ---
	//if ( row < 0 || col < 0 || row >= NOSETS || col >= LINESIZE )
	//	return 0;	// error!

	//--- sets the data ---
	//array[row][col] = datum;

	//--- tell the world ---
	//emit dataChanged( createIndex( row, col ), createIndex( row, col ) );

	return 1;		// success!
}

#ifndef TASKTABLEVIEW
#define TASKTABLEVIEW

#include <QtGui>
#include "jobmodel.h"

class TaskTableView : public QTableView
{
	Q_OBJECT
	
	public:
		TaskTableView(QWidget *parent);
		~TaskTableView();
		
		void setJobModel(JobModel *model);
		JobModel *getJobModel();
	protected slots:
		void taskSelected(const QModelIndex &current);
	
	signals:
		void taskDoubleClicked(Task *newTask);
	
	private:
		JobModel *job;
};

#endif

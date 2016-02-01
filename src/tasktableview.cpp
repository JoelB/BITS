#include "tasktableview.h"

TaskTableView::TaskTableView(QWidget *parent) : QTableView(parent)
{
	QSizePolicy sizePolicy10(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
	sizePolicy10.setHorizontalStretch(0);
	sizePolicy10.setVerticalStretch(2);
	sizePolicy10.setHeightForWidth(sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy10);
	setBaseSize(QSize(200, 0));
	setAlternatingRowColors(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setShowGrid(true);
	
	connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(taskSelected(QModelIndex)));
}

TaskTableView::~TaskTableView()
{
}

void TaskTableView::setJobModel(JobModel *model)
{
	job = model;
	setModel(model);
}

void TaskTableView::taskSelected(const QModelIndex &current)
{
	emit taskDoubleClicked(job->curJob->getTask(current.row()));
}

JobModel *TaskTableView::getJobModel()
{
	return job;
}

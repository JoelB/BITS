#include <QtGui>

#include <iostream>

#include "projectmodel.h"

ProjectModel::ProjectModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void ProjectModel::setParent(Project *parent)
{
	curProject = parent;
}

ProjectModel::~ProjectModel()
{
}

bool ProjectModel::hasChildren(const QModelIndex &index) const
{
	if (index.parent().isValid())
		return false;
	return true;
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
{
	Job *tempJob;
	if (hasIndex(row, column, parent))
	{
		tempJob = dynamic_cast<Job*>(static_cast<Role*>(parent.internalPointer()));
		if (tempJob) // task
			return createIndex(row,0,curProject->getJob(parent.row())->getTask(row));
		else if (!parent.isValid())
			return createIndex(row,0,curProject->getJob(row));
	}
	
    return QModelIndex();
}

QModelIndex ProjectModel::parent(const QModelIndex &child) const
{
	Role *roleNode;
	Task *taskNode;
	if (child.isValid())
	{
		roleNode = static_cast<Role*>(child.internalPointer());
		taskNode = dynamic_cast<Task*>(roleNode);
		if (taskNode)
			return createIndex(curProject->jobPosition(taskNode->parentJob),0,taskNode->parentJob);
	}
	return QModelIndex();
}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return curProject->getJob(parent.row())->taskCount();
	else
		return curProject->jobCount();
}

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid()) // at least a job
	{
		if (index.parent().isValid()) // task
		{
			if (role == Qt::DisplayRole)
				return curProject->getJob(index.parent().row())->getTask(index.row())->getAttributeValue("Name").toString();
			else if (role == Qt::DecorationRole)
				return QPixmap("Icons/task.png");
		}
		if (role == Qt::DisplayRole)
			return curProject->getJob(index.row())->getAttributeValue("Name").toString();
		else if (role == Qt::DecorationRole)
			return QPixmap("Icons/project.png");
	}
	return QVariant();
}

void ProjectModel::reload()
{
	reset();
}

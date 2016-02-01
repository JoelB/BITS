#include "project.h"

Project::Project()
{
	projectTreeModel = new ProjectModel;
	projectTreeModel->setParent(this);
}

Job* Project::getJob(int index)
{
	return jobs.at(index);
}

void Project::addJob(Job *newJob)
{
	newJob->parentProject = this;
	for (int i=0;i<newJob->taskCount();i++)
		newJob->getTask(i)->setHotKey();
	newJob->makeTaskIDs();
	jobs.append(newJob);
	qDebug() << "Add Job Called";
}

int	Project::jobCount()
{
	return jobs.size();
}

int Project::jobPosition(Job *j)
{
	return jobs.indexOf(j);
}

bool Project::addHotKey(QString newShortcut)
{
	if (!newShortcut.isEmpty())
		if (hotkeys.indexOf(QString(newShortcut)) != -1)
		{
			qDebug() << "Duplicate hot key not added!";
			 return false;			
		}
	hotkeys << newShortcut;
	return true;
}

bool Project::removeHotKey(QString shortcut)
{
	if (!shortcut.isEmpty())
		if (hotkeys.indexOf(QString(shortcut)) == -1)
			return false;
	hotkeys.remove(hotkeys.indexOf(shortcut));
	return true;
}

ProjectModel* Project::getTreeModel()
{
	return projectTreeModel;
}

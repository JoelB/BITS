#ifndef PROJECTH
#define PROJECTH

#include <QtGui>

#include "job.h"
#include "projectmodel.h"

class ProjectModel;

class Project
{
	public:
		Project();
		Job* getJob(int index);
		void addJob(Job *newJob);
		int  jobCount();
		int  jobPosition(Job *j);
		ProjectModel* getTreeModel();
		
		bool addHotKey(QString newShortcut);
		bool removeHotKey(QString shortcut);
	private:
		QVector<Job*> jobs;
		ProjectModel *projectTreeModel;
		QVector<QString> hotkeys;

};

#endif

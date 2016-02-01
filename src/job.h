#ifndef JOB_H
#define JOB_H

#include <QtGui>
#include "task.h"
#include "jobmodel.h"
#include "project.h"
#include "xmlparser.h"

class Project;
class JobModel;
class Task;

typedef struct
{
	QDateTime start;
	QDateTime end;
} taskSession;

class Job : public Role
{	
	Q_OBJECT
	
	public:
		Job();
		~Job();
		
		bool 	createFromFile(QString filename);
		
		bool	addTask(Task *newTask);
		Task*	getTask(int index);
		int		taskCount();
		void 	makeTaskIDs();

		JobModel*	getUIModel();
		
		bool 	generateReport(QDateTime startDate,QDateTime endDate, QString filename);
		bool 	writeConfigFile();
		
		Project *parentProject;
		
	signals:
		void 	statusChanged(QString);
		
	private:
		int		taskIds;
		int		num;
		QVector<Task *> tasks;
		JobModel *taskTableModel;
		

		
		
};

#endif

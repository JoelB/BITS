#ifndef TASK_H
#define TASK_H

#include <QtGui>

#include "role.h"
#include "job.h"
#include "taskattributemodel.h"
#include "hotkeydialog.h"
#include "ui_bits.h"

class Job;
class TableModel;

class Task : public Role
{
	Q_OBJECT
	
	public:
		Task();
		~Task();
		
		QMap<QString,QVariant> *getMap();
		TableModel  *getAttributeModel();
		
		Job *parentJob;
		
		void changeAttributeValue(QString attributeName,QVariant attributeValue);
		
		bool setHotKey();
		void removeHotKey();
		
		bool isRunning();
		bool isCompleted();
		
		Ui::BitsGUI *guiLink;	
		bool writeConfigFile();		
		
	public slots:
		void startWork();
		void stopWork();
		void toggleWork();
		
	signals:
		void statusChanged(QString);
		
		void taskStarted(Task*);
		void taskStopped(Task*);
		
	private:
		TableModel *attributeModel;
		QDateTime sessionStartTime;
		QDateTime sessionEndTime;
		bool working;
		
		QAction *hotKeyAct;
};

#endif

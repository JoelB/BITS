#ifndef bitsmw_H
#define bitsmw_H

#include "ui_bits.h"
#include "projectmodel.h"
#include "jobmodel.h"
#include "tasktableview.h"
#include "reportdialog.h"

class ProjectModel;

class BitsMW : public QMainWindow
{
    Q_OBJECT

	public:
		BitsMW(QWidget *parent = 0);
	protected slots:
		void treeItemSelected(const QModelIndex &current);
		void updateTotalTime();
	
		void newJobClicked();
		
		void switchToJob(Job *selectedJob);
		void switchToTask(Task *selectedTask);
		void setHotKey();
		void startTaskPressed();
		void stopTaskPressed();
		
		void taskStarted(Task*);
		void taskStopped(Task*);
		
		void progressToggle(bool selected);
		
		void updateStatusBar(QString message);
		
		void generateReportPressed();
	private:
		Ui::BitsGUI ui;
		ProjectModel *model;
		TaskTableView *taskTable;
		Project *bitsProject;
		Task *curTask;
		Job *curJob;

};
#endif

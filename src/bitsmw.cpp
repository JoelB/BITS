#include <QtGui>

#include <iostream>

#include "bitsmw.h"
#include "hotkeyedit.h"
#include "hotkeydialog.h"
#include "newjobdialog.h"

BitsMW::BitsMW(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	
	bitsProject = new Project;
	
	QDir  jobsDirectory("Jobs");
	
	QStringList filters;
    filters << "*.xml";
    jobsDirectory.setNameFilters(filters);	
	
	QStringList jobFileList = jobsDirectory.entryList();

	QStringList property;
	Job *tempJob = 0;
	Task *tempTask = 0;
	for (int i=0;i<jobFileList.size();i++)
	{
		tempJob = new Job;
		tempJob->createFromFile(jobsDirectory.filePath(jobFileList.at(i)));
		if (!tempJob)
			continue;
		for (int j=0; j < tempJob->taskCount(); j++)
		{
			tempTask = tempJob->getTask(j);
			tempTask->guiLink = &ui;
			connect(tempTask,SIGNAL(statusChanged(QString)),this,SLOT(updateStatusBar(QString)));
			connect(tempTask,SIGNAL(taskStarted(Task*)),this,SLOT(taskStarted(Task*)));
			connect(tempTask,SIGNAL(taskStopped(Task*)),this,SLOT(taskStopped(Task*)));
		}
		connect(tempJob,SIGNAL(statusChanged(QString)),this,SLOT(updateStatusBar(QString)));
		bitsProject->addJob(tempJob);
	}
		
	
	
	model = bitsProject->getTreeModel();
	
	ui.treeView->setModel(model);

	QList<int> list;
	list.append(200);
	list.append(600);
	ui.mainLayout->setSizes(list);
	
	taskTable = new TaskTableView(ui.dataViewSplitter);
	ui.dataViewSplitter->addWidget(taskTable);
	ui.taskProperties->hide();
	
	connect(ui.treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(treeItemSelected(QModelIndex)));	
	connect(taskTable,SIGNAL(taskDoubleClicked(Task*)),this,SLOT(switchToTask(Task*)));
	
	connect(ui.generateReport,SIGNAL(pressed()),this,SLOT(generateReportPressed()));
	connect(ui.newJobAction,SIGNAL(triggered()),this,SLOT(newJobClicked()));
}
void BitsMW::switchToJob(Job *selectedJob)
{
	curJob = selectedJob;
	
	if (!curTask)
	{
		ui.taskProperties->hide();
		taskTable->show();		
	}

	disconnect(ui.jobNameTextBox,SIGNAL(textChanged(QString)),0,0);
	disconnect(ui.jobStartDate,SIGNAL(dateChanged(QDate)),0,0);
	disconnect(ui.jobEndDate,SIGNAL(dateChanged(QDate)),0,0);
	disconnect(ui.jobStartTime,SIGNAL(timeChanged(QTime)),0,0);
	disconnect(ui.jobEndTime,SIGNAL(timeChanged(QTime)),0,0);

	ui.jobNameTextBox->setText(selectedJob->getAttributeValue("Name").toString());
	ui.jobStartDate->setDate(selectedJob->getAttributeValue("StartDate").toDateTime().date());
	ui.jobEndDate->setDate(selectedJob->getAttributeValue("EndDate").toDateTime().date());
	ui.jobStartTime->setTime(selectedJob->getAttributeValue("StartDate").toDateTime().time());
	ui.jobEndTime->setTime(selectedJob->getAttributeValue("EndDate").toDateTime().time());
	ui.jobFilename->setText(selectedJob->getAttributeValue("Filename").toString());
	
	connect(ui.jobNameTextBox,SIGNAL(textChanged(QString)),selectedJob,SLOT(changeName(QString)));
	connect(ui.jobStartDate,SIGNAL(dateChanged(QDate)),selectedJob,SLOT(changeStartDate(QDate)));
	connect(ui.jobEndDate,SIGNAL(dateChanged(QDate)),selectedJob,SLOT(changeEndDate(QDate)));
	connect(ui.jobStartTime,SIGNAL(timeChanged(QTime)),selectedJob,SLOT(changeStartTime(QTime)));
	connect(ui.jobEndTime,SIGNAL(timeChanged(QTime)),selectedJob,SLOT(changeEndTime(QTime)));
	
	taskTable->setJobModel(selectedJob->getUIModel());

	QHeaderView *hv = taskTable->horizontalHeader();
	hv->setResizeMode(0,QHeaderView::Stretch);
	hv->setResizeMode(1,QHeaderView::Stretch);
	hv->setResizeMode(2,QHeaderView::Stretch);
	hv->setResizeMode(3,QHeaderView::Stretch);	
}
void BitsMW::switchToTask(Task *selectedTask)
{
	taskTable->hide();
	ui.taskProperties->show();
	
	curTask = selectedTask;
	
	switchToJob(selectedTask->parentJob);

	updateTotalTime();
	
	if (selectedTask->isCompleted())
	{
		ui.startWorkButton->setEnabled(false);
		ui.stopWorkButton->setEnabled(false);		
	}
	else if (selectedTask->isRunning())
	{
		ui.startWorkButton->setEnabled(false);
		ui.stopWorkButton->setEnabled(true);
	}
	else
	{
		ui.startWorkButton->setEnabled(true);
		ui.stopWorkButton->setEnabled(false);
	}
	
	if (!selectedTask->getAttributeValue("Completed").toString().isEmpty())
		ui.completeRadio->setChecked(true);
	else
		ui.incompleteRadio->setChecked(true);
		
		
	
	disconnect(ui.startWorkButton,SIGNAL(clicked()),0,0);
	disconnect(ui.stopWorkButton,SIGNAL(clicked()),0,0);
	disconnect(ui.setHotKey,SIGNAL(clicked()),0,0);
	
	disconnect(ui.taskStartDate,SIGNAL(dateChanged(QDate)),0,0);
	disconnect(ui.taskEndDate,SIGNAL(dateChanged(QDate)),0,0);
	disconnect(ui.taskStartTime,SIGNAL(timeChanged(QTime)),0,0);
	disconnect(ui.taskEndTime,SIGNAL(timeChanged(QTime)),0,0);
	disconnect(ui.taskName,SIGNAL(textChanged(QString)),0,0);
	
	disconnect(ui.incompleteRadio,SIGNAL(toggled(bool)),0,0);

	ui.taskName->setText(selectedTask->getAttributeValue("Name").toString());
	ui.taskStartDate->setDate(selectedTask->getAttributeValue("StartDate").toDateTime().date());
	ui.taskEndDate->setDate(selectedTask->getAttributeValue("EndDate").toDateTime().date());
	ui.taskStartTime->setTime(selectedTask->getAttributeValue("StartDate").toDateTime().time());
	ui.taskEndTime->setTime(selectedTask->getAttributeValue("EndDate").toDateTime().time());
	
	int timeSpent = selectedTask->getAttributeValue("TimeSpent").toInt();
	QString timeTotal;
	timeTotal = QString::number(timeSpent/3600) + " hours " + QString::number((timeSpent%3600)/60) + " minutes " + QString::number((timeSpent%3600)%60) + " seconds";
	ui.taskTotalTime->setText(timeTotal);

	ui.attributeView->setModel(selectedTask->getAttributeModel());
	
	connect(ui.startWorkButton,SIGNAL(clicked()),this,SLOT(startTaskPressed()));
	connect(ui.stopWorkButton,SIGNAL(clicked()),this,SLOT(stopTaskPressed()));
	connect(ui.setHotKey,SIGNAL(clicked()),this,SLOT(setHotKey()));
	
	connect(ui.taskStartDate,SIGNAL(dateChanged(QDate)),selectedTask,SLOT(changeStartDate(QDate)));
	connect(ui.taskEndDate,SIGNAL(dateChanged(QDate)),selectedTask,SLOT(changeEndDate(QDate)));
	connect(ui.taskStartTime,SIGNAL(timeChanged(QTime)),selectedTask,SLOT(changeStartTime(QTime)));
	connect(ui.taskEndTime,SIGNAL(timeChanged(QTime)),selectedTask,SLOT(changeEndTime(QTime)));
	connect(ui.taskName,SIGNAL(textChanged(QString)),selectedTask,SLOT(changeName(QString)));
	
	connect(ui.completeRadio,SIGNAL(toggled(bool)),this,SLOT(progressToggle(bool)));
	
	QHeaderView *hv = ui.attributeView->horizontalHeader();
	hv->setStretchLastSection(true);
}

void BitsMW::treeItemSelected(const QModelIndex &current)
{
	if (current.isValid())
	{
		if (current.parent().isValid()) // task selected
		{
			switchToTask(bitsProject->getJob(current.parent().row())->getTask(current.row()));
		}
		else // job selected
		{
			curTask = 0; // not working on a task
			switchToJob(bitsProject->getJob(current.row()));
		}
	}
}

void BitsMW::updateTotalTime()
{
	if (curTask)
	{
		int timeSpent = curTask->getAttributeValue("TimeSpent").toInt();
		QString timeTotal;
		timeTotal = QString::number(timeSpent/3600) + " hours " + QString::number((timeSpent%3600)/60) + " minutes " + QString::number((timeSpent%3600)%60) + " seconds";
		ui.taskTotalTime->setText(timeTotal);
		
		QTimer::singleShot(500, this, SLOT(updateTotalTime()));
	}
}

void BitsMW::setHotKey()
{
	HotKeyDialog *newHK;
	if (curTask)
	{
		newHK = new HotKeyDialog(curTask->getAttributeValue("HotKey").toString());
		if (newHK->exec() == QDialog::Accepted)
		{
			if (newHK->getHotKey().isEmpty())
			{
				curTask->removeHotKey();
			}
			else
			{
				curTask->addAttribute("HotKey");
				curTask->setAttributeValue("HotKey",newHK->getHotKey());
				curTask->getAttributeModel()->reload();
				if (curTask->setHotKey())
					ui.statusbar->showMessage(QString("Hot key ").append(newHK->getHotKey()).append(" added"),3000);
				else
					ui.statusbar->showMessage("Hot key is use! Not set.",3000);
			}
		}
		delete newHK;
	}
}

void BitsMW::startTaskPressed()
{
	if (!curTask->isRunning())
		curTask->startWork();
	ui.stopWorkButton->setEnabled(true);
	ui.startWorkButton->setEnabled(false);
}

void BitsMW::stopTaskPressed()
{
	if (curTask->isRunning())
		curTask->stopWork();
	ui.stopWorkButton->setEnabled(false);
	ui.startWorkButton->setEnabled(true);
}

void BitsMW::generateReportPressed()
{
	ReportDialog reportDlg(this);
	if (reportDlg.exec() == QDialog::Accepted)
	{
		QDateTime startDate;
		QDateTime endDate;
		startDate.setDate(reportDlg.getStartDate());
		startDate.setTime(reportDlg.getStartTime());
		endDate.setDate(reportDlg.getEndDate());
		endDate.setTime(reportDlg.getEndTime());
		curJob->generateReport(startDate,endDate,reportDlg.getFilename());
	}
}

void BitsMW::progressToggle(bool selected)
{
	if (ui.incompleteRadio->isChecked())
	{
		curTask->changeAttributeValue("Completed","");
		if (curTask->isRunning())
		{
			ui.stopWorkButton->setEnabled(true);
			ui.startWorkButton->setEnabled(false);			
		}
		else
		{
			ui.stopWorkButton->setEnabled(false);
			ui.startWorkButton->setEnabled(true);
		}

		curTask->getAttributeModel()->reload();
	}
	else
	{
		if (curTask->getAttributeValue("Completed").toString().isEmpty())
		{
			curTask->changeAttributeValue("Completed",QDateTime::currentDateTime());
			if (curTask->isRunning())
				curTask->stopWork();
			ui.stopWorkButton->setEnabled(false);
			ui.startWorkButton->setEnabled(false);
			curTask->getAttributeModel()->reload();
		}
	}
}	

void BitsMW::updateStatusBar(QString message)
{
	ui.statusbar->showMessage(message,4000);
}

void BitsMW::newJobClicked()
{
	NewJobDialog dialog;
	if (dialog.exec() == QDialog::Accepted)
	{
		if (dialog.fromProfileSelected())
		{
			QString oldFilename = dialog.getFilename();
			QString newFilename = QFileDialog::getSaveFileName(this, tr("Save Job As"), "Jobs",
                            tr("XML Files (*.xml)"));
			
			if (!newFilename.contains("Jobs/"))
			{
				updateStatusBar("Invalid file location: Job not added!");
				return;
			}
				
			if (newFilename.right(4) != ".xml")
				newFilename.append(".xml");
				
			QFile::copy(oldFilename,newFilename);
			Job *tempJob = new Job;
			tempJob->createFromFile(newFilename);
			Task *tempTask;
			if (tempJob)
			{
				for (int j=0; j < tempJob->taskCount(); j++)
				{
					tempTask = tempJob->getTask(j);
					tempTask->guiLink = &ui;
					connect(tempTask,SIGNAL(statusChanged(QString)),this,SLOT(updateStatusBar(QString)));
					connect(tempTask,SIGNAL(taskStarted(Task*)),this,SLOT(taskStarted(Task*)));
					connect(tempTask,SIGNAL(taskStopped(Task*)),this,SLOT(taskStopped(Task*)));
				}
				connect(tempJob,SIGNAL(statusChanged(QString)),this,SLOT(updateStatusBar(QString)));
				bitsProject->addJob(tempJob);
				model->reload();
			}
		}
		else
		{
			Task *tempTask;
			Job *tempJob = new Job;

			QDateTime startDate = QDateTime::currentDateTime();//fromString("2006-01-01T08:00:00",Qt::ISODate);
			QDateTime endDate = QDateTime::currentDateTime().addDays(7);//fromString("2006-12-01T17:00:00",Qt::ISODate);
			
			QString newFilename = QFileDialog::getSaveFileName(this, tr("Save Job As"), "Jobs",
                            tr("XML Files (*.xml)"));
			
			if (!newFilename.contains("Jobs/"))
			{
				updateStatusBar("Invalid file location: Job not added!");
				return;
			}
			if (newFilename.right(4) != ".xml")
				newFilename.append(".xml");
			
			tempJob->setAttributeValue("Name",dialog.getJobName());
			tempJob->setAttributeValue("StartDate",startDate);
			tempJob->setAttributeValue("EndDate",endDate);
			tempJob->setAttributeValue("Filename",newFilename);
			for (int i = 0;i < dialog.getNumTasks(); i++)
			{
				tempTask = new Task;
				tempTask->setAttributeValue("Name",QString("Task %1").arg(i+1));
				tempTask->setAttributeValue("StartDate",startDate);
				tempTask->setAttributeValue("EndDate",endDate);
				tempTask->setAttributeValue("Filename",newFilename);
				tempTask->guiLink = &ui;
				tempJob->addTask(tempTask);
				connect(tempTask,SIGNAL(statusChanged(QString)),this,SLOT(updateStatusBar(QString)));
				connect(tempTask,SIGNAL(taskStarted(Task*)),this,SLOT(taskStarted(Task*)));
				connect(tempTask,SIGNAL(taskStopped(Task*)),this,SLOT(taskStopped(Task*)));
			}
			tempJob->writeConfigFile();
			bitsProject->addJob(tempJob);
			connect(tempJob,SIGNAL(statusChanged(QString)),this,SLOT(updateStatusBar(QString)));
			model->reload();
		}
	}
}

void BitsMW::taskStarted(Task *startedTask)
{
	if (startedTask == curTask)
		startTaskPressed();
}

void BitsMW::taskStopped(Task *stoppedTask)
{
	if (stoppedTask == curTask)
		stopTaskPressed();
}


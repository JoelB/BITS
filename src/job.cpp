#include "job.h"

Job::Job()
{
	taskTableModel = new JobModel();
	taskTableModel->curJob = this;
	parentProject = 0;
	taskIds = 0;
}

bool Job::createFromFile(QString filename)
{
	XMLParser bitsParser;
	Task *tempTask;
	QFile *xmlFile = new QFile(filename);
	bitsParser.parseFile(xmlFile);
	
	QMapIterator<QString, QMap<QString,QVariant> > it(bitsParser.properties);
	while (it.hasNext())
	{
		it.next();
		if (it.key() == "Job")
			setAttributeValue("Filename",filename);
		else if (it.key() == "Task")
		{
			tempTask = new Task;
			tempTask->setAttributeValue("Filename",filename);
		}
		else
			continue;
		
		QMapIterator<QString,QVariant> j(it.value());
		while (j.hasNext())
		{
			j.next();
			if (it.key() == "Job")
			{
				addAttribute(j.key());
				setAttributeValue(j.key(),j.value());
			}
			else if (it.key() == "Task" && tempTask)
			{
				if (j.key() != "Name" && j.key() != "StartDate" && j.key() != "EndDate" && j.key() != "TimeSpent" && j.key() != "TaskID" && j.key() != "HotKey" && j.key() != "Completed")
					tempTask->addAttribute(j.key());
				tempTask->setAttributeValue(j.key(),j.value());
			}
		}
		if (it.key() == "Task" && tempTask)
		{
			if (tempTask->isValid())
				addTask(tempTask);	
			else
				emit statusChanged("Invalid Task not added!");
		}
			
	}
	if (isValid())
	{
		makeTaskIDs();
		writeConfigFile();
	}
	else
		emit statusChanged(QString("Invalid Job in %1 not added!").arg(filename));
	xmlFile->close();
	delete xmlFile;
	return true;
}

Job::~Job()
{
	
}

JobModel* Job::getUIModel()
{
	return taskTableModel;
}

bool Job::addTask(Task *newTask)
{
	newTask->parentJob = this;
	tasks.append(newTask);
	return true;
}

Task* Job::getTask(int index)
{
	return tasks.at(index);
}

int	Job::taskCount()
{
	return tasks.size();
}

void Job::makeTaskIDs()
{
	/* find maximum taskID */
	int highest = 0;
	bool convertOK;
	Task *tempTask;
	QVector<int> needTaskID; // holds indexes of tasks that need an ID
	QVector<int> currentTaskIDs; // holds used IDs
	for (int i = 0; i < taskCount(); i++)
	{
		tempTask = getTask(i);
		int tempID = tempTask->getAttributeValue("TaskID").toInt(&convertOK);
		if (convertOK && tempID > 0 && !currentTaskIDs.contains(tempID))
		{
			currentTaskIDs.append(tempID);
			if (tempID > highest)
				highest = tempID;				
		}
		else
			needTaskID.append(i);
	}
	highest++;
	
	for (int i=0; i < needTaskID.size(); i++)
		getTask(needTaskID.at(i))->changeAttributeValue("TaskID",highest++);		
}

bool Job::writeConfigFile()
{
	QFile configFile(getAttributeValue("Filename").toString());
	if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		emit statusChanged(QString("Failed to open config file for job \'").append(getAttributeValue("Name").toString()).append("\""));
		return false;
	}

	
	QTextStream out(&configFile);
	out << "<Job>\n";
	QMapIterator<QString,QVariant> it(attributeValues);
	while (it.hasNext())
	{
		it.next();
		if (it.key() == "Filename")
			continue;
		out << "\t<" << it.key();
		
		if (it.value().type() == QVariant::DateTime)
			out << " type:DateTime>" << it.value().toDateTime().toString(Qt::ISODate);
		else if (it.value().type() == QVariant::Int)
			out << " type:Integer>" << QString().setNum(it.value().toInt());
		else
			out << ">" << it.value().toString();
		
		out << "</" << it.key() << ">" << "\n";
	}
	out << "</Job>\n\n";
	
	Task *tempTask;
	QVectorIterator<Task*> taskIter(tasks);
 	while (taskIter.hasNext())
 	{
 		tempTask = taskIter.next();
 		out << "<Task>\n";
 		QMapIterator<QString,QVariant> attIter(*tempTask->getMap());
 		while (attIter.hasNext())
 		{
 			attIter.next();
 			if (attIter.key() == "Filename")
 				continue;
 			out << "\t<" << attIter.key();
		
			if (attIter.value().type() == QVariant::DateTime)
				out << " type:DateTime>" << attIter.value().toDateTime().toString(Qt::ISODate);
			else if (attIter.value().type() == QVariant::Int)
				out << " type:Integer>" << QString().setNum(attIter.value().toInt());
			else
				out << ">" << attIter.value().toString();
			
			out << "</" << attIter.key() << ">" << "\n";
		}
 		out << "</Task>\n";
 		
	}
	configFile.close();
	emit statusChanged(QString("Updated config file for job \'").append(getAttributeValue("Name").toString()).append("\"")); 
	return true;
}

bool Job::generateReport(QDateTime startDate,QDateTime endDate,QString filename)
{
	QString currentLine;
	QStringList sessionParts;
	int taskId;

	QMultiMap<int,taskSession> taskSessions;
	
	QFile sessionFile(attributeValues["Filename"].toString().replace(QString(".xml"),QString(".session")));
	if (!sessionFile.open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << "Failed to session file!";

	while (!sessionFile.atEnd())
	{
		currentLine = QString(sessionFile.readLine());
		sessionParts = currentLine.split("%");
		taskSession tempsession;
		taskId = sessionParts.at(0).toInt();
		tempsession.start = QDateTime::fromString(sessionParts.at(1),Qt::ISODate);
		tempsession.end = QDateTime::fromString(sessionParts.at(2),Qt::ISODate);
		taskSessions.insert(taskId,tempsession);
	}
	sessionFile.close();
	
	QMap<int,int> totalTimes;
	QMapIterator<int,taskSession> iter(taskSessions);
	while (iter.hasNext())
	{
		iter.next();
		int timeSum = 0;
		QList<taskSession> sessions = taskSessions.values(iter.key());
		for (int i = 0; i < sessions.size(); i++)
		{
			taskSession tempSession = sessions.at(i);
			if (tempSession.start >= startDate && tempSession.end <= endDate) // session happened completely in range
				timeSum += tempSession.start.secsTo(tempSession.end);
			else if (tempSession.start <= startDate && tempSession.end <=endDate) // started out of range, ended in range
				timeSum += startDate.secsTo(tempSession.end);
			else if (tempSession.start >= startDate && tempSession.end >= endDate) // started in range, ended out of range
				timeSum += tempSession.start.secsTo(endDate);
			else if (tempSession.start <= startDate && tempSession.end >= endDate) // continued through entire range
				timeSum += startDate.secsTo(endDate);
		}
		totalTimes.insert(iter.key(),timeSum);
	}
	
	QFile reportFile(filename);
	if (!reportFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Failed to open new report file!";
		return false;
	}
	
	QTextStream out(&reportFile);
	out << "Job Session Report for " << startDate.toString() << " to " << endDate.toString() << "\n\n";
	out << "Job Name: " << getAttributeValue("Name").toString() << "\n";
	out << "Job Start Date: " << getAttributeValue("StartDate").toDateTime().toString() << "\n";
	out << "Job End Date: " << getAttributeValue("EndDate").toDateTime().toString() << "\n\n";
	Task *tempTask;
	int allTime = 0;
	QStringList completedTasksOutput;
	QStringList otherTasksOutput;
	for (int i=0;i<taskCount();i++)
	{
		tempTask = getTask(i);
		QString name = tempTask->getAttributeValue("Name").toString();
		QString taskNum = QString::number(tempTask->getAttributeValue("TaskID").toInt()).prepend("   ");
		
		for (int j=taskNum.size(); j < 12; j++)
			taskNum.append(" ");
		for (int j=name.size(); j < 25; j++)
			name.append(" ");
		int timeSpent = totalTimes.value(tempTask->getAttributeValue("TaskID").toInt());
		allTime += timeSpent; // keep track of total time for percentages
		QString timeTotal = QString::number(timeSpent/3600) + "hr " + QString::number((timeSpent%3600)/60) + "min " + QString::number((timeSpent%3600)%60) + "sec";

		QDateTime completedDate = QDateTime::fromString(tempTask->getAttributeValue("Completed").toString(),Qt::ISODate);
		if (completedDate >= startDate && completedDate <= endDate)
			completedTasksOutput.append(QString(taskNum + name + timeTotal));
		else if (timeSpent > 0)
			otherTasksOutput.append(QString(taskNum + name + timeTotal));
	}
	completedTasksOutput.sort();
	otherTasksOutput.sort();
	
	out << "Tasks completed during selected time:\n\n";
	out << "\tTask ID     Task Name                 Time Spent            % of Total Time\n";
	out << "\t------------------------------------------------------------------------\n";
	QString tempOutput;
	for (int i = 0; i < completedTasksOutput.size(); i++)
	{
		tempOutput = completedTasksOutput.at(i);
		for (int j = tempOutput.size(); j < 65; j++)
			tempOutput.append(" ");
		QString percentage = QString::number(((double)totalTimes.value(tempOutput.left(10).simplified().toInt())/(double)allTime)*100.0,'f',2);
		out << "\t" << tempOutput << percentage << "\n";
	}
		
	out << "\n\nTasks worked on but not completed during selected time:\n\n";
	out << "\tTask ID     Task Name                 Time Spent            % of Total Time\n";
	out << "\t------------------------------------------------------------------------\n";
	for (int i = 0; i < otherTasksOutput.size(); i++)
	{
		tempOutput = otherTasksOutput.at(i);
		for (int j = tempOutput.size(); j < 65; j++)
			tempOutput.append(" ");
		QString percentage = QString::number((double)(totalTimes.value(tempOutput.left(10).simplified().toInt())/(double)allTime)*100.0,'f',2);
		out << "\t" << tempOutput << percentage << "\n";
	}
		
	reportFile.close();
	return true;
}

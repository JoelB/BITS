#include "task.h"

Task::Task()
{
	attributeModel = new TableModel(this);
	hotKeyAct = new QAction(this);
	working = false;
	guiLink = 0;
	connect(hotKeyAct, SIGNAL(triggered()), this, SLOT(toggleWork()));
}

Task::~Task()
{
}

void Task::changeAttributeValue(QString attributeName,QVariant attributeValue)
{
	attributeValues[attributeName] = attributeValue;
	parentJob->writeConfigFile();
}

TableModel *Task::getAttributeModel()
{
	return attributeModel;
}

void Task::startWork()
{
	if (!isCompleted())
	{
		if (!isRunning())
		{
			sessionStartTime = QDateTime::currentDateTime();
			working = true;
			emit statusChanged(QString("Task \"").append(getAttributeValue("Name").toString()).append("\" started."));	
			emit taskStarted(this);	
		}
	}
	else
		emit statusChanged("Cannot start a completed task!");
}

void Task::toggleWork()
{
	if (working)
		stopWork();
	else
		startWork();
}

void Task::stopWork()
{
	if (working)
	{
		sessionEndTime = QDateTime::currentDateTime();
		attributeValues["TimeSpent"] = attributeValues["TimeSpent"].toInt() + -1*sessionEndTime.secsTo(sessionStartTime);
		QFile file(attributeValues["Filename"].toString().replace(QString(".xml"),QString(".session")));
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		{
			emit statusChanged("Failed to opens session file!");
			return;
		}
		QTextStream out(&file);
	    out << getAttributeValue("TaskID").toString() << "%" << sessionStartTime.toString(Qt::ISODate) << "%" << sessionEndTime.toString(Qt::ISODate) << "\n";
	    file.close();
	    parentJob->writeConfigFile();
    	working = false;
    	emit statusChanged(QString("Task \"").append(getAttributeValue("Name").toString()).append("\" stopped."));
    	emit taskStopped(this);
	}
}

QMap<QString,QVariant> *Task::getMap()
{
	return &attributeValues;
}



bool Task::setHotKey()
{
	if (parentJob->parentProject->addHotKey(attributeValues["HotKey"].toString()))
	{
		hotKeyAct->setShortcut(QKeySequence::fromString(attributeValues["HotKey"].toString()));
		if (guiLink)
			guiLink->hotkeyHolder->addAction(hotKeyAct);
		writeConfigFile();
		return true;
	}
	else
	{
		attributeValues["HotKey"] = QString("");
		writeConfigFile();
		return false;
	}
}

void Task::removeHotKey()
{
	if (!attributeValues["HotKey"].toString().isEmpty())
		parentJob->parentProject->removeHotKey(attributeValues["HotKey"].toString());
	attributeValues["HotKey"] = QString("");
	if (guiLink)
		guiLink->hotkeyHolder->removeAction(hotKeyAct);
}

bool Task::isRunning()
{
	return working;
}

bool Task::writeConfigFile()
{
	return parentJob->writeConfigFile();
}

bool Task::isCompleted()
{
	if (attributeValues["Completed"].toString().isEmpty())
		return false;
	return true;
}

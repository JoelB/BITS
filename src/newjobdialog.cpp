#include "newjobdialog.h"

NewJobDialog::NewJobDialog()
{
	dialog.setupUi(this);
	
	connect(dialog.profileRadio,SIGNAL(toggled(bool)),this,SLOT(radioToggled(bool)));
	
	XMLParser bitsParser;
	QDir  profilesDirectory("Profiles");
	QFile *xmlFile;
	
	QStringList filters;
    filters << "*.xml";
    profilesDirectory.setNameFilters(filters);	
	
	QStringList profileFileList = profilesDirectory.entryList();

	QStringList property;
	for (int i=0;i<profileFileList.size();i++)
	{
		xmlFile = new QFile(profilesDirectory.filePath(profileFileList.at(i)));
		bitsParser.parseFile(xmlFile);
		QString name;
		
		if (bitsParser.properties.contains("Job"))
			if (bitsParser.properties.value("Job").contains("Name"))
			{
				name = bitsParser.properties.value("Job").value("Name").toString();
				QString fileName = xmlFile->fileName();
				dialog.profiles->addItem(name,fileName);			
			}

		xmlFile->close();
		delete xmlFile;
	}
}

NewJobDialog::~NewJobDialog()
{
	
}

bool NewJobDialog::fromProfileSelected()
{
	return dialog.profileRadio->isChecked();
}

QString NewJobDialog::getFilename()
{
	return dialog.profiles->itemData(dialog.profiles->currentIndex()).toString();
}

QString NewJobDialog::getJobName()
{
	return dialog.jobName->text();
}

int NewJobDialog::getNumTasks()
{
	return dialog.numTasks->value();
}

void NewJobDialog::radioToggled(bool selected)
{
	if (dialog.profileRadio->isChecked())
	{
		dialog.profileWidget->setEnabled(true);
		dialog.emptyWidget->setEnabled(false);
	}
	else
	{
		dialog.profileWidget->setEnabled(false);
		dialog.emptyWidget->setEnabled(true);
	}
}

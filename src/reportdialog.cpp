#include "reportdialog.h"

ReportDialog::ReportDialog(QWidget *parent) : QDialog(parent)
{
	reportDialog.setupUi(this);
	setModal(true);
	connect(reportDialog.genButton,SIGNAL(pressed()),this,SLOT(generatePressed()));
	connect(reportDialog.cancelButton,SIGNAL(pressed()),this,SLOT(cancelPressed()));
}

ReportDialog::~ReportDialog()
{
	
}

void ReportDialog::cancelPressed()
{
	done(QDialog::Rejected);
}

void ReportDialog::generatePressed()
{
	fileName = QFileDialog::getSaveFileName(this, tr("Save Report As"), "Reports",
                            tr("Text Files (*.txt)"));
                            
    if (fileName.right(4) != ".txt")
		fileName.append(".txt");
	
	done(QDialog::Accepted);
}

QDate ReportDialog::getStartDate()
{
	return reportDialog.startDate->date();
}

QTime ReportDialog::getStartTime()
{
	return reportDialog.startTime->time();
}

QDate ReportDialog::getEndDate()
{
	return reportDialog.endDate->date();
}

QTime ReportDialog::getEndTime()
{
	return reportDialog.endTime->time();
}

QString ReportDialog::getFilename()
{
	return fileName;
}

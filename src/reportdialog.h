#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QtGui>
#include "ui_report.h"

class ReportDialog : public QDialog
{
	Q_OBJECT
	
	public:
		ReportDialog(QWidget *parent);
		~ReportDialog();
	
		QDate getStartDate();
		QTime getStartTime();
		QDate getEndDate();
		QTime getEndTime();
		QString getFilename();

	protected slots:
		void cancelPressed();
		void generatePressed();
		
	private:
		Ui::ReportDlg reportDialog;
		
		QString fileName;
};

#endif

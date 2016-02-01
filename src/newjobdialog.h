#ifndef NEWJOBDIALOG_H
#define NEWJOBDIALOG_H

#include "ui_newjob.h"
#include "xmlparser.h"

class NewJobDialog : public QDialog
{
	Q_OBJECT
	
	public:
		NewJobDialog();
		~NewJobDialog();
	
		bool 	fromProfileSelected();
		QString getFilename();
		QString getJobName();
		int		getNumTasks();
		
	protected slots:
		void radioToggled(bool selected);
	
	private:
		Ui::NewJobDlg dialog;
		QStandardItemModel *comboModel;
};

#endif

#ifndef HOTKEYDIALOG_H
#define HOTKEYDIALOG_H

#include <QtGui>
#include "ui_hotkeydialog.h"
#include "hotkeyedit.h"

class HotKeyDialog : public QDialog
{
	Q_OBJECT
	
	public:
		HotKeyDialog(QString currentHotKey = "",QWidget *parent = 0);
		~HotKeyDialog();
		
		QString getHotKey();
	public slots:
		void checkBoxChanged(int state);
	private:
		Ui::HotKeyDlg hkDialog;
		HotKeyEdit *hkLineEdit;
};

#endif

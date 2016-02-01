#ifndef HOTKEYEDIT_H
#define HOTKEYEDIT_H

#include <QtGui>

class HotKeyEdit : public QLineEdit
{
	Q_OBJECT
	public:
		HotKeyEdit(QWidget *parent = 0);
		~HotKeyEdit();
		
	protected:
		bool eventFilter(QObject *obj, QEvent *event);
};

#endif

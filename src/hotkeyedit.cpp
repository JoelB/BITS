#include "hotkeyedit.h"

HotKeyEdit::HotKeyEdit(QWidget *parent) : QLineEdit(parent)
{
	installEventFilter(this);
}

HotKeyEdit::~HotKeyEdit()
{
	
}

bool HotKeyEdit::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) 
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		QFlags<Qt::KeyboardModifier> modifiers = keyEvent->modifiers();
		QString shortcut;
		if (modifiers.testFlag(Qt::ControlModifier))
			shortcut.append("Ctrl").append('+');
		if (modifiers.testFlag(Qt::AltModifier))
			shortcut.append("Alt").append('+');
		if (modifiers.testFlag(Qt::ShiftModifier))
			shortcut.append("Shift").append('+');
		int key = keyEvent->key();
		if (key >= Qt::Key_A && key <= Qt::Key_Z)
			shortcut.append((char)key);
		else if (key >= Qt::Key_F1 && key <= Qt::Key_F12)
			shortcut.append('F').append(QString().setNum(key-Qt::Key_F1+1));
		else
			key = 0;
			
		if (key)
			setText(shortcut);
		shortcut.clear();
		qDebug("Ate key press %X", keyEvent->key()); 
		return true;
	} 
	else 
	{
		return QObject::eventFilter(obj, event);
	}
}

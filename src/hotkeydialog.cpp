#include "hotkeydialog.h"

HotKeyDialog::HotKeyDialog(QString currentHotKey,QWidget *parent) : QDialog(parent)
{
	hkDialog.setupUi(this);
	hkLineEdit = new HotKeyEdit(this);
	
	hkDialog.tempLineEdit->hide();
	hkDialog.hboxLayout->addWidget(hkLineEdit);
	
	if (!currentHotKey.isEmpty())
	{
		hkDialog.hotKeyEnable->setCheckState(Qt::Checked);
		hkDialog.hkBox->setEnabled(true);
		hkLineEdit->setText(currentHotKey);
	}
	connect(hkDialog.hotKeyEnable,SIGNAL(stateChanged(int)),this,SLOT(checkBoxChanged(int)));
}

HotKeyDialog::~HotKeyDialog()
{
	delete hkLineEdit;
}

QString HotKeyDialog::getHotKey()
{
	return hkLineEdit->text();
}

void HotKeyDialog::checkBoxChanged(int state)
{
	if (state == Qt::Checked)
	{
		hkDialog.hkBox->setEnabled(true);
		hkLineEdit->setFocus();
	}
	else
	{
		hkLineEdit->setText("");
		hkDialog.hkBox->setEnabled(false);
	}
		
}

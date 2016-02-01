#ifndef ROLE_H
#define ROLE_H
#include <QtGui>

class Role : public QObject
{
	Q_OBJECT
	
	public:
		Role();
		virtual ~Role() {}

		
		QVariant getAttributeValue(QString attributeName);
		bool	setAttributeValue(QString attributeName,QVariant attributeValue);
		
		QString	getAttribute(int index);
		
		QMapIterator<QString,QVariant> getAttributeIter();
		bool	addAttribute(QString newAttribute);
		int 	attributeCount();
		
		bool	isValid();
		virtual bool writeConfigFile();
		
	public slots:
		void changeStartDate(const QDate& newStartDate);
		void changeEndDate(const QDate& newEndDate);
		
		void changeStartTime(const QTime& newStartTime);
		void changeEndTime(const QTime& newEndTime);
		
		void changeName(const QString& newName);
		
	signals:
		void statusChanged(QString);
		
	protected:
		QVector<QString> customAttributes;
		QMap<QString,QVariant> attributeValues;
		QString filename;
};
#endif

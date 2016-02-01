#include "role.h"

Role::Role()
{
}

bool Role::setAttributeValue(QString attributeName,QVariant attributeValue)
{
	attributeValues[attributeName] = attributeValue;
	return true;
}

QVariant Role::getAttributeValue(QString attributeName)
{
	return attributeValues[attributeName];
}


QString Role::getAttribute(int index)
{
	if (index >= customAttributes.size())
		return "Name";
	return customAttributes.at(index);
}

QMapIterator<QString,QVariant> Role::getAttributeIter()
{
	return QMapIterator<QString,QVariant>(attributeValues);
}

bool Role::addAttribute(QString newAttribute)
{
	if (customAttributes.contains(newAttribute))
		return false;
	else
		customAttributes.append(newAttribute);
	return true;
}

int Role::attributeCount()
{
	return customAttributes.size();
}

bool Role::isValid()
{
	if ((attributeValues.contains("Name") && (attributeValues.value("Name").type() == QVariant::String)) == false)
		return false;
	if ((attributeValues.contains("StartDate") && (attributeValues.value("EndDate").type() == QVariant::DateTime)) == false)
		return false;
	if ((attributeValues.contains("EndDate") && (attributeValues.value("EndDate").type() == QVariant::DateTime)) == false)
		return false;
	return true;
}

void Role::changeStartDate(const QDate& newStartDate)
{
	attributeValues["StartDate"] = QDateTime(newStartDate,attributeValues["StartDate"].toDateTime().time());
	this->writeConfigFile();
}

void Role::changeEndDate(const QDate& newEndDate)
{
	attributeValues["EndDate"] = QDateTime(newEndDate,attributeValues["EndDate"].toDateTime().time());
	this->writeConfigFile();
}
		
void Role::changeStartTime(const QTime& newStartTime)
{
	attributeValues["StartDate"] = QDateTime(attributeValues["StartDate"].toDateTime().date(), newStartTime);
	this->writeConfigFile();
}
void Role::changeEndTime(const QTime& newEndTime)
{
	attributeValues["EndDate"] = QDateTime(attributeValues["EndDate"].toDateTime().date(), newEndTime);
	this->writeConfigFile();
}
		
void Role::changeName(const QString& newName)
{
	attributeValues["Name"] = newName;
	this->writeConfigFile();
}

bool Role::writeConfigFile()
{
	return false;
}

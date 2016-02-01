#include "xmlparser.h"

#include <iostream>

XMLParser::XMLParser()
{
}

XMLParser::~XMLParser()
{
}

bool XMLParser::parseFile(QFile *file)
{

	bool readingProperty = false;
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
         return false;
         
	QString curProperty;
	QString curMainProperty;
	
	QString tempValue;
	QString currentLine;
	int tempLoc;
	int parsedTo = 0;
	int typePos = 0;
	QString type;
	
	//QVariant::Type tempType;
	
	properties.clear();
	
	QMap<QString, QVariant > tempProperties;
	while (!file->atEnd())
	{
		parsedTo = 0;
		typePos = 0;
		type = "";
		currentLine = QString(file->readLine()).simplified();
		tempLoc = currentLine.indexOf("<",parsedTo)+1;
		if (!tempLoc)
			continue;
		curProperty = currentLine.mid(tempLoc,currentLine.indexOf(">",parsedTo)-tempLoc);
		parsedTo = currentLine.indexOf(">",parsedTo);
		if (currentLine.indexOf("<",parsedTo) == -1)
		{
			if (curProperty[0] == '/')
			{
				properties.insert(curMainProperty,tempProperties);
				tempProperties.clear();
				readingProperty = false;
			}
			else
			{
				if (readingProperty == false)
				{
					curMainProperty = curProperty;
					readingProperty = true;
				}
				else
					return false;
			}
			continue;
		}
		typePos = curProperty.indexOf(" type:");
		if (typePos != -1)
		{
			type = curProperty.mid(typePos+6,curProperty.length()-typePos);
			curProperty.truncate(typePos);
		}
		else
			type = "String";

		tempLoc = parsedTo + 1;
		tempValue = currentLine.mid(tempLoc,currentLine.indexOf("<",parsedTo)-tempLoc);
		
		if (type == "DateTime")
			tempProperties.insert(curProperty,QDateTime::fromString(tempValue,Qt::ISODate));	
		else if (type == "String")
			tempProperties.insert(curProperty,tempValue);
		else if (type == "Integer")
			tempProperties.insert(curProperty,tempValue.toInt());
	}
	return true;
}

/*int main()
{
	XMLParser testParser;
	if (testParser.parseFile(new QFile("test.xml")));
	std::cout << "Success!" << std::endl;
	
	QMapIterator<QString, QMap<QString,QString> > i(testParser.properties);
	while (i.hasNext())
	{
		i.next();
		std::cout << i.key().toStdString() << std::endl;
		QMapIterator<QString,QString> j(i.value());
		while (j.hasNext())
		{
			j.next();
			std::cout << "\t" << j.key().toStdString() << "\t" << j.value().toStdString() << std::endl;
		}
	}
	return 0;
}*/

#ifndef XMLPARSER
#define XMLPARSER

#include <QtGui>

class XMLParser
{
	public:
		XMLParser();
		~XMLParser();
		
		bool parseFile(QFile *file);
		
		QMultiMap<QString,QMap<QString,QVariant > > properties;

};

#endif

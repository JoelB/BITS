#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

//#include "projectnode.h"
#include "project.h"

class Project;



class ProjectModel : public QAbstractItemModel
{
public:
    ProjectModel(QObject *parent = 0);
    ~ProjectModel();
	void setParent(Project *parent);
	void reload();
	
    // The Model Interface
	bool hasChildren(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    Project* curProject;
};

#endif

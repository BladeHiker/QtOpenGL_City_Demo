#pragma once
#include <QFile>
#include <vector>

class ObjReader
{
public:
	QString ObjUrl;
	ObjReader(QString ObjUrl);
	std::vector<double> GetVertices();
private:
	bool Process();
	bool AddData(const QString& str);


	QList<double> v;
	QList<double> vn;
	QList<unsigned int> fv;
	QList<unsigned int> fvn;
	QString ObjName;
};

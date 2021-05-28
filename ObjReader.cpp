#include "ObjReader.h"
#include <QDebug>
#include <QVector3D>

ObjReader::ObjReader(QString ObjUrl)
{
	this->ObjUrl = ObjUrl;
	Process();
}

bool ObjReader::Process()
{
	QFile file(ObjUrl);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!file.atEnd())
		{
			QByteArray line = file.readLine();
			QString str(line);
			AddData(line);
		}
		file.close();
	}
	else
	{
		return false;
	}
	return true;
}

bool ObjReader::AddData(const QString& str)
{
	if (str == "\n" || str.isEmpty())return 0;
	QStringList segList = str.split(" ");
	if (segList[0] == "v")
	{
		v.append(segList[1].toDouble());
		v.append(segList[2].toDouble());
		v.append(segList[3].toDouble());
	}
	else if (segList[0] == "f")
	{
		for (int i = 1; i < 4; ++i)
		{
			QStringList fList = segList[i].split("/");
			fv.append(fList[0].toUInt());
			// fvn.append(fList[2].toUInt());
		}
	}
	else if (segList[0] == "vn")
	{
		for (int i = 1; i < 4; ++i)
		{
			vn.append(segList[1].toDouble());
			vn.append(segList[2].toDouble());
			vn.append(segList[3].toDouble());
		}
	}
	return true;
}

std::vector<double> ObjReader::GetVertices()
{
	std::vector<double> vertices;
	int v_cnt = fv.size();
	for (int i = 0; i < v_cnt; i += 3)
	{
		QVector3D tri[3];
		for (int j = 0; j < 3; ++j)
		{
			tri[j].setX(v[(fv[j + i] - 1) * 3]);
			tri[j].setY(v[(fv[j + i] - 1) * 3 + 1]);
			tri[j].setZ(v[(fv[j + i] - 1) * 3 + 2]);
		}
		QVector3D normal = QVector3D::normal((tri[1] - tri[0]), (tri[2] - tri[0]));//法线计算
		// qDebug() << normal;
		for (int j = i; j < i + 3; ++j)
		{
			vertices.push_back(v[(fv[j] - 1) * 3]);
			vertices.push_back(v[(fv[j] - 1) * 3 + 1]);
			vertices.push_back(v[(fv[j] - 1) * 3 + 2]);
			vertices.push_back(normal.x());
			vertices.push_back(normal.y());
			vertices.push_back(normal.z());
		}
		//顶点坐标x,y,z,法线x,y,z
	}
	return vertices;
}
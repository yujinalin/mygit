#pragma once
#include <QSettings>
#include <QString>
#include <QFile>
#include <qset.h>
#include <map>
#include <vector>
#include "comm/commFunc.hpp"

//my test add

//branch 1111111



struct STypeAnalysisRule
{
	int m_pointType;
	double m_HDis;//水平距离
	double m_VDis;//垂直距离
	double m_Dis;//净空距离
	QString m_analysisType;//分析类型
	QString m_pointTypeName;
	QString m_pointTypeZhName;
	double m_crossDis;//交夸距离
	QString m_crossRule;//交夸规则
};



class CAnalysisRule
{
public:
	CAnalysisRule(QString areaName=QString::fromLocal8Bit("国网"),QString vLevel = "110", int aType = 0, QString projName = "")
	{
		setProj(areaName,vLevel, aType, projName);
	}
	virtual ~CAnalysisRule()
	{

	}
	void setProj(QString areaName,QString vLevel,int aType,QString projName)
	{
		m_pointTypeRuleMap.clear();
		m_analysisTypeRuleMap.clear();
		m_crossTypeMap.clear();

		m_valid = false;
		m_areaName = areaName;
		m_vLevel = vLevel;
		m_aType = aType;
		m_projName = projName;
		//m_ruleFileName = QString::fromLocal8Bit("架空输电线路运行规程-") + m_areaName +m_vLevel + QString::fromLocal8Bit("kV");
		m_ruleFileName = QString::fromLocal8Bit("架空输电线路运行规程-") + m_vLevel + QString::fromLocal8Bit("kV");
		if (aType == 1)
			m_ruleFileName += QString::fromLocal8Bit("直流.txt");
		else
			m_ruleFileName += QString::fromLocal8Bit("交流.txt");

		QString fn = m_projName + "/" +m_ruleFileName;
		if (!QFile::exists(fn))
		{
			fn = CCommFunc::getSysPath();
			fn += "/config/runRule/" + areaName + "/" + m_ruleFileName;
		}
		if (!QFile::exists(fn))
			return;
		QFile rfile(fn);
		if (!rfile.open(QFile::ReadOnly))
			return;
		QString line;
		line = QString::fromLocal8Bit(rfile.readLine().data());
		while (!rfile.atEnd())
		{
			line = QString::fromLocal8Bit(rfile.readLine().data());
			QStringList slist = line.split("	");
			if (slist.size() < 9)
				continue;
			STypeAnalysisRule rule;
			rule.m_pointType = slist[0].toInt();
			rule.m_HDis = slist[1].toDouble();
			rule.m_VDis = slist[2].toDouble();
			rule.m_Dis = slist[3].toDouble();
			rule.m_analysisType = slist[4];
			rule.m_pointTypeName = slist[5];
			rule.m_pointTypeZhName = slist[6];
			rule.m_crossDis = slist[7].toDouble();
			rule.m_crossRule = slist[8].trimmed();
			m_pointTypeRuleMap[rule.m_pointType] = rule;
			m_analysisTypeRuleMap[rule.m_analysisType].push_back(rule);
			m_crossTypeMap[rule.m_crossRule].push_back(rule);
		}
		rfile.close();
		m_valid = true;
	}
public:
	bool m_valid;

	QString m_areaName;
	int m_aType;//电流类型 0交流 1直流
	QString m_vLevel;
	QString m_projName;
	QString m_ruleFileName;

	std::map<int, STypeAnalysisRule> m_pointTypeRuleMap;
	std::map<QString, std::vector<STypeAnalysisRule>> m_analysisTypeRuleMap;
	std::map<QString, std::vector<STypeAnalysisRule>> m_crossTypeMap;
};
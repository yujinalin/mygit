#pragma once
#include <QSettings>
#include <QString>
#include <QFile>
#include <qset.h>
#include <map>
#include <vector>
#include "comm/commFunc.hpp"

struct STypeAnalysisRule
{
	int m_pointType;
	double m_HDis;//ˮƽ����
	double m_VDis;//��ֱ����
	double m_Dis;//���վ���
	QString m_analysisType;//��������
	QString m_pointTypeName;
	QString m_pointTypeZhName;
	double m_crossDis;//�������
	QString m_crossRule;//�������
};



class CAnalysisRule
{
public:
	CAnalysisRule(QString areaName=QString::fromLocal8Bit("����"),QString vLevel = "110", int aType = 0, QString projName = "")
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
		//m_ruleFileName = QString::fromLocal8Bit("�ܿ������·���й��-") + m_areaName +m_vLevel + QString::fromLocal8Bit("kV");
		m_ruleFileName = QString::fromLocal8Bit("�ܿ������·���й��-") + m_vLevel + QString::fromLocal8Bit("kV");
		if (aType == 1)
			m_ruleFileName += QString::fromLocal8Bit("ֱ��.txt");
		else
			m_ruleFileName += QString::fromLocal8Bit("����.txt");

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
	int m_aType;//�������� 0���� 1ֱ��
	QString m_vLevel;
	QString m_projName;
	QString m_ruleFileName;

	std::map<int, STypeAnalysisRule> m_pointTypeRuleMap;
	std::map<QString, std::vector<STypeAnalysisRule>> m_analysisTypeRuleMap;
	std::map<QString, std::vector<STypeAnalysisRule>> m_crossTypeMap;
};
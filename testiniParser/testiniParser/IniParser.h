/*
���ߣ����
�汾��v1.0.0.3
*/
#ifndef _INIPARSER_
#define _INIPARSER_
#include <string>
#include <vector>
#include <map>
using namespace std;


//ֻ֧��ASCII�ļ����粻�ǣ���������ת��
//֧��section������ ֧��һ��section��������key
//�˰汾֧�ֶ�INI,��֧��дINI
class CIniParser
{
public:
	//�ɹ�����0, ʧ��<0
	int load(const char* path);
	//�������ͬ��section, sectionNum��ʾ�ǵڼ���ͬ����section, 1��ʾ��һ��
	//���section����ͬ����key, keyNum��ʾ�ǵڼ���ͬ����key�� 1��ʾ��һ��
	//�������section����Ҫ���[]
	//����ֵ-1��ʾû��ָ����section
	//����ֵ-2��ʾû��ָ����key
	//�ɹ�����0
	int GetValue(const char* section, const char* key, string& value, int sectionNum = 1, int keyNum = 1);

	//��ȡĳ��section����Ŀ
	int GetSectionNum(const char* section);
	//��ȡָ��section��ָ��key����Ŀ
	//����ֵ-1��ʾû��ָ����section
	int GetKeyNum(const char* section, const char* key, int sectionNum = 1);
	void Disp();

public: 
	//��Ϊpublic,��Ϊ��ʹ�����Զ���Ľ������ܹ���ȡ�������Ϣ��
	//INI���е����ݶ��洢��2�����ݽṹ�У�����ֻ�ṩ��GetValue����
	//��Ҫload֮�����������
	vector<string> m_section; //INI�ļ����е�section
	vector< multimap<string, string> > m_keyToValue; //����section�д洢�ļ�ֵ��


public:
	CIniParser(void);
	~CIniParser(void);

private:
	void delUseless(const char* before, int len, string& after);
	int analyse(string& content);
	bool IsSection(string& line);
	int GetKeyAndValue(string& line, string& key, string& value);
	void trim(string& str);
};
#endif

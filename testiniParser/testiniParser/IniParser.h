/*
���ߣ����
�汾��v1.0.0.5
*/
#ifndef _INIPARSER_
#define _INIPARSER_
#include <string>
#include <vector>
#include <map>
using namespace std;


//�˰汾��д�����ļ�ʱ��ȥ������ע�ͺͶ���Ŀհ׷��������Զ�������ע�⣨��һ�汾���ܻ�֧�ֱ���ע�ͣ�
//��ȡʱ��֧���ļ���ע��
//ֻ֧��ASCII�ļ����粻�ǣ���������ת��
//֧��section������ ֧��һ��section��������key


class CIniParser
{
public:
    int load(const char* path);
    void Save(const char* path = NULL);
    //�������ͬ��section, sectionNum��ʾ�ǵڼ���ͬ����section, 1��ʾ��һ��
    //���section����ͬ����key, keyNum��ʾ�ǵڼ���ͬ����key�� 1��ʾ��һ��
    //�������section����Ҫ���[]
    //����ֵ-1��ʾû��ָ����section
    //����ֵ-2��ʾû��ָ����key
    //�ɹ�����0
    int GetValue(const char* section, const char* key, string& value, const unsigned int sectionNum = 1, const unsigned int keyNum = 1);
    //���û��������Ͳ����¼�
    void Update(const char* section, const char* key, const char* value, const unsigned int sectionNum = 1, const unsigned int keyNum = 1);
    int DelKey(const char* section, const char* key, const unsigned int sectionNum = 1, const unsigned int keyNum = 1);

    //��ȡĳ��section����Ŀ
    int GetSectionNum(const char* section);
    //��ȡָ��section��ָ��key����Ŀ
    //����ֵ-1��ʾû��ָ����section
    int GetKeyNum(const char* section, const char* key, const unsigned int sectionNum = 1);
    

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

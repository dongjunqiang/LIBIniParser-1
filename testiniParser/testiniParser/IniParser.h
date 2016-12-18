/*
作者：李海涛
版本：v1.0.0.5
*/
#ifndef _INIPARSER_
#define _INIPARSER_
#include <string>
#include <vector>
#include <map>
using namespace std;


//此版本在写配置文件时会去掉所有注释和多余的空白符，键会自动排序，请注意（下一版本可能会支持保留注释）
//读取时，支持文件有注释
//只支持ASCII文件，如不是，请先自行转换
//支持section重名， 支持一个section下有重名key


class CIniParser
{
public:
    int load(const char* path);
    void Save(const char* path = NULL);
    //如果具有同名section, sectionNum表示是第几个同名的section, 1表示第一个
    //如果section下有同名的key, keyNum表示是第几个同名的key， 1表示第一个
    //输入参数section不需要添加[]
    //返回值-1表示没有指定的section
    //返回值-2表示没有指定的key
    //成功返回0
    int GetValue(const char* section, const char* key, string& value, const unsigned int sectionNum = 1, const unsigned int keyNum = 1);
    //如果没有这个键就插入新键
    void Update(const char* section, const char* key, const char* value, const unsigned int sectionNum = 1, const unsigned int keyNum = 1);
    int DelKey(const char* section, const char* key, const unsigned int sectionNum = 1, const unsigned int keyNum = 1);

    //获取某个section的数目
    int GetSectionNum(const char* section);
    //获取指定section下指定key的数目
    //返回值-1表示没有指定的section
    int GetKeyNum(const char* section, const char* key, const unsigned int sectionNum = 1);
    

public: 
    //成为public,是为了使用者自定义的解析，能够获取更多的信息，
    //INI所有的数据都存储这2个数据结构中，本类只提供了GetValue方法
    //需要load之后才能有数据
    vector<string> m_section; //INI文件所有的section
    vector< multimap<string, string> > m_keyToValue; //所有section中存储的键值对


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

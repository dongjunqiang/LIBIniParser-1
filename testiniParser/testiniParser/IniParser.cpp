#include "IniParser.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#pragma warning(disable: 4996)


CIniParser::CIniParser(void) {}
CIniParser::~CIniParser(void) {}


void CIniParser::trim(string& str)
{
    int i = 0;

    while (isspace((unsigned char)str[i])) 
    {
        i++;
    }

    str.erase(0, i);

    //tail
    int len = str.length();

    for (i = len - 1; i >= 0; --i)
    {
        if (!isspace((unsigned char)str[i])) 
        {
            break;
        }
    }

    str.erase(i + 1, len - i - 1);
}


int CIniParser::load(const char* path)
{
    m_section.clear();
    m_keyToValue.clear();
    FILE* fp = fopen(path, "rb");
    if (NULL == fp)
    {
        return -1;
    }

    if (fseek(fp, 0, SEEK_END))
    {
        fclose(fp);
        return -2;
    }

    int fileSize = ftell(fp);
    if (fseek(fp, 0, SEEK_SET))
    {
        fclose(fp);
        return -3;
    }

    char* buf = (char*)malloc(fileSize);
    if (NULL == buf)
    {
        fclose(fp);
        return -4;
    }

    int readed = fread(buf, sizeof(char), fileSize, fp);
    if (readed != fileSize)
    {
        fclose(fp);
        free(buf);
        return -5;
    }

    fclose(fp);
    string content;
    delUseless(buf, fileSize, content);
    free(buf);
    analyse(content);
    
    return 0;
}


void CIniParser::delUseless(const char* before, int len, string& after)
{
    for (int i = 0; i < len; i++)
    {
        if (('#' == before[i]) || (';' == before[i]))
        {
            i++;
            for (; i < len; i++)
            {
                if (('\r' == before[i]) || ('\n' == before[i]))
                {
                    after += before[i];
                    break;
                }
            }
        }
        else
        {
            after += before[i];
        }
    }
}


bool CIniParser::IsSection(string& line)
{
    /*int len = line.length();
    if (len <= 0)//不会等于0，前面去掉了空行
    {
        return false;
    }*/
    trim(line);
    if (('[' == line[0]) && (']' == line[line.length() - 1]))
    {
        return true;
    }

    return false;
}


int CIniParser::GetKeyAndValue(string& line, string& key, string& value)
{
    int pos = line.find('=');
    if (pos < 0)
    {
        return -1;
    }

    key = line.substr(0, pos);
    trim(key);
    if (key.length() == 0)
    {
        return -2;
    }

    value = line.substr(pos + 1);
    trim(value);
    return 0;
}


int CIniParser::analyse(string& content)
{
    int len = content.length();
    string line;
    multimap<string, string> keyToValue;
    string key;
    string value;


    for (int i = 0; i < len; i++)
    {
        if (('\r' == content[i]) || ('\n' == content[i]))
        {
            if (line.length() != 0)
            {
                if (IsSection(line))
                {
                    m_section.push_back(line.substr(1, line.length() - 2));
                    m_keyToValue.push_back(keyToValue); //插入的是上一条section的
                    keyToValue.clear();
                }
                else
                {
                    if (GetKeyAndValue(line, key, value) == 0) //扔掉不符合格式的
                    {
                        keyToValue.insert(make_pair(key, value));
                    }
                }

                line = "";
            }
        }
        else
        {
            line += content[i];
        }
    }

    m_keyToValue.push_back(keyToValue);
    m_keyToValue.erase(m_keyToValue.begin());//push_back在前防止空数组
        
    return 0;
}


int CIniParser::GetValue(const char* section, const char* key, string& value, const unsigned int sectionNum, const unsigned int keyNum)
{
    vector<string>::iterator it = m_section.begin();
    for (unsigned int i = 0; i < sectionNum; i++)
    {
        it = find(it, m_section.end(), section);
        if (m_section.end() == it)
        {
            return -1;
        }

        it++;
    }

    int index = it - 1 - m_section.begin(); //下标，0开始
    unsigned int cnt = m_keyToValue[index].count(key);
    if (keyNum > cnt)
    {
        return -2;
    }
    
    //pair<multimap<string, string>::iterator, multimap<string, string>::iterator> pos = m_keyToValue[index].equal_range(key);
    multimap<string, string>::iterator pValue = m_keyToValue[index].lower_bound(key); //前面count有元素,不会返回末端迭代器
    for (unsigned int i = 0; i < (keyNum - 1); i++)
    {
        pValue++;
    }

    value = pValue->second;

    return 0;
}

void CIniParser::Update(const char* section, const char* key, const char* value, const unsigned int sectionNum, const unsigned int keyNum)
{
    vector<string>::iterator it = m_section.begin();
    for (unsigned int i = 0; i < sectionNum; i++)
    {
        it = find(it, m_section.end(), section);
        if (m_section.end() == it)
        {
            m_section.push_back(section);
            pair<string, string> keyToValue(key, value);
            multimap<string, string> map;
            map.insert(keyToValue);
            m_keyToValue.push_back(map);
            return;
        }

        it++;
    }

    int index = it - 1 - m_section.begin(); //下标，0开始
    unsigned int cnt = m_keyToValue[index].count(key);
    if (keyNum > cnt)
    {
        pair<string, string> keyToValue(key, value);
        m_keyToValue[index].insert(keyToValue);
    }
    else
    {
        multimap<string, string>::iterator pValue = m_keyToValue[index].lower_bound(key);
        for (unsigned int i = 0; i < (keyNum - 1); i++)
        {
            pValue++;
        }

        pValue->second = value;
    }
}

int CIniParser::DelKey(const char* section, const char* key, const unsigned int sectionNum, const unsigned int keyNum)
{
    vector<string>::iterator it = m_section.begin();
    for (unsigned int i = 0; i < sectionNum; i++)
    {
        it = find(it, m_section.end(), section);
        if (m_section.end() == it)
        {
            return -1;
        }

        it++;
    }

    int index = it - 1 - m_section.begin(); //下标，0开始
    unsigned int cnt = m_keyToValue[index].count(key);
    if (keyNum > cnt)
    {
        return -2;
    }

    multimap<string, string>::iterator pValue = m_keyToValue[index].lower_bound(key);
    for (unsigned int i = 0; i < (keyNum - 1); i++)
    {
        pValue++;
    }

    m_keyToValue[index].erase(pValue);
    return 0;
}


int CIniParser::GetSectionNum(const char* section)
{
    return count(m_section.begin(), m_section.end(), section);
}


int CIniParser::GetKeyNum(const char* section, const char* key, const unsigned int sectionNum)
{
    vector<string>::iterator it = m_section.begin();

    for (unsigned int i = 0; i < sectionNum; i++)
    {
        it = find(it, m_section.end(), section);
        if (m_section.end() == it)
        {
            return -1;
        }

        it++;
    }

    it--;

    return m_keyToValue[it - m_section.begin()].count(key);
}


void CIniParser::Save(const char* path)
{
    FILE* fp = NULL;
    if (NULL == path)
    {
        fp = stdout;
    }
    else
    {
        fp = fopen(path, "wb");
        if (NULL == fp)
        {
            fp = stdout;
        }
    }

    vector<string>::iterator it = m_section.begin();
    multimap<string, string>::iterator keyToValue;

    for (int i = 0; it != m_section.end(); it++, i++)
    {
        fprintf(fp, "[%s]\n", it->c_str());
        keyToValue = m_keyToValue[i].begin();
        for (; keyToValue != m_keyToValue[i].end(); keyToValue++)
        {
            fprintf(fp, "%s = %s\n", keyToValue->first.c_str(), keyToValue->second.c_str());
        }

        fprintf(fp, "\n");
    }

    if (fp != stdout)
    {
        fclose(fp);
    }
}
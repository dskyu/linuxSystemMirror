#ifndef TOOL_H
#define TOOL_H
#include <QString>

const QString getInfoByField(const char *path,const char *field);
const QString getContentFromCmd(const QString &s);
const QString getContentFromFile(const QString &s);
const QString getCpuInfo(int n);
const QString getSysUptime();
const QString getSysStartTime();
bool isDigitString(const QString& src);

#endif // TOOL_H

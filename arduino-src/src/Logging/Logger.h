#ifndef MITM_RS485_SNIFFER_AND_REPEATER_SERIAL_LOGGER_H
#define MITM_RS485_SNIFFER_AND_REPEATER_SERIAL_LOGGER_H

#include <Arduino.h>
#include <utility>
#include "models/LogLevelEnum.h"

#define GetOriginalFileAndLine() FileAndLine(__LINE__, __FILE__)

class Logger
{
public:
    struct FileAndLine
    {
        FileAndLine(int line, std::string  file) : line(line), file(std::move(file)) {}

        std::string file;
        int line;
    };

    explicit Logger(LogLevel logLevel);

    void Debug(const String &message, const FileAndLine &originalFileAndLine = GetOriginalFileAndLine());

    void Information(const String &message, const FileAndLine &originalFileAndLine = GetOriginalFileAndLine());

    void Warning(const String &message, const FileAndLine &originalFileAndLine = GetOriginalFileAndLine());

    void Error(const String &message, const FileAndLine &originalFileAndLine = GetOriginalFileAndLine());

    void Fatal(const String &message, const FileAndLine &originalFileAndLine = GetOriginalFileAndLine());

private:
    LogLevel _logLevel;

    String FormatFileAndLine(const FileAndLine& originalFileAndLine);

    void WriteLogLine(const String &message, const String &logLevelAbbreviation, const FileAndLine &originalFileAndLine);

    String GetPaddedMillis(int i);

};

#endif //MITM_RS485_SNIFFER_AND_REPEATER_SERIAL_LOGGER_H

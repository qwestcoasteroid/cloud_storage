#ifndef NTCONFIG_H
#define NTCONFIG_H

#include <QString>

#include <memory>

class NtConfig {
public:
    static std::shared_ptr<NtConfig> get();

    QString serverAddress;
    QString serverPort;
    QString accessToken;

private:
    NtConfig();

    bool parseConfigFile();
    void getParameter(const QByteArray &param);
};

#endif // NTCONFIG_H

#include "ntconfig.h"

#include <QFile>
#include <QDataStream>
#include <QDir>

#include <stdexcept>

static QString remoteAddressLine = "remoteAddress=";
static QString remotePortLine = "remotePort=";
static QString accessTokenLine = "accessToken=";

NtConfig::NtConfig() {
    if (!parseConfigFile()) {
        throw std::runtime_error("Unable to parse \"config.txt\"!");
    }
}

std::shared_ptr<NtConfig> NtConfig::get() {
    static std::shared_ptr<NtConfig> instance(new NtConfig);

    return instance;
}

bool NtConfig::parseConfigFile() {
    QFile file("config.txt");
    QByteArray line;

    file.open(QIODevice::ReadWrite);

    while (!file.atEnd()) {
        line = file.readLine();
        getParameter(line);
    }

    if (serverAddress.size() == 0 ||
        serverPort.size() == 0) {

        return false;
    }

    return true;
}

void NtConfig::getParameter(const QByteArray &param) {
    QString line(param);
    QString *configParam{ nullptr };

    if (line.contains(remoteAddressLine)) {
        configParam = &serverAddress;
    }
    else if (line.contains(remotePortLine)) {
        configParam = &serverPort;
    }
    else if (line.contains(accessTokenLine)) {
        configParam = &accessToken;
    }
    else {
        return;
    }

    *configParam = line.mid(line.indexOf('=') + 1).trimmed();
}

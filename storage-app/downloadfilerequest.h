#ifndef DOWNLOADFILEREQUEST_H
#define DOWNLOADFILEREQUEST_H

#include "irequest.h"

#include "cipher.h"
#include "filewriter.h"
#include "storageinfo.h"

class DownloadFileRequest : public IRequest {
    Q_OBJECT

public:
    DownloadFileRequest(const QString &uuid, const QString &path, qint64 size);

signals:
    void progress(int value);

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;
    void stop();

protected:
     void operator()() override;

private:
    QString uuid;
    QString path;
    qint64 fileSize;
    qint64 received{};
    Cipher cipher;
    FileWriter fileWriter;
};

#endif // DOWNLOADFILEREQUEST_H

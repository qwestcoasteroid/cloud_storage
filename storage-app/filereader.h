#ifndef FILEREADER_H
#define FILEREADER_H

#include <QFile>
#include <QDataStream>
#include <QByteArray>

constexpr int kDefaultChunkSize = (1 << 20) * 4;

class FileReader {
public:
    explicit FileReader(const QString &path);
    ~FileReader();

    QByteArray read(int size = kDefaultChunkSize);

private:
    QFile file;
    QDataStream stream;
};

#endif // FILEREADER_H

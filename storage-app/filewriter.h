#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QFile>
#include <QDataStream>
#include <QByteArray>

class FileWriter {
public:
    explicit FileWriter(const QString &path, qint64 initialSize = 0);
    ~FileWriter();

    int write(const QByteArray &bytes);

private:
    QFile file;
    QDataStream stream;
    qint64 initialSize;
};

#endif // FILEWRITER_H

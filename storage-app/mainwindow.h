#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDateTime>
#include <QVector>
#include <QMap>
#include <optional>
#include <QMutex>

#include "transferitem.h"
#include "storagecontentrequest.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*struct TableItem {
    QString name;
    qint64 size;
    QDateTime date;
};*/

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void uploadFile();
    void downloadFile();
    void deleteFile();
    void setEnabledDeleteButton();
    void setEnabledDownloadButton();
    void setVisibleTransfers();
    void setVisibleProfile();
    void dropTransfer(TransferItem *item);
    void dropFile(QByteArray *data = nullptr);
    void setProfileInfo(QByteArray *data = nullptr);
    void setStorageContent(QByteArray *data = nullptr);
    void getStorageContent(QByteArray *data = nullptr);
    void getProfileInfo(QByteArray *data = nullptr);

private:
    void addTableItem(const StorageFileStructure &file);
    void clearTable();

    Ui::MainWindow *ui;;
    QVector<TransferItem *> transfers;
    QMap<int, StorageFileStructure> files;
    QMutex tableMutex;
};

#endif // MAINWINDOW_H

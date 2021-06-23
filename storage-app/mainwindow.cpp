#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Qt>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>

#include <QDebug>
#include <QResizeEvent>

#include "profilerequest.h"
#include "storagecontentrequest.h"
#include "uploadfilerequest.h"
#include "downloadfilerequest.h"
#include "deletefilerequest.h"

void MainWindow::addTableItem(const StorageFileStructure &file) {
    int column{};

    auto table = ui->fileTable;

    table->setRowCount(table->rowCount() + 1);
    QTableWidgetItem *newItem = new QTableWidgetItem;
    newItem->setData(Qt::DisplayRole, file.name);
    table->setItem(table->rowCount() - 1, column++, newItem);
    newItem = new QTableWidgetItem;
    newItem->setData(Qt::DisplayRole, file.size);
    table->setItem(table->rowCount() - 1, column++, newItem);
    newItem = new QTableWidgetItem;
    newItem->setData(Qt::DisplayRole, file.date);
    table->setItem(table->rowCount() - 1, column++, newItem);

    files.insert(table->rowCount() - 1, file);
}

void MainWindow::clearTable() {
    for (int i = ui->fileTable->rowCount(); i > 0; --i) {
        ui->fileTable->removeRow(0);
    }
}

void MainWindow::setEnabledDeleteButton() {
    ui->deleteFileButton->setEnabled(!ui->fileTable->selectedItems().empty());
}

void MainWindow::setEnabledDownloadButton() {
    ui->downloadFileButton->setEnabled(!ui->fileTable->selectedItems().empty());
}

void MainWindow::setProfileInfo(QByteArray *data) {
    QDataStream stream(*data);
    stream.setVersion(QDataStream::Qt_5_0);

    ProfileResponse response;

    stream >> response;

    ui->accountNameLabel->setText(response.name);
    ui->freeSpaceAmountLabel->setText(QString::number(response.freeSpace));
    ui->filesAmountLabel->setText(QString::number(response.files));
}

void MainWindow::getProfileInfo(QByteArray *data) {
    Q_UNUSED(data)

    auto request = new ProfileRequest;

    connect(request, &ProfileRequest::complete, this, &MainWindow::setProfileInfo);
    connect(request, &ProfileRequest::complete, request, &ProfileRequest::deleteLater);
    //connect(request, &ProfileRequest::error, this, &MainWindow::showError);

    request->execute();
}

void MainWindow::setStorageContent(QByteArray *data) {
    QMutexLocker locker(&tableMutex);
    clearTable();
    files.clear();

    QDataStream stream(*data);
    stream.setVersion(QDataStream::Qt_5_0);

    StorageFileStructure file;

    while (!stream.atEnd()) {
        stream >> file;

        addTableItem(file);
    }
}

void MainWindow::getStorageContent(QByteArray *data) {
    Q_UNUSED(data)

    auto storageRequest = new StorageContentRequest;

    connect(storageRequest, &StorageContentRequest::complete, this, &MainWindow::setStorageContent);
    connect(storageRequest, &StorageContentRequest::complete, storageRequest, &StorageContentRequest::deleteLater);
    //connect(storageRequest, &StorageContentRequest::error, this, &MainWindow::showError);

    storageRequest->execute();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    ui->fileTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    auto profileRequest = new ProfileRequest;

    connect(profileRequest, &ProfileRequest::complete, this, &MainWindow::setProfileInfo);
    connect(profileRequest, &ProfileRequest::complete, profileRequest, &ProfileRequest::deleteLater);
    //connect(profileRequest, &ProfileRequest::error, this, &MainWindow::showError);

    profileRequest->execute();

    auto storageRequest = new StorageContentRequest;

    connect(storageRequest, &StorageContentRequest::complete, this, &MainWindow::setStorageContent);
    connect(storageRequest, &StorageContentRequest::complete, storageRequest, &StorageContentRequest::deleteLater);
    //connect(storageRequest, &StorageContentRequest::error, this, &MainWindow::showError);

    storageRequest->execute();

    connect(ui->uploadFileButton, &QPushButton::clicked, this, &MainWindow::uploadFile);
    connect(ui->downloadFileButton, &QPushButton::clicked, this, &MainWindow::downloadFile);
    connect(ui->deleteFileButton, &QPushButton::clicked, this, &MainWindow::deleteFile);

    connect(ui->fileTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::setEnabledDeleteButton);
    connect(ui->fileTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::setEnabledDownloadButton);

    connect(ui->transfersButton, &QPushButton::clicked, this, &MainWindow::setVisibleTransfers);
    connect(ui->profileButton, &QPushButton::clicked, this, &MainWindow::setVisibleProfile);

    ui->optionLayout->setAlignment(Qt::AlignTop);
    ui->profileLayout->setAlignment(Qt::AlignTop);

    ui->accountLabel->setVisible(false);
    ui->accountNameLabel->setVisible(false);
    ui->freeSpaceLabel->setVisible(false);
    ui->freeSpaceAmountLabel->setVisible(false);
    ui->filesLabel->setVisible(false);
    ui->filesAmountLabel->setVisible(false);

    ui->fileTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    resize(minimumSize());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::uploadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select File to Upload");

    if (fileName.size() == 0) {
        return;
    }

    QFileInfo file(fileName);

    auto request = new UploadFileRequest(file.filePath(), file.size());

    connect(request, &UploadFileRequest::complete, this, &MainWindow::getStorageContent);
    connect(request, &UploadFileRequest::complete, this, &MainWindow::getProfileInfo);
    //connect(request, &UploadFileRequest::error, this, &MainWindow::showError);

    auto transfer = new TransferItem(file.fileName(), ui->transferLayout, this);

    connect(request, &UploadFileRequest::progress, transfer, &TransferItem::setProgress);
    connect(transfer, &TransferItem::canceled, request, &UploadFileRequest::stop);
    connect(transfer, &TransferItem::canceled, this, &MainWindow::dropTransfer);
    connect(request, &UploadFileRequest::complete, transfer, &TransferItem::forwardCancelation);
    connect(request, &UploadFileRequest::complete, request, &UploadFileRequest::deleteLater);

    transfers.append(transfer);

    request->execute();
}

void MainWindow::downloadFile() {
    auto file = files[ui->fileTable->currentRow()];

    QString downloadPath = QFileDialog::getSaveFileName(this, "Downloading Location", file.name);

    if (downloadPath.size() == 0) {
        return;
    }

    auto request = new DownloadFileRequest(file.uuid, downloadPath, file.size);

    auto transfer = new TransferItem(file.name, ui->transferLayout, this);

    connect(request, &DownloadFileRequest::progress, transfer, &TransferItem::setProgress);
    connect(transfer, &TransferItem::canceled, request, &DownloadFileRequest::stop);
    connect(transfer, &TransferItem::canceled, this, &MainWindow::dropTransfer);
    connect(request, &DownloadFileRequest::complete, transfer, &TransferItem::forwardCancelation);
    connect(request, &DownloadFileRequest::complete, request, &DownloadFileRequest::deleteLater);
    //connect(request, &DownloadFileRequest::error, this, &MainWindow::showError);

    transfers.append(transfer);

    request->execute();
}

void MainWindow::deleteFile() {
    auto file = files[ui->fileTable->currentRow()];

    auto request = new DeleteFileRequest(file.uuid);

    connect(request, &DeleteFileRequest::complete, this, &MainWindow::getProfileInfo);
    connect(request, &DeleteFileRequest::complete, this, &MainWindow::getStorageContent);
    connect(request, &DeleteFileRequest::complete, this, &MainWindow::dropFile);
    connect(request, &DeleteFileRequest::complete, request, &DeleteFileRequest::deleteLater);
    //connect(request, &DeleteFileRequest::error, this, &MainWindow::showError);

    request->execute();
}

void MainWindow::setVisibleTransfers() {
    if (transfers.empty()) {
        return;
    }

    for (int i = 0; i < transfers.size(); ++i) {
        transfers[i]->setVisible(!transfers[i]->isVisible());
    }
}

void MainWindow::setVisibleProfile() {
    ui->accountLabel->setVisible(!ui->accountLabel->isVisible());
    ui->accountNameLabel->setVisible(!ui->accountNameLabel->isVisible());
    ui->freeSpaceLabel->setVisible(!ui->freeSpaceLabel->isVisible());
    ui->freeSpaceAmountLabel->setVisible(!ui->freeSpaceAmountLabel->isVisible());
    ui->filesLabel->setVisible(!ui->filesLabel->isVisible());
    ui->filesAmountLabel->setVisible(!ui->filesAmountLabel->isVisible());
}

void MainWindow::dropTransfer(TransferItem *item) {
    int index = transfers.indexOf(item);
    transfers[index]->deleteLater();
    transfers.remove(index);
}

void MainWindow::dropFile(QByteArray *data) {
    QDataStream stream(*data);
    stream.setVersion(QDataStream::Qt_5_0);
    QString uuid;

    stream >> uuid;

    for (auto &item : files.values()) {
        if (item.uuid == uuid) {
            int key = files.key(item);
            ui->fileTable->removeRow(key);
            files.remove(key);
            break;
        }
    }
}

#include "authorizationwindow.h"
#include "ui_authorizationwindow.h"

#include "authorizationrequest.h"
#include "registrationrequest.h"
#include "tariffsrequest.h"

#include "mainwindow.h"

#include <QMessageBox>

#define TO_GiB(x) (x >> 30)

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthorizationWindow),
    mode(WindowMode::Authorization) {

    ui->setupUi(this);

    ui->buttonsLayout->setAlignment(Qt::AlignCenter);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->passwordConfirmLabel->setVisible(false);
    ui->passwordConfirmEdit->setVisible(false);
    ui->tariffLabel->setVisible(false);
    ui->tariffComboBox->setVisible(false);

    connect(ui->registrationButton, &QPushButton::clicked, this, &AuthorizationWindow::switchMode);
    connect(ui->confirmButton, &QPushButton::clicked, this, &AuthorizationWindow::confirmOperation);
}

AuthorizationWindow::~AuthorizationWindow() {
    delete ui;
}

void AuthorizationWindow::setTariffComboBox(const QList<QString> &list) {
    for (int i = 0; i < list.size(); ++i) {
        ui->tariffComboBox->addItem(list[i]);
    }
}

void AuthorizationWindow::getTariffList(QByteArray *data) {
    QDataStream stream(*data);
    stream.setVersion(QDataStream::Qt_5_0);

    QList<QString> list;
    Tariff tariff;

    while (not stream.atEnd()) {
        stream >> tariff;
        list.append(QString("%1 (%2 GiB)").arg(tariff.name).arg(TO_GiB(tariff.space)));
    }

    setTariffComboBox(list);
}

void AuthorizationWindow::switchMode() {
    switch (mode) {
    case WindowMode::Authorization:
    {
        auto request = new TariffsRequest;
        connect(request, &TariffsRequest::complete, this, &AuthorizationWindow::getTariffList);
        connect(request, &TariffsRequest::complete, request, &TariffsRequest::deleteLater);
        connect(request, &TariffsRequest::error, this, &AuthorizationWindow::showError);
        connect(request, &TariffsRequest::error, request, &TariffsRequest::deleteLater);
        request->execute();
        mode = WindowMode::Registration;
        ui->passwordConfirmLabel->setVisible(true);
        ui->passwordConfirmEdit->setVisible(true);
        ui->tariffLabel->setVisible(true);
        ui->tariffComboBox->setVisible(true);
        ui->registrationButton->setText("Авторизация");
        setWindowTitle("Регистрация");
        break;
    }
    case WindowMode::Registration:
        mode = WindowMode::Authorization;
        ui->passwordConfirmLabel->setVisible(false);
        ui->passwordConfirmEdit->setVisible(false);
        ui->tariffLabel->setVisible(false);
        ui->tariffComboBox->setVisible(false);
        ui->registrationButton->setText("Регистрация");
        setWindowTitle("Авторизация");
        break;
    }
}

void AuthorizationWindow::confirmOperation() {
    switch (mode) {
    case WindowMode::Authorization:
        authorizeUser();
        break;
    case WindowMode::Registration:
        registrateAccount();
        break;
    }
}

void AuthorizationWindow::registrateAccount() {
    if (ui->loginEdit->text().size() == 0 || ui->passwordEdit->text().size() == 0) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль");
        return;
    }
    if (ui->passwordEdit->text() != ui->passwordConfirmEdit->text()) {
        QMessageBox::warning(this, "Ошибка", "Пароли должны совпадать");
        return;
    }
    if (ui->tariffComboBox->currentText().size() == 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите тариф");
        return;
    }

    auto request = new RegistrationRequest(ui->loginEdit->text(), ui->passwordEdit->text(),
                                           ui->tariffComboBox->currentText().section(' ', 0, 0));

    connect(request, &RegistrationRequest::error, request, &RegistrationRequest::deleteLater);
    connect(request, &RegistrationRequest::complete, request, &RegistrationRequest::deleteLater);
    connect(request, &RegistrationRequest::error, this, &AuthorizationWindow::showError);
    connect(request, &RegistrationRequest::complete, this, &AuthorizationWindow::finishRegistration);


    request->execute();
}

void AuthorizationWindow::authorizeUser() {
    if (ui->loginEdit->text().size() == 0 || ui->passwordEdit->text().size() == 0) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль");
        return;
    }

    auto request = new AuthorizationRequest(ui->loginEdit->text(), ui->passwordEdit->text());

    connect(request, &RegistrationRequest::error, request, &RegistrationRequest::deleteLater);
    connect(request, &RegistrationRequest::complete, request, &RegistrationRequest::deleteLater);
    connect(request, &AuthorizationRequest::error, this, &AuthorizationWindow::showError);
    connect(request, &AuthorizationRequest::complete, this, &AuthorizationWindow::finishAuthorization);

    request->execute();
}

void AuthorizationWindow::showError(QString *error) {
    QMessageBox::warning(this, "Ошибка", *error);
}

void AuthorizationWindow::finishAuthorization(QByteArray *data) {
    Q_UNUSED(data)

    auto window = new MainWindow(this);

    this->hide();
    window->show();
}

void AuthorizationWindow::finishRegistration(QByteArray *data) {
    Q_UNUSED(data)

    QMessageBox::information(this, "Уведомление", "Регистрация прошла успешно");
}

#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H

#include <QDialog>
#include <QList>
#include <QString>

namespace Ui { class AuthorizationWindow; }

enum class WindowMode { Authorization, Registration };

class AuthorizationWindow : public QDialog {
    Q_OBJECT

public:
    explicit AuthorizationWindow(QWidget *parent = nullptr);
    ~AuthorizationWindow();

public slots:
    void switchMode();
    void confirmOperation();
    void showError(QString *error = nullptr);
    void finishAuthorization(QByteArray *data = nullptr);
    void finishRegistration(QByteArray *data = nullptr);
    void getTariffList(QByteArray *data = nullptr);

private:
    void registrateAccount();
    void authorizeUser();
    void setTariffComboBox(const QList<QString> &list);

    Ui::AuthorizationWindow *ui;
    WindowMode mode;
};

#endif // AUTHORIZATIONWINDOW_H

#pragma once

#include <QDialog>

class QLineEdit;
class QDateEdit;
class QTableWidget;

#include "person.h"

class PersonDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PersonDialog(QWidget* parent = nullptr);

    void setPerson(const Person& p);
    Person personFromUi() const;

private slots:
    void addPhoneRow();
    void removeSelectedPhoneRow();

private:
    void setupUi();

    QLineEdit* nameEdit_;
    QLineEdit* lastNameEdit_;
    QLineEdit* patronimicEdit_;
    QLineEdit* addressEdit_;
    QLineEdit* emailEdit_;
    QDateEdit* birthDateEdit_;

    QTableWidget* phonesTable_;
};

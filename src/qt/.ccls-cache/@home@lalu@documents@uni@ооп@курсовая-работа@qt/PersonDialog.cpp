#include "PersonDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QDialogButtonBox>

static QString toQ(const std::string& s) { return QString::fromStdString(s); }
static std::string toS(const QString& s) { return s.toStdString(); }

PersonDialog::PersonDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi();
}

void PersonDialog::setupUi()
{
    setWindowTitle("Contact");

    nameEdit_       = new QLineEdit(this);
    lastNameEdit_   = new QLineEdit(this);
    patronimicEdit_ = new QLineEdit(this);
    addressEdit_    = new QLineEdit(this);
    emailEdit_      = new QLineEdit(this);

    birthDateEdit_ = new QDateEdit(this);
    birthDateEdit_->setCalendarPopup(true);
    // TU BACKEND exige YYYY-MM-DD
    birthDateEdit_->setDisplayFormat("yyyy-MM-dd");

    auto form = new QFormLayout();
    form->addRow("Name*", nameEdit_);
    form->addRow("Last name*", lastNameEdit_);
    form->addRow("Patronymic", patronimicEdit_);
    form->addRow("Address", addressEdit_);
    form->addRow("Birth date", birthDateEdit_);
    form->addRow("Email*", emailEdit_);

    phonesTable_ = new QTableWidget(this);
    phonesTable_->setColumnCount(2);
    phonesTable_->setHorizontalHeaderLabels({"Number*", "Type*"});
    phonesTable_->horizontalHeader()->setStretchLastSection(true);
    phonesTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    phonesTable_->setSelectionMode(QAbstractItemView::SingleSelection);

    auto addBtn = new QPushButton("+ phone", this);
    auto delBtn = new QPushButton("- phone", this);
    connect(addBtn, &QPushButton::clicked, this, &PersonDialog::addPhoneRow);
    connect(delBtn, &QPushButton::clicked, this, &PersonDialog::removeSelectedPhoneRow);

    auto phonesBtns = new QHBoxLayout();
    phonesBtns->addWidget(addBtn);
    phonesBtns->addWidget(delBtn);
    phonesBtns->addStretch();

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    // Qt: accepted/rejected conectados a accept/reject :contentReference[oaicite:5]{index=5}
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto root = new QVBoxLayout(this);
    root->addLayout(form);
    root->addWidget(phonesTable_);
    root->addLayout(phonesBtns);
    root->addWidget(buttons);

    addPhoneRow(); // mínimo 1 teléfono
}

void PersonDialog::addPhoneRow()
{
    int r = phonesTable_->rowCount();
    phonesTable_->insertRow(r);
    phonesTable_->setItem(r, 0, new QTableWidgetItem(""));
    phonesTable_->setItem(r, 1, new QTableWidgetItem("home"));
}

void PersonDialog::removeSelectedPhoneRow()
{
    auto sel = phonesTable_->selectionModel()->selectedRows();
    if (sel.isEmpty()) return;
    if (phonesTable_->rowCount() <= 1) return;
    phonesTable_->removeRow(sel.first().row());
}

void PersonDialog::setPerson(const Person& p)
{
    nameEdit_->setText(toQ(p.getName()));
    lastNameEdit_->setText(toQ(p.getLastName()));
    patronimicEdit_->setText(toQ(p.getPatronimic()));
    addressEdit_->setText(toQ(p.getAddress()));
    emailEdit_->setText(toQ(p.getEmail()));

    QDate d = QDate::fromString(toQ(p.getBirhtDate()), "yyyy-MM-dd");
    if (d.isValid()) birthDateEdit_->setDate(d);

    phonesTable_->setRowCount(0);
    const auto& nums = p.getNumbers();
    if (nums.empty()) {
        addPhoneRow();
        return;
    }
    for (const auto& n : nums) {
        int r = phonesTable_->rowCount();
        phonesTable_->insertRow(r);
        phonesTable_->setItem(r, 0, new QTableWidgetItem(toQ(n.getNumber())));
        phonesTable_->setItem(r, 1, new QTableWidgetItem(toQ(n.getType())));
    }
}

Person PersonDialog::personFromUi() const
{
    Person p;
    p.setName(toS(nameEdit_->text()));
    p.setLastName(toS(lastNameEdit_->text()));
    p.setPatronimic(toS(patronimicEdit_->text()));
    p.setAddress(toS(addressEdit_->text()));
    p.setEmail(toS(emailEdit_->text()));

    // guardar en YYYY-MM-DD (lo que valida tu backend)
    p.setBirhtDate(toS(birthDateEdit_->date().toString("yyyy-MM-dd")));

    for (int r = 0; r < phonesTable_->rowCount(); ++r) {
        QString num  = phonesTable_->item(r,0)->text();
        QString type = phonesTable_->item(r,1)->text();

        PhoneNumber pn;
        pn.setNumber(toS(num));
        pn.setType(toS(type));
        p.addNumber(pn);
    }
    return p;
}

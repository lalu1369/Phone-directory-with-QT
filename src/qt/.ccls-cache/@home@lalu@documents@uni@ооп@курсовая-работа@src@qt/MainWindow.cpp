#include "MainWindow.h"

#include <QToolBar>
#include <QAction>
#include <QTableView>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>

#include "PersonDialog.h"

static QString toQ(const std::string& s) { return QString::fromStdString(s); }

MainWindow::MainWindow(StorageType type, QWidget* parent)
    : QMainWindow(parent)
    , controller_(type)   // IMPORTANTE: usamos archivo
    , model_(new QStandardItemModel(this))
    , proxy_(new MultiFilterProxy(this))
    , view_(new QTableView(this))
    , searchEdit_(new QLineEdit(this))
{
    setWindowTitle("Phonebook (Qt)");

    model_->setColumnCount(8);
    model_->setHorizontalHeaderLabels({"ID","Name","Last Name","Patronymic","Address","Birth Date","Email","Phones"});

    proxy_->setSourceModel(model_);
    view_->setModel(proxy_);

    view_->setSortingEnabled(true);
    view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    view_->setSelectionMode(QAbstractItemView::SingleSelection);
    view_->horizontalHeader()->setStretchLastSection(true);

    setCentralWidget(view_);

    auto tb = addToolBar("Main");
    auto actAdd  = tb->addAction("Add");
    auto actEdit = tb->addAction("Edit");
    auto actDel  = tb->addAction("Delete");

    tb->addSeparator();
    searchEdit_->setPlaceholderText("Search (any field)");
    tb->addWidget(searchEdit_);

    connect(actAdd,  &QAction::triggered, this, &MainWindow::onAdd);
    connect(actEdit, &QAction::triggered, this, &MainWindow::onEdit);
    connect(actDel,  &QAction::triggered, this, &MainWindow::onDelete);
    connect(searchEdit_, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);

    reloadTable();
}

void MainWindow::onSearchChanged(const QString& text)
{
    proxy_->setSearchText(text);
}

void MainWindow::reloadTable()
{
    model_->removeRows(0, model_->rowCount());

    const auto people = controller_.getPeople();

    for (const auto& p : people)
    {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(p.getID()));
        row << new QStandardItem(toQ(p.getName()));
        row << new QStandardItem(toQ(p.getLastName()));
        row << new QStandardItem(toQ(p.getPatronimic()));
        row << new QStandardItem(toQ(p.getAddress()));
        row << new QStandardItem(toQ(p.getBirhtDate()));
        row << new QStandardItem(toQ(p.getEmail()));

        QString phones;
        for (const auto& n : p.getNumbers()) {
            phones += toQ(n.getNumber()) + " (" + toQ(n.getType()) + "); ";
        }
        row << new QStandardItem(phones.trimmed());

        model_->appendRow(row);
    }
}

int MainWindow::selectedId() const
{
    const auto rows = view_->selectionModel()->selectedRows();
    if (rows.isEmpty()) return -1;

    const QModelIndex proxyIdx = rows.first();
    const QModelIndex srcIdx = proxy_->mapToSource(proxyIdx);

    bool ok = false;
    const int id = model_->item(srcIdx.row(), 0)->text().toInt(&ok);
    return ok ? id : -1;
}

void MainWindow::onAdd()
{
    PersonDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;

    try {
        Person p = dlg.personFromUi();

        // IMPORTANTE: asignar ID como en consola (max+1)
        int newId = 1;
        for (const auto& person : controller_.getPeople())
            newId = std::max(newId, person.getID() + 1);
        p.setID(newId);

        controller_.addP(p);
        reloadTable();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::onEdit()
{
    const int id = selectedId();
    if (id < 0) return;

    Person current;
    bool found = false;
    for (const auto& p : controller_.getPeople()) {
        if (p.getID() == id) { current = p; found = true; break; }
    }
    if (!found) return;

    PersonDialog dlg(this);
    dlg.setPerson(current);

    if (dlg.exec() != QDialog::Accepted) return;

    try {
        Person updated = dlg.personFromUi();
        controller_.updateP(id, updated);
        reloadTable();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::onDelete()
{
    const int id = selectedId();
    if (id < 0) return;

    if (QMessageBox::question(this, "Delete", "Delete selected contact?") != QMessageBox::Yes)
        return;

    try {
        controller_.deleteP(id);
        reloadTable();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

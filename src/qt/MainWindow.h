#pragma once

#include <QMainWindow>

class QTableView;
class QLineEdit;
class QStandardItemModel;

#include "AppController.h"
#include "MultiFilterProxy.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
   explicit MainWindow(StorageType type, QWidget* parent = nullptr);



private slots:
    void reloadTable();
    void onAdd();
    void onEdit();
    void onDelete();
    void onSearchChanged(const QString& text);

private:
    AppController controller_;

    QStandardItemModel* model_;
    MultiFilterProxy* proxy_;
    QTableView* view_;
    QLineEdit* searchEdit_;

    int selectedId() const;
};

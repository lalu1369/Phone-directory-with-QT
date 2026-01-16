#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QStringList items;
    items << "File (file.txt)" << "PostgreSQL";

    bool ok = false;
    QString choice = QInputDialog::getItem(
        nullptr,
        "Choose storage",
        "Storage type:",
        items,
        0,      // default index
        false,  // editable
        &ok
    );

    StorageType type = StorageType::FileFs;
    if (ok && choice == "PostgreSQL")
        type = StorageType::Pgsql;

    try
    {
        MainWindow w(type);
        w.resize(1200, 650);
        w.show();
        return app.exec();
    }
    catch (const std::exception& e)
    {
        // Si falla PgSQL (no compilado o no conecta), caemos a File y avisamos
        QMessageBox::warning(nullptr, "Storage error",
                             QString("Cannot start with selected storage.\n\nReason: %1\n\nFalling back to File.")
                             .arg(e.what()));

        MainWindow w(StorageType::FileFs);
        w.resize(1200, 650);
        w.show();
        return app.exec();
    }
}

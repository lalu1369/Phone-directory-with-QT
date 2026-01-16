#pragma once

#include <string>
#include <list>
#include "IFileSystem.h"

// Backend de archivo para Qt: usa QFile/QTextStream (requisito del PDF).
class FileSystemQt : public IFileSystem
{
private:
    std::string fileName;

public:
    FileSystemQt();
    explicit FileSystemQt(const std::string&);

    std::list<Person> getPeopleF() override;
    void storeF(Person) override;
    void updateF(int, Person) override;
    void deleteF(int) override;
};

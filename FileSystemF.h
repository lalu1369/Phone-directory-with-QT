#pragma once
#include <string>
#include <list>
#include "IFileSystem.h"

class FileSystemF : public IFileSystem {
private:
    std::string fileName;

public:
    FileSystemF();
    FileSystemF(const std::string&);

    std::list<Person> getPeopleF()  override;
    void storeF (Person)            override;
    void updateF(int, Person)       override;
    void deleteF(int)               override;
};

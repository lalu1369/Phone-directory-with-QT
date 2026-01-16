#pragma once

#include "person.h"
#include <list>

class IFileSystem {
public:
    virtual ~IFileSystem() {}
    virtual void storeF  (Person)      = 0;
    virtual void updateF (int, Person) = 0;
    virtual void deleteF (int)         = 0;
    virtual std::list<Person> getPeopleF() = 0;
};

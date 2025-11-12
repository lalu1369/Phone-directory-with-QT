#pragma once
#include <list>
#include <string>
#include "person.h"
#include "IFileSystem.h"

enum class StorageType {
   FileFs = 0,
   Pgsql  = 1
};

class AppController
{
private:
   std::list<Person> people;
   IFileSystem* fileSystem;

private:
   void loadPeople();

public:
   AppController(StorageType type);

public:
   std::list<Person> getPeople();

   void addP           (Person        );
   void deleteP        (int           );
   void updateP        (int,Person    );
   void sortByName     (bool ascending);
   void sortByLastName (bool ascending);
   std::list<Person> search(const std::string& value);
};
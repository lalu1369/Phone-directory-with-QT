#pragma once
#include <list>
#include <string>
#include "person.h"
#include "IFileSystem.h"

// enum FileTypeEnum {
//   FILE,
//   PGSQL
// };

class AppController
{
private:
   std::list<Person> people;
   IFileSystem* fileSystem;

private:
   void loadPeople();

public:
   AppController(int);

public:
   std::list<Person> getPeople();

   void addP           (Person        );
   void deleteP        (int           );
   void updateP        (int,Person    );
   void sortByName     (bool ascending);
   void sortByLastName (bool ascending);
   std::list<Person> search(const std::string& value);
};
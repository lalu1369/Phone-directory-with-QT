#pragma once

#include <list>
#include <string>

#include "person.h"
#include "IFileSystem.h"

enum class StorageType {
   FileFs = 0,
   Pgsql  = 1
};

enum class SortField {
   ID = 0,
   Name,
   LastName,
   Patronymic,
   Address,
   BirthDate,
   Email
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
   ~AppController();

public:
   // IMPORTANTE: referencia const para no copiar toda la lista
  const std::list<Person> getPeople() const;
  
   void addP(Person);
   void deleteP(int);
   void updateP(int, Person);

   void sortByName(bool ascending);
   void sortByLastName(bool ascending);
   void sortByField(SortField field, bool ascending);


  std::list<Person> search(const std::string& value) const;

};

#include <string>
#include "FileSystemF.h"
#include <fstream>
#include <list>
#include <utility> 
#include "person.h"
#include <iostream>
#include "AppController.h"

using namespace std;

void AppController::loadPeople()
{
    this->people = this->fileSystem->getPeopleF();
}

AppController::AppController(StorageType type) {
    switch (type) {
        case StorageType::FileFs:
            this->fileSystem = new FileSystemF();
            break;
        case StorageType::Pgsql:
            // this->fileSystem = new FileSystemPGSQL();
            break;
        default:
            throw string("Invalid FileSystem");
    }
    this->loadPeople();
}

list<Person> AppController::getPeople()
{
    return this->people;
}

void AppController::addP(Person p)
{
    this->fileSystem->storeF(p);
    this->people.push_back(p);
}
void AppController::deleteP(int id)
{
    this->fileSystem->deleteF(id);
    //logica para eliminar de la Ram
    
    list<Person>::iterator it = this->people.begin();
    do {
        if(it->getID() == id){
            this->people.erase(it);
        }
        ++it;
    }while (it != this->people.end());
}
void AppController::updateP(int id, Person p)
{
    this->fileSystem->updateF(id,p);
    //logica para actualizar en la ram
    for (auto it = people.begin(); it != people.end(); ) {
    if (it->getID() == id) it = people.erase(it);
    else ++it;}
}

void AppController::sortByName(bool ascending)
{
    people.sort([](const Person& lhs, const Person& rhs) { return lhs.getName() < rhs.getName(); });
}

void AppController::sortByLastName(bool ascending)
{
    if (ascending)
        people.sort([](const Person& lhs, const Person& rhs) { return lhs.getLastName() < rhs.getLastName(); });
    else
        people.sort([](const Person& lhs, const Person& rhs) { return lhs.getLastName() > rhs.getLastName(); });
}
list<Person> AppController::search(const string& value)
{
     list<Person> result;

    for (Person p : people)
    {
        // Si el valor aparece en alguno de estos campos
        if (p.getName().find(value) != string::npos ||
            p.getLastName().find(value) != string::npos ||
            p.getPatronimic().find(value) != string::npos ||
            p.getAddress().find(value) != string::npos ||
            p.getEmail().find(value) != string::npos)
        {result.push_back(p);}
        else
        { // buscar también en los números de teléfono
            for (PhoneNumber n : p.getNumbers())
            {if (n.getNumber().find(value) != string::npos)
                {result.push_back(p); break;}}
        }
    }

    return result;
}
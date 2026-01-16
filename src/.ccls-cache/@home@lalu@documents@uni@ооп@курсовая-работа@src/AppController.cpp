#include "AppController.h"
#include "helpers.h"
#include <stdexcept>
#include "PgsqlSystem.h"
#ifdef QT_CORE_LIB
#include "FileSystemQt.h"
#else
#include "FileSystemF.h"
#endif


using namespace std;

static int birthDateKey(const std::string& s)
{
    if (s.size() != 10) return 99999999;

    try
    {
        int year = 0, month = 0, day = 0;

        // YYYY-MM-DD
        if (s[4] == '-' && s[7] == '-')
        {
            year  = std::stoi(s.substr(0, 4));
            month = std::stoi(s.substr(5, 2));
            day   = std::stoi(s.substr(8, 2));
        }
        // DD-MM-YYYY (legacy)
        else if (s[2] == '-' && s[5] == '-')
        {
            day   = std::stoi(s.substr(0, 2));
            month = std::stoi(s.substr(3, 2));
            year  = std::stoi(s.substr(6, 4));
        }
        else
        {
            return 99999999;
        }

        return year * 10000 + month * 100 + day;
    }
    catch (...)
    {
        return 99999999;
    }
}


void AppController::loadPeople()
{
    this->people = this->fileSystem->getPeopleF();
}

AppController::~AppController()
{
  delete this->fileSystem;
  this->fileSystem = nullptr;
}

AppController::AppController(StorageType type)
: fileSystem(nullptr)
{
  switch (type) {
  case StorageType::FileFs:
#ifdef QT_CORE_LIB
  this->fileSystem = new FileSystemQt();
#else
  this->fileSystem = new FileSystemF();
#endif
  break;


  case StorageType::Pgsql:
#ifdef ENABLE_PGSQL
    this->fileSystem = new PgsqlSystem(
        "host=localhost port=5432 dbname=phonebook user=phonebook_user password=lalu"
    );
#else
    throw runtime_error("PostgreSQL disabled in Qt build (ENABLE_PGSQL not set)");
#endif
    break;

  default:
    throw runtime_error("Invalid FileSystem");
  }

  this->loadPeople();
}



const std::list<Person> AppController::getPeople() const
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

   
    for (auto it = this->people.begin(); it != this->people.end(); )
    {
        if (it->getID() == id)
            it = this->people.erase(it);
        else
            ++it;
    }
}

void AppController::updateP(int id, Person p)
{
    p.setID(id);
    this->fileSystem->updateF(id, p);

    for (auto it = this->people.begin(); it != this->people.end(); ++it)
    {
        if (it->getID() == id)
        {
            *it = p;
            return;
        }
    }
}

void AppController::sortByName(bool ascending)
{
    if (ascending)
        people.sort([](const Person& a, const Person& b){ return a.getName() < b.getName(); });
    else
        people.sort([](const Person& a, const Person& b){ return a.getName() > b.getName(); });
}

void AppController::sortByLastName(bool ascending)
{
    if (ascending)
        people.sort([](const Person& lhs, const Person& rhs) { return lhs.getLastName() < rhs.getLastName(); });
    else
        people.sort([](const Person& lhs, const Person& rhs) { return lhs.getLastName() > rhs.getLastName(); });
}

list<Person> AppController::search(const string& value) const
{
    list<Person> result;

    // 1) Limpiamos el texto que escribe el usuario (quitamos espacios al inicio/fin)
    // 2) Lo pasamos a minúsculas para que la búsqueda NO sea case-sensitive
    string query = value;
    trim(query);
    query = toLowerCopy(query);

    // Si el usuario no escribió nada (o solo espacios), no buscamos nada.
    if (query.empty())
        return result;

    for (const Person& p : people)
    {
        // Convertimos los campos del contacto a minúsculas para comparar bien.
        string name       = toLowerCopy(p.getName());
        string lastName   = toLowerCopy(p.getLastName());
        string patronimic = toLowerCopy(p.getPatronimic());
        string address    = toLowerCopy(p.getAddress());
        string email      = toLowerCopy(p.getEmail());

        if (name.find(query)       != string::npos ||
            lastName.find(query)   != string::npos ||
            patronimic.find(query) != string::npos ||
            address.find(query)    != string::npos ||
            email.find(query)      != string::npos)
        {
            result.push_back(p);
        }
        else
        {
            for (const PhoneNumber& n : p.getNumbers())
            {
                string number = toLowerCopy(n.getNumber());
                string type   = toLowerCopy(n.getType());

                if (number.find(query) != string::npos ||
                    type.find(query)   != string::npos)
                {
                    result.push_back(p);
                    break;
                }
            }
        }
    }

    return result;
}
void AppController::sortByField(SortField field, bool ascending)
{
    if (ascending)
    {
        switch (field)
        {
            case SortField::ID:
                people.sort([](const Person& a, const Person& b){ return a.getID() < b.getID(); });
                break;

            case SortField::Name:
                people.sort([](const Person& a, const Person& b){ return a.getName() < b.getName(); });
                break;

            case SortField::LastName:
                people.sort([](const Person& a, const Person& b){ return a.getLastName() < b.getLastName(); });
                break;

            case SortField::Patronymic:
                people.sort([](const Person& a, const Person& b){ return a.getPatronimic() < b.getPatronimic(); });
                break;

            case SortField::Address:
                people.sort([](const Person& a, const Person& b){ return a.getAddress() < b.getAddress(); });
                break;

            case SortField::BirthDate:
                people.sort([](const Person& a, const Person& b){
                    return birthDateKey(a.getBirhtDate()) < birthDateKey(b.getBirhtDate());
                });
                break;

            case SortField::Email:
                people.sort([](const Person& a, const Person& b){ return a.getEmail() < b.getEmail(); });
                break;
        }
    }
    else
    {
        switch (field)
        {
            case SortField::ID:
                people.sort([](const Person& a, const Person& b){ return a.getID() > b.getID(); });
                break;

            case SortField::Name:
                people.sort([](const Person& a, const Person& b){ return a.getName() > b.getName(); });
                break;

            case SortField::LastName:
                people.sort([](const Person& a, const Person& b){ return a.getLastName() > b.getLastName(); });
                break;

            case SortField::Patronymic:
                people.sort([](const Person& a, const Person& b){ return a.getPatronimic() > b.getPatronimic(); });
                break;

            case SortField::Address:
                people.sort([](const Person& a, const Person& b){ return a.getAddress() > b.getAddress(); });
                break;

            case SortField::BirthDate:
                people.sort([](const Person& a, const Person& b){
                    return birthDateKey(a.getBirhtDate()) > birthDateKey(b.getBirhtDate());
                });
                break;

            case SortField::Email:
                people.sort([](const Person& a, const Person& b){ return a.getEmail() > b.getEmail(); });
                break;
        }
    }
}


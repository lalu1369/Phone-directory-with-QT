#include "person.h"
#include "iostream"
#include <regex>
#include <stdexcept>
#include <list>
#include <ctime>
#include "helpers.h"

using namespace std;

// VALIDACIONES
bool Person::validateNames(const string& _name) const
{
    return (regex_match(_name, regex(R"(^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё0-9 -]*[A-Za-zА-Яа-яЁё0-9]$)")));
}
#include <regex>
#include <ctime>

bool Person::validateBirthDate(const std::string& _birthdate_) const 
{
    std::string _birthdate = _birthdate_;

    
    while (!_birthdate.empty() && (_birthdate.front() == ' ' || _birthdate.front() == '\t'))
        _birthdate.erase(_birthdate.begin());

    
    while (!_birthdate.empty() && (_birthdate.back() == ' ' || _birthdate.back() == '\t'))
        _birthdate.pop_back();

    
    std::regex re(R"(^((?:0[1-9]|[12]\d|3[01]))-((?:0[1-9]|1[0-2]))-(\d{4})$)");
    std::smatch m;

   
    if (!std::regex_match(_birthdate, m, re))
        return false;

    
    int day   = std::stoi(m[1].str());
    int month = std::stoi(m[2].str());
    int year  = std::stoi(m[3].str());

    if (year < 1900) return false;

    int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    if (leap && month == 2)
        mdays[1] = 29;

    if (day > mdays[month - 1])
        return false;

    time_t t = time(nullptr);
    tm* now = localtime(&t);

    int cY = now->tm_year + 1900;
    int cM = now->tm_mon + 1;
    int cD = now->tm_mday;

    if (year > cY) return false;
    if (year == cY && month > cM) return false;
    if (year == cY && month == cM && day >= cD) return false;

    return true;
}


bool Person::validateEmail(const string& _email) const 
{
    return (regex_match(_email, regex(R"(^[A-Za-z0-9]+@[A-Za-z0-9]+(\.[A-Za-z0-9]+)+$)")));
}

// CONSTRUCTORES
Person::Person()
    : name(""), lastName(""), patronimic(""), address(""),
      birthDate(""), email(""), ID(0) {}

Person::Person(const string& _name,       const string&    _lastName,
               const string& _patronimic, const string&    _address,
               const string& _birthDate,  const string&    _email,
               int    _ID,   const PhoneNumber& _phoneNumber)
{
    this->setName       (_name);
    this->setLastName   (_lastName);
    this->setPatronimic (_patronimic);
    this->setAddress    (_address);
    this->setBirhtDate  (_birthDate);
    this->setEmail      (_email);
    this->setID         (_ID);
    this->addPhoneNumber(_phoneNumber);
}

Person::Person(const string& _name,       const string& _lastName,
               const string& _patronimic, const string& _address,
               const string& _birthDate,  const string& _email,
               int _ID,           const list<PhoneNumber>& _numbers)
{
    this->setName       (_name      );
    this->setLastName   (_lastName  );
    this->setPatronimic (_patronimic);
    this->setAddress    (_address   );
    this->setBirhtDate  (_birthDate );
    this->setEmail      (_email     );
    this->setID         (_ID        );

    for (auto it = _numbers.begin(); it != _numbers.end(); ++it)
        {this->addPhoneNumber(*it);}

}

Person::Person(const Person &other)
{
    this->name       = other.name;
    this->lastName   = other.lastName;
    this->patronimic = other.patronimic;
    this->address    = other.address;
    this->birthDate  = other.birthDate;
    this->email      = other.email;
    this->ID         = other.ID;
    this->numbers    = other.numbers;
}

// GETTERS
const string& Person::getName   ()      const { return this->name;       }
const string& Person::getLastName()     const { return this->lastName;   }
const string& Person::getPatronimic()   const { return this->patronimic; }
const string& Person::getAddress()      const { return this->address;    }
const string& Person::getBirhtDate()    const { return this->birthDate;  }
const string& Person::getEmail()        const { return this->email;      }
int Person::getID()                     const { return this->ID;         }
const list<PhoneNumber>& Person::getNumbers() const { return this->numbers; }

// SETERS
void Person::setName(const string& name_)
{
    string tmp = name_;
    trim(tmp);
    if (!this->validateNames(name_))
        throw string("Invalid name");
    this->name = name_;
}
void Person::setLastName(const string& lastName_)
{
    string tmp = lastName_;
    trim(tmp);
    if (!this->validateNames(lastName_))
        throw string("Invalid lastname");
    this->lastName = lastName_;
}
void Person::setPatronimic(const string& patronimic_)
{
    string tmp = patronimic_;
    trim(tmp);
    if (!this->validateNames(patronimic_))
        throw string("Invalid patronimic");
    this->patronimic = patronimic_;
}
void Person::setAddress(const string& address_)
{
    string tmp = address_;
    trim(tmp);
    this->address = address_;
}
void Person::setBirhtDate(const string& birthDate_)
{
    if (!this->validateBirthDate(birthDate_))
        throw string("Invalid birthdate");
    this->birthDate = birthDate_;
}
void Person::setEmail(const string& email_)
{
    string tmp = email_;
    trim(tmp);
    if (!this->validateEmail(email_))
        throw string("Invalid email");
    this->email = email_;
}
void Person::setID(int ID_)
{
    this->ID = ID_;
}

// OTHERS
void Person::addPhoneNumber(const PhoneNumber& number) 
{
    this->numbers.push_front(number);
}

bool Person::removePhoneNumberByValue(const PhoneNumber& number)
{
       for (auto it = this->numbers.begin(); it != this->numbers.end(); ++it)
    {
        if (it->getNumber() == number.getNumber())
        {
            this->numbers.erase(it);
            return true;  // encontrado y eliminado
        }
    }
    return false; // no se encontró
}

string Person::toString(){
    string res = "";
    res = res + this-> getName      () + " "
              + this-> getLastName  () + " " 
              + this-> getPatronimic() + " " 
              + this-> getAddress   () + " " 
              + this-> getBirhtDate () + " "
              + this-> getEmail     () + "\n";
    
    return res;
}


string Person::toStringNumbers()  
{
    string result;
    for (auto it = this->numbers.begin(); it != this->numbers.end(); ++it)
        {result = result + "[" + it->getNumber() + "," + it->getType() + "]\n";}
    return result;
}

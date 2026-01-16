#include "person.h"
#include "iostream"
#include <regex>
#include <stdexcept>
#include <list>
#include <ctime>
#include <regex>

#include "helpers.h"

using namespace std;

// VALIDACIONES

static std::string normalizeBirthDate(std::string s)
{
    trim(s);
    if (s.empty()) return s;

    std::smatch m;

    // DD-MM-YYYY
    std::regex re_dmy(R"(^([0-2]\d|3[01])-(0[1-9]|1[0-2])-(\d{4})$)");
    if (std::regex_match(s, m, re_dmy))
    {
        const std::string dd = m[1].str();
        const std::string mm = m[2].str();
        const std::string yyyy = m[3].str();
        return yyyy + "-" + mm + "-" + dd;
    }

    // YYYY-MM-DD o cualquier otra cosa: lo devuelve tal cual
    return s;
}

static bool parseYMD(const std::string& s, int& year, int& month, int& day)
{
    std::smatch m;
    std::regex re_ymd(R"(^(\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])$)");
    if (!std::regex_match(s, m, re_ymd)) return false;

    year  = std::stoi(m[1].str());
    month = std::stoi(m[2].str());
    day   = std::stoi(m[3].str());
    return true;
}

bool Person::validateNames(const string& _name) const
{
    return (regex_match(_name, regex(R"(^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё0-9 -]*[A-Za-zА-Яа-яЁё0-9]$)")));
}

bool Person::validateBirthDate(const std::string& birthdate_) const
{
    std::string s = birthdate_;
    trim(s);

    if (s.empty()) return true; // opcional

    // Normaliza DD-MM-YYYY -> YYYY-MM-DD
    s = normalizeBirthDate(s);

    int year = 0, month = 0, day = 0;
    if (!parseYMD(s, year, month, day)) return false;

    if (year < 1900) return false;

    int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap) mdays[1] = 29;

    if (day > mdays[month - 1]) return false;

    // No permitir fecha futura
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int cY = now->tm_year + 1900;
    int cM = now->tm_mon + 1;
    int cD = now->tm_mday;

    if (year > cY) return false;
    if (year == cY && month > cM) return false;
    if (year == cY && month == cM && day > cD) return false;

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
    if (!this->validateNames(tmp))
        throw string("Invalid name");
    this->name = tmp;
}
void Person::setLastName(const string& lastName_)
{
    string tmp = lastName_;
    trim(tmp);
    if (!validateNames(tmp))
        throw string("Invalid last name");
    this->lastName = tmp;
}
void Person::setPatronimic(const std::string& patronimic_)
{
    std::string tmp = patronimic_;
    trim(tmp); 
    if (tmp.empty()) {
        this->patronimic.clear();   // guarda ""
        return;
    }

    if (!validateNames(tmp)) {
        throw std::invalid_argument("Invalid patronymic");
    }

    this->patronimic = tmp; // guarda ya limpio
}

void Person::setAddress(const string& address_)
{
  string tmp = address_;
  trim(tmp);

  if (tmp.empty())
    {this->address.clear(); // guarda ""
        return;}
  this->address = tmp;
}

void Person::setBirhtDate(const std::string& birthDate_)
{
    std::string tmp = birthDate_;
    trim(tmp);

    if (tmp.empty())
    {
        this->birthDate.clear();
        return;
    }

    // Normaliza si venía DD-MM-YYYY
    tmp = normalizeBirthDate(tmp);

    if (!validateBirthDate(tmp))
        throw std::string("Invalid birthdate");

    // Guardamos SIEMPRE normalizado
    this->birthDate = tmp;
}


void Person::setEmail(const string& email_)
{
    string tmp = email_;
    trim(tmp);
    remove_all_whitespace(tmp);

    if (!this->validateEmail(tmp))
        throw invalid_argument("Email doesn't match required format");

    this->email = tmp;
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

string Person::toString() const {
    string res = "";
    res = res + this-> getName      () + " "
              + this-> getLastName  () + " " 
              + this-> getPatronimic() + " " 
              + this-> getAddress   () + " " 
              + this-> getBirhtDate () + " "
              + this-> getEmail     () + "\n";
    
    return res;
}


string Person::toStringNumbers() const   
{
    string result;
    for (auto it = this->numbers.begin(); it != this->numbers.end(); ++it)
        {result = result + "[" + it->getNumber() + "," + it->getType() + "]\n";}
    return result;
}

#include "PhoneNumber.h"
#include <regex>
#include <stdexcept>
#include "helpers.h"

using namespace std;

bool PhoneNumber::validateNumber(const string& number_) const
{
    return (regex_match(number_, regex(R"(^(\+7|8)\s*\(?\d{3}\)?\s*\d{3}[-\s]?\d{2}[-\s]?\d{2}$)")));
}  

PhoneNumber::PhoneNumber() : number(""), type("") {}

PhoneNumber::PhoneNumber(const std::string& _number, const string& _type) {
    this->setType(_type);
    this->setNumber(_number);
}

const string& PhoneNumber::getNumber() const {return this->number;};
const string& PhoneNumber::getType  () const {return this->type;  };

void PhoneNumber::setNumber(const string& number_)
{
    string clean = number_;
    trim(clean);
    if(!this->validateNumber(clean))
     throw string("invalid phone number");
    this->number = number_;
} 
void PhoneNumber::setType(const string& type_)
{
    this->type = type_;
}
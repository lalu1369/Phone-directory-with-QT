#pragma once
#include <string>

class PhoneNumber {
private:
    std::string number;
    std::string type; // home, work, mobile

    bool validateNumber(const std::string& number_) const;

public:
    PhoneNumber();
    PhoneNumber(const std::string& number, const std::string& type);

    const std::string& getNumber() const;
    const std::string& getType  () const;

    void setNumber(const std::string&);
    void setType  (const std::string&);
};

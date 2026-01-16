#pragma once 

#include <string>
#include <list> 
#include "PhoneNumber.h"

class Person{ 

    private: 
        std::string name; 
        std::string lastName; 
        std::string patronimic; 
        std::string address; 
        std::string birthDate;
        std::string email; 
        int ID;
        std::list<PhoneNumber> numbers; 

        bool validateNames    (const std::string&) const; 
        bool validateBirthDate(const std::string&) const; 
        bool validateEmail    (const std::string&) const; 


    public: 
        Person(); 
        Person( const std::string& name,       const std::string& lastName, 
                const std::string& patronimic, const std::string& address, 
                const std::string& birthDate,  const std::string& email, 
                int ID,const PhoneNumber& _phoneNumber);
        Person( const std::string& name,       const std::string& lastName, 
                const std::string& patronimic, const std::string& address, 
                const std::string& birthDate,  const std::string& email, 
                int ID, const std::list<PhoneNumber>& numbers); 

        Person(const Person&)                = default;
        Person& operator=(const Person&)     = default;
        Person(Person&&) noexcept            = default;
        Person& operator=(Person&&) noexcept = default;
        ~Person()                            = default;


    public: 
        const std::string& getName       () const;
        const std::string& getLastName   () const;
        const std::string& getPatronimic () const;
        const std::string& getAddress    () const;
        const std::string& getBirhtDate  () const;
        const std::string& getEmail      () const;
        int           getID         () const; 
        const std::list<PhoneNumber>& getNumbers() const;

    public:  
        void setName       (const std::string&);
        void setLastName   (const std::string&);
        void setPatronimic (const std::string&);
        void setAddress    (const std::string&);
        void setBirhtDate  (const std::string&);
        void setEmail      (const std::string&);
        void setID         (int);
        
    public: 
        void addPhoneNumber          (const PhoneNumber&); 
        bool removePhoneNumberByValue(const PhoneNumber&);
        std::string toString       () const;
        std::string toStringNumbers() const;
    

};

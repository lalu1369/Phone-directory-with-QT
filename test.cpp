#include "iostream"
//#include <regex>
#include "person.h"
#include "PhoneNumber.h"
#include <string>
#include <list>
//#include "FileSystemF.h"
//#include "IFileSystem.h"
using namespace std;

int main()
{

    try
    {
        PhoneNumber pn1=PhoneNumber("+78121234567","home");
         PhoneNumber pn2=PhoneNumber("+78121234567","otro");
         PhoneNumber pn3=PhoneNumber("+78121234567","otro2");

         list<PhoneNumber> numbers;
        numbers.push_back(pn1);
        numbers.push_back(pn2);
         numbers.push_back(pn3);

        Person person1 = Person("Luciana", "Limpias", "Algo", "la13", "20-02-2005","luciana@mail.com", 3,numbers);
         //Person person1 = Person("Luciana", "Limpias", "Algo", "la13", "20-02-2005","mail.com", 1,pn1);
         //person1.addPhoneNumber(pn1);

         cout << person1.toString();
         cout << person1.toStringNumbers();

        //IFileSystem* file = new FileSystemF();
        // file->deleteF(3);
         //list<Person> people = file->getPeopleF();

         //file->storeF(person1);

         //person1.setName("Nombre cambiado");

         //file->updateF(person1.getID(),person1);
    }
    catch (string e)
    {
        cout << e;
    }
     
   


}
// g++ test.cpp -o test.exe

// g++ person.cpp PhoneNumber.cpp test.cpp person.h PhoneNumber.h helpers.cpp FileSystemF.h FileSystemF.cpp IFileSystem.h AppController.h AppController.cpp  -o test.exe
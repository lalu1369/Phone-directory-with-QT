#include <string>
#include "FileSystemF.h"
#include <fstream>
#include <list>
#include "person.h"
#include <iostream>

using namespace std;

// FileSystemF::~FileSystemF(){

// }

FileSystemF::FileSystemF()
{
    this->fileName = "file.txt";
}

FileSystemF::FileSystemF(const string& _fileName)
{
    //validar que no sea nulo y que sea de tipo .txt
    this->fileName = _fileName;

}

list<Person> FileSystemF::getPeopleF(){
    // Open a file in read mode
    ifstream file(this->fileName);
    string strok1,strok2;

    list<Person> people = list<Person>();

    // Check if the file is opened
    if (!file.is_open()) {
        throw string("Error: Unable to open file!");
    }

    while (!file.eof()){
        // Read string from the file
        getline(file, strok1);
        if (strok1 == ""){
            file.close();
            return people;
        }
        getline(file, strok2);

        string id = strok1.substr(0, strok1.find("|"));
        strok1 = strok1.substr(id.length()+1,strok1.length());
        string name = strok1.substr(0, strok1.find("|"));
        strok1 = strok1.substr(name.length()+1,strok1.length());
        string lastname = strok1.substr(0, strok1.find("|"));
        strok1 = strok1.substr(lastname.length()+1,strok1.length());
        string patronimic = strok1.substr(0, strok1.find("|"));
        strok1 = strok1.substr(patronimic.length()+1,strok1.length());
        string address = strok1.substr(0, strok1.find("|"));
        strok1 = strok1.substr(address.length()+1,strok1.length());
        string birthdate = strok1.substr(0, strok1.find("|"));
        strok1 = strok1.substr(birthdate.length()+1,strok1.length());
        string email = strok1.substr(0, strok1.find("|"));

        // cout << id << " , " << name << " , " << lastname << " , " <<patronimic <<" , "<<address << " , "<<birthdate << " , " <<email <<"\n";

        list<PhoneNumber> numbers;
        string nro_phones = strok2.substr(0, strok2.find("|"));
        strok2 = strok2.substr(nro_phones.length()+1,strok2.length());
        int nro = atoi( nro_phones.c_str() );
        for (int i=0; i<nro; i++){
            string number = strok2.substr(0, strok2.find("|"));
            strok2 = strok2.substr(number.length()+1,strok2.length());
            string type = strok2.substr(0, strok2.find("|"));
            strok2 = strok2.substr(type.length()+1,strok2.length());

            PhoneNumber phoneNumber = PhoneNumber(number,type);
            numbers.push_back(phoneNumber);
            // cout << number << " , " << type<<"\n";
        }

        Person person = Person(name,lastname,patronimic,address,birthdate,email,atoi( id.c_str() ),numbers);
        people.push_back(person);
    }

    file.close();
    return people;
}

void FileSystemF::storeF(Person p){
    ofstream file;
    file.open(this->fileName, std::ios_base::app);

    // Check if the file is opened
    if (!file.is_open()) {
        throw string("Error: Unable to open file!");
    }

    file << p.getID() << "|" <<p.getName() << "|" << p.getLastName() << "|"
    << p.getPatronimic() << "|" << p.getAddress() << "|" << p.getBirhtDate() << "|"
    << p.getEmail() << "\n";

    // recorrer telefonos
    string phonesString = to_string(p.getNumbers().size()) + "|";
    list<PhoneNumber> numbersList = p.getNumbers();
    list<PhoneNumber>::iterator it = numbersList.begin();
    do {
        phonesString = phonesString + it->getNumber() + "|" + it->getType() + "|";
        ++it;
    }while (it != numbersList.end());
    phonesString = phonesString + "\n";

    file << phonesString;

    file.close();

}

void FileSystemF::updateF(int id, Person p){
    //read the data
    list<Person> people = this->getPeopleF();
    //delete the data
    ofstream file;
    file.open(this->fileName,std::ios_base::trunc);
    file.close();

    //write the data
    list<Person>::iterator it = people.begin();
    do{
        if (it->getID() == id){
            *it = move(p);
        }
        this->storeF(*it);
        ++it;
    }while (it != people.end());
}

void FileSystemF::deleteF(int id){

    //ARREGLARLO 
    
    //read the data
    list<Person> people = this->getPeopleF();
    //delete the data
    ofstream file;
    file.open(this->fileName,std::ios_base::trunc);
    file.close();

    //write the data
    list<Person>::iterator it = people.begin();
    do{
        if (it->getID() != id){
            this->storeF(*it);   
        }
        ++it;
    }while (it != people.end());

}
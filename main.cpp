#include <iostream>
#include <string>
#include <list>
#include "AppController.h"
#include "person.h"
#include "PhoneNumber.h"

using namespace std;

void showMenu() {
    cout << "\n========== MENU ==========\n";
    cout << "1. Add person\n";
    cout << "2. Show all people\n";
    cout << "3. Search person\n";
    cout << "4. Delete person\n";
    cout << "0. Exit\n";
    cout << "==========================\n";
    cout << "Option: ";
}

Person createPerson() {
    string name, lastname, patronymic, address, birthdate, email;
    int id;

    cout << "ID: ";
    cin >> id; cin.ignore();
    cout << "Name: ";
    getline(cin, name);
    cout << "Last name: ";
    getline(cin, lastname);
    cout << "Patronymic: ";
    getline(cin, patronymic);
    cout << "Address: ";
    getline(cin, address);
    cout << "Birth date (dd-mm-yyyy): ";
    getline(cin, birthdate);
    cout << "Email: ";
    getline(cin, email);

    int phoneCount;
    cout << "How many phone numbers do you want to add? ";
    cin >> phoneCount; cin.ignore();

    list<PhoneNumber> phones;
    for (int i = 0; i < phoneCount; i++) {
        string number, type;
        cout << "Phone #" << i + 1 << ": ";
        getline(cin, number);
        cout << "Type (home, work, etc): ";
        getline(cin, type);
        try {
            PhoneNumber p(number, type);
            phones.push_back(p);
        } catch (string e) {
            cout << "Error: " << e << endl;
            i--; // repeat this one
        }
    }

    try {
        Person p(name, lastname, patronymic, address, birthdate, email, id, phones);
        return p;
    } catch (string e) {
        cout << "Error creating person: " << e << endl;
        return Person();
    }
}

void showPeople(list<Person> people) {
    for (Person p : people) {
        cout << "-------------------------\n";
        cout << p.toString();
        cout << p.toStringNumbers();
    }
}

int main() {
    AppController app(StorageType::FileFs);

    int option;
    do {
        showMenu();
        cin >> option;
        cin.ignore();

        switch (option) {
        case 1: {
            Person newPerson = createPerson();
            app.addP(newPerson);
            cout << "Person added!\n";
            break;
        }
        case 2: {
            list<Person> people = app.getPeople();
            showPeople(people);
            break;
        }
        case 3: {
            string text;
            cout << "Enter text to search: ";
            getline(cin, text);
            list<Person> results = app.search(text);
            showPeople(results);
            break;
        }
        case 4: {
            int id;
            cout << "Enter ID to delete: ";
            cin >> id;
            app.deleteP(id);
            cout << "Person deleted (if existed)\n";
            break;
        }
        case 0:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid option.\n";
        }
    } while (option != 0);

    return 0;
}

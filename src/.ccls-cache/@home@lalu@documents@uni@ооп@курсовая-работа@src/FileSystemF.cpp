#include <string>
#include "FileSystemF.h"
#include <fstream>
#include <list>
#include "person.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm> 

using namespace std;


static vector<string> split_pipe(const string& s)
{
    vector<string> out;
    string token;
    stringstream ss(s);
    while (getline(ss, token, '|'))
        out.push_back(token);
    return out;
}

static bool is_unsigned_int(const string& s)
{
    if (s.empty()) return false;
    for (unsigned char c : s)
        if (!isdigit(c)) return false;
    return true;
}

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

list<Person> FileSystemF::getPeopleF()
{
    ifstream file(this->fileName);
    if (!file.is_open())
        throw runtime_error("Error: Unable to open file!");

    list<Person> people;
    string line;

    int fallback_id = 1;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        try
        {
            auto parts = split_pipe(line);

            bool has_id = parts.size() >= 8 && is_unsigned_int(parts[0]);
            size_t idx = 0;

            int id = 0;
            if (has_id) id = stoi(parts[idx++]);
            else        id = fallback_id;

            if (parts.size() < (has_id ? 8u : 7u))
                throw runtime_error("Not enough fields");

            string name       = parts[idx++];
            string lastname   = parts[idx++];
            string patronimic = parts[idx++];
            string address    = parts[idx++];
            string birthdate  = parts[idx++];
            string email      = parts[idx++];
            int phone_count   = stoi(parts[idx++]);

            list<PhoneNumber> numbers;
            for (int i = 0; i < phone_count; ++i)
            {
                if (idx + 1 >= parts.size())
                    throw runtime_error("Phone fields mismatch");

                const string& num  = parts[idx++];
                const string& type = parts[idx++];
                numbers.emplace_back(num, type);
            }

            people.emplace_back(name, lastname, patronimic, address, birthdate, email, id, numbers);
        }
        catch (...) {
            // si una línea está mal, la saltas sin romper todo
        }

        ++fallback_id;
    }

    return people;
}


void FileSystemF::storeF(Person p)
{
    ofstream file(this->fileName, ios_base::app);
    if (!file.is_open())
        throw runtime_error("Error: Unable to open file!");

    file << p.getID() << "|" << p.getName() << "|" << p.getLastName() << "|"
         << p.getPatronimic() << "|" << p.getAddress() << "|" << p.getBirhtDate() << "|"
         << p.getEmail() << "|";

    const auto& nums = p.getNumbers();
    file << nums.size() << "|";

    for (const auto& n : nums)
        file << n.getNumber() << "|" << n.getType() << "|";

    file << "\n";
}

void FileSystemF::updateF(int id, Person p)
{
    auto people = this->getPeopleF();

    bool found = false;
    for (auto& it : people)
    {
        if (it.getID() == id)
        {
            it = p;
            it.setID(id);
            found = true;
            break;
        }
    }

    if (!found) return;

    ofstream trunc_file(this->fileName, ios_base::trunc);
    trunc_file.close();

    for (const auto& person : people)
        this->storeF(person);
}

void FileSystemF::deleteF(int id)
{
    auto people = this->getPeopleF();

    for (auto it = people.begin(); it != people.end(); )
    {
        if (it->getID() == id)
            it = people.erase(it);
        else
            ++it;
    }

    ofstream trunc_file(this->fileName, ios_base::trunc);
    trunc_file.close();

    for (const auto& person : people)
        this->storeF(person);
}



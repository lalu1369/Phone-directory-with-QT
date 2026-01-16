#include "FileSystemQt.h"

#include <QFile>
#include <QTextStream>
#include <QString>

#include <vector>
#include <sstream>
#include <cctype>
#include <stdexcept>

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

FileSystemQt::FileSystemQt()
{
    this->fileName = "file.txt";
}

FileSystemQt::FileSystemQt(const std::string& _fileName)
{
    this->fileName = _fileName;
}

list<Person> FileSystemQt::getPeopleF()
{
    QFile file(QString::fromStdString(this->fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw runtime_error("Error: Unable to open file (QFile)!");

    QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    in.setCodec("UTF-8");
#endif

    list<Person> people;
    int fallback_id = 1;

    while (!in.atEnd())
    {
        QString qline = in.readLine();
        if (qline.isEmpty())
            continue;

        string line = qline.toStdString();

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
        catch (...)
        {
            // Si la línea está mal, la saltamos sin romper todo.
        }

        ++fallback_id;
    }

    return people;
}

void FileSystemQt::storeF(Person p)
{
    QFile file(QString::fromStdString(this->fileName));
    if (!file.open(QIODevice::Append | QIODevice::Text))
        throw runtime_error("Error: Unable to open file (QFile)!");

    QTextStream out(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    out.setCodec("UTF-8");
#endif

    out << p.getID() << "|"
        << QString::fromStdString(p.getName()) << "|"
        << QString::fromStdString(p.getLastName()) << "|"
        << QString::fromStdString(p.getPatronimic()) << "|"
        << QString::fromStdString(p.getAddress()) << "|"
        << QString::fromStdString(p.getBirhtDate()) << "|"
        << QString::fromStdString(p.getEmail()) << "|";

    const auto& nums = p.getNumbers();
    out << nums.size() << "|";

    for (const auto& n : nums)
        out << QString::fromStdString(n.getNumber()) << "|"
            << QString::fromStdString(n.getType()) << "|";

    out << "\n";
}

void FileSystemQt::updateF(int id, Person p)
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

    // Truncar el archivo
    {
        QFile file(QString::fromStdString(this->fileName));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            throw runtime_error("Error: Unable to open file for truncation (QFile)!");
        file.close();
    }

    // Reescribir todo
    for (const auto& person : people)
        this->storeF(person);
}

void FileSystemQt::deleteF(int id)
{
    auto people = this->getPeopleF();

    for (auto it = people.begin(); it != people.end(); )
    {
        if (it->getID() == id)
            it = people.erase(it);
        else
            ++it;
    }

    // Truncar
    {
        QFile file(QString::fromStdString(this->fileName));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            throw runtime_error("Error: Unable to open file for truncation (QFile)!");
        file.close();
    }

    // Reescribir
    for (const auto& person : people)
        this->storeF(person);
}

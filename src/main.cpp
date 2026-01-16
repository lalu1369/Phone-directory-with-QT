#include <iostream>
#include <string>
#include <list>
#include <limits>
#include <algorithm>

#include "AppController.h"

using namespace std;

/* ===================== Helpers ===================== */

static int readIntFromLine(const string& prompt)
{
    while (true)
    {
        cout << prompt;
        string line;
        getline(cin, line);

        // rechaza vacío o solo espacios
        bool onlySpaces = true;
        for (char ch : line)
        {
            if (!isspace(static_cast<unsigned char>(ch)))
            {
                onlySpaces = false;
                break;
            }
        }
        if (onlySpaces)
        {
            cout << "Invalid number. Try again.\n";
            continue;
        }

        try
        {
            size_t idx = 0;
            int value = stoi(line, &idx);

            // si quedó basura al final (ej: "12abc"), no es válido
            while (idx < line.size() && isspace(static_cast<unsigned char>(line[idx]))) idx++;
            if (idx != line.size())
            {
                cout << "Invalid number. Try again.\n";
                continue;
            }

            return value;
        }
        catch (...)
        {
            cout << "Invalid number. Try again.\n";
        }
    }
}

static void showPeople(const list<Person>& people)
{
    if (people.empty())
    {
        cout << "No people found.\n";
        return;
    }

    for (const auto& p : people)
    {
        cout << "----------------------------------\n";
        cout << p.toString() << "\n";
        cout << p.toStringNumbers() << "\n";
    }
}

/* ===================== Create Person ===================== */

static Person createPerson(int id)
{
    string name, lastname, patronimic, address, birthdate, email;

    cout << "Name: ";
    getline(cin, name);

    cout << "Last name: ";
    getline(cin, lastname);

    cout << "Patronymic (can be empty): ";
    getline(cin, patronimic);

    cout << "Address: ";
    getline(cin, address);

    cout << "Birth date (YYYY-MM-DD): ";
    getline(cin, birthdate);

    cout << "Email: ";
    getline(cin, email);

    int phoneCount = 0;
    while (true)
    {
        phoneCount = readIntFromLine("Phone count (>=1): ");
        if (phoneCount >= 1) break;
        cout << "At least one phone number is required.\n";
    }

    list<PhoneNumber> numbers;
    for (int i = 0; i < phoneCount; ++i)
    {
        string num, type;

        cout << "Phone #" << (i + 1) << " number: ";
        getline(cin, num);

        cout << "Phone #" << (i + 1) << " type: ";
        getline(cin, type);

        numbers.emplace_back(num, type); // aquí puede lanzar si formato inválido
    }

    // Aquí también puede lanzar por validación de Person (fecha, email, etc.)
    return Person(name, lastname, patronimic, address, birthdate, email, id, numbers);
}

/* ===================== Main ===================== */

int main()
{
  try
    {

      cout << "Choose storage type:\n";
      cout << "1. File (file.txt)\n";
      cout << "2. PostgreSQL\n";

      int storageChoice = readIntFromLine("Choice: ");

      StorageType type = StorageType::FileFs;
      if (storageChoice == 2)
	type = StorageType::Pgsql;

      AppController app(type);


      while (true)
        {
          cout << "\n========= MENU =========\n";
          cout << "1. Show all people\n";
          cout << "2. Add person\n";
          cout << "3. Delete person by ID\n";
          cout << "4. Search\n";
          cout << "5. Edit person by ID\n";
          cout << "6. Sort people (choose field)\n";
          cout << "0. Exit\n";

          int choice = readIntFromLine("Choose option: ");
          if (choice == 0) break;

          switch (choice)
            {
            case 1:
              {
                showPeople(app.getPeople());
                break;
              }

            case 2:
              {
                // calcula newId como max(ID)+1
                auto people = app.getPeople();
                int newId = 1;
                for (const auto& person : people)
                  newId = max(newId, person.getID() + 1);

                try
                  {
                    Person p = createPerson(newId);
                    app.addP(p);
                    cout << "Person added successfully.\n";
                  }
                    catch (const std::exception& e)
                      {
                        cout << "Person was NOT added. Reason: " << e.what() << "\n";
                      }
                    catch (const std::string& e)
                      {
                        cout << "Person was NOT added. Reason: " << e << "\n";
                      }
                    catch (const char* e)
                      {
                        cout << "Person was NOT added. Reason: " << e << "\n";
                      }
                    catch (...)
                      {
                        cout << "Person was NOT added. Reason: unknown error\n";
                      }

                break;
              }

            case 3:
              {
                int id = readIntFromLine("Enter ID to delete: ");
                try
                  {
                    app.deleteP(id);
                    cout << "Delete operation finished.\n";
                  }
                    catch (const std::exception& e)
                      {
                        cout << "Delete failed. Reason: " << e.what() << "\n";
                      }
                    catch (const std::string& e)
                      {
                        cout << "Delete failed. Reason: " << e << "\n";
                      }
                    catch (const char* e)
                      {
                        cout << "Delete failed. Reason: " << e << "\n";
                      }
                    catch (...)
                      {
                        cout << "Delete failed. Reason: unknown error\n";
                      }
                break;
              }

            case 4:
              {
                cout << "Search value: ";
                string value;
                getline(cin, value);

                try
                  {
                    auto result = app.search(value);
                    showPeople(result);
                  }
                    catch (const std::exception& e)
                      {
                        cout << "Search failed. Reason: " << e.what() << "\n";
                      }
                    catch (const std::string& e)
                      {
                        cout << "Search failed. Reason: " << e << "\n";
                      }
                    catch (const char* e)
                      {
                        cout << "Search failed. Reason: " << e << "\n";
                      }
                    catch (...)
                      {
                        cout << "Search failed. Reason: unknown error\n";
                      }
                break;
              }

            case 5:
              {
                int id = readIntFromLine("Enter ID to edit: ");

                auto people = app.getPeople();
                bool exists = false;
                for (const auto& person : people)
                  {
                    if (person.getID() == id)
                      {
                        exists = true;
                        break;
                      }
                  }

                if (!exists)
                  {
                    cout << "No person found with ID " << id << ".\n";
                    break;
                  }

                try
                  {
                    Person updated = createPerson(id);
                    app.updateP(id, updated);
                    cout << "Person updated successfully.\n";
                  }
                    catch (const std::exception& e)
                      {
                        cout << "Person was NOT updated. Reason: " << e.what() << "\n";
                      }
                    catch (const std::string& e)
                      {
                        cout << "Person was NOT updated. Reason: " << e << "\n";
                      }
                    catch (const char* e)
                      {
                        cout << "Person was NOT updated. Reason: " << e << "\n";
                      }
                    catch (...)
                      {
                        cout << "Person was NOT updated. Reason: unknown error\n";
                      }

                break;
              }

            case 6:
              {
                cout << "Sort by:\n";
                cout << "1. ID\n";
                cout << "2. Name\n";
                cout << "3. Last Name\n";
                cout << "4. Patronymic\n";
                cout << "5. Address\n";
                cout << "6. Birth date\n";
                cout << "7. Email\n";

                int fieldChoice = readIntFromLine("Choose field: ");
                int asc = readIntFromLine("1 = ASC, 0 = DESC: ");

                SortField field = SortField::Name;
                switch (fieldChoice)
                  {
                  case 1: field = SortField::ID; break;
                  case 2: field = SortField::Name; break;
                  case 3: field = SortField::LastName; break;
                  case 4: field = SortField::Patronymic; break;
                  case 5: field = SortField::Address; break;
                  case 6: field = SortField::BirthDate; break;
                  case 7: field = SortField::Email; break;
                  default:
                    cout << "Invalid field. Sorting by Name.\n";
                    field = SortField::Name;
                    break;
                  }

                try
                  {
                    app.sortByField(field, asc == 1);
                    cout << "Sorted.\n";
                    showPeople(app.getPeople());
                  }
                    catch (const std::exception& e)
                      {
                        cout << "Sort failed. Reason: " << e.what() << "\n";
                      }
                    catch (const std::string& e)
                      {
                        cout << "Sort failed. Reason: " << e << "\n";
                      }
                    catch (const char* e)
                      {
                        cout << "Sort failed. Reason: " << e << "\n";
                      }
                    catch (...)
                      {
                        cout << "Sort failed. Reason: unknown error\n";
                      }

                break;
              }

            default:
              {
                cout << "Invalid option.\n";
                break;
              }
            }
        }
    }
    catch (const std::exception& e)
      {
        cout << "Fatal error: " << e.what() << "\n";
      }
    catch (const std::string& e)
      {
        cout << "Fatal error: " << e << "\n";
      }
    catch (const char* e)
      {
        cout << "Fatal error: " << e << "\n";
      }
    catch (...)
      {
        cout << "Fatal error: unknown\n";
      }

  return 0;
}

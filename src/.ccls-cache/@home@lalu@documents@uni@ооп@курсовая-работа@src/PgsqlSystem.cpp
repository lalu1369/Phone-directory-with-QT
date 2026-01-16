#include "PgsqlSystem.h"
#include <pqxx/pqxx>
#include <stdexcept>

PgsqlSystem::PgsqlSystem(const std::string& connectionString)
    : connStr_(connectionString)
{
    ensureSchema_();
}

void PgsqlSystem::ensureSchema_()
{
    pqxx::connection c(connStr_);
    pqxx::work tx(c);

    tx.exec(R"sql(
        CREATE TABLE IF NOT EXISTS contacts (
          id           INT PRIMARY KEY,
          name         TEXT NOT NULL,
          last_name    TEXT NOT NULL,
          patronymic   TEXT,
          address      TEXT,
          birth_date   TEXT,
          email        TEXT NOT NULL
        );
    )sql");

    tx.exec(R"sql(
        CREATE TABLE IF NOT EXISTS contact_phones (
          contact_id INT NOT NULL REFERENCES contacts(id) ON DELETE CASCADE,
          phone      TEXT NOT NULL,
          type       TEXT NOT NULL,
          PRIMARY KEY (contact_id, phone, type)
        );
    )sql");

    tx.exec("CREATE INDEX IF NOT EXISTS idx_contacts_email ON contacts(email);");
    tx.exec("CREATE INDEX IF NOT EXISTS idx_contacts_name  ON contacts(name, last_name);");

    tx.commit();
}

std::list<Person> PgsqlSystem::getPeopleF()
{
    pqxx::connection c(connStr_);
    pqxx::read_transaction tx(c);

    std::list<Person> people;

    auto rows = tx.exec(R"sql(
        SELECT id, name, last_name, patronymic, address, birth_date, email
        FROM contacts
        ORDER BY id ASC
    )sql");

    for (auto const& r : rows)
    {
        int id = r["id"].as<int>();
        std::string name = r["name"].as<std::string>();
        std::string last = r["last_name"].as<std::string>();
        std::string patr = r["patronymic"].is_null() ? "" : r["patronymic"].as<std::string>();
        std::string addr = r["address"].is_null() ? "" : r["address"].as<std::string>();
        std::string birth = r["birth_date"].is_null() ? "" : r["birth_date"].as<std::string>();
        std::string email = r["email"].as<std::string>();

        std::list<PhoneNumber> nums;
        auto nrows = tx.exec_params(
            "SELECT phone, type FROM contact_phones WHERE contact_id = $1",
            id
        );

        for (auto const& nr : nrows)
        {
            nums.emplace_back(nr["phone"].as<std::string>(), nr["type"].as<std::string>());
        }

        people.emplace_back(name, last, patr, addr, birth, email, id, nums);
    }

    return people;
}

void PgsqlSystem::storeF(Person p)
{
    pqxx::connection c(connStr_);
    pqxx::work tx(c);

    tx.exec_params(
        "INSERT INTO contacts(id, name, last_name, patronymic, address, birth_date, email) "
        "VALUES ($1,$2,$3,$4,$5,$6,$7)",
        p.getID(),
        p.getName(),
        p.getLastName(),
        p.getPatronimic().empty() ? nullptr : p.getPatronimic().c_str(),
        p.getAddress().empty() ? nullptr : p.getAddress().c_str(),
        p.getBirhtDate().empty() ? nullptr : p.getBirhtDate().c_str(),
        p.getEmail()
    );

    for (const auto& n : p.getNumbers())
    {
        tx.exec_params(
            "INSERT INTO contact_phones(contact_id, phone, type) VALUES ($1,$2,$3)",
            p.getID(), n.getNumber(), n.getType()
        );
    }

    tx.commit();
}

void PgsqlSystem::deleteF(int id)
{
    pqxx::connection c(connStr_);
    pqxx::work tx(c);

    tx.exec_params("DELETE FROM contacts WHERE id = $1", id);

    tx.commit();
}

void PgsqlSystem::updateF(int id, Person p)
{
    pqxx::connection c(connStr_);
    pqxx::work tx(c);

    tx.exec_params(
        "UPDATE contacts SET name=$2, last_name=$3, patronymic=$4, address=$5, birth_date=$6, email=$7 "
        "WHERE id=$1",
        id,
        p.getName(),
        p.getLastName(),
        p.getPatronimic().empty() ? nullptr : p.getPatronimic().c_str(),
        p.getAddress().empty() ? nullptr : p.getAddress().c_str(),
        p.getBirhtDate().empty() ? nullptr : p.getBirhtDate().c_str(),
        p.getEmail()
    );

    tx.exec_params("DELETE FROM contact_phones WHERE contact_id = $1", id);

    for (const auto& n : p.getNumbers())
    {
        tx.exec_params(
            "INSERT INTO contact_phones(contact_id, phone, type) VALUES ($1,$2,$3)",
            id, n.getNumber(), n.getType()
        );
    }

    tx.commit();
}

#pragma once
#include "IFileSystem.h"
#include <string>

class PgsqlSystem : public IFileSystem {
private:
    std::string connStr_;
    void ensureSchema_();

public:
    explicit PgsqlSystem(const std::string& connectionString);

    std::list<Person> getPeopleF() override;
    void storeF(Person) override;
    void updateF(int, Person) override;
    void deleteF(int) override;
};

#pragma once
#include <iostream>
#include <string>

class Employee {
public:
    int id;
    std::string name;
    double hours;

    Employee() = default;
    Employee(int id, const std::string& name, double hours);

    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
};

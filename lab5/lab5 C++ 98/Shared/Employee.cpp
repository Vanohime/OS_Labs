#include "Employee.h"

Employee::Employee(int id, const std::string& name, double hours)
    : id(id), name(name), hours(hours) {}

void Employee::serialize(std::ostream& out) const {
    out << id << " " << name << " " << hours << "\n";
}

void Employee::deserialize(std::istream& in) {
    in >> id >> name >> hours;
}

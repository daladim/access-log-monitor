#ifndef _HTML_SERIALIZER_HPP_
#define _HTML_SERIALIZER_HPP_

#include <iostream>

#include "utils/SerializerInterface.hpp"
#include "database/Database.hpp"

namespace LogSupervisor::Serializer{
using Database = LogSupervisor::Database::Database;

//! This classes serializes as formatted HTML
class HTML : public Interface{
public:
    HTML(const Database& db);
    ostream& serialize(ostream& lhs) override;
    virtual ~HTML() {}

private:
    const Database& db;

    void addHeader(ostream& lhs) const;
};

} // namespace

#endif // _HTML_SERIALIZER_HPP_
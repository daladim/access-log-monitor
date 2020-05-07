#ifndef _TEXT_SERIALIZER_HPP_
#define _TEXT_SERIALIZER_HPP_

#include <iostream>
#include <ostream>

#include "SerializerInterface.hpp"
#include "../database/Database.hpp"

namespace LogSupervisor::Serializer{
using Database = LogSupervisor::Database::Database;

//! This classes serializes as raw text
class Text : public Interface{
public:
    Text(const Database& db);
    ostream& serialize(ostream& lhs) override;
    virtual ~Text() {}

private:
    const Database& db;
};

} // namespace

#endif // _TEXT_SERIALIZER_HPP_
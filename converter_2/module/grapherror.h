#ifndef GRAPHERROR_H
#define GRAPHERROR_H

#include <string>
#include <stdexcept>

class GraphError : public std::runtime_error
{
public:
    GraphError(size_t line_num, const std::string& txt);
    std::string complete_message() const;

private:
    size_t m_line_num{ 0 };
};

#endif // GRAPHERROR_H

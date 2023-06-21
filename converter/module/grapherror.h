#ifndef GRAPHERROR_H
#define GRAPHERROR_H

#include <string>
#include <stdexcept>

class GraphError : public std::runtime_error
{
public:
    GraphError(const size_t line_num, const std::string& txt);
    const std::string& only_text() const;
    size_t only_line_num() const;

private:
    size_t m_line_num{ 0 };
    std::string m_text;
};

#endif // GRAPHERROR_H

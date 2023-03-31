#ifndef ARROW_H
#define ARROW_H

#include <string>
#include <vector>

namespace lenv
{

class Arrow final
{
public:
    Arrow(std::string str);

public:
    std::string raw() const;
    const std::string& left_head() const;
    const std::string& rght_head() const;
    const std::string& body() const;

private:
    std::string m_lhead;
    std::string m_rhead;
    std::string m_body;
};

}

#endif // ARROW_H

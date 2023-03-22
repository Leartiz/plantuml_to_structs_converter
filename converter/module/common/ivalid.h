#ifndef IVALID_H
#define IVALID_H

namespace lenv
{

class IValid
{
public:
    virtual ~IValid() {}

    virtual bool is_valid() const = 0;
};

}

#endif // IVALID_H

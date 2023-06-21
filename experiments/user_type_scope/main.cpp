#include <iostream>

class User_type
{
public:
    class In_user_type
    {
    public:
        void foo(const User_type& ut)
        {
            std::cout << ut.val << std::endl;
        }
        //...

    private:
        double number{ 0 };
    };

public:
    explicit User_type(const std::string& v)
        : val{ v } {};

    void foo(const In_user_type& iut)
    {
        // Не взаимный доступ к приватным полям!
        //std::cout << iut.number << std::endl;
    }

private:
    std::string val;
};

int main()
{
    User_type ut{ "123" };
    User_type::In_user_type iut;
    iut.foo(ut);

    return 0;
}

#include <string>
#include <sstream>
#include <iostream>

void print_stream_states(std::istream& in)
{
    std::cout << "sin.good: " << in.good() << std::endl;
    std::cout << "sin.fail: " << in.fail() << std::endl;
    std::cout << "sin.bad: " << in.bad() << std::endl;
    std::cout << "sin.eof: " << in.eof() << std::endl;
    std::cout << std::endl;
}

/*

- Trying to use putback as an internal buffer (-> err).

*/
int main()
{
    {
        std::stringstream sin{ "1 2 3" };
        std::istream& in{ sin };

        std::cout << "sin: " << sin.str() << std::endl; // always the same!
        print_stream_states(in);

        in.get();
        std::cout << "sin: " << sin.str() << std::endl;

        in.putback('S');

        /* -> bad AND -> fail */
        //in.putback('S');
        print_stream_states(in);
        std::cout << "sin: " << sin.str() << std::endl;

        char one{ 0 };
        while (in >> std::noskipws >> one) {
            std::cout << '\'' << one << '\'' << std::endl;
        }

        print_stream_states(in);
        std::cout << "sin: " << sin.str() << std::endl;
    }
    return 0;
}


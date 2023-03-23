#include <string>
#include <cassert>
#include <iostream>
#include <type_traits>

/* POD types in C++ */

int main()
{
    {
        struct MyStruct final
        {
            int int_val;
            double double_val;
            std::string str_val;
        };
        assert(std::is_pod<MyStruct>{} == false);
    }

    // -------------------------------------------------------------------

    {
        struct MyStruct final
        {
            int int_val;
            double double_val;
        };
        assert(std::is_pod<MyStruct>{} == true);
    }

    // -------------------------------------------------------------------

    {
        struct MyStruct final
        {
            int int_val{ 0 }; // implicit constructor?
            double double_val;
        };
        assert(std::is_pod<MyStruct>{} == false);
    }

    // -------------------------------------------------------------------

    {
        enum class PodEnum
        {
            VAL0, VAL1, VAL2,
        };

        struct MyStruct final
        {
            int int_val{ 20 };
            double double_val;
            PodEnum enum_val;
        };

        MyStruct ms{
            .int_val = 0,
            .double_val = 0.0,
            .enum_val = PodEnum::VAL0,
        };
        static_cast<void>(ms);

        assert(std::is_pod<PodEnum>{} == true);
        assert(std::is_pod<MyStruct>{} == false);
    }

    // -------------------------------------------------------------------

    {
        struct MyStruct final
        {
            MyStruct() : int_val{ 0 }, double_val{ 0.0 } {}

            int int_val;
            double double_val;
        };

        assert(std::is_pod<MyStruct>{} == false);
    }

    // -------------------------------------------------------------------

    {
        struct MyStruct final
        {
        public:
            int get_data() { return m_data; }
        private:
            int m_data;
        };

        assert(std::is_pod<MyStruct>{} == true);
    }

    // -------------------------------------------------------------------

    {
        struct MyStruct final
        {
        public:
            int get_data0() { return m_data0; }
            int get_data1() { return m_data1; }
            int get_data2() { return m_data2; }

        private:
            int m_data0;
            int m_data1;
            int m_data2;
        };

        /*
        MyStruct ms{
            .m_data0 = 0,
            .m_data1 = 1,
            .m_data2 = 2,
        };
        */

        assert(std::is_pod<MyStruct>{} == true);
    }

    // -------------------------------------------------------------------

    {
        struct MyBaseStruct
        {
            int m_data0;
            int m_data1;
            int m_data2;
        };

        MyBaseStruct ms{
            .m_data0 = 0,
            .m_data1 = 1,
            .m_data2 = 2,
        };
        static_cast<void>(ms);

        struct MyDervStruct : MyBaseStruct
        {
        };

        assert(std::is_pod<MyBaseStruct>{} == true);
        assert(std::is_pod<MyDervStruct>{} == true);
    }

    // -------------------------------------------------------------------

    {
        struct MyBaseStruct
        {
            int m_data0;
            int m_data1;
            int m_data2;
        };

        MyBaseStruct ms{
            .m_data0 = 0,
            .m_data1 = 1,
            .m_data2 = 2,
        };
        static_cast<void>(ms);

        struct MyDervStruct : MyBaseStruct
        {
            int get_data0() { return m_data0 * 0; }
            int get_data1() { return m_data1 * 1; }
            int get_data2() { return m_data2 * 2; }
        };

        assert(std::is_pod<MyBaseStruct>{} == true);
        assert(std::is_pod<MyDervStruct>{} == true);
    }

    // -------------------------------------------------------------------

    {
        struct MyStruct
        {
            std::string val;
        };

        assert(std::is_pod<MyStruct>{} == false);
    }

    // -------------------------------------------------------------------

    {
        struct MyStruct
        {
            struct Impl
            {
                int val0;
                float val1;
                double val2;
            };

            Impl impl;
        };

        assert(std::is_pod<MyStruct>{} == true);
        assert(std::is_pod<MyStruct::Impl>{} == true);
    }

    // -------------------------------------------------------------------

    // ...

    std::cout << "[OK]" << std::endl;
    return 0;
}

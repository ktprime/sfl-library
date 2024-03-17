//
// g++ -std=c++11 -g -O0 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -I ../include static_vector.cpp
// valgrind --leak-check=full ./a.out
//

#undef NDEBUG // This is very important. Must be in the first line.

#include "sfl/static_vector.hpp"

#include "check.hpp"
#include "print.hpp"

#include "xint.hpp"

#include <sstream>
#include <vector>

int main()
{
    using sfl::test::xint;

    PRINT("Test non-modifying member functions (empty container)");
    {
        // Non-const container
        {
            sfl::static_vector<xint, 5> v;

            CHECK(v.empty() == true);
            CHECK(v.full() == false);
            CHECK(v.size() == 0);
            CHECK(v.max_size() == 5);
            CHECK(v.capacity() == 5);
            CHECK(v.available() == 5);
            CHECK(v.begin() == v.end());
            CHECK(v.cbegin() == v.cend());
            CHECK(v.begin() == v.cbegin());
            CHECK(v.rbegin() == v.rend());
            CHECK(v.crbegin() == v.crend());
            CHECK(v.rbegin() == v.crbegin());
            CHECK(v.begin() == v.nth(0));
            CHECK(v.cbegin() == v.nth(0));
            CHECK(v.index_of(v.nth(0)) == 0);
        }

        // Const container
        {
            const sfl::static_vector<xint, 5> v;

            CHECK(v.empty() == true);
            CHECK(v.full() == false);
            CHECK(v.size() == 0);
            CHECK(v.max_size() == 5);
            CHECK(v.capacity() == 5);
            CHECK(v.available() == 5);
            CHECK(v.begin() == v.end());
            CHECK(v.cbegin() == v.cend());
            CHECK(v.begin() == v.cbegin());
            CHECK(v.rbegin() == v.rend());
            CHECK(v.crbegin() == v.crend());
            CHECK(v.rbegin() == v.crbegin());
            CHECK(v.begin() == v.nth(0));
            CHECK(v.cbegin() == v.nth(0));
            CHECK(v.index_of(v.nth(0)) == 0);
        }
    }

    PRINT("Test emplace_back(Args&&...)");
    {
        sfl::static_vector<xint, 5> v;

        {
            PRINT(">");
            const auto& res = v.emplace_back(10);
            PRINT("<");

            CHECK(res == 10);
            CHECK(v.size() == 1);
            CHECK(v[0] == 10);
        }

        {
            PRINT(">");
            const auto& res = v.emplace_back(20);
            PRINT("<");

            CHECK(res == 20);
            CHECK(v.size() == 2);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
        }

        {
            PRINT(">");
            const auto& res = v.emplace_back(30);
            PRINT("<");

            CHECK(res == 30);
            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        {
            PRINT(">");
            const auto& res = v.emplace_back(40);
            PRINT("<");

            CHECK(res == 40);
            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            PRINT(">");
            const auto& res = v.emplace_back(50);
            PRINT("<");

            CHECK(res == 50);
            CHECK(v.size() == 5);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
            CHECK(v[4] == 50);
        }
    }

    PRINT("Test non-modifying member functions (non-empty container)");
    {
        sfl::static_vector<xint, 3> v;

        {
            CHECK(v.empty() == true);
            CHECK(v.full() == false);
            CHECK(v.size() == 0);
            CHECK(v.capacity() == 3);
            CHECK(v.available() == 3);

            v.emplace_back(10);

            CHECK(v.empty() == false);
            CHECK(v.full() == false);
            CHECK(v.size() == 1);
            CHECK(v.capacity() == 3);
            CHECK(v.available() == 2);

            v.emplace_back(20);

            CHECK(v.empty() == false);
            CHECK(v.full() == false);
            CHECK(v.size() == 2);
            CHECK(v.capacity() == 3);
            CHECK(v.available() == 1);

            v.emplace_back(30);

            CHECK(v.empty() == false);
            CHECK(v.full() == true);
            CHECK(v.size() == 3);
            CHECK(v.capacity() == 3);
            CHECK(v.available() == 0);
        }

        {
            auto it = v.begin();
            CHECK(*it == 10); ++it;
            CHECK(*it == 20); ++it;
            CHECK(*it == 30); ++it;
            CHECK(it == v.end());
        }

        {
            auto it = v.cbegin();
            CHECK(*it == 10); ++it;
            CHECK(*it == 20); ++it;
            CHECK(*it == 30); ++it;
            CHECK(it == v.cend());
        }

        {
            auto it = v.rbegin();
            CHECK(*it == 30); ++it;
            CHECK(*it == 20); ++it;
            CHECK(*it == 10); ++it;
            CHECK(it == v.rend());
        }

        {
            auto it = v.crbegin();
            CHECK(*it == 30); ++it;
            CHECK(*it == 20); ++it;
            CHECK(*it == 10); ++it;
            CHECK(it == v.crend());
        }

        {
            CHECK(*v.nth(0) == 10);
            CHECK(*v.nth(1) == 20);
            CHECK(*v.nth(2) == 30);
            CHECK(v.nth(3) == v.end());
        }

        {
            CHECK(std::next(v.begin(), 0) == v.nth(0));
            CHECK(std::next(v.begin(), 1) == v.nth(1));
            CHECK(std::next(v.begin(), 2) == v.nth(2));
        }

        {
            CHECK(std::next(v.cbegin(), 0) == v.nth(0));
            CHECK(std::next(v.cbegin(), 1) == v.nth(1));
            CHECK(std::next(v.cbegin(), 2) == v.nth(2));
        }

        {
            CHECK(v.nth(0) < v.nth(1));
            CHECK(v.nth(0) < v.nth(2));
            CHECK(v.nth(0) < v.nth(3));

            CHECK(v.nth(1) < v.nth(2));
            CHECK(v.nth(1) < v.nth(3));

            CHECK(v.nth(2) < v.nth(3));
        }

        {
            CHECK(v.index_of(v.nth(0)) == 0);
            CHECK(v.index_of(v.nth(1)) == 1);
            CHECK(v.index_of(v.nth(2)) == 2);
        }

        {
            CHECK(v.at(0) == 10);
            CHECK(v.at(1) == 20);
            CHECK(v.at(2) == 30);
        }

        {
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        {
            CHECK(v.front() == 10);
        }

        {
            CHECK(v.back() == 30);
        }

        {
            auto data = v.data();
            CHECK(*data == 10); ++data;
            CHECK(*data == 20); ++data;
            CHECK(*data == 30); ++data;
        }
    }

    PRINT("Test emplace(const_iterator, Args&&...)");
    {
        sfl::static_vector<xint, 6> v;

        // Emplace at the end
        {
            PRINT(">");
            auto res = v.emplace(v.end(), 10);
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 1);
            CHECK(v[0] == 10);
        }

        // Emplace at the end
        {
            PRINT(">");
            auto res = v.emplace(v.end(), 20);
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 2);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
        }

        // Emplace at the end
        {
            PRINT(">");
            auto res = v.emplace(v.end(), 30);
            PRINT("<");

            CHECK(res == v.nth(2));
            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        // Emplace at middle (closer to the end)
        {
            PRINT(">");
            auto res = v.emplace(v.nth(2), 25);
            PRINT("<");

            CHECK(res == v.nth(2));
            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 25);
            CHECK(v[3] == 30);
        }

        // Emplace at middle (closer to the begin)
        {
            PRINT(">");
            auto res = v.emplace(v.nth(1), 15);
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 5);
            CHECK(v[0] == 10);
            CHECK(v[1] == 15);
            CHECK(v[2] == 20);
            CHECK(v[3] == 25);
            CHECK(v[4] == 30);
        }

        // Emplace at the begin
        {
            PRINT(">");
            auto res = v.emplace(v.begin(), 5);
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 6);
            CHECK(v[0] == 5);
            CHECK(v[1] == 10);
            CHECK(v[2] == 15);
            CHECK(v[3] == 20);
            CHECK(v[4] == 25);
            CHECK(v[5] == 30);
        }
    }

    PRINT("Test clear()");
    {
        sfl::static_vector<xint, 5> v;

        {
            PRINT(">");
            v.emplace_back(10);
            v.emplace_back(20);
            v.emplace_back(30);
            v.emplace_back(40);
            PRINT("<");

            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            PRINT(">");
            v.clear();
            PRINT("<");

            CHECK(v.size() == 0);
        }

        {
            PRINT(">");
            v.emplace_back(50);
            v.emplace_back(60);
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 50);
            CHECK(v[1] == 60);
        }

        {
            PRINT(">");
            v.clear();
            PRINT("<");

            CHECK(v.size() == 0);
        }
    }

    PRINT("Test insert(const_iterator, const T&)");
    {
        sfl::static_vector<xint, 5> v;

        {
            const xint elem(10);

            PRINT(">");
            auto res = v.insert(v.end(), elem);
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 1);
            CHECK(v[0] == 10);
        }
    }

    PRINT("Test insert(const_iterator, T&&)");
    {
        sfl::static_vector<xint, 5> v;

        {
            xint elem(10);

            PRINT(">");
            auto res = v.insert(v.end(), std::move(elem));
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(elem == -10);
            CHECK(v.size() == 1);
            CHECK(v[0] == 10);
        }
    }

    PRINT("Test insert(const_iterator, size_type, const T&)");
    {
        sfl::static_vector<xint, 12> v;

        {
            PRINT(">");
            auto res = v.insert(v.nth(0), 3, xint(10));
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 10);
            CHECK(v[2] == 10);
        }

        {
            PRINT(">");
            auto res = v.insert(v.nth(1), 3, xint(20));
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 6);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 20);
            CHECK(v[3] == 20);
            CHECK(v[4] == 10);
            CHECK(v[5] == 10);
        }

        {
            PRINT(">");
            auto res = v.insert(v.nth(1), 2, xint(30));
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 8);
            CHECK(v[0] == 10);
            CHECK(v[1] == 30);
            CHECK(v[2] == 30);
            CHECK(v[3] == 20);
            CHECK(v[4] == 20);
            CHECK(v[5] == 20);
            CHECK(v[6] == 10);
            CHECK(v[7] == 10);
        }

        {
            PRINT(">");
            auto res = v.insert(v.nth(1), 4, xint(40));
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 12);
            CHECK(v[0]  == 10);
            CHECK(v[1]  == 40);
            CHECK(v[2]  == 40);
            CHECK(v[3]  == 40);
            CHECK(v[4]  == 40);
            CHECK(v[5]  == 30);
            CHECK(v[6]  == 30);
            CHECK(v[7]  == 20);
            CHECK(v[8]  == 20);
            CHECK(v[9]  == 20);
            CHECK(v[10] == 10);
            CHECK(v[11] == 10);
        }
    }

    PRINT("Test insert(const_iterator, InputIt, InputIt)");
    {
        sfl::static_vector<xint, 100> v;

        {
            std::istringstream iss("10 20 30 40");

            PRINT(">");
            auto res = v.insert
            (
                v.nth(0),
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            std::istringstream iss("31 32 33 34");

            PRINT(">");
            auto res = v.insert
            (
                v.nth(3),
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(res == v.nth(3));
            CHECK(v.size() == 8);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 31);
            CHECK(v[4] == 32);
            CHECK(v[5] == 33);
            CHECK(v[6] == 34);
            CHECK(v[7] == 40);
        }

        {
            std::istringstream iss("11 12");

            PRINT(">");
            auto res = v.insert
            (
                v.nth(1),
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 10);
            CHECK(v[0] == 10);
            CHECK(v[1] == 11);
            CHECK(v[2] == 12);
            CHECK(v[3] == 20);
            CHECK(v[4] == 30);
            CHECK(v[5] == 31);
            CHECK(v[6] == 32);
            CHECK(v[7] == 33);
            CHECK(v[8] == 34);
            CHECK(v[9] == 40);
        }
    }

    PRINT("Test insert(const_iterator, ForwardIt, ForwardIt)");
    {
        sfl::static_vector<xint, 100> v;

        {
            const std::vector<xint> data({10, 20, 30, 40});

            PRINT(">");
            auto res = v.insert(v.begin(), data.begin(), data.end());
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            const std::vector<xint> data({31, 32, 33, 34});

            PRINT(">");
            auto res = v.insert(v.nth(3), data.begin(), data.end());
            PRINT("<");

            CHECK(res == v.nth(3));
            CHECK(v.size() == 8);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 31);
            CHECK(v[4] == 32);
            CHECK(v[5] == 33);
            CHECK(v[6] == 34);
            CHECK(v[7] == 40);
        }

        {
            const std::vector<xint> data({11, 12});

            PRINT(">");
            auto res = v.insert(v.nth(1), data.begin(), data.end());
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 10);
            CHECK(v[0] == 10);
            CHECK(v[1] == 11);
            CHECK(v[2] == 12);
            CHECK(v[3] == 20);
            CHECK(v[4] == 30);
            CHECK(v[5] == 31);
            CHECK(v[6] == 32);
            CHECK(v[7] == 33);
            CHECK(v[8] == 34);
            CHECK(v[9] == 40);
        }
    }

    PRINT("Test insert(const_iterator, MoveIterator, MoveIterator)");
    {
        sfl::static_vector<xint, 100> v;

        {
            std::vector<xint> data({10, 20, 30, 40});

            PRINT(">");
            auto res = v.insert
            (
                v.begin(),
                std::make_move_iterator(data.begin()),
                std::make_move_iterator(data.end())
            );
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(data[0] == -10);
            CHECK(data[1] == -20);
            CHECK(data[2] == -30);
            CHECK(data[3] == -40);
            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            std::vector<xint> data({31, 32, 33, 34});

            PRINT(">");
            auto res = v.insert
            (
                v.nth(3),
                std::make_move_iterator(data.begin()),
                std::make_move_iterator(data.end())
            );
            PRINT("<");

            CHECK(res == v.nth(3));
            CHECK(data[0] == -31);
            CHECK(data[1] == -32);
            CHECK(data[2] == -33);
            CHECK(data[3] == -34);
            CHECK(v.size() == 8);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 31);
            CHECK(v[4] == 32);
            CHECK(v[5] == 33);
            CHECK(v[6] == 34);
            CHECK(v[7] == 40);
        }

        {
            std::vector<xint> data({11, 12});

            PRINT(">");
            auto res = v.insert
            (
                v.nth(1),
                std::make_move_iterator(data.begin()),
                std::make_move_iterator(data.end())
            );
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(data[0] == -11);
            CHECK(data[1] == -12);
            CHECK(v.size() == 10);
            CHECK(v[0] == 10);
            CHECK(v[1] == 11);
            CHECK(v[2] == 12);
            CHECK(v[3] == 20);
            CHECK(v[4] == 30);
            CHECK(v[5] == 31);
            CHECK(v[6] == 32);
            CHECK(v[7] == 33);
            CHECK(v[8] == 34);
            CHECK(v[9] == 40);
        }
    }

    PRINT("Test insert(const_iterator, std::initializer_list)");
    {
        sfl::static_vector<xint, 100> v;

        {
            PRINT(">");
            auto res = v.insert(v.nth(0), {10, 20, 30, 40});
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            PRINT(">");
            auto res = v.insert(v.nth(3), {31, 32, 33, 34});
            PRINT("<");

            CHECK(res == v.nth(3));
            CHECK(v.size() == 8);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 31);
            CHECK(v[4] == 32);
            CHECK(v[5] == 33);
            CHECK(v[6] == 34);
            CHECK(v[7] == 40);
        }

        {
            PRINT(">");
            auto res = v.insert(v.nth(1), {11, 12});
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 10);
            CHECK(v[0] == 10);
            CHECK(v[1] == 11);
            CHECK(v[2] == 12);
            CHECK(v[3] == 20);
            CHECK(v[4] == 30);
            CHECK(v[5] == 31);
            CHECK(v[6] == 32);
            CHECK(v[7] == 33);
            CHECK(v[8] == 34);
            CHECK(v[9] == 40);
        }
    }

    PRINT("Test push_back(const T&)");
    {
        sfl::static_vector<xint, 5> v;

        {
            const xint elem(10);

            PRINT(">");
            v.push_back(elem);
            PRINT("<");

            CHECK(v.size() == 1);
            CHECK(v[0] == 10);
        }

        {
            const xint elem(20);

            PRINT(">");
            v.push_back(elem);
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
        }

        {
            const xint elem(30);

            PRINT(">");
            v.push_back(elem);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        {
            const xint elem(40);

            PRINT(">");
            v.push_back(elem);
            PRINT("<");

            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            const xint elem(50);

            PRINT(">");
            v.push_back(elem);
            PRINT("<");

            CHECK(v.size() == 5);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
            CHECK(v[4] == 50);
        }
    }

    PRINT("Test push_back(T&&)");
    {
        sfl::static_vector<xint, 5> v;

        {
            xint elem(10);

            PRINT(">");
            v.push_back(std::move(elem));
            PRINT("<");

            CHECK(elem == -10);
            CHECK(v.size() == 1);
            CHECK(v[0] == 10);
        }

        {
            xint elem(20);

            PRINT(">");
            v.push_back(std::move(elem));
            PRINT("<");

            CHECK(elem == -20);
            CHECK(v.size() == 2);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
        }

        {
            xint elem(30);

            PRINT(">");
            v.push_back(std::move(elem));
            PRINT("<");

            CHECK(elem == -30);
            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        {
            xint elem(40);

            PRINT(">");
            v.push_back(std::move(elem));
            PRINT("<");

            CHECK(elem == -40);
            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            xint elem(50);

            PRINT(">");
            v.push_back(std::move(elem));
            PRINT("<");

            CHECK(elem == -50);
            CHECK(v.size() == 5);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
            CHECK(v[4] == 50);
        }
    }

    PRINT("Test pop_back()");
    {
        sfl::static_vector<xint, 10> v;

        {
            PRINT(">");
            v.emplace_back(10);
            v.emplace_back(20);
            v.emplace_back(30);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        {
            PRINT(">");
            v.pop_back();
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
        }

        {
            PRINT(">");
            v.pop_back();
            PRINT("<");

            CHECK(v.size() == 1);
            CHECK(v[0] == 10);
        }

        {
            PRINT(">");
            v.pop_back();
            PRINT("<");

            CHECK(v.size() == 0);
        }
    }

    PRINT("Test erase(const_iterator)");
    {
        sfl::static_vector<xint, 10> v;

        {
            PRINT(">");
            v.emplace_back(10);
            v.emplace_back(20);
            v.emplace_back(30);
            v.emplace_back(40);
            PRINT("<");

            CHECK(v.size() == 4);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
        }

        {
            PRINT(">");
            auto res = v.erase(v.nth(1));
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 30);
            CHECK(v[2] == 40);
        }

        {
            PRINT(">");
            auto res = v.erase(v.nth(0));
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 2);
            CHECK(v[0] == 30);
            CHECK(v[1] == 40);
        }

        {
            PRINT(">");
            auto res = v.erase(v.nth(1));
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 1);
            CHECK(v[0] == 30);
        }

        {
            PRINT(">");
            auto res = v.erase(v.nth(0));
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 0);
        }
    }

    PRINT("Test erase(const_iterator, const_iterator)");
    {
        sfl::static_vector<xint, 10> v;

        {
            PRINT(">");
            v.emplace_back(10);
            v.emplace_back(20);
            v.emplace_back(30);
            v.emplace_back(40);
            v.emplace_back(50);
            v.emplace_back(60);
            v.emplace_back(70);
            v.emplace_back(80);
            v.emplace_back(90);
            PRINT("<");

            CHECK(v.size() == 9);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
            CHECK(v[4] == 50);
            CHECK(v[5] == 60);
            CHECK(v[6] == 70);
            CHECK(v[7] == 80);
            CHECK(v[8] == 90);
        }

        {
            PRINT(">");
            // Erase nothing
            auto res = v.erase(v.nth(3), v.nth(3));
            PRINT("<");

            CHECK(res == v.nth(3));
            CHECK(v.size() == 9);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 40);
            CHECK(v[4] == 50);
            CHECK(v[5] == 60);
            CHECK(v[6] == 70);
            CHECK(v[7] == 80);
            CHECK(v[8] == 90);
        }

        {
            PRINT(">");
            // Erase two elements near begin
            auto res = v.erase(v.nth(1), v.nth(3));
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 7);
            CHECK(v[0] == 10);
            CHECK(v[1] == 40);
            CHECK(v[2] == 50);
            CHECK(v[3] == 60);
            CHECK(v[4] == 70);
            CHECK(v[5] == 80);
            CHECK(v[6] == 90);
        }

        {
            PRINT(">");
            // Erase two elements near end
            auto res = v.erase(v.nth(4), v.nth(6));
            PRINT("<");

            CHECK(res == v.nth(4));
            CHECK(v.size() == 5);
            CHECK(v[0] == 10);
            CHECK(v[1] == 40);
            CHECK(v[2] == 50);
            CHECK(v[3] == 60);
            CHECK(v[4] == 90);
        }

        {
            PRINT(">");
            // Remove two elements at begin
            auto res = v.erase(v.nth(0), v.nth(2));
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 3);
            CHECK(v[0] == 50);
            CHECK(v[1] == 60);
            CHECK(v[2] == 90);
        }

        {
            PRINT(">");
            // Remove two elements at end
            auto res = v.erase(v.nth(1), v.nth(3));
            PRINT("<");

            CHECK(res == v.nth(1));
            CHECK(v.size() == 1);
            CHECK(v[0] == 50);
        }

        {
            PRINT(">");
            // Remove from begin to end
            auto res = v.erase(v.nth(0), v.nth(1));
            PRINT("<");

            CHECK(res == v.nth(0));
            CHECK(v.size() == 0);
        }
    }

    PRINT("Test resize(size_type)");
    {
        sfl::static_vector<xint, 10> v;

        {
            PRINT(">");
            v.emplace_back(10);
            v.emplace_back(20);
            v.emplace_back(30);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        // n == size
        {
            PRINT(">");
            v.resize(3);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        // n > size
        {
            PRINT(">");
            v.resize(6);
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == SFL_TEST_XINT_DEFAULT_VALUE);
            CHECK(v[4] == SFL_TEST_XINT_DEFAULT_VALUE);
            CHECK(v[5] == SFL_TEST_XINT_DEFAULT_VALUE);
        }

        // n < size
        {
            PRINT(">");
            v.resize(2);
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
        }

        // n > size
        {
            PRINT(">");
            v.resize(6);
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == SFL_TEST_XINT_DEFAULT_VALUE);
            CHECK(v[3] == SFL_TEST_XINT_DEFAULT_VALUE);
            CHECK(v[4] == SFL_TEST_XINT_DEFAULT_VALUE);
            CHECK(v[5] == SFL_TEST_XINT_DEFAULT_VALUE);
        }
    }

    PRINT("Test resize(size_type, const T&)");
    {
        sfl::static_vector<xint, 10> v;

        {
            PRINT(">");
            v.emplace_back(10);
            v.emplace_back(20);
            v.emplace_back(30);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        // n == size
        {
            PRINT(">");
            v.resize(3, 111);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        // n > size
        {
            PRINT(">");
            v.resize(6, 222);
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
            CHECK(v[3] == 222);
            CHECK(v[4] == 222);
            CHECK(v[5] == 222);
        }

        // n < size
        {
            PRINT(">");
            v.resize(2, 333);
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
        }

        // n > size
        {
            PRINT(">");
            v.resize(6, 444);
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 444);
            CHECK(v[3] == 444);
            CHECK(v[4] == 444);
            CHECK(v[5] == 444);
        }
    }

    PRINT("Test swap(static_vector&)");
    {
        // Swap with self
        {
            sfl::static_vector<xint, 10> v;

            PRINT(">");
            v.emplace_back(10);
            v.emplace_back(20);
            v.emplace_back(30);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);

            PRINT(">");
            v.swap(v);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 20);
            CHECK(v[2] == 30);
        }

        // v1.size() == v2.size()
        {
            sfl::static_vector<xint, 10> v1;
            sfl::static_vector<xint, 10> v2;

            PRINT(">");
            v1.emplace_back(10);
            v1.emplace_back(11);
            v1.emplace_back(12);
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            PRINT(">");
            v2.emplace_back(20);
            v2.emplace_back(21);
            v2.emplace_back(22);
            PRINT("<");

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);

            PRINT(">");
            v1.swap(v2);
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 10);
            CHECK(v2[1] == 11);
            CHECK(v2[2] == 12);

            PRINT(">");
            v1.swap(v2);
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
        }

        // v1.size() != v2.size()
        {
            sfl::static_vector<xint, 10> v1;
            sfl::static_vector<xint, 10> v2;

            PRINT(">");
            v1.emplace_back(10);
            v1.emplace_back(11);
            v1.emplace_back(12);
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            PRINT(">");
            v2.emplace_back(20);
            v2.emplace_back(21);
            v2.emplace_back(22);
            v2.emplace_back(23);
            v2.emplace_back(24);
            PRINT("<");

            CHECK(v2.size() == 5);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
            CHECK(v2[3] == 23);
            CHECK(v2[4] == 24);

            PRINT(">");
            v1.swap(v2);
            PRINT("<");

            CHECK(v1.size() == 5);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);
            CHECK(v1[3] == 23);
            CHECK(v1[4] == 24);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 10);
            CHECK(v2[1] == 11);
            CHECK(v2[2] == 12);

            PRINT(">");
            v1.swap(v2);
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            CHECK(v2.size() == 5);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
            CHECK(v2[3] == 23);
            CHECK(v2[4] == 24);
        }
    }

    PRINT("Test constructor()");
    {
        sfl::static_vector<xint, 10> v;

        CHECK(v.size() == 0);
    }

    PRINT("Test constructor(size_type)");
    {
        sfl::static_vector<xint, 10> v(3);

        CHECK(v.size() == 3);
        CHECK(v[0] == SFL_TEST_XINT_DEFAULT_VALUE);
        CHECK(v[1] == SFL_TEST_XINT_DEFAULT_VALUE);
        CHECK(v[2] == SFL_TEST_XINT_DEFAULT_VALUE);
    }

    PRINT("Test constructor(size_type, const T&)");
    {
        sfl::static_vector<xint, 10> v(3, 42);

        CHECK(v.size() == 3);
        CHECK(v[0] == 42);
        CHECK(v[1] == 42);
        CHECK(v[2] == 42);
    }

    PRINT("Test constructor(InputIt, InputIt)");
    {
        std::istringstream iss("10 20 30 40");

        PRINT(">");
        sfl::static_vector<xint, 10> v
        (
            (std::istream_iterator<int>(iss)),
            (std::istream_iterator<int>())
        );
        PRINT("<");

        CHECK(v.size() == 4);
        CHECK(v[0] == 10);
        CHECK(v[1] == 20);
        CHECK(v[2] == 30);
        CHECK(v[3] == 40);
    }

    PRINT("Test constructor(ForwardIt, ForwardIt)");
    {
        const std::vector<xint> data({10, 20, 30, 40});

        PRINT(">");
        sfl::static_vector<xint, 10> v(data.begin(), data.end());
        PRINT("<");

        CHECK(v.size() == 4);
        CHECK(v[0] == 10);
        CHECK(v[1] == 20);
        CHECK(v[2] == 30);
        CHECK(v[3] == 40);
    }

    PRINT("Test constructor(MoveIterator, MoveIterator)");
    {
        std::vector<xint> data({10, 20, 30, 40});

        PRINT(">");
        sfl::static_vector<xint, 10> v
        (
            std::make_move_iterator(data.begin()),
            std::make_move_iterator(data.end())
        );
        PRINT("<");

        CHECK(data[0] == -10);
        CHECK(data[1] == -20);
        CHECK(data[2] == -30);
        CHECK(data[3] == -40);
        CHECK(v.size() == 4);
        CHECK(v[0] == 10);
        CHECK(v[1] == 20);
        CHECK(v[2] == 30);
        CHECK(v[3] == 40);
    }

    PRINT("Test constructor(std::initializer_list)");
    {
        PRINT(">");
        sfl::static_vector<xint, 10> v({10, 20, 30, 40});
        PRINT("<");

        CHECK(v.size() == 4);
        CHECK(v[0] == 10);
        CHECK(v[1] == 20);
        CHECK(v[2] == 30);
        CHECK(v[3] == 40);
    }

    PRINT("Test constructor(const static_vector&)");
    {
        sfl::static_vector<xint, 10> v1({10, 20, 30, 40});
        sfl::static_vector<xint, 10> v2(v1);

        CHECK(v1.size() == 4);
        CHECK(v1[0] == 10);
        CHECK(v1[1] == 20);
        CHECK(v1[2] == 30);
        CHECK(v1[3] == 40);

        CHECK(v2.size() == 4);
        CHECK(v2[0] == 10);
        CHECK(v2[1] == 20);
        CHECK(v2[2] == 30);
        CHECK(v2[3] == 40);
    }

    PRINT("Test constructor(static_vector&&)");
    {
        sfl::static_vector<xint, 10> v1({10, 20, 30, 40});
        sfl::static_vector<xint, 10> v2(std::move(v1));

        // v1 is in valid but unspecified state
        CHECK(v1.size() == 4);
        CHECK(v1[0] == -10);
        CHECK(v1[1] == -20);
        CHECK(v1[2] == -30);
        CHECK(v1[3] == -40);

        CHECK(v2.size() == 4);
        CHECK(v2[0] == 10);
        CHECK(v2[1] == 20);
        CHECK(v2[2] == 30);
        CHECK(v2[3] == 40);
    }

    PRINT("Test assign(size_type, const T&)");
    {
        sfl::static_vector<xint, 10> v;

        {
            PRINT(">");
            v.assign(3, 10);
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 10);
            CHECK(v[1] == 10);
            CHECK(v[2] == 10);
        }

        {
            PRINT(">");
            v.assign(2, 20);
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 20);
            CHECK(v[1] == 20);
        }

        {
            PRINT(">");
            v.assign(4, 30);
            PRINT("<");

            CHECK(v.size() == 4);
            CHECK(v[0] == 30);
            CHECK(v[1] == 30);
            CHECK(v[2] == 30);
            CHECK(v[3] == 30);
        }

        {
            PRINT(">");
            v.assign(6, 40);
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 40);
            CHECK(v[1] == 40);
            CHECK(v[2] == 40);
            CHECK(v[3] == 40);
            CHECK(v[4] == 40);
            CHECK(v[5] == 40);
        }

        {
            PRINT(">");
            v.assign(0, 50);
            PRINT("<");

            CHECK(v.size() == 0);
        }
    }

    PRINT("Test assign(InputIt, InputIt)");
    {
        sfl::static_vector<xint, 10> v;

        {
            std::istringstream iss("11 12 13");

            PRINT(">");
            v.assign
            (
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 11);
            CHECK(v[1] == 12);
            CHECK(v[2] == 13);
        }

        {
            std::istringstream iss("21 22");

            PRINT(">");
            v.assign
            (
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 21);
            CHECK(v[1] == 22);
        }

        {
            std::istringstream iss("31 32 33 34");

            PRINT(">");
            v.assign
            (
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(v.size() == 4);
            CHECK(v[0] == 31);
            CHECK(v[1] == 32);
            CHECK(v[2] == 33);
            CHECK(v[3] == 34);
        }

        {
            std::istringstream iss("41 42 43 44 45 46");

            PRINT(">");
            v.assign
            (
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 41);
            CHECK(v[1] == 42);
            CHECK(v[2] == 43);
            CHECK(v[3] == 44);
            CHECK(v[4] == 45);
            CHECK(v[5] == 46);
        }

        {
            std::istringstream iss("");

            PRINT(">");
            v.assign
            (
                std::istream_iterator<int>(iss),
                std::istream_iterator<int>()
            );
            PRINT("<");

            CHECK(v.size() == 0);
            CHECK(v.capacity() == 10);
        }
    }

    PRINT("Test assign(ForwardIt, ForwardIt)");
    {
        sfl::static_vector<xint, 10> v;

        {
            const std::vector<int> data({11, 12, 13});

            PRINT(">");
            v.assign(data.begin(), data.end());
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 11);
            CHECK(v[1] == 12);
            CHECK(v[2] == 13);
        }

        {
            const std::vector<int> data({21, 22});

            PRINT(">");
            v.assign(data.begin(), data.end());
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 21);
            CHECK(v[1] == 22);
        }

        {
            const std::vector<int> data({31, 32, 33, 34});

            PRINT(">");
            v.assign(data.begin(), data.end());
            PRINT("<");

            CHECK(v.size() == 4);
            CHECK(v[0] == 31);
            CHECK(v[1] == 32);
            CHECK(v[2] == 33);
            CHECK(v[3] == 34);
        }

        {
            const std::vector<int> data({41, 42, 43, 44, 45, 46});

            PRINT(">");
            v.assign(data.begin(), data.end());
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 41);
            CHECK(v[1] == 42);
            CHECK(v[2] == 43);
            CHECK(v[3] == 44);
            CHECK(v[4] == 45);
            CHECK(v[5] == 46);
        }

        {
            const std::vector<int> data;

            PRINT(">");
            v.assign(data.begin(), data.end());
            PRINT("<");

            CHECK(v.size() == 0);
        }
    }

    PRINT("Test assign(MoveIterator, MoveIterator)");
    {
        sfl::static_vector<xint, 10> v;

        {
            std::vector<xint> data({11, 12, 13});

            PRINT(">");
            v.assign
            (
                std::make_move_iterator(data.begin()),
                std::make_move_iterator(data.end())
            );
            PRINT("<");

            CHECK(data[0] == -11);
            CHECK(data[1] == -12);
            CHECK(data[2] == -13);
            CHECK(v.size() == 3);
            CHECK(v[0] == 11);
            CHECK(v[1] == 12);
            CHECK(v[2] == 13);
        }

        {
            std::vector<xint> data({21, 22});

            PRINT(">");
            v.assign
            (
                std::make_move_iterator(data.begin()),
                std::make_move_iterator(data.end())
            );
            PRINT("<");

            CHECK(data[0] == -21);
            CHECK(data[1] == -22);
            CHECK(v.size() == 2);
            CHECK(v[0] == 21);
            CHECK(v[1] == 22);
        }

        {
            std::vector<xint> data({31, 32, 33, 34});

            PRINT(">");
            v.assign
            (
                std::make_move_iterator(data.begin()),
                std::make_move_iterator(data.end())
            );
            PRINT("<");

            CHECK(data[0] == -31);
            CHECK(data[1] == -32);
            CHECK(data[2] == -33);
            CHECK(data[3] == -34);
            CHECK(v.size() == 4);
            CHECK(v[0] == 31);
            CHECK(v[1] == 32);
            CHECK(v[2] == 33);
            CHECK(v[3] == 34);
        }

        {
            std::vector<xint> data({41, 42, 43, 44, 45, 46});

            PRINT(">");
            v.assign
            (
                std::make_move_iterator(data.begin()),
                std::make_move_iterator(data.end())
            );
            PRINT("<");

            CHECK(data[0] == -41);
            CHECK(data[1] == -42);
            CHECK(data[2] == -43);
            CHECK(data[3] == -44);
            CHECK(data[4] == -45);
            CHECK(data[5] == -46);
            CHECK(v.size() == 6);
            CHECK(v[0] == 41);
            CHECK(v[1] == 42);
            CHECK(v[2] == 43);
            CHECK(v[3] == 44);
            CHECK(v[4] == 45);
            CHECK(v[5] == 46);
        }
    }

    PRINT("Test assign(std::initializer_list)");
    {
        sfl::static_vector<xint, 10> v;

        // n <= capacity && n > size
        {
            PRINT(">");
            v.assign({11, 12, 13});
            PRINT("<");

            CHECK(v.size() == 3);
            CHECK(v[0] == 11);
            CHECK(v[1] == 12);
            CHECK(v[2] == 13);
        }

        // n <= capacity && n <= size
        {
            PRINT(">");
            v.assign({21, 22});
            PRINT("<");

            CHECK(v.size() == 2);
            CHECK(v[0] == 21);
            CHECK(v[1] == 22);
        }

        // n <= capacity && n > size
        {
            PRINT(">");
            v.assign({31, 32, 33, 34});
            PRINT("<");

            CHECK(v.size() == 4);
            CHECK(v[0] == 31);
            CHECK(v[1] == 32);
            CHECK(v[2] == 33);
            CHECK(v[3] == 34);
        }

        // n > capacity
        {
            PRINT(">");
            v.assign({41, 42, 43, 44, 45, 46});
            PRINT("<");

            CHECK(v.size() == 6);
            CHECK(v[0] == 41);
            CHECK(v[1] == 42);
            CHECK(v[2] == 43);
            CHECK(v[3] == 44);
            CHECK(v[4] == 45);
            CHECK(v[5] == 46);
        }

        {
            PRINT(">");
            v.assign({});
            PRINT("<");

            CHECK(v.size() == 0);
        }
    }

    PRINT("Test operator=(const static_vector&)");
    {
        // v1.size() == v2.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12});
            sfl::static_vector<xint, 10> v2({20, 21, 22});

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);

            PRINT(">");
            v1 = v2;
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
        }

        // v1.size() < v2.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12});
            sfl::static_vector<xint, 10> v2({20, 21, 22, 23, 24});

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            CHECK(v2.size() == 5);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
            CHECK(v2[3] == 23);
            CHECK(v2[4] == 24);

            PRINT(">");
            v1 = v2;
            PRINT("<");

            CHECK(v1.size() == 5);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);
            CHECK(v1[3] == 23);
            CHECK(v1[4] == 24);

            CHECK(v2.size() == 5);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
            CHECK(v2[3] == 23);
            CHECK(v2[4] == 24);
        }

        // v1.size() > v2.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12, 13, 14});
            sfl::static_vector<xint, 10> v2({20, 21, 22});

            CHECK(v1.size() == 5);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);
            CHECK(v1[3] == 13);
            CHECK(v1[4] == 14);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);

            PRINT(">");
            v1 = v2;
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
        }
    }

    PRINT("Test operator=(static_vector&&)");
    {
        // v1.size() == v2.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12});
            sfl::static_vector<xint, 10> v2({20, 21, 22});

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);

            PRINT(">");
            v1 = std::move(v2);
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);

            // v2 is in a valid but unspecified state
            CHECK(v2.size() == 3);
            CHECK(v2[0] == -20);
            CHECK(v2[1] == -21);
            CHECK(v2[2] == -22);
        }

        // v1.size() < v2.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12});
            sfl::static_vector<xint, 10> v2({20, 21, 22, 23, 24});

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            CHECK(v2.size() == 5);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);
            CHECK(v2[3] == 23);
            CHECK(v2[4] == 24);

            PRINT(">");
            v1 = std::move(v2);
            PRINT("<");

            CHECK(v1.size() == 5);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);
            CHECK(v1[3] == 23);
            CHECK(v1[4] == 24);

            // v2 is in a valid but unspecified state
            CHECK(v2.size() == 5);
            CHECK(v2[0] == -20);
            CHECK(v2[1] == -21);
            CHECK(v2[2] == -22);
            CHECK(v2[3] == -23);
            CHECK(v2[4] == -24);
        }

        // v1.size() > v2.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12, 13, 14});
            sfl::static_vector<xint, 10> v2({20, 21, 22});

            CHECK(v1.size() == 5);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);
            CHECK(v1[3] == 13);
            CHECK(v1[4] == 14);

            CHECK(v2.size() == 3);
            CHECK(v2[0] == 20);
            CHECK(v2[1] == 21);
            CHECK(v2[2] == 22);

            PRINT(">");
            v1 = std::move(v2);
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);

            // v2 is in a valid but unspecified state
            CHECK(v2.size() == 3);
            CHECK(v2[0] == -20);
            CHECK(v2[1] == -21);
            CHECK(v2[2] == -22);
        }
    }

    PRINT("Test operator=(std::initializer_list)");
    {
        // v1.size() == ilist.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12});

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            PRINT(">");
            v1 = {20, 21, 22};
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);
        }

        // v1.size() < ilist.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12});

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);

            PRINT(">");
            v1 = {20, 21, 22, 23, 24};
            PRINT("<");

            CHECK(v1.size() == 5);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);
            CHECK(v1[3] == 23);
            CHECK(v1[4] == 24);
        }

        // v1.size() > ilist.size()
        {
            sfl::static_vector<xint, 10> v1({10, 11, 12, 13, 14});

            CHECK(v1.size() == 5);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 11);
            CHECK(v1[2] == 12);
            CHECK(v1[3] == 13);
            CHECK(v1[4] == 14);

            PRINT(">");
            v1 = {20, 21, 22};
            PRINT("<");

            CHECK(v1.size() == 3);
            CHECK(v1[0] == 20);
            CHECK(v1[1] == 21);
            CHECK(v1[2] == 22);
        }
    }

    PRINT("Test non-member comparison operators");
    {
        sfl::static_vector<xint, 10> v1({10, 20, 30});
        sfl::static_vector<xint, 10> v2({10, 20, 30, 40});

        CHECK(v1 == v1);
        CHECK(!(v1 != v1));
        CHECK(!(v1 == v2));
        CHECK(v1 != v2);
        CHECK(!(v1 < v1));
        CHECK(v1 < v2);
        CHECK(v1 <= v1);
        CHECK(v1 >= v1);
        CHECK(v1 <= v2);
        CHECK(v2 > v1);
        CHECK(v2 >= v1);
        CHECK(v2 >= v2);
    }

    PRINT("Test non-member swap");
    {
        sfl::static_vector<xint, 10> v1({10, 11, 12});
        sfl::static_vector<xint, 10> v2({20, 21, 22});

        using std::swap;
        swap(v1, v2);
    }

    PRINT("Test non-member erase(static_vector&, const U&)");
    {
        sfl::static_vector<xint, 10> v1({10, 20, 20, 30, 30, 30});

        CHECK(v1.size() == 6);
        CHECK(v1[0] == 10);
        CHECK(v1[1] == 20);
        CHECK(v1[2] == 20);
        CHECK(v1[3] == 30);
        CHECK(v1[4] == 30);
        CHECK(v1[5] == 30);

        {
            PRINT(">");
            auto res = erase(v1, 42);
            PRINT("<");

            CHECK(res == 0);
            CHECK(v1.size() == 6);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 20);
            CHECK(v1[2] == 20);
            CHECK(v1[3] == 30);
            CHECK(v1[4] == 30);
            CHECK(v1[5] == 30);
        }

        {
            PRINT(">");
            auto res = erase(v1, 20);
            PRINT("<");

            CHECK(res == 2);
            CHECK(v1.size() == 4);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 30);
            CHECK(v1[2] == 30);
            CHECK(v1[3] == 30);
        }
    }

    PRINT("Test non-member erase_if(small_vector&, Predicate)");
    {
        sfl::static_vector<xint, 10> v1({10, 20, 20, 30, 30, 30});

        CHECK(v1.size() == 6);
        CHECK(v1[0] == 10);
        CHECK(v1[1] == 20);
        CHECK(v1[2] == 20);
        CHECK(v1[3] == 30);
        CHECK(v1[4] == 30);
        CHECK(v1[5] == 30);

        {
            PRINT(">");
            auto res = erase_if(v1, [](const xint& x){ return x == 42; });
            PRINT("<");

            CHECK(res == 0);
            CHECK(v1.size() == 6);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 20);
            CHECK(v1[2] == 20);
            CHECK(v1[3] == 30);
            CHECK(v1[4] == 30);
            CHECK(v1[5] == 30);
        }

        {
            PRINT(">");
            auto res = erase_if(v1, [](const xint& x){ return x == 20; });
            PRINT("<");

            CHECK(res == 2);
            CHECK(v1.size() == 4);
            CHECK(v1[0] == 10);
            CHECK(v1[1] == 30);
            CHECK(v1[2] == 30);
            CHECK(v1[3] == 30);
        }
    }
}
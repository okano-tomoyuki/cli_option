/**
 * @file cli_option.hpp
 * @author okano tomoyuki (okano.development@gmail.com)
 * @brief コマンドライン引数パーサクラス
 * @version 0.1
 * @date 2024-12-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef UTILITY_CLI_OPTION_HPP
#define UTILITY_CLI_OPTION_HPP
 
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
 
class CliOption
{
 
public:
    enum ArgType
    {
        ARG_OPTIONAL    = (1 << 0),
        ARG_NONE        = (1 << 1),
        ARG_REQUIRE     = (1 << 2),
    };
 
    template<ArgType A>
    CliOption& add(const char& short_name, const std::string& long_name, const std::string& description)
    {
        option_list_.push_back(Arg{A, short_name, long_name, description});
        return *this;
    }
 
    void parse(int argc, char** argv)
    {
        parsed_list_.clear();
        std::vector<std::string> list;
        while (argc > 1)
        {
            std::string elem(argv[argc - 1]);
            if (elem.size() == 2 && elem[0] == '-' && elem[1] != '-')
            {
                const auto iter = std::find_if(option_list_.begin(), option_list_.end(), [&elem](const Option& option){ return (elem[1] == option.short_name); });
                if (iter != option_list_.end())
                {
                    std::reverse(list.begin(), list.end());
                    parsed_list_.push_back(Option(iter->arg_type, iter->short_name, iter->long_name, iter->description, list));                    
                    list.clear();
                }
            }
            else if (elem.size() > 2 && elem[0] == '-' && elem[1] == '-')
            {
                const auto iter = std::find_if(option_list_.begin(), option_list_.end(), [&elem](const Option& option){ return (elem.substr(2) == option.long_name); });
                if (iter != option_list_.end())
                {
                    std::reverse(list.begin(), list.end());
                    parsed_list_.push_back(Option(iter->arg_type, iter->short_name, iter->long_name, iter->description, list));                    
                    list.clear();
                }
            }
            else
            {
                list.push_back(elem);
            }
            argc--;
        }
        std::reverse(parsed_list_.begin(), parsed_list_.end());
    }
 
    void usage(const std::string& header = "", const std::string& footer = "") const
    {
        if (!header.empty())
            std::cout << header << std::endl;
 
        for (const auto& option : option_list_)
        {
            std::string prefix = "   -" + std::string(1, option.short_name) + ", --" + option.long_name;
            std::cout << prefix << std::string(30 - prefix.size(), ' ') << " ... " << option.description << std::endl;
        }
 
        if (!footer.empty())
            std::cout << footer << std::endl;
    }
 
    void print_value() const
    {
        for (const auto& option : option_list_)
        {
            std::string prefix = "   -" + std::string(1, option.short_name) + ", --" + option.long_name;
            std::cout << prefix << std::string(30 - prefix.size(), ' ') << " ... ";
            const auto iter = std::find_if(parsed_list_.begin(), parsed_list_.end(), [&option](const Option& parsed) { return (option.short_name == parsed.short_name && option.long_name == parsed.long_name);});
            std::cout << "specified : " << ((iter != parsed_list_.end()) ? "[o] " : "[x] ");
            if (iter != parsed_list_.end() && !iter->arg_list.empty())
            {
                std::cout << "arguments :";
                for (const auto& v : iter->arg_list)
                {
                    std::cout << ' ' << v;
                }
            }
            std::cout << std::endl;
        }
    }
 
    bool found(const char& key) const
    {
        return std::find_if(parsed_list_.begin(), parsed_list_.end(), [&key](const Option& option) { return (key == option.short_name); }) != parsed_list_.end();
    }
 
    bool found(const std::string& key) const
    {
        return std::find_if(parsed_list_.begin(), parsed_list_.end(), [&key](const Option& option) { return (key == option.long_name); }) != parsed_list_.end();
    }

private:
 
    struct Option
    {
        ArgType                     arg_type;
        char                        short_name;
        std::string                 long_name;
        std::string                 description;
        std::vector<std::string>    arg_list;
 
        Option(const ArgType& arg_type, const char& short_name, const std::string& long_name, const std::string& description, const std::vector<std::string>& arg_list = {})
            : arg_type(arg_type)
            , short_name(short_name)
            , long_name(long_name)
            , description(description)
            , arg_list(arg_list)
        {}
    };
 
    std::vector<Option>   option_list_;
    std::vector<Option>   parsed_list_;
 
};
 
#endif // UTILITY_CLI_OPTION_HPP
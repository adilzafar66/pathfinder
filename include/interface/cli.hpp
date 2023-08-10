#pragma once

#include <iostream>
#include <getopt.h>
#include <stdexcept>
#include <string>

namespace interface
{
    class CLIInterface
    {
    public:
        CLIInterface(int argc, char **argv);
        std::string get_algorithm() const;
        std::string get_input_file() const;
        std::string get_output_file() const;
        bool get_path_only() const;

    private:
        std::string algorithm;
        std::string input_file;
        std::string output_file;
        bool path_only;
        void display_help();
    };

    CLIInterface::CLIInterface(int argc, char **argv)
    {
        if (argc < 2)
        {
            display_help();
            exit(1);
        }
        bool has_algorithm = false;
        bool has_input_file = false;
        int option;
        while ((option = getopt(argc, argv, "a:f:o:p")) != -1)
        {
            switch (option)
            {
            case 'a':
                algorithm = optarg;
                has_algorithm = true;
                break;
            case 'f':
                input_file = optarg;
                has_input_file = true;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'p':
                path_only = true;
                break;
            default:
                throw std::invalid_argument("Invalid command line argument");
            }
        }
        if (!has_algorithm || !has_input_file)
        {
            throw std::invalid_argument("Both '-a' and '-f' must be provided.");
        }
        if (algorithm != "astar" && algorithm != "dijkstra" && algorithm != "all")
        {
            throw std::invalid_argument("Invalid algorithm option. Use 'a_star', 'dijkstra' or 'all'.");
        }
    }

    inline std::string CLIInterface::get_algorithm() const
    {
        return algorithm;
    }

    inline std::string CLIInterface::get_input_file() const
    {
        return input_file;
    }

    inline std::string CLIInterface::get_output_file() const
    {
        return output_file;
    }

    inline bool CLIInterface::get_path_only() const
    {
        return path_only;
    }

    void CLIInterface::display_help()
    {
        std::cout << "Usage: program <input_file> [options]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -a <algorithm>      Specify the algorithm (astar or dijkstra). Use 'all' for both." << std::endl;
        std::cout << "  -f <output_file>    Read input path to a text file." << std::endl;
        std::cout << "  -o <output_file>    Save output path to a text file." << std::endl;
        std::cout << "  -p                  Output path only (no additional information)." << std::endl;
    }

} // namespace interface

//
// Created by luke on 22-08-21.
//

#include "FileIO.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

//#include "bout/Console.h"

namespace Utils
{
    /* static */ bool FileIO::readFile(Path path, Buffer buffer, bool fromRoot, int16_t index)
    {
        std::filesystem::path filePath = ((fromRoot) ? getenv("HOME") : "") + path;

        try
        {
            if (!exists(filePath))
            {
                std::cerr << "file does not exit -> " << path << std::endl;
                return false;
            }
            else
            {
                std::ifstream file(filePath);
                std::string line;

                if (index > -1)
                {
                    uint16_t lineIndex = 0;
                    while (getline(file, line) && lineIndex < index)
                        ++lineIndex;
                    buffer.emplace_back(line);
                }
                else
                {
                    while (getline(file, line))
                        buffer.emplace_back(line);
                }
                file.close();
            }
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
        return true;
    }
    /* static */ bool FileIO::createFile(Path path, Name name, bool fromRoot)
    {
        std::filesystem::path filePath = ((fromRoot) ? getenv("HOME") : "") + path + name;
        try
        {
            if (exists(filePath))
            {
                std::cerr << "file already exists -> " << path << std::endl;

                return false;
            }
            else
            {
                std::ofstream file(filePath);
                file.close();
            }
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
        return true;
    }
    /* static */ bool FileIO::removeFile(Path path, bool fromRoot)
    {
        std::filesystem::path filePath = ((fromRoot) ? getenv("HOME") : "") + path;
        try
        {
            if (!exists(filePath))
            {
                std::cerr << "file does not exit -> " << path << std::endl;

                return false;
            }
            else
            {
                std::filesystem::remove(filePath);
            }
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
        return true;
    }
    /* static */ bool FileIO::readDirectory(Path path, Buffer buffer, bool fromRoot)
    {
        std::filesystem::path dirPath = ((fromRoot) ? getenv("HOME") : "") + path;

        try
        {
            if (!exists(dirPath))
            {
                std::cerr << "directory does not exit -> " << path << std::endl;

                return false;
            }
            else
            {
                for (const auto& entry : std::filesystem::directory_iterator(dirPath))
                {
                    buffer.emplace_back(entry.path().generic_string());
                }
            }
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
        return true;
    }
    /* static */ bool FileIO::createDirectory(Path path, Name name, bool fromRoot)
    {
        std::filesystem::path dirPath = ((fromRoot) ? getenv("HOME") : "") + path + name;
        try
        {
            if (exists(dirPath))
            {
                std::cerr << "directory already exists -> " << path << std::endl;

                return false;
            }
            else
            {
                std::filesystem::create_directory(dirPath);
            }
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
        return true;
    }
    /* static */ bool FileIO::removeDirectory(Path path, bool fromRoot)
    {
        std::filesystem::path dirPath = ((fromRoot) ? getenv("HOME") : "") + path;
        try
        {
            if (!exists(dirPath))
            {
                std::cerr << "directory does not exist -> " << path << std::endl;

                return false;
            }
            else
            {
                std::filesystem::remove_all(dirPath);
            }
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
        return true;
    }
    /* static */ bool FileIO::writeToFile(Path path, Lines lines, bool append, bool fromRoot)
    {
        std::filesystem::path filePath = ((fromRoot) ? getenv("HOME") : "") + path;
        try
        {
            std::ofstream file;

            if (append)
            {
                file.open(filePath, std::ios_base::app);
            }
            else
            {
                file.open(filePath, std::ofstream::trunc | std::ofstream::out);
            }
            for_each(lines.begin(), lines.end(), [&](const std::string& line) {
                file << line << "\n";
            });
            file.close();
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
        return true;
    }
    /* static */ bool FileIO::elementExist(Path path, bool fromRoot)
    {
        std::filesystem::path elementPath = ((fromRoot) ? getenv("HOME") : "") + path;
        try
        {
            return (exists(elementPath));
        }
        catch (std::filesystem::filesystem_error& e)
        {
            std::cerr << "filesystem error -> " << e.what() << ", " << path << std::endl;
            return false;
        }
    }
    /* static */ std::string FileIO::getFileName(Path path)
    {
        return std::string(path.begin() + static_cast<long>(path.rfind('/')) + 1, path.end());
    }

}// namespace Utils
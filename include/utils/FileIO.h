//
// Created by luke on 10-08-21.
//

#ifndef FILEIO_FILEIO_H
#define FILEIO_FILEIO_H

#include <string>
#include <vector>

namespace Utils
{
    class FileIO
    {
    private:
        typedef const std::string& Path;
        typedef const std::string& Name;
        typedef std::vector<std::string>& Buffer;
        typedef const std::vector<std::string>& Lines;

    public:
        /**
         * read content of file
         *
         * @param path      path to file
         * @param buffer    buffer for file content
         * @param fromRoot  true if path is from root, false for absolute
         * @param index     index >= 0 for specific line
         * @return          true if file was successfully read
         */
        [[maybe_unused]] static bool readFile(Path path, Buffer buffer, bool fromRoot = true, int16_t index = -1);
        /**
         * create new file
         *
         * @param path      path to new file
         * @param name      name of new file
         * @param fromRoot  true if path is from root, false for absolute
         * @return          true if file was successfully created
         */
        [[maybe_unused]] static bool createFile(Path path, Name name, bool fromRoot = true);
        /**
         * remove file
         *
         * @param path      path to file
         * @param fromRoot  true if path is from root, false for absolute
         * @return          true if file was successfully removed
         */
        [[maybe_unused]] static bool removeFile(Path path, bool fromRoot = true);
        /**
         * read content of directory
         *
         * @param path      path to directory
         * @param buffer    buffer for directory content
         * @param fromRoot  true if path is from root, false for absolute
         * @return          true if directory was successfully read
         */
        [[maybe_unused]] static bool readDirectory(Path path, Buffer buffer, bool fromRoot = true);
        /**
         * create new directory
         *
         * @param path      path to new directory
         * @param name      name of new directory
         * @param fromRoot  true if path is from root, false for absolute
         * @return          true if directory was successfully created
         */
        [[maybe_unused]] static bool createDirectory(Path path, Name name, bool fromRoot = true);
        /**
         * remove directory
         *
         * @param path      path to directory
         * @param fromRoot  true if path is from root, false for absolute
         * @return          true if directory was successfully removed
         */
        [[maybe_unused]] static bool removeDirectory(Path path, bool fromRoot = true);
        /**
         * write to file
         *
         * @param path      path to file
         * @param lines     lines to write
         * @param append    true if append, false if replace
         * @param fromRoot  true if path is from root, false for absolute
         * @return          true if writing was successful
         */
        [[maybe_unused]] static bool writeToFile(Path path, Lines lines, bool append = true, bool fromRoot = true);
        /**
         * check if element exists
         *
         * @param path      path to element
         * @param fromRoot  true if path is from root, false for absolute
         * @return          true if element exists
         */
        [[maybe_unused]] static bool elementExist(Path path, bool fromRoot = true);

    private:
        [[maybe_unused]] static std::string getFileName(Path path);
    };
}// namespace utils

#endif// FILEIO_FILEIO_H

#ifdef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>
#include <cstdlib>
#include "root_directory.h"

class file_system
{
    private:
        typedef std::string (*Builder) (const std::string& path);

    public:
        static std::string getPath(const std::string& path)
        {
            static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
            return (*pathBuilder)(path);
        }

    private:
        static std::string const & getRoot()
        {
            static char const * envRoot = getenv("LOGL_ROOT_PATH");
            static char const * givenRoot = (envRoot != nullptr ? envRoot : logl_root);
            static std::string root = (givenRoot != nullptr ? givenRoot : "");
            return root;
        }
}


#endif
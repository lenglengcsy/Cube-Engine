#ifndef TFILE_H
#define TFILE_H
#include <string>
#include <vector>
#include "Data.h"

namespace tzw
{
class Tfile
{
public:
    static Tfile * shared();
    Data getData(std::string filename, bool forString);
    std::string fullPathFromRelativeFile(const std::string &filename, const std::string &relativeFile);
    void findPreFix(const char * fileName, char * dst);
    std::string guessFileType(std::string filePath);
    void addSearchPath(std::string searchPath);
    std::string getReleativePath(std::string filePath);
    std::string getFolder(std::string filePath);
    std::vector<std::string> getAbsolutlyFilePath(std::string filePath);
	std::string toAbsFilePath(std::string filePath, std::string workingCpy);
private:
    Tfile();
    ~Tfile();
    std::vector<std::string> m_searchPath;
    static Tfile * m_instance;
};
}


#endif // TFILE_H

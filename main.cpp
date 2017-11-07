#include <boost/filesystem.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <future>
#include <iostream>

using namespace std;
namespace fs = boost::filesystem;

double getDirSize(string dirAddress, double size)
 {
    fs::path dirPath = dirAddress;
    fs::path textDir = dirPath.filename();
    if(fs::exists(dirPath)){
      fs::directory_iterator end_itr;
      for(fs::directory_iterator dirIte(dirPath); dirIte != end_itr; ++dirIte)
      {
        fs::path filePath(dirIte->path());
        try
        {
          if(!fs::is_directory(dirIte->status()))
          {
            size = size + fs::file_size(filePath);
          }
          else
          {
            getDirSize(filePath.string(), size);
          }
        }
        catch(exception& e)
        {
          cout << e.what() << endl;
        }
        //Проверка на то, является ли файл, размер которого мы хотим узнать, директорией
      }
    }
    return size;
}

string getDirPath(string path)
 {
    fs::path  dirPath = path;
    fs::path textDir = dirPath.parent_path();
    return textDir.string();
}

void getDirPerm(string path)
{
  fs::path dirPath = path;
  fs::file_status s = status(dirPath);
    printf("%o\n",s.permissions());
}

void GetInfo(string path)
{
    //Проверка на существование репозитория
    if(!fs::exists(path))
    {
      throw runtime_error("Такой директории не существует!!!");
    }
    //Проверка на то, является ли файл директорией
    if(!fs::is_directory(path))
    {
      throw runtime_error("Это не директория!!!");
    }

    std::packaged_task<string(string)> taskPath(getDirPath);
    std::packaged_task<double(string, double)> taskSize(getDirSize);
    std::packaged_task<void(string)> taskPerm(getDirPerm);

    std::future<string> dirPath = taskPath.get_future();
    std::future<double> dirSize = taskSize.get_future();
    std::future<void> dirPerm = taskPerm.get_future();

    taskPath(path);
    taskSize(path, 0);

    cout << " " << endl;
    cout << "Полный путь: " << dirPath.get() << endl;
    cout << "Размер: " << dirSize.get() << endl;
    cout << "Права доступа: ";
    taskPerm(path);
    cout << "" << endl;
    /*cout << "Права доступа: ";
    std::async (getDirPerm,path);

    std::future<string> futureDirPath = std::async (getDirPath,path);
    std::future<double> futureDirSize = std::async (getDirSize,path, 0);
    string dirName = futureDirPath.get();
    double dirSize = futureDirSize.get();

    //Вывод данных о директории
    cout << " " << endl;
    cout << "Полный путь: " << dirName << endl;
    cout << "Размер: " << dirSize << endl;
    cout << "Права доступа: ";
    std::async (getDirPerm,path);
    cout << " " << endl;
*/
 }
void rec()
{
  string path;
  cout << "Укажите дирректорию:" << endl;
  getline(cin,path);
  if (path.empty())
  {
    cout << "------------ END ------------" << endl;
  }
  else
  {
    GetInfo(path);
    rec();
  }
}

int main()
{
    try
     {
       cout << "------------ START ------------" << endl;
       cout << "Чтобы закончить работу программы нажмите Enter" << endl;

      rec();
     }
    catch(const std::exception& e)
     {
        cout << e.what() << endl;
     }
    return 0;
  }


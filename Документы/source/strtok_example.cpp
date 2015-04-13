#include <iostream>
#include <cstring>
 
int main ()
{
  char str[] = "Особенности национальной рыбалки - художественный, комедийный фильм.";
 
  std::cout << "Разделение строки "" << str << "" на лексемы:\n";
  char * pch = strtok (str," ,.-"); // во втором параметре указаны разделитель (пробел, запятая, точка, тире)
 
  while (pch != NULL)                         // пока есть лексемы
  {
      std::cout << pch  << "\n";
      pch = strtok (NULL, " ,.-");
  }
  return 0;
}

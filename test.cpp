#include <iostream>
// #include <fstream>
// #include <cstring>
// #include <charconv>
// #include <iomanip>
#include <string>

using namespace std;

// #include "delimbuffer.h"
// #include "bufferfile.h"
// #include "recfile.h"
// #include "dvd.h"

// int main() {
//   Dvd dvd;
//   DelimFieldBuffer::SetDefaultDelim('|');
//   DelimFieldBuffer Buff;
//   RecordFile<Dvd> DvdFile(Buff);

//   DvdFile.Open("dvd-list.txt", ios_base::in);

//   int recAddr = 149;
//   int result = DvdFile.Read(dvd, recAddr);
//   cout << "Record read from address " << result << "\n";
//   cout << dvd << "\n";
//   DvdFile.Close();
//   return 0;
// }

int main ()
{
  string str;
  str = "Misran|Mohammed|Jayanagar";
  cout << str.substr(0, 6) << "\n";
  cout << str.substr(7, 8) << "\n";
  cout << str.substr(16, 9) << "\n";
  return 0;
}
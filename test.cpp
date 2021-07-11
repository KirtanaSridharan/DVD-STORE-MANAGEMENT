#include <iostream>
#include <fstream>
#include <cstring>
#include <charconv>
#include <iomanip>

using namespace std;

#include "delimbuffer.h"
#include "bufferfile.h"
#include "recfile.h"
#include "dvd.h"

int main() {
  Dvd dvd;
  DelimFieldBuffer::SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  RecordFile<Dvd> DvdFile(Buff);

  DvdFile.Open("dvd-list.txt", ios_base::in);

  int recAddr = 149;
  int result = DvdFile.Read(dvd, recAddr);
  cout << "Record read from address " << result << "\n";
  cout << dvd << "\n";
  DvdFile.Close();
  return 0;
}
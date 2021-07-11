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
  cout << setiosflags(ios::left);
  int choice, result, recAddr;
  Dvd dvd;
  DelimFieldBuffer::SetDefaultDelim('|');
  DelimFieldBuffer Buff;
  RecordFile<Dvd> DvdFile(Buff);

  do {
    cout << "\n1. Insert\t2. Display\t3. Exit";
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        DvdFile.Open("dvd-list.txt", ios_base::out | ios_base::app | ios_base::ate);
        cout << "\nEnter the DVD details:\n";
        cin >> dvd;
        recAddr = DvdFile.Write(dvd);
        cout << "New record written at this address: " << recAddr << "\n";
        break;
      
      case 2:
        DvdFile.Open("dvd-list.txt", ios_base::in);
        cout << "\nThe DVD details are:\n";
        dvd.PrintHeadings();
        while (true) {
          result = DvdFile.Read(dvd);
          if (result == -1) break;
          dvd.PrintRecord();
        }
        break;
      
      case 3: cout << "\nProgram terminated.\n"; break;
      default: cout << "\nInvalid choice. Please try again!\n";
    }
    DvdFile.Close();
  } while (choice != 3);
  return 0;
}
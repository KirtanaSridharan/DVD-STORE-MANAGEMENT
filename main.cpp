#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iomanip>
#include <stdexcept>

using namespace std;

const string RESET =  "\x1B[0m";
const string RED = "\x1B[31m";
const string GREEN = "\x1B[32m";
const string YELLOW = "\x1B[33m";
const string BLUE = "\x1B[34m";
const string MAGENTA = "\x1B[35m";
const string CYAN = "\x1B[36m";
const string BOLD = "\x1B[1m";
const string ITALIC = "\x1B[3m";
const string REVERSED = "\x1B[7m";

#include "btreenode.h"
#include "btree.h"
#include "utils.h"
#include "iobuffer.h"
#include "dvd.h"
#include "index.h"
#include "bufferfile.h"
#include "recfile.h"
#include "indexfile.h"

void addNewDvdRecord(RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  File.Open(ios_base::out | ios_base::app | ios_base::ate);
  cout << "\nEnter the DVD details:\n\n";
  dvd.ReadFromStandardInput(tree);
  int recAddr = File.Write(dvd);
  cout << GREEN << "\nNew record written at address " << recAddr << RESET << "\n";
  File.Close();
  tree.insert(dvd.getID(), recAddr);
}

void displayAllDvdRecordsEntrySequenced(RecordFile<Dvd> & File, Dvd & dvd) {
  File.Open(ios_base::in);
  cout << "\n" << BOLD << "DVD RECORDS (ENTRY-SEQUENCED):" << RESET << "\n";
  dvd.PrintHeadings();
  while (true) {
    if (File.Read(dvd) == -1) break;
    if (dvd.ID[0] != '$') dvd.PrintRecord();
  }
  cout << BLUE << string(140, '-') << "\n" << RESET;
  File.Close();
}

void displayAllDvdRecordsSortedById(RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  File.Open(ios_base::in);
  cout << "\n" << BOLD << "DVD RECORDS (SORTED BY ID):" << RESET << "\n";
  vector<pair<int, int>> nodes = tree.fetchAll();
  dvd.PrintHeadings();
  for (auto node: nodes) {
    if (File.Read(dvd, node.second) == -1) break;
    dvd.PrintRecord();
  }
  cout << BLUE << string(140, '-') << "\n" << RESET;
  File.Close();
}

int searchDvdRecord(int key, RecordFile<Dvd> & File, Dvd & dvd, BTree tree) {
  int result = tree.search(key);
  if (result != -1) {
    File.Open(ios_base::in);
    File.Read(dvd, result);
    File.Close();
  }
  return result;
}

int removeDvdRecord(int key, RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  int recAddr = searchDvdRecord(key, File, dvd, tree);
  if (recAddr != -1) {
    cout << GREEN << "\nRecord found.\n" << RESET;
    cout << dvd;
    cout << BLUE << ITALIC << "\nAre you sure you want to delete the record? [y/n]: ";
    char ch;
    cin >> ch;
    cout << RESET;

    if (ch == 'y' || ch == 'Y') {
      File.Open(ios_base::in | ios_base::out);
      File.MarkAsDeleted(recAddr);
      File.Close();
      tree.remove(key);
      cout << GREEN << "\nRecord deleted successfully.\n" << RESET;
    }
    else
      return -1;
  }
  else
    cout << RED << "\nRecord not found.\n" << RESET;
  return recAddr;
}

int modifyDvdRecord(int key, RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  int recAddr = searchDvdRecord(key, File, dvd, tree);
  int result;
  if (recAddr != -1) {
    File.Open(ios_base::in | ios_base::out);
    File.MarkAsDeleted(recAddr);
    File.Close();
    tree.remove(key);
    dvd.ModifyRecord();
    File.Open(ios_base::out | ios_base::app | ios_base::ate);
    result = File.Write(dvd);
    cout << GREEN << "\nModified record written at address " << result << "\n" << RESET;
    File.Close();
    tree.insert(dvd.getID(), result);
    return result;
  } else
    cout << RED << "\nRecord not found.\n" << RESET;
  return recAddr;
}

void treeTraversal(BTree & tree) {
  vector<pair<int, int>> nodes = tree.fetchAll();
  cout << "\n" << BOLD << "B TREE (" << nodes.size() << ")" << RESET << "\n";  
  cout << BLUE;
  cout << string(30, '-') << "\n";
  cout << setw(10) << "KEY" << setw(15) << "REC ADDR" << "\n";
  cout << string(30, '-') << "\n";
  cout << RESET;
  for (auto node: nodes)
    cout << setw(10) << node.first << setw(20) << node.second << "\n";
  cout << BLUE;
  cout << string(30, '-') << "\n";
  cout << RESET;
}

void printMenu() {
  cout << "\n" << BOLD << "DVD STORE MANAGEMENT SYSTEM" << RESET;
  cout << YELLOW;
  cout << "\n" << string(40, '_') << "\n";
  cout << "\n1. INSERT NEW DVD RECORD";
  cout << "\n2. DISPLAY (ENTRY-SEQUENCED)";
  cout << "\n3. DISPLAY (SORTED BY ID)";
  cout << "\n4. SEARCH BY ID";
  cout << "\n5. DELETE BY ID";
  cout << "\n6. MODIFY BY ID";
  cout << "\n7. TRAVERSE THE BTREE";
  cout << "\n8. EXIT";
  cout << "\n" << string(40, '_') << "\n";
  cout << RESET;
}

// Main function
int main() {
  cout << setiosflags(ios_base::left);
  int choice, key, rewriteFlag = false;
  IOBuffer RecordBuff('|');
  IOBuffer IndexBuff('|');
  Dvd dvd;
  BTree tree(5);
  RecordFile<Dvd> DvdFile(RecordBuff, "dvds.txt");
  IndexFile BTreeIndexFile(IndexBuff, "btreeindex.txt");

  DvdFile.Create();
  BTreeIndexFile.Create();

  BTreeIndexFile.Open(ios_base::in);
  BTreeIndexFile.Read(tree);
  BTreeIndexFile.Close();

  do {
    printMenu();
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        addNewDvdRecord(DvdFile, dvd, tree);
        rewriteFlag = true;
        break;
      
      case 2:
        displayAllDvdRecordsEntrySequenced(DvdFile, dvd);
        break;
      
      case 3:
        displayAllDvdRecordsSortedById(DvdFile, dvd, tree);
        break;
      
      case 4:
        cout << "\n" << BOLD << "SEARCH BY ID:" << RESET;
        cout << "\nEnter the key: ";
        cin >> key;
        if (searchDvdRecord(key, DvdFile, dvd, tree) == -1) {
          cout << RED << "\nRecord not found.\n" << RESET;
        } else {
          cout << GREEN << "\nRecord found.\n" << RESET;
          cout << dvd;
        }
        break;

      case 5:
        cout << "\n" << BOLD << "DELETE BY ID:" << RESET;
        cout << "\nEnter the key: ";
        cin >> key;
        if (removeDvdRecord(key, DvdFile, dvd, tree) != -1)
          rewriteFlag = true;
        break;

      case 6:
        cout << "\n" << BOLD << "MODIFY BY ID:" << RESET;
        cout << "\nEnter the key: ";
        cin >> key;
        if (modifyDvdRecord(key, DvdFile, dvd, tree) != -1)
          rewriteFlag = true;
        break;
      
      case 7:
        treeTraversal(tree);
        break;

      case 8:
        cout << GREEN << "\nProgram terminated.\n" << RESET;
        break;
      
      default:
        cout << RED << "\nInvalid choice. Please try again!\n" << RESET;
    }
  } while (choice != 8);

  if (rewriteFlag) {
    cout << BLUE << ITALIC << "Rewriting Index File....\n" << RESET;
    BTreeIndexFile.Open(ios_base::out);
    BTreeIndexFile.Write(tree);
    BTreeIndexFile.Close();
    cout << GREEN << "Success!\n" << RESET;
  }

  return 0;
}
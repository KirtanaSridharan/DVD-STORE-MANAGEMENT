#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iomanip>
#include <stdexcept>

using namespace std;

#include "color.h"
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
  cout << GREEN << "\nNew record written at address " << recAddr << "\n";
  File.Close();
  tree.insert(dvd.getID(), recAddr);
}

void displayAllDvdRecordsEntrySequenced(RecordFile<Dvd> & File, Dvd & dvd) {
  File.Open(ios_base::in);
  cout << "\nDVD Records - Based on entry-sequence:\n\n";
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
  cout << "\nDVD Records - Sorted by ID:\n\n";
  vector<pair<int, int>> nodes = tree.fetchAll();
  dvd.PrintHeadings();
  for (auto node: nodes) {
    if (File.Read(dvd, node.second) == -1) break;
    dvd.PrintRecord();
  }
  cout << BLUE << string(140, '-') << "\n" << RESET;
  File.Close();
}

int searchDvdRecord(int key, RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  cout << BLUE << ITALIC << "\nSearching based on ID...\n\n" << RESET;
  int result = tree.search(key);
  if (result == -1) 
    cout << GREEN <<"\nRecord not found.\n" << RESET;
  else {
    File.Open(ios_base::in);
    File.Read(dvd, result);
    File.Close();
    cout << GREEN << "\nRecord found.\n" << RESET;
    cout << dvd << "\n";
  }
  return result;
}

int removeDvdRecord(int key, RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  int recAddr = searchDvdRecord(key, File, dvd, tree);
  if (recAddr != -1) {
    cout << BLUE << ITALIC << "\nAre you sure you want to delete the record? [y/n]\n" << RESET;
    char ch;
    cin >> ch;

    if (ch == 'y' || ch == 'Y') {
      File.Open(ios_base::in | ios_base::out);
      File.MarkAsDeleted(recAddr);
      File.Close();
      tree.remove(key);
      cout << GREEN << "\nRecord deleted successfully.\n" << RESET;
    }
    else return -1;
  }
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
    cout << GREEN << "Modified record written at address " << result << "\n" << RESET;
    File.Close();
    tree.insert(dvd.getID(), result);
    return result;
  }
  return recAddr;
}

void treeTraversal(BTree & tree) {
  vector<pair<int, int>> nodes = tree.fetchAll();
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
  cout << ITALIC;
  cout << YELLOW;
  cout << "\n" << string(40, '_') << "\n";
  cout << "\n1. Insert new DVD Record";
  cout << "\n2. Display (Entry-Sequenced)";
  cout << "\n3. Display (Sorted by ID)";
  cout << "\n4. Search by ID";
  cout << "\n5. Delete by ID";
  cout << "\n6. Modify by ID";
  cout << "\n7. Traverse the BTree";
  cout << "\n8. Exit the program";
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
  RecordFile<Dvd> DvdFile(RecordBuff, "dvd-list.txt");
  // RecordFile<Vinyl> VinylFile()
  IndexFile BTreeIndexFile(IndexBuff, "dvd-index.txt");

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
        cout << "\nEnter the key to be searched: ";
        cin >> key;
        searchDvdRecord(key, DvdFile, dvd, tree);
        break;

      case 5:
        cout << "\nEnter the key to be deleted: ";
        cin >> key;
        if (removeDvdRecord(key, DvdFile, dvd, tree) != -1)
          rewriteFlag = true;
        break;

      case 6:
        cout << "\nEnter the key to be modified: ";
        cin >> key;
        if (modifyDvdRecord(key, DvdFile, dvd, tree) != -1)
          rewriteFlag = true;
        break;
      
      case 7:
        treeTraversal(tree);
        break;

      case 8:
        cout <<GREEN << "\nProgram terminated.\n"<<RESET;
        break;
      
      default:
        cout << "\nInvalid choice. Please try again!\n";
    }
  } while (choice != 8);

  if (rewriteFlag) {
    cout << BLUE << ITALIC "\nRewriting Index File....\n" << RESET;
    BTreeIndexFile.Open(ios_base::out);
    BTreeIndexFile.Write(tree);
    BTreeIndexFile.Close();
    cout << GREEN << "\nSuccess!\n" << RESET;
  }

  return 0;
}
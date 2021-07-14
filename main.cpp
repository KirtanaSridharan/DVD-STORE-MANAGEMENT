#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iomanip>
#include <stdexcept>

using namespace std;

#include "btreenode.h"
#include "btree.h"
#include "iobuffer.h"
#include "dvd.h"
#include "index.h"
#include "bufferfile.h"
#include "recfile.h"
#include "indexfile.h"

void addNewDvdRecord(RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  File.Open(ios_base::out | ios_base::app | ios_base::ate);
  cout << "\nEnter the DVD details:\n";
  dvd.ReadFromStandardInput(tree);
  int recAddr = File.Write(dvd);
  cout << "New record written at address " << recAddr << "\n";
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
  File.Close();
}

void displayAllDvdRecordsSortedById(RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  File.Open(ios_base::in);
  cout << "\nDVD Records - Sorted by ID:\n\n";
  vector<pair<int, int>> nodes = tree.fetchAll();
  for (auto node: nodes) {
    if (File.Read(dvd, node.second) == -1) break;
    dvd.PrintRecord();
  }
  File.Close();
}

int searchDvdRecord(int key, RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  cout << "\nSearching based on ID...\n\n";
  int result = tree.search(key);
  if (result == -1) 
    cout << "\nRecord not found.\n";
  else {
    File.Open(ios_base::in);
    File.Read(dvd, result);
    File.Close();
    cout << "\nRecord found.\n";
    cout << dvd << "\n";
  }
  return result;
}

int removeDvdRecord(int key, RecordFile<Dvd> & File, Dvd & dvd, BTree & tree) {
  int recAddr = searchDvdRecord(key, File, dvd, tree);
  if (recAddr != -1) {
    cout << "\nAre you sure you want to delete the record? [y/n]\n";
    char ch;
    cin >> ch;

    if (ch == 'y' || ch == 'Y') {
      File.Open(ios_base::in | ios_base::out);
      File.MarkAsDeleted(recAddr);
      File.Close();
      tree.remove(key);
      cout << "\nRecord deleted successfully.\n";
    }
    else return -1;
  }
  return recAddr;
}

void treeTraversal(BTree & tree) {
  vector<pair<int, int>> nodes = tree.fetchAll();
  for (auto node: nodes)
    cout << "Key = " << node.first << " Record Address = " << node.second << "\n";
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
    cout << "\n1. Insert\n2. Display (Entry-Sequenced)\n3. Display (Sorted by ID)\n4. Search\n5. Delete \n6. Tree Traversal\n7. Exit";
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
        treeTraversal(tree);
        break;

      case 7:
        cout << "\nProgram terminated.\n";
        break;
      
      default:
        cout << "\nInvalid choice. Please try again!\n";
    }
  } while (choice != 7);

  if (rewriteFlag) {
    BTreeIndexFile.Open(ios_base::out);
    BTreeIndexFile.Write(tree);
    BTreeIndexFile.Close();
  }

  return 0;
}
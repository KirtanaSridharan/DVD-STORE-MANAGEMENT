class BTree {
  BTNode * root;    // Pointer to root node
  int t;            // Minimum degree

  public:
  // Constructor
  BTree(int _t);

  // A function to traverse the tree
  void traverse();

  vector<pair<int, int>> fetchAll();

  // A function to search a key in this tree
  // BTNode * search(string key);
  int search(int key);

  // A function that inserts a new key in this BTree
  // void insert(string key);
  void insert(int key, int recAddr);

  // A function that removes a key in this BTree
  void remove(int key);

  friend class BTreePrinter;
};

BTree::BTree(int _t) {
  root = NULL;
  t = _t;
}

void BTree::traverse() {
  if (root != NULL)
    root->traverse();
}

vector<pair<int, int>> BTree::fetchAll() {
  vector<pair<int, int>> nodes;
  if (root != NULL)
    nodes = root->fetchAll();
  return nodes;
}

// BTNode * BTree::search(string key) {
int BTree::search(int key) {
  return (root == NULL) ? -1 : root->search(key);
}

void BTree::insert(int key, int recAddr) {
  if (root == NULL) {
    root = new BTNode(t, true);
    root->Keys[0] = key;
    root->RecAddrs[0] = recAddr;
    root->n = 1;
  }
  else {
    if (root->n == (2 * t - 1)) {
      BTNode * s = new BTNode(t, false);
      s->C[0] = root;
      s->splitChild(0, root);
      int i = 0;
      if (s->Keys[0] < key)
        i++;
      s->C[i]->insertNonFull(key, recAddr);
      root = s;
    }
    else
      root->insertNonFull(key, recAddr);
  }
}

void BTree::remove(int key) {
  if (root == NULL) {
    cout << "The tree is empty\n";
  }
  root->remove(key);
  if (root->n == 0) {
    BTNode * temp = root;
    if (root->leaf)
      root = NULL;
    else
      root = root->C[0];
    delete temp;
  }
}
class BTreePrinter
{
	struct NodeInfo
	{
		string text;
		unsigned text_pos, text_end;  // half-open range
	};

	typedef vector<NodeInfo> LevelInfo;

	vector<LevelInfo> levels;

	string node_text (int const keys[], unsigned key_count);

	void before_traversal ()
	{
		levels.resize(0);
		levels.reserve(10);   // far beyond anything that could usefully be printed
	}

	void visit (BTNode const *node, unsigned level = 0, unsigned child_index = 0);

	void after_traversal ();

public:
	void print (BTree const &tree)
	{
		before_traversal();
		visit(tree.root);
		after_traversal();
	}
};

void BTreePrinter::visit (BTNode const *node, unsigned level, unsigned child_index)
{
	if (level >= levels.size())
		levels.resize(level + 1);

	LevelInfo &level_info = levels[level];
	NodeInfo info;

	info.text_pos = 0;
	if (!level_info.empty())  // one blank between nodes, one extra blank if left-most child
		info.text_pos = level_info.back().text_end + (child_index == 0 ? 2 : 1);

	info.text = node_text(node->Keys, unsigned(node->n));

	if (node->leaf)
	{
		info.text_end = info.text_pos + unsigned(info.text.length());
	}
	else // non-leaf -> do all children so that .text_end for the right-most child becomes known
	{
		for (unsigned i = 0, e = unsigned(node->n); i <= e; ++i)  // one more pointer than there are keys
			visit(node->C[i], level + 1, i);

		info.text_end = levels[level + 1].back().text_end;
	}

	levels[level].push_back(info);
}

string BTreePrinter::node_text (int const keys[], unsigned key_count)
{
	ostringstream os;
	char const *sep = "";

	os << "[";
	for (unsigned i = 0; i < key_count; ++i, sep = " ")
		os << sep << keys[i];
	os << "]";

	return os.str();
}

void print_blanks (unsigned n)
{
	while (n--)
		cout << ' ';
}

void BTreePrinter::after_traversal ()
{
	for (size_t l = 0, level_count = levels.size(); ; )
	{    
		auto const &level = levels[l];
		unsigned prev_end = 0;

		for (auto const &node: level)
		{         
			unsigned total = node.text_end - node.text_pos;
			unsigned slack = total - unsigned(node.text.length());
			unsigned blanks_before = node.text_pos - prev_end;

			print_blanks(blanks_before + slack / 2);
			cout << node.text;

			if (&node == &level.back())
				break;

			print_blanks(slack - slack / 2);

			prev_end += blanks_before + total;
		}

		if (++l == level_count)
			break;

		cout << "\n\n";
	}

	cout << "\n";
}
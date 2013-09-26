#include <boost\lexical_cast.hpp>

#include <vector>
#include <deque>
#include <iostream>
#include <string>


template<class T>
class BinaryTreeBuilder;

template<class T>
class BTNode {
public:
	typedef BTNode<T> Node;

	explicit BTNode(const T& data) :
		data_(data),
		left_(nullptr),
		right_(nullptr)
	{}

	const Node* left() const {
		return left_;
	}

	const Node* right() const {
		return right_;
	}

	const T& data() const {
		return data_;
	}

private:
	friend class BinaryTreeBuilder<T>;

	T data_;
	const BTNode* left_;
	const BTNode* right_;
};

template<class T>
class BinaryTree {
public:
	typedef BTNode<T> Node;
	typedef std::deque<Node> NodesContainer;

	BinaryTree(NodesContainer&& nodes, const Node* root) :
		nodes_(std::move(nodes)),
		root_(root)
	{}

	const Node* root() const {
		return root_;
	}

private:
	NodesContainer nodes_;
	const Node* root_;
};

template<class T>
class BinaryTreeBuilder {
public:
	typedef BTNode<T> Node;
	typedef BinaryTree<T> Tree;

	const Node* addNode(const T& data, const Node* left = nullptr, const Node* right = nullptr) {
		Node node(data);
		node.left_ = left;
		node.right_ = right;
		nodes_.push_back(node);
		return &nodes_.back();
	}

	void setRoot(const Node* root) {
		root_ = root;
	}

	Tree build() {
		size_t rootPosition;
		for (rootPosition = 0; rootPosition < nodes_.size(); ++rootPosition) {
			if (root_ == &nodes_[rootPosition]) {
				break;
			}
		}
		if (rootPosition == nodes_.size()) {
			throw std::out_of_range("Wrong root");
		}

		return Tree(std::move(nodes_), root_);
	}

private:
	std::deque<Node> nodes_;
	const Node* root_;
};


std::string serialize(int data) {
	return boost::lexical_cast<std::string>(data);
}

class TreeSerializer {
public:
	static const char TREE_STARTS_SYMBOL = '(';
	static const char TREE_ENDS_SYMBOL = ')';

	template<class T>
	void serialize(const BTNode<T>* node) {
		string_.push_back(TREE_STARTS_SYMBOL);
		if (node) {
			serialize(node->left());
			string_ += ::serialize(node->data());
			serialize(node->right());
		}
		string_.push_back(TREE_ENDS_SYMBOL);
	}

	const std::string& string() const {
		return string_;
	}

private:
	std::string string_;
};

template<class T>
std::string serialize(const BinaryTree<T>& tree) {
	TreeSerializer serializer;
	serializer.serialize(tree.root());
	return serializer.string();
}


template<class T>
T deserialize(const std::string&);

template<>
int deserialize(const std::string& string) {
	return boost::lexical_cast<int>(string);
}

template<class T>
class TreeDeserializer {
public:
	explicit TreeDeserializer(const std::string& string) :
		string_(string)
	{}

	BinaryTree<T> deserialize() {
		size_t position = 0;
		const Node* root = readNode(position);
		builder_.setRoot(root);
		return builder_.build();
	}

private:
	typedef BTNode<T> Node;

	const Node* readNode(size_t& position) {
		if (position + 2 > string_.length()) {
			throw std::out_of_range("Position is too big");
		}

		if (string_[position] != TreeSerializer::TREE_STARTS_SYMBOL) {
			throw std::runtime_error(std::string("Unexpected node start symbol: ") + string_[position]);
		}
		++position;

		if (string_[position] == TreeSerializer::TREE_ENDS_SYMBOL) {
			++position;
			return nullptr;
		}
		const Node* left = readNode(position);
		size_t nextPosition = string_.find_first_of(TreeSerializer::TREE_STARTS_SYMBOL, position);
		T data = ::deserialize<T>(string_.substr(position, nextPosition - position));
		position = nextPosition;
		const Node* right = readNode(position);

		if (string_[position] != TreeSerializer::TREE_ENDS_SYMBOL) {
			throw std::runtime_error(std::string("Unexpected node end symbol: ") + string_[position]);
		}
		++position;

		return builder_.addNode(data, left, right);
	}

	BinaryTreeBuilder<T> builder_;
	std::string string_;
};

template<class T>
BinaryTree<T> deserializeTree(const std::string& string) {
	return TreeDeserializer<T>(string).deserialize();
}


template<class T>
void printInOrderTraversal(const BinaryTree<T>& tree, std::ostream& os = std::cout) {
	printInOrderTraversal(tree.root(), os);
}

template<class T>
void printInOrderTraversal(const BTNode<T>* node, std::ostream& os) {
	if (!node) {
		return;
	}
	printInOrderTraversal(node->left(), os);
	os << node->data() << " ";
	printInOrderTraversal(node->right(), os);
}


BinaryTree<int> generateTree() {
	BinaryTreeBuilder<int> builder;
	auto leaf1 = builder.addNode(5);
	auto leaf2 = builder.addNode(6);
	auto leaf3 = builder.addNode(7);
	auto next1 = builder.addNode(3, leaf1, leaf2);
	auto next2 = builder.addNode(4, nullptr, leaf3);
	auto next3 = builder.addNode(2, next1, next2);
	auto root = builder.addNode(1, next3);
	builder.setRoot(root);
	return builder.build();
}

int main() {
	auto tree = generateTree();
	printInOrderTraversal(tree);
	std::cout << std::endl;

	std::string serialized = serialize(tree);
	std::cout << serialized << std::endl;

	auto deserialized = deserializeTree<int>(serialized);
	printInOrderTraversal(deserialized);
	std::cout << std::endl;

	return 0;
}
#if !defined SUFFIXTREE_H_INCLUDED
#define SUFFIXTREE_H_INCLUDED 1

#include <list>
#include <string>
#include <vector>

typedef long long t_int;
struct Vertex;
class SuffixTree;
//class CompareVertices;


struct Vertex {
	enum VertexType {VT_ROOT, VT_INTERN, VT_LEAF};
	static const int LEAF_INDEX = -1;

	std::list<Vertex*> children;
	Vertex() {
		children.clear();
		suffixLink = NULL;
		parent = NULL;
	};
	Vertex* suffixLink;
	Vertex* parent;
	int leftIndex;
	int rightIndex;
	char firstChar;
};


class SuffixTree {
public:
	SuffixTree (const std::string& _treeString);
	~SuffixTree();
	t_int getNumberOfSubstrings() const;
	int getSubstringPosition (const std::string& substring) const;
	std::string getBWTstring ();

private:
	enum ExtensionRule {ER_NONE, ER_LEAF, ER_NEW_VERTEX, ER_EXISTING_VERTEX};
	
	t_int getNumberOfPrefixes (const Vertex* vertex) const;
	void sortAllChildren ();
	void sortChildrenInVertex (Vertex* vertex);
	std::string getCharsWithPosition (const Vertex* vertex, int position) const;

	void destructVertex (Vertex* vertex);
	Vertex* findChildWithChar (const Vertex* vertex, char ch) const;
	// positionOnTheEdge = 0 means exactly in the vertex edgeEnd
	void proceedString (Vertex*& edgeEnd, int& positionOnTheEdge,
						int stringLeftIndex, int stringRightIndex) const;
	void splitEdge (Vertex*& edgeEnd, int positionOnTheEdge, int phaseNumber) const;
	void createLeafVertex (Vertex*& newVertex, Vertex* parentVertex, int phaseNumber) const;
	void performFirstPhase (Vertex*& firstSon);


	Vertex* root_;
	std::string treeString_;
};

#endif
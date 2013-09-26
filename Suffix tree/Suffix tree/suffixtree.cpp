#include "suffixtree.h"

Vertex::VertexType getVertexType (const Vertex* vertex) {
	if (vertex->parent == NULL)
		return Vertex::VT_ROOT;
	else if (vertex->rightIndex == -1)
		return Vertex::VT_LEAF;
	else
		return Vertex::VT_INTERN;
}





Vertex* SuffixTree::findChildWithChar (const Vertex* vertex, char neededChar) const {
bool foundNeededChild = false;
Vertex* foundChild;
for (std::list<Vertex*>::const_iterator iChild = vertex->children.begin();
	 iChild != vertex->children.end() && (!foundNeededChild); ++iChild) {
		 Vertex* currentChild = *iChild;
		 char firstChildChar = treeString_[currentChild->leftIndex];
		 if (firstChildChar == neededChar) {
			 foundNeededChild = true;
			 foundChild = currentChild;
		 }
}

if (foundNeededChild)
	return foundChild;
else
	return NULL;
}

void SuffixTree::proceedString (Vertex*& edgeEnd, int& positionOnTheEdge,
								int stringLeftIndex, int stringRightIndex) const {
	
	if (stringLeftIndex > stringRightIndex)
		return;

	int stringLength = stringRightIndex - stringLeftIndex + 1;
	bool finished = false;

	if (positionOnTheEdge == 0) {
		edgeEnd = findChildWithChar (edgeEnd, treeString_[stringLeftIndex]);
		positionOnTheEdge = 1;		
		++stringLeftIndex;
		--stringLength;
	}

	if (stringLeftIndex > stringRightIndex)
		finished = true;
	if (!finished) {
		int remainingEdgeLength = edgeEnd->rightIndex - edgeEnd->leftIndex + 1 - positionOnTheEdge;
		if (remainingEdgeLength >= stringLength || edgeEnd->rightIndex == Vertex::LEAF_INDEX) {
			positionOnTheEdge += stringLength;
			finished = true;
		}
		else {
			positionOnTheEdge = 0;
			stringLeftIndex += remainingEdgeLength;
		}
	}

	if (!finished)
		proceedString (edgeEnd, positionOnTheEdge, stringLeftIndex, stringRightIndex);
	if (positionOnTheEdge == edgeEnd->rightIndex - edgeEnd->leftIndex + 1)
		positionOnTheEdge = 0;
}




void SuffixTree::splitEdge (Vertex*& edgeEnd, int positionOnTheEdge, int phaseNumber) const{
	Vertex* newEdgeVertex = new Vertex();
	Vertex* newLeafVertex = new Vertex();
	newLeafVertex->children.clear();
	newLeafVertex->leftIndex = phaseNumber;
	newLeafVertex->rightIndex = Vertex::LEAF_INDEX;
	newLeafVertex->parent = newEdgeVertex;
	newLeafVertex->firstChar = treeString_[newLeafVertex->leftIndex];

	Vertex* edgeEndParent = edgeEnd->parent;

	newEdgeVertex->children.clear();
	newEdgeVertex->children.push_back (edgeEnd);
	newEdgeVertex->children.push_back (newLeafVertex);
	newEdgeVertex->leftIndex = edgeEnd->leftIndex;
	newEdgeVertex->rightIndex = newEdgeVertex->leftIndex + positionOnTheEdge - 1;
	newEdgeVertex->parent = edgeEndParent;
	newEdgeVertex->firstChar = treeString_[newEdgeVertex->leftIndex];

	if (newEdgeVertex->parent == root_ && newEdgeVertex->leftIndex == newEdgeVertex->rightIndex)
		newEdgeVertex->suffixLink = root_;

	for (std::list<Vertex*>::iterator child = edgeEndParent->children.begin(); 
		child != edgeEndParent->children.end(); ++child) {
			if (*child == edgeEnd)
				*child = newEdgeVertex;
		}


	edgeEnd->leftIndex += positionOnTheEdge;
	edgeEnd->parent = newEdgeVertex;

	edgeEnd = newEdgeVertex;
}


void SuffixTree::createLeafVertex (Vertex*& newVertex, Vertex* parentVertex, int phaseNumber) const {
	newVertex = new Vertex();
	newVertex->children.clear();
	newVertex->leftIndex = phaseNumber;
	newVertex->rightIndex = Vertex::LEAF_INDEX;
	newVertex->parent = parentVertex;
	newVertex->firstChar = treeString_[newVertex->leftIndex];
	parentVertex->children.push_back (newVertex);
}

void SuffixTree::performFirstPhase (Vertex*& firstSon) {
	root_ = new Vertex();
	root_->parent = NULL;

	firstSon = new Vertex();
	createLeafVertex (firstSon, root_, 0);
}


SuffixTree::SuffixTree (const std::string& _treeString) {
	treeString_ = _treeString;
	const int length = treeString_.length();

	Vertex* currentVertex;
	performFirstPhase (currentVertex);

	int currentExtension = 1;
	int finishedPosition = 0;

	for (int iPhase = 1; iPhase < length; ++iPhase) {
		ExtensionRule ruleUsed = ER_NONE;
		--currentExtension;
		Vertex* lastInternalVertexCreated = NULL;
		while (ruleUsed != ER_EXISTING_VERTEX && currentExtension <= iPhase) {
			Vertex* vertexWalk = currentVertex;
			int positionOnTheEdge = 0;

			if (ruleUsed == ER_NONE) {
				// first extension of the phase, no need to move
				positionOnTheEdge = finishedPosition;
			}
			else {
				if (vertexWalk->suffixLink != NULL) {
					// there is a suffix link from the current vertex
					vertexWalk = vertexWalk->suffixLink;
				}
				else {
					// first we lift up one vertex remembering the string passed
					int proceedStringLeftIndex, proceedStringRightIndex;
					proceedStringLeftIndex = vertexWalk->leftIndex;
					proceedStringRightIndex = vertexWalk->rightIndex;
					vertexWalk = vertexWalk->parent;
					if (proceedStringRightIndex == Vertex::LEAF_INDEX)
						proceedStringRightIndex = iPhase - 1;
					if (proceedStringRightIndex - proceedStringLeftIndex > iPhase - 1 - currentExtension) {
						// we should not pass more than we need
						proceedStringLeftIndex = currentExtension;
						proceedStringRightIndex = iPhase - 1;
					}
					
					// if we are not in the root then there must be a suffix link to walk
					if (getVertexType (vertexWalk) != Vertex::VT_ROOT) 
						vertexWalk = vertexWalk->suffixLink;

					positionOnTheEdge = 0;
					proceedString (vertexWalk, positionOnTheEdge, proceedStringLeftIndex, proceedStringRightIndex);
				}
			}

			if (positionOnTheEdge != 0) {
				// we are in the middle of an edge
				char nextEdgeChar = treeString_ [vertexWalk->leftIndex + positionOnTheEdge];
				if (nextEdgeChar == treeString_ [iPhase]) {
					// the next char is equal to one we are adding
					finishedPosition = positionOnTheEdge + 1;
					ruleUsed = ER_EXISTING_VERTEX;
				}
				else {
					// the next char differes from one we are adding, need to split
					splitEdge (vertexWalk, positionOnTheEdge, iPhase);

					if (lastInternalVertexCreated != NULL)
						lastInternalVertexCreated->suffixLink = vertexWalk;
					lastInternalVertexCreated = vertexWalk;
					ruleUsed = ER_NEW_VERTEX;
				}
			}
			else {
				// we are right in the vertex
				if (getVertexType (vertexWalk) == Vertex::VT_LEAF) {
					// we are in a leaf, hence nothing to do
					ruleUsed = ER_LEAF;
				}
				else {
					// we are in a non-leaf vertex
					if (lastInternalVertexCreated != NULL)
						lastInternalVertexCreated->suffixLink = vertexWalk;

					Vertex* neededVertex = findChildWithChar (vertexWalk, treeString_[iPhase]);
					if (neededVertex != NULL) {
						// vertex with the next char already existst
						finishedPosition = 1;
						ruleUsed = ER_EXISTING_VERTEX;
					}
					else {
						// no vertex with the next char, need to create it
						createLeafVertex (neededVertex, vertexWalk, iPhase);

						finishedPosition = 0;
						ruleUsed = ER_NEW_VERTEX;
					}
					vertexWalk = neededVertex;
				}
				lastInternalVertexCreated = NULL;
			}
			// if we reach the end of the edge we mark it with position = 0
			if (finishedPosition == vertexWalk->rightIndex - vertexWalk->leftIndex + 1)
				finishedPosition = 0;
			++currentExtension;
			currentVertex = vertexWalk;
		}
	}
}

int SuffixTree::getSubstringPosition (const std::string& substring) const {
	int substringPosition = 0;
	int positionOnTheEdge = 0;
	Vertex* vertexWalk = root_;
	for (int i = 0; i < (int)substring.length() && substringPosition != -1; ++i) {
		if (positionOnTheEdge == 0) {
			Vertex* neededChild = findChildWithChar (vertexWalk, substring[i]);
			if (neededChild != NULL)
				vertexWalk = neededChild;
			else
				substringPosition = -1;
		}
		if (substringPosition != -1) {
			if (treeString_ [vertexWalk->leftIndex + positionOnTheEdge] == substring[i]) {
				++positionOnTheEdge;
				if (vertexWalk->rightIndex >=0 &&
					vertexWalk->leftIndex + positionOnTheEdge > vertexWalk->rightIndex)
						positionOnTheEdge = 0;
			}
			else
				substringPosition = -1;
		}
	}
	if (substringPosition == 0)
		substringPosition = vertexWalk->leftIndex + positionOnTheEdge - 1 - substring.length() + 1;
	return substringPosition;
}

void SuffixTree::destructVertex (Vertex *vertex) {
	for (std::list<Vertex*>::iterator child = vertex->children.begin();
		child != vertex->children.end(); ++child) {
		destructVertex (*child);
	}
	vertex->children.clear();
	delete vertex;
}

SuffixTree::~SuffixTree() {
	destructVertex (root_);
}

t_int SuffixTree::getNumberOfPrefixes (const Vertex* vertex) const {
	t_int answer = 0;
	if (getVertexType (vertex) == Vertex::VT_LEAF) 
		answer = (t_int)(treeString_.length() - vertex->leftIndex);
	else
	{
		for (std::list<Vertex*>::const_iterator child = vertex->children.begin();
			child != vertex->children.end(); ++child) {
				if (getVertexType(*child) == Vertex::VT_INTERN)
					answer += (t_int)((*child)->rightIndex - (*child)->leftIndex + 1);
				answer += getNumberOfPrefixes (*child);
		}
	}
	return answer;
}

t_int SuffixTree::getNumberOfSubstrings() const {
	return getNumberOfPrefixes (root_);
}
std::string SuffixTree::getBWTstring () {
	const int baseStringLength = (int)treeString_.length() / 2;
	sortAllChildren();
	std::string firstBWTstring = getCharsWithPosition (root_, baseStringLength - 1);
	std::string answer = "";
	const int firstBWTstringLength = (int) firstBWTstring.length();
	for (int i = 0; i < firstBWTstringLength; ++i)
		for (int repeat = 0; repeat < baseStringLength / firstBWTstringLength; ++repeat)
			answer += firstBWTstring[i];
	return answer;
}

void SuffixTree::sortAllChildren () {
	sortChildrenInVertex (root_);
}

bool CompareVertices (const Vertex* v1, const Vertex* v2) {
	if (getVertexType (v1) != Vertex::VT_ROOT && getVertexType (v2) != Vertex::VT_ROOT)
		return (v1->firstChar < v2->firstChar);
	else
		return false;
}

void SuffixTree::sortChildrenInVertex (Vertex* vertex) {
	vertex->children.sort (CompareVertices); 

	for (std::list<Vertex*>::iterator child = vertex->children.begin();
		child != vertex->children.end(); ++child)
			sortChildrenInVertex (*child);

}

std::string SuffixTree::getCharsWithPosition (const Vertex* vertex, int position) const {
	const int treeStringLength = (int) treeString_.length();
	std::string answer = "";
	for (std::list<Vertex*>::const_iterator itChild = vertex->children.begin();
		itChild != vertex->children.end(); ++itChild) {
			Vertex* child = *itChild;
			int edgeLength;
			if (child->rightIndex != Vertex::LEAF_INDEX)
				edgeLength = child->rightIndex - child->leftIndex + 1;
			else
				edgeLength = treeStringLength - child->leftIndex;
			if (position < edgeLength)
				answer += treeString_[child->leftIndex + position];
			else
				answer += getCharsWithPosition (child, position - edgeLength);
	}
	return answer;
}
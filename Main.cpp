/*Things I need to do
Get createEdge to work properly
get Prim's alg to work properly
*/
//#include<unistd.h>//usleep(microseconds)
#include<iostream>
#include<vector>
#include<SFML/Graphics.hpp>
#include<cmath>

//This cannot be greater than the maximum of the monitor else weird things start to happen.
const int width = 1920;
const int height = 1080;

// Class for the nodes which the user can place on the screen
class Node {
public:
	
	Node(sf::Vector2i mousePos);
	sf::CircleShape getShape();
	sf::Vector2f getPosition();
private:
	//Shape which represents the node
	sf::CircleShape nodeShape;
	//Its position in px
	sf::Vector2i nodePos;
};

//Returns the position of the node
sf::Vector2f Node::getPosition() {
	return static_cast<sf::Vector2f>(this->nodePos);
}
//Creates the node 
Node::Node(sf::Vector2i mousePos) {
	this->nodePos = mousePos;
	this->nodeShape.setOrigin(10, 10);
	this->nodeShape.setPosition(this->nodePos.x, this->nodePos.y);
	this->nodeShape.setFillColor(sf::Color(255, 255, 255));
	this->nodeShape.setRadius(10);
}
//Returns the shape of the node to be drawn
sf::CircleShape Node::getShape() {
	return this->nodeShape;
}
std::vector<Node> nodeArray;
std::vector<sf::VertexArray>edgeArray;
std::vector<std::vector<double > > distanceMatrix;//n*n matrix

//Creates an edge which will be pushed to the edgeArray vector
void createEdge(sf::Vector2f node1Pos, sf::Vector2f node2Pos) {
	sf::VertexArray newEdge(sf::Lines, 2);
	newEdge[0].position = node1Pos;
	newEdge[1].position = node2Pos;
	newEdge[0].color = sf::Color::Red;
	newEdge[1].color = sf::Color::Red;
	edgeArray.push_back(newEdge);

}
//Creates the distance matrix which will be required for the alg
void createDistanceMatrix(sf::RenderWindow& window) {
	distanceMatrix.clear();
	sf::Vector2f node1, node2;
	sf::RenderWindow& windowRef = window;
	int dy, dx;
	double distBetweenNodes;
	for (int itr1 = 0; itr1<nodeArray.size(); itr1++) {
		std::vector<double> newRow;
		for (int itr2 = 0; itr2<nodeArray.size(); itr2++) {
			node1 = nodeArray[itr1].getPosition();
			node2 = nodeArray[itr2].getPosition();
			dy = std::abs(node1.y - node2.y);
			dx = std::abs(node1.x - node2.x);
			distBetweenNodes = sqrt(pow(dx, 2) + pow(dy, 2));
			//For whatever reason I cannot replace the lines below with edgeArray.push_back(node1,node2) even though it does the same thing!
			sf::VertexArray newEdge(sf::Lines, 2);
			newEdge[0].position = node1;
			newEdge[1].position = node2;
			newEdge[0].color = sf::Color::Red;
			newEdge[1].color = sf::Color::Red;
			newRow.push_back(distBetweenNodes);
			edgeArray.push_back(newEdge);
			for (int itr5 = 0; itr5<edgeArray.size(); itr5++) {
				windowRef.draw(edgeArray[itr5]);
			}
		}
		distanceMatrix.push_back(newRow);
		newRow.clear();
		windowRef.display();
		_sleep(100);// usleep for GNU/UNIX systems
		edgeArray.clear();
	}
}
//Prints the distance Matrix
void printMatrix(std::vector < std::vector < double > > matrix) {
	for (int row = 0; row < matrix.size(); row++) {
		for (int col = 0; col < matrix.size(); col++) {
			if (col == nodeArray.size() - 1) {
				std::cout << matrix[row][col] << '\n';
			}
			else {
				std::cout << matrix[row][col]<< '\t';
			}
		}
	}
}

// The algorithm for prim's with a distance matrix.
void prims() {
	edgeArray.clear();
	sf::Vector2f node1, node2;
	double smallestNum = sqrt(pow(height, 2) + pow(width, 2)); //The maximum distance between two nodes
	int smallesNumPos=0;
	std::vector<int> allowedCol;// A list of all of the columns that can be searched
	allowedCol.push_back(0);
	for (int itr = 0; itr < nodeArray.size()  ;itr++) {
		double smallestNum = (width*height) + 1; //Should be sqrt(width^2+height^2)
		int smallesNumPos = 0;
		int previousNode =0;
		printMatrix(distanceMatrix);
		//Search matrix for smallest num
		for (int col = 0; col < allowedCol.size(); col++) {
			for (int row = 0; row < distanceMatrix[0].size(); row++) {
				if (distanceMatrix[row][allowedCol[col]] < smallestNum && distanceMatrix[row][allowedCol[col]] != 0) {
					smallestNum = distanceMatrix[row][allowedCol[col]];
					smallesNumPos = row;
					previousNode = col;
				}
			}
		}
		//Draw Line between nodes
		node1 = nodeArray[previousNode].getPosition();
		node2 = nodeArray[smallesNumPos].getPosition();
		createEdge(node1,node2);
		allowedCol.push_back(smallesNumPos);
		for (int col2 = 0; col2 < distanceMatrix.size(); col2++) {
			distanceMatrix[smallesNumPos][col2] = 0;
		}
	}
}


int main() {
	sf::RenderWindow window(sf::VideoMode(width, height), "TITLE");
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed: {
				Node newNode(sf::Mouse::getPosition(window));
				nodeArray.push_back(newNode);
				break;
			}
			case sf::Event::KeyPressed:
				createDistanceMatrix(window);
				prims();
				break;
			}
		}
		window.clear();
		for (int itr = 0; itr<nodeArray.size(); itr++) {
			window.draw(nodeArray[itr].getShape());
		}
		for (int itr3 = 0; itr3<edgeArray.size(); itr3++) {
			window.draw(edgeArray[itr3]);
		}
		window.display();
	}
	return 0;
}


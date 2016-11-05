/*Things I need to add/fix
+The actual algs
*/
//#include<unistd.h>//usleep(microseconds)
#include<iostream>
#include<vector>
#include<SFML/Graphics.hpp>
#include<cmath>
const int width = 1920	;
const int height = 1080;
class Node {
  public:
	  sf::CircleShape nodeShape;
	  Node(sf::Vector2i mousePos);
	  sf::Vector2f getPosition();
  private:
	  sf::Vector2i nodePos;
 };

sf::Vector2f Node::getPosition() {
	return static_cast<sf::Vector2f>(this->nodePos);
}

Node::Node(sf::Vector2i mousePos) {
    this->nodePos = mousePos;
	  this->nodeShape.setOrigin(10, 10);
	  this->nodeShape.setPosition(this->nodePos.x, this->nodePos.y);
	  this->nodeShape.setFillColor(sf::Color(255, 255, 255));
	  this->nodeShape.setRadius(10);
}
std::vector<Node> nodeArray;
std::vector<sf::VertexArray>edgeArray;
std::vector<std::vector<double > > distanceMatrix;

void createDistanceMatrix(sf::RenderWindow& window) {
	  sf::Vector2f node1, node2;
	  sf::RenderWindow& windowRef=window;
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
			      sf::VertexArray newEdge(sf::Lines, 2);
			      newEdge[0].position = node1;
			      newEdge[1].position = node2;
			      newEdge[0].color = sf::Color::Red;
			      newEdge[1].color = sf::Color::Red;
			      newRow.push_back(distBetweenNodes);
			      edgeArray.push_back(newEdge);
			      if (itr2 == nodeArray.size() - 1) {
				std::cout << distBetweenNodes << '\n';
			}else {
				std::cout << distBetweenNodes << '\t';
			}
			for (int itr5 = 0; itr5<edgeArray.size(); itr5++) {
				windowRef.draw(edgeArray[itr5]);
			}
		}
		windowRef.display();
		_sleep(100);// usleep for GNU/UNIX systems
		edgeArray.clear();  
	}
}

void primms() {
	sf::Vector2f node1, node2;
	double smallestNum=0;
	int smallesNumPos;
	std::vector<int> allowedCol(0);// Start from the first node
	while (distanceMatrix.size() > 0) { // Should be n-1
		for (int itr5 = 0; itr5 < allowedCol.size(); itr5++) {
			//Find the smalles element in the allowed cols and then make that the next 
			//Need to find the path from the distance matrix
			sf::VertexArray newEdge(sf::Lines, 2);

			newEdge[0].position = sf::Vector2f(node1);
			newEdge[1].position = sf::Vector2f(node2);
			edgeArray.push_back(newEdge);
		}
		//distanceMatrix.erase(0); Remove the row with the position of node1
		smallesNumPos = 0;
		smallestNum = 0;
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
				                  break;
			            }
		        }

		   window.clear();
		   for (int itr = 0; itr<nodeArray.size(); itr++) {
			    window.draw(nodeArray[itr].nodeShape);
		    }
		   for (int itr3 = 0; itr3<edgeArray.size(); itr3++) {
			    window.draw(edgeArray[itr3]);
		    }
		    //Draw nodes&edges and whatever else.
		    window.display();
	}
	return 0;
}


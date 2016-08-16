#include <iostream>
#include <vector>
#include <string>

#include <random>
#include <functional>

#include <SFML/Graphics.hpp>


using namespace std;

vector<uint8_t> dangerToRGB(uint8_t danger){
	vector<uint8_t> rgb (3); //{red, green, blue}
	
	//Convert 0->255 danger value to green(0) -> yellow (127.5) -> red(255) color
	//here blue is always null
	rgb[2]=0;
	//Are we in green shape ?
	if(danger < 127.5){
		rgb[1] = 255; //Green is max
		rgb[0] = 255*danger/127; //Red increase with danger
	}
	else{ //We re in red shape
		rgb[0] = 255; //Red is max
		rgb[1] = 255 - 255 * (danger-128)/127; //Green descrease with danger
	}

	return rgb;
}

void syncMap(vector< vector<uint8_t> >& map){
	const size_t size_x = map.size();
	const size_t size_y = map[0].size();

	vector< vector<uint64_t> > mean_neighbours (size_x, vector<uint64_t>(size_y, 0));
	
	//Get neighbours values for all cases
	for(size_t x = 0; x < size_x; x++){
		for(size_t y = 0; y < size_y; y++){
			//in middle
			if(x > 0 and x < size_x-1 and y > 0 and y < size_y-1){
				mean_neighbours[x][y] += map[x-1][y-1];
				mean_neighbours[x][y] += map[x-1][y];
				mean_neighbours[x][y] += map[x-1][y+1];
				mean_neighbours[x][y] += map[x][y-1];
				mean_neighbours[x][y] += map[x][y+1];
				mean_neighbours[x][y] += map[x+1][y-1];
				mean_neighbours[x][y] += map[x+1][y];
				mean_neighbours[x][y] += map[x+1][y+1];
				mean_neighbours[x][y] /= 8;
			}
			//Corner Up Left
			else if(x ==0 and y==0){
				mean_neighbours[x][y] += map[x][y+1];
				mean_neighbours[x][y] += map[x+1][y];
				mean_neighbours[x][y] += map[x+1][y+1];
				mean_neighbours[x][y] /= 3;
			}
			//Corner Down Left
			else if(x ==0 and y==size_y-1){
				mean_neighbours[x][y] += map[x][y-11];
				mean_neighbours[x][y] += map[x+1][y];
				mean_neighbours[x][y] += map[x+1][y-1];
				mean_neighbours[x][y] /= 3;
			}
			//Corner Up Right
			else if(x ==size_x-1 and y==0){
				mean_neighbours[x][y] += map[x][y+1];
				mean_neighbours[x][y] += map[x-1][y];
				mean_neighbours[x][y] += map[x-1][y+1];
				mean_neighbours[x][y] /= 3;
			}
			//Corner Down right
			else if(x ==size_x-1 and y==size_y-1){
				mean_neighbours[x][y] += map[x][y-1];
				mean_neighbours[x][y] += map[x-1][y];
				mean_neighbours[x][y] += map[x-1][y-1];
				mean_neighbours[x][y] /= 3;
			}
			//Border Left
			else if(x == 0){
				mean_neighbours[x][y] += map[x][y-1];
				mean_neighbours[x][y] += map[x][y+1];
				mean_neighbours[x][y] += map[x+1][y-1];
				mean_neighbours[x][y] += map[x+1][y];
				mean_neighbours[x][y] += map[x+1][y+1];
				mean_neighbours[x][y] /= 5;
			}
			//Border Right
			else if(x == size_x-1){
				mean_neighbours[x][y] += map[x][y-1];
				mean_neighbours[x][y] += map[x][y+1];
				mean_neighbours[x][y] += map[x-1][y-1];
				mean_neighbours[x][y] += map[x-1][y];
				mean_neighbours[x][y] += map[x-1][y+1];
				mean_neighbours[x][y] /= 5;
			}
			//Border Up
			else if(y == 0){
				mean_neighbours[x][y] += map[x-1][y];
				mean_neighbours[x][y] += map[x-1][y+1];
				mean_neighbours[x][y] += map[x][y+1];
				mean_neighbours[x][y] += map[x+1][y];
				mean_neighbours[x][y] += map[x+1][y+1];
				mean_neighbours[x][y] /= 5;
			}
			//Border Down
			else if(y == size_y-1){
				mean_neighbours[x][y] += map[x-1][y];
				mean_neighbours[x][y] += map[x-1][y-1];
				mean_neighbours[x][y] += map[x][y-1];
				mean_neighbours[x][y] += map[x+1][y];
				mean_neighbours[x][y] += map[x+1][y-1];
				mean_neighbours[x][y] /= 5;
			}
		}
	}
	
	//Reduce divisors (stronger it is, weaker will be the edits)
	//uint8_t d = 1;
	//Edit values of cases (based on neighbours at the same time !)
	for(size_t x=0; x < size_x; x++){
		for(size_t y=0; y < size_y; y++){
			/*
			if(mean_neighbours[x][y]-1 > map[x][y]){ map[x][y] += (mean_neighbours[x][y]-map[x][y])/d;}
			else if(mean_neighbours[x][y]+1 < map[x][y]){map[x][y] -= (map[x][y]-mean_neighbours[x][y])/d;}
			*/
			if(mean_neighbours[x][y]-1 > map[x][y]){ map[x][y] += 1;}
			else if(mean_neighbours[x][y]+1 < map[x][y]){map[x][y] -= 1;}
		}
	}
}

int main(){
	const size_t size_x = 200;
	const size_t size_y = 200;
	vector< vector<uint8_t> > map (size_x, vector<uint8_t>(size_y, 127)); //Initialize all neutral
	
	//Paramètres d'affichage de la map
	const size_t mapThickness=2;
	int startX=10;
	int startY=10;
	size_t resX=460;
	size_t resY=500;
	
	//Initialisation de la map
	//Random
	random_device rd;
	default_random_engine generator(rd());
	uniform_int_distribution<uint8_t> distribution(0,255);
	auto rg = bind(distribution, generator);
	for(size_t x=0; x < size_x; x++){
		for(size_t y=0; y < size_y; y++){
			map[x][y] = rg();
		}
	}

	/*
	//Zone sure
	for(size_t x = 100; x < 120; x++){
		for(size_t y= 100; y< 120; y++){
			map[x][y] = 0;
		}
	}
	
	//Zone dangeureuse
	for(size_t x = 10; x < 45; x++){
		for(size_t y= 20; y < 60; y++){
			map[x][y] = 255;
		}
	}

	//Zone safe entourée de danger
	for(size_t x = 100; x<120; x++){
		for(size_t y=20; y < 60; y++){
			if(x > 108 and x < 112 and y > 35 and y < 45){map[x][y] = 0;}
			else{map[x][y]=255;}
		}
	}

	//Confrontation
	for(size_t x=10; x < 60; x ++){
		for(size_t y=100; y<150; y++){
			if(x <= 35){map[x][y] = 255;}
			else{map[x][y]=0;}

		}

	}
	*/
	



	//démarrage du chrono
	sf::Clock clock;

	//Préparation du text
	sf::Font font;
	if (!font.loadFromFile("/usr/share/fonts/oiio/DroidSerif.ttf")){
	    //cerr << "Impossible to load arial.ttf" << endl;
	    return 1;
	}
	sf::Text text;
	size_t ftSize=24;
	text.setFont(font);
	text.setCharacterSize(ftSize);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Blue);
	text.setOutlineThickness(0.8);
	text.setPosition(10,resY-ftSize-10);

	// création de la fenêtre
	sf::RenderWindow window(sf::VideoMode(resX, resY), "My window", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	// on fait tourner le programme tant que la fenêtre n'a pas été fermée
	while (window.isOpen()){
		// on traite tous les évènements de la fenêtre qui ont été générés depuis la dernière itération de la boucle
		sf::Event event;
		while (window.pollEvent(event)){
			// fermeture de la fenêtre lorsque l'utilisateur le souhaite
			if (event.type == sf::Event::Closed){window.close();}
		}

		// effacement de la fenêtre en noir
		window.clear(sf::Color::Black);

		//Draw the map
		sf::VertexArray dangerMap (sf::Quads, size_x*size_y*4);
		//Look for all squares (== pixels == elements of the maps)
		size_t e = 0;
		vector<uint8_t> rgb;
		for(size_t x = 0; x < size_x-1; x++){
			for(size_t y = 0; y < size_y-1; y++){
					rgb = dangerToRGB(map[x][y]);
					sf::Color c (rgb[0], rgb[1], rgb[2]);
					
					//Up left
					dangerMap[e].position = sf::Vector2f((startX+x)*mapThickness, (startY+y)*mapThickness);
					dangerMap[e].color = c;
					e++; // next vertex

					//Up right
					dangerMap[e].position = sf::Vector2f((startX+x)*mapThickness, (startY+y+1)*mapThickness);
					dangerMap[e].color = c;
					e++; // next vertex
					
					//Down right
					dangerMap[e].position = sf::Vector2f((startX+x+1)*mapThickness, (startY+y+1)*mapThickness);
					dangerMap[e].color = c;
					e++; // next vertex
					
					//Down left
					dangerMap[e].position = sf::Vector2f((startX+x+1)*mapThickness, (startY+y)*mapThickness);
					dangerMap[e].color = c;
					e++; // next vertex
			}
		}
		//Draw it
		window.draw(dangerMap);

		
		//FPS measure
		sf::Time elapsed = clock.getElapsedTime();
		float fps = 1/elapsed.asSeconds();
		text.setString("FPS: "+to_string(fps));
		window.draw(text);
		clock.restart();


		// fin de la frame courante, affichage de tout ce qu'on a dessiné
		window.display();

		//Get mouse position, edition on the map
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			sf::Vector2i mpos = sf::Mouse::getPosition(window);
			size_t pos_x_map = (mpos.x / mapThickness)-startX;
			size_t pos_y_map = (mpos.y / mapThickness)-startY;
			if(pos_x_map < size_x and pos_y_map < size_y){map[pos_x_map][pos_y_map] = 0;}
		}
		else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
			sf::Vector2i mpos = sf::Mouse::getPosition(window);
			size_t pos_x_map = (mpos.x / mapThickness)-startX;
			size_t pos_y_map = (mpos.y / mapThickness)-startY;
			if(pos_x_map < size_x and pos_y_map < size_y){map[pos_x_map][pos_y_map] = 255;}
		}

		//Actualize the danger map
		syncMap(map);
	}

	return 0;
}

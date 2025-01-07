
#include <iostream>
#include <queue>
#include <stack>
#include <fstream>
#include <string> 
#include <sstream> 
#include <map> 
#include <vector> 
using namespace std;



class Item
{
public:
	string name;

	Item(string _name)
	{
		name = _name;
	}
};

class Entity
{
public:
	string name;
	string description;

	Entity(string _name, string _description)
	{
		name = _name;
		description = _description;
	}

};

class Inventory
{
public:
	vector<Item> items;

	void Add(Item item)
	{
		items.push_back(item);
		cout << "You add the " << item.name << endl;
	}

	bool Remove(string item)
	{
		for (auto it = begin(items); it != end(items); ++it)
		{
			if (it.operator*().name == item)
			{
				cout << "You remove the " << it.operator*().name << endl;
				items.erase(it);

				return true;
			}

		}
		cout << "You could not find the " << item << endl;
		return false;
	}

	bool View(string item)
	{
		for (auto it = begin(items); it != end(items); ++it)
		{
			if (it.operator*().name == item)
			{
				cout << "You look at the " << it.operator*().name << endl;
				return true;
			}

		}
		cout << "You could not find the " << item << endl;
		return false;
	}
};



class Location
{
public:
	string name;
	string description;
	map<string, int> connections;
	vector<Entity> entities;

	Location(string bits[3])
	{
		name = bits[0];
		description = bits[1];

		string connectionsString = bits[2];


		stringstream split(connectionsString);
		string temp;

		while (getline(split, temp, ';'))
		{
			string action = "";
			int dir = 0;

			for (int i = 0; i < temp.size(); i++)
			{
				if (temp[i] == '=')
				{
					action = temp.substr(0, i);
					dir = atoi(temp.substr(i + 1, temp.size()).c_str());
				}
			}

			connections.insert(pair<string, int>(action, dir));
		}


	}

	~Location()
	{
		entities.clear();
	}

};


class Adventure
{
public:
	string name;
	vector<Location*> locations;

	Adventure(string _name)
	{
		name = _name;
	}
	void Add(Location* location)
	{
		locations.push_back(location);
	}

	void Load(string name)
	{
		string text;
		ifstream file(name, ios::in);
		bool processEntities = false;

		while (getline(file, text))
		{

			string details[3];
			int pos = 0;
			string temp;
			stringstream split(text);

			if (!processEntities)
			{
				if (text[0] == '#')
				{
					processEntities = true;
				}
				else
				{

					while (getline(split, temp, ':'))
					{
						details[pos] = temp;
						pos++;
					}

					Location* location = new Location(details);
					Add(location);
				}
			}
			else
			{
				while (getline(split, temp, ':'))
				{
					details[pos] = temp;
					pos++;
				}

				

				stringstream split(details[2]);
				while (getline(split, temp, ';'))
				{
					Entity* entity = new Entity(details[0], details[1]);
					locations.at(atoi(temp.c_str()))->entities.push_back(*entity);
				}
			}


		}
	}

	~Adventure()
	{
		locations.clear();
	}

};



class Player
{
public:
	Inventory* inventory;
	Adventure* world;
	int pos = 0;
	bool playing;

	Player(Adventure* _world)
	{
		world = _world;
		inventory = new Inventory();
		playing = true;
	}


};





class Command
{
public:
	Player* player;



	Command(Player* _player)
	{
		player = _player;
	}

	virtual string Process(vector<string> args) = 0;
};


class MoveCommand : public Command
{
public:
	using Command::Command;

	string Process(vector<string> args) override
	{
		Location* playerLocation = player->world->locations[player->pos];

		string input;

		for (size_t i = 1; i < args.size(); i++)
		{
			input += args[i];
		}

		if (playerLocation->connections.count(input))
		{
			player->pos = playerLocation->connections[input];
		}
		else
		{
			return "You cannot go " + input;
		}

		
		return "You moved " + input + " to " + player->world->locations[player->pos]->name;
	}

	
};


class HelpCommand : public Command
{
public:
	map<string, Command*> cmds;
	
	HelpCommand(Player* _player, map<string, Command*> _cmds) : Command(_player), cmds(_cmds)
	{
		
	}

	string Process(vector<string> args) override
	{
		
		map<string, Command*>::iterator itr;
		
		string s = "";

		for (itr = cmds.begin(); itr != cmds.end(); ++itr)
		{
			
			s += itr.operator*().first + "\n";

			

		}


		return s;


	}

};


class InventoryCommand : public Command
{
public:
	using Command::Command;

	string Process(vector<string> args) override
	{
		string s = "You have: ";
		if (player->inventory->items.size() > 0)
		{
			for (size_t i = 0; i < player->inventory->items.size(); i++)
			{
				s += "\n- " + player->inventory->items.at(i).name;
			}
		}
		else
		{
			return "You have nothing.";
		}

		return s;
		
	}

};

class LookCommand : public Command
{
public:
	using Command::Command;

	string Process(vector<string> args) override
	{
		string s = "";
		if (args.size() == 1)
		{
			Location* playerLocation = player->world->locations[player->pos];
			s += "You are at " + playerLocation->name + ". " + playerLocation->description + "\n";


			s += "You can: \n";

			map<string, int>::iterator itr;
			for (itr = playerLocation->connections.begin(); itr != playerLocation->connections.end(); ++itr)
			{
				s += "go " + itr->first + "\n";
			}

			s += "\nYou see: \n";
			
			if (playerLocation->entities.size() > 0)
			{
				for (auto itr = begin(playerLocation->entities); itr != end(playerLocation->entities); ++itr)
				{
					s += " - " + itr->name + ". " + itr->description + "\n";
				}
			}
			else
			{
				s += "Nothing interesting nearby\n";
			}

		}
		else if (args.size() == 3)
		{
			Location* playerLocation = player->world->locations[player->pos];
			string lower = playerLocation->name;

			transform(lower.begin(), lower.end(), lower.begin(),
				[](unsigned char c) { return std::tolower(c); });

			

			if (args[2] == lower)
			{
				return playerLocation->description;
			}
			for (auto itr = begin(playerLocation->entities); itr != end(playerLocation->entities); ++itr)
			{
				string lower = itr->name;

				transform(lower.begin(), lower.end(), lower.begin(),
					[](unsigned char c) { return std::tolower(c); });


				if (args[2] == lower)
				{
					return itr->description;
				}
			}


			return "You can't see " + args[2];

		}
		else
		{
			return "You look at nothing";
		}

		return s;

	}

};

class CommandManager;

class AliasCommand : public Command
{
public:
	using Command::Command;
	CommandManager* manager;

	string Process(vector<string> args) override;


	

};

class QuitCommand : public Command
{
public:
	using Command::Command;
	CommandManager* manager;

	string Process(vector<string> args) override
	{
		player->playing = false;
		return "Thus ends another grand Zorkish adventure.";
	}




};


class DebugTree : public Command
{
public:
	using Command::Command;

	string Process(vector<string> args) override
	{
		string s = "--------- debug info ------------\n";
		
		for (size_t i = 0; i < player->world->locations.size(); i++)
		{
			Location* location = player->world->locations[i];

			s += "\nLocation " + to_string(i) + '\n';

			s += "name: " + location->name + '\n';
			s += "description: " + location->description + '\n';


			s += "\nEdges:\n";
			map<string, int>::iterator itr;
			for (itr = location->connections.begin(); itr != location->connections.end(); ++itr)
			{
				s += itr->first + ": " + to_string(itr->second) + " (" + player->world->locations[itr->second]->name + ")\n";
			}
			s += "\nEntities: \n";

			for (auto itr = begin(location->entities); itr != end(location->entities); ++itr)
			{
				s += " - " + itr->name + ". " + itr->description + "\n";
			}
			s += "\n----------------------";
		}

		return s;
		
	}




};

class Hiscore : public Command
{
public:
	using Command::Command;

	string Process(vector<string> args) override
	{
		player->playing = false;
		return "";
	}

};

class CommandManager
{
public:
	Player* player;
	map<string, Command*> cmds;

	CommandManager(Player* _player)
	{
		player = _player;

		MoveCommand* move = new MoveCommand(player);
		HelpCommand* help = new HelpCommand(player, cmds);
		InventoryCommand* inv = new InventoryCommand(player);
		LookCommand* look = new LookCommand(player);
		AliasCommand* alias = new AliasCommand(player);
		alias->manager = this;
		QuitCommand* quit = new QuitCommand(player);
		DebugTree* debug = new DebugTree(player);
		Hiscore* hiscore = new Hiscore(player);

		cmds.insert(pair<string, MoveCommand*>("move", move));
		cmds.insert(pair<string, Command*>("go", cmds["move"]));

		cmds.insert(pair<string, HelpCommand*>("help", help));

		cmds.insert(pair<string, InventoryCommand*>("inventory", inv));
		cmds.insert(pair<string, InventoryCommand*>("inv", inv));

		cmds.insert(pair<string, LookCommand*>("look", look));

		cmds.insert(pair<string, AliasCommand*>("alias", alias));

		cmds.insert(pair<string, QuitCommand*>("quit", quit));

		cmds.insert(pair<string, DebugTree*>("debug", debug));

		cmds.insert(pair<string, Hiscore*>("hiscore", hiscore));

		help->cmds = cmds;
	}

	string Process(string input)
	{
		vector<string> args;
		string temp;
		stringstream split(input);



		while (getline(split, temp, ' '))
		{
			args.push_back(temp);

		}


		if (args.empty())
		{
			return " ";
		}


		if (cmds.count(args[0]))
		{
			return cmds[args[0]]->Process(args);
		}
		else
		{
			return "Not a command";
		}



	}
};

string AliasCommand::Process(vector<string> args)
{
	if (args.size() == 3)
	{
		if (manager->cmds.count(args[1]))
		{
			manager->cmds.insert(pair<string, Command*>(args[2], manager->cmds[args[1]]));
			return "alias " + args[2] + " set for command " + args[1];
		}
		else
		{
			return "Not a command";
		}
	}

	return "use alias [command] [alias]";
}







class Game;


class State {
protected:
	Game* game = nullptr;
	
public:
	virtual void Update() = 0;
	virtual void Render() = 0;



	void DisplayHeader(string name)
	{
		cout << endl;
		cout << "Zorkish :: " << name << endl;
		cout << "-----------------------" << endl << endl;


	}
};




class Game {
private:
	queue<State*> states;
public:
	bool running() const {
		return !states.empty();
	}

	State* current() {
		return states.back();
	}

	void push_state(State* state) {
		states.push(state);
	}

	void pop_state() {
		delete states.front();
		states.pop();
	}

	~Game() {
		while (!states.empty())
			pop_state();
	}

	Game();

};



class MainMenu : public State
{
public:
	explicit MainMenu(Game* _game) {
		game = _game;
	}

	void Render() override
	{
		DisplayHeader("Main Menu");

		cout << "Welcome to Zorkish Adventures" << endl;

		cout << "1. Select Adventure and Play" << endl;
		cout << "2. Hall Of Fame" << endl;
		cout << "3. Help" << endl;
		cout << "4. About" << endl;
		cout << "5. Quit" << endl;

		cout << "Select 1 - 5: > ";
	}

	
	void Update() override;

};


class About : public State
{
public:
	explicit About(Game* _game) {
		game = _game;
	}

	void Render() override
	{
		DisplayHeader("About");

		cout << "Written by: Dean M" << endl;
		cout << "Press Enter to return to the Main Menu" << endl;

	}

	void Update() override
	{

		char temp;

		cin.ignore(10, '\n');
		cin.get();

		
		game->push_state(new MainMenu(game));
		game->pop_state();
		
	}
};


class Help : public State
{
public:
	explicit Help(Game* _game) {
		game = _game;
	}

	void Render() override
	{
		DisplayHeader("Help");

		cout << "The following commands are supported: " << endl;
		cout << "quit, hiscore" << endl;
		cout << "Press Enter to return to the Main Menu" << endl;
	}

	void Update() override
	{

		int input;

		cin.ignore(10, '\n');
		cin.get();

		game->push_state(new MainMenu(game));
		game->pop_state();
		
	}
};


class HallOfFame : public State
{
public:
	explicit HallOfFame(Game* _game) {
		game = _game;
	}

	void Render() override
	{
		DisplayHeader("Hall of Fame");

		cout << R""""(Top 10 Zorkish Adventure Champions 
 
			1. Fred, Mountain World, 5000
			2. Mary, Mountain World, 4000
			3. Joe, Water World, 3000
			4. Henry, Mountain World, 2000
			5. Susan, Mountain World, 1000
			6. Alfred, Water World, 900
			7. Clark, Mountain World, 800
			8. Harold, Mountain World, 500
			9. Julie, Water World, 300
			10. Bill, Box World, -5)"""" << endl;
		cout << "Press Enter to return to the Main Menu" << endl;
	}

	void Update() override
	{

		int input;

		cin.ignore(10, '\n');
		cin.get();

		game->push_state(new MainMenu(game));
		game->pop_state();
		
	}
};

class HighScore : public State
{
private:
	string world = "";
	int score = 0;
	int moves = 0;


public:
	explicit HighScore(Game* _game, string _world, int _score, int _moves) {
		game = _game;
		world = _world;
		score = _score;
		moves = _moves;
	}

	void Render() override
	{
		DisplayHeader("New High Score ");

		cout << "Congratulations!" << endl;
		cout << "You have made it to the Zorkish Hall Of Fame" << endl;
		cout << "Adventure: " << world << endl;
		cout << "Score: " << score << endl;
		cout << "Moves: " << moves << endl;

		cout << "Please type your name and press enter: " << endl;
		cout << ":> ";
	}

	void Update() override
	{

		string input;

		cin >> input;



		
		game->push_state(new HallOfFame(game));
		game->pop_state();




	}
};

class Gameplay : public State
{
public:
	Player* player;
	CommandManager* manager;

	explicit Gameplay(Game* _game, Adventure* _world) {
		game = _game;
		player = new Player(_world);
		manager = new CommandManager(player);
	}

	void Render() override
	{
		DisplayHeader("Gameplay");

		cout << "Welcome to Zorkish: " << player->world->name << endl;
		
	}

	void Update() override
	{
		
		string input;
		int moves = 0;


		while (player->playing)
		{
			cout << ":> ";
			getline(cin, input);

			transform(input.begin(), input.end(), input.begin(),
				[](unsigned char c) { return std::tolower(c); });





			cout << manager->Process(input) << endl;
			moves++;


		}
		

		if (input == "hiscore")
		{
			cout << "You have entered the magic word and will now see the “New High Score” screen." << endl;
			game->push_state(new HighScore(game, player->world->name, 0, moves-1));
			game->pop_state();
		}
		else
		{
			cout << "Your adventure has ended without fame or fortune." << endl;
			game->push_state(new MainMenu(game));
			game->pop_state();
		}

		
		



	}
};






class SelectAdventure : public State
{
public:
	explicit SelectAdventure(Game* _game) {
		game = _game;
	}

	void Render() override
	{
		DisplayHeader("Choose your adventure");

		cout << "1. Rock World" << endl;
		cout << "2. Za World" << endl;
		cout << "Select 1 - 2: > ";
		
	}

	void Update() override
	{
		
		string input;

		getline(cin, input);

		

		if (input == "1")
		{
			Adventure* adventure = new Adventure("Rock World");
			adventure->Load("world2.txt");
			game->push_state(new Gameplay(game, adventure));
			game->pop_state();
		}
		else
		{
			cout << "No world file" << endl;
			game->push_state(new SelectAdventure(game));
			game->pop_state();
		}
		
		
	}
};








Game::Game() {
	push_state(new MainMenu(this));
}

void MainMenu::Update()
{

	int input;

	cin >> input;

	if (input == 1)
	{
		game->push_state(new SelectAdventure(game));
		game->pop_state();
	}
	else if (input == 2)
	{
		game->push_state(new HallOfFame(game));
		game->pop_state();
		
	}
	else if (input == 3)
	{
		game->push_state(new Help(game));
		game->pop_state();
	}
	else if (input == 4)
	{
		
		game->push_state(new About(game));
		game->pop_state();
		
		
	}
	else
	{
		game->pop_state();
	}


	
	
}



int main()
{
	Game* game = new Game();



	
	while (game->running())
	{
		
		game->current()->Render();
		game->current()->Update();
		
		
	}

}
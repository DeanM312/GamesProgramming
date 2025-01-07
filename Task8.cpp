
#include <iostream>
#include <queue>
#include <stack>
using namespace std;



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
	string world = "";

	explicit Gameplay(Game* _game, string _world) {
		game = _game;
		world = _world;
	}

	void Render() override
	{
		DisplayHeader("Gameplay");

		cout << "Welcome to Zorkish: " << world << endl;
		cout << ":> ";
	}

	void Update() override
	{

		string input;

		cin >> input;

		if (input == "hiscore")
		{
			cout << "You have entered the magic word and will now see the “New High Score” screen." << endl;
			game->push_state(new HighScore(game, world, 0, 1));
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

		int input;

		cin >> input;

		

		if (input == 1)
		{
			game->push_state(new Gameplay(game, "Rock World"));
			game->pop_state();
		}
		else
		{
			game->push_state(new Gameplay(game, "Za World"));
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
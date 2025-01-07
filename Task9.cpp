
#include <iostream>
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


class Player
{
public:
	string name;
	Inventory* inventory;

	Player(string _name)
	{
		name = _name;
		inventory = new Inventory();
	}
};


int main()
{
	Player* player = new Player("Bob");

	Item* rock = new Item("rock");

	player->inventory->Add(*rock);

	player->inventory->View(rock->name);

	player->inventory->Remove(rock->name);

	player->inventory->View(rock->name);

}
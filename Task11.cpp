

#include <iostream>
#include <fstream>
#include <string> 
#include <sstream> 
#include <map> 
#include <vector> 

using namespace std;


class Location
{
public:
    string name;
    string description;
    map<string, int> connections;

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
                    dir = atoi(temp.substr(i+1, temp.size()).c_str());
                }
            }

            connections.insert(pair<string, int>(action, dir));
        }

      
    }

};


class World
{
public:
    vector<Location*> locations;


    void Add(Location* location)
    {
        locations.push_back(location);
    }


};

class Player
{
public:
    int pos = 0;

    Player(int _pos) : pos(_pos)
    {

    }
};



int main(int argc, char* argv[])
{
    ifstream file(argv[1], ios::in);


    string text;

    World* world = new World();
    

    // GRAPHING
    while (getline(file, text))
    {
        
        string details[3];
        int pos = 0;
        string temp;
        stringstream split(text);

        while (getline(split, temp, ':'))
        {
            details[pos] = temp;
            pos++;
        }

        Location* location = new Location(details);
        world->Add(location);
       

    }

    Player* player = new Player(0);

    // GO COMMAND
    while (true)
    {
        string input;
        Location* playerLocation = world->locations[player->pos];
        cout << "-------------------" << playerLocation->name << "-------------------" << endl;
        cout << playerLocation->description << endl << endl;

        cout << "You can:" << endl;
        map<string, int>::iterator itr;
        for (itr = playerLocation->connections.begin(); itr != playerLocation->connections.end(); ++itr)
        {
            cout << "go " << itr->first << " (" << world->locations[itr->second]->name << ")" << endl;
            cout << "|" << itr->first << "|" << endl;
        }

        cout << endl << "go: ";
        getline(cin, input);
        
        if (playerLocation->connections.count(input))
        {
            player->pos = playerLocation->connections[input];
        }

    }






    return 0;

}
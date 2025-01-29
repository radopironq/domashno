#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct game_card
{
    string name;
    int attack;
    int health;
    string description;
};

void write_cards(game_card *cards, int size, string &filename)
{
    ofstream file(filename);
    if (file.is_open())
    {
        file << size << endl;
        for (int a = 0; a < size; a++)
        {
            file << cards[a].name << ": " << cards[a].attack << " | " << cards[a].health << " - " << cards[a].description << endl;
        }
        file.close();
    }
}

game_card *read_cards(string &filename, int &size)
{
    ifstream file(filename);
    game_card *cards = nullptr;
    if (file.is_open())
    {
        file >> size;
        file.ignore();
        cards = new game_card[size];
        for (int a = 0; a < size; a++)
        {
            getline(file, cards[a].name, ':');
            file >> cards[a].attack;
            file.ignore(3);
            file >> cards[a].health;
            file.ignore(3);
            getline(file, cards[a].description);
        }
        file.close();
    }
    return cards;
}

void remove_card(string &name, string &filename)
{
    int size;
    game_card *cards = read_cards(filename, size);
    if (!cards)
        return;
    int newSize = size;
    for (int a = 0; a < size; a++)
    {
        if (cards[a].name == name)
        {
            newSize--;
        }
    }

    game_card *updated_cards = new game_card[newSize];
    int index = 0;
    for (int a = 0; a < size; a++)
    {
        if (cards[a].name != name)
        {
            updated_cards[index++] = cards[a];
        }
    }

    write_cards(updated_cards, newSize, filename);

    delete[] cards;
    delete[] updated_cards;
}

void add_card(game_card &card, string &filename)
{
    int size;
    game_card *cards = read_cards(filename, size);
    if (!cards)
    {
        size = 0;
        cards = new game_card[1];
    }
    else
    {
        game_card *temp = new game_card[size + 1];
        for (int a = 0; a < size; a++)
        {
            temp[a] = cards[a];
        }
        delete[] cards;
        cards = temp;
    }
    cards[size] = card;
    size++;

    write_cards(cards, size, filename);

    delete[] cards;
}

bool contains(string &str, string &substr)
{
    return str.find(substr) != string::npos;
}

void search_cards(string *keys, int num_keys, string &filename)
{
    int size;
    game_card *cards = read_cards(filename, size);
    if (!cards)
        return;
    for (int a = 0; a < size; a++)
    {
        bool found = false;
        for (int b = 0; b < num_keys; b++)
        {
            if (contains(cards[a].name, keys[b]) || contains(cards[a].description, keys[b]))
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            cout << cards[a].name << ": " << cards[a].attack << " | " << cards[a].health << " - " << cards[a].description << endl;
        }
    }
    delete[] cards;
}

int compare(game_card &card1, game_card &card2)
{
    if (card1.attack != card2.attack)
        return card1.attack > card2.attack ? 1 : -1;
    if (card1.health != card2.health)
        return card1.health > card2.health ? 1 : -1;
    if (card1.name != card2.name)
        return card1.name > card2.name ? 1 : -1;
    return card1.description.compare(card2.description);
}

void bubble_sort(game_card *cards, int n)
{
    for (int a = 0; a < n - 1; a++)
    {
        for (int b = 0; b < n - a - 1; b++)
        {
            if (compare(cards[b], cards[b + 1]) > 0)
            {
                swap(cards[b], cards[b + 1]);
            }
        }
    }
}

void sort_cards(string &filename)
{
    int size;
    game_card *cards = read_cards(filename, size);
    if (!cards)
        return;
    bubble_sort(cards, size);
    write_cards(cards, size, filename);
    delete[] cards;
}

int main()
{
    string filename = "./cards.txt";
    int choice = 0;
    while (choice != 6)
    {
        cout << "Menu:\n"
             << "1. Writes\n"
             << "2. Read\n"
             << "3. Remove\n"
             << "4. Search\n"
             << "5. Sort\n"
             << "6. Exit\n"
             << "Enter your choice: ";

        cin >> choice;
        cin.ignore();
        if (choice == 1)
        {
            int n;
            cout << "How many cards do you want to add? ";
            cin >> n;
            cin.ignore();
            for (int a = 0; a < n; a++)
            {
                game_card card;
                cout << "Name: ";
                getline(cin, card.name);
                cout << "Attack: ";
                cin >> card.attack;
                cout << "Health: ";
                cin >> card.health;
                cin.ignore();
                cout << "Description: ";
                getline(cin, card.description);
                add_card(card, filename);
            }
        }
        else if (choice == 2)
        {
            int size;
            game_card *cards = read_cards(filename, size);
            for (int a = 0; a < size; a++)
            {
                cout << cards[a].name << ": " << cards[a].attack << " | " << cards[a].health << " - " << cards[a].description << endl;
            }
            delete[] cards;
        }
        else if (choice == 3)
        {
            string name;
            cout << "Name of the card to remove: ";
            getline(cin, name);
            remove_card(name, filename);
        }
        else if (choice == 4)
        {
            int n;
            cout << "How many keywords do you want to search for? ";
            cin >> n;
            cin.ignore();
            string *keys = new string[n];
            for (int a = 0; a < n; a++)
            {
                cout << "Keyword " << a + 1 << ": ";
                getline(cin, keys[a]);
            }
            search_cards(keys, n, filename);
            delete[] keys;
        }
        else if (choice == 5)
        {
            sort_cards(filename);
            cout << "Cards sorted." << endl;
        }
    }
    return 0;
}

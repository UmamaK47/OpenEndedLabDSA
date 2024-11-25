#include<iostream>
#include<string>
#include<fstream>
#include <algorithm> 

using namespace std;

int getValidInt() {
	int value;
	while (!(cin >> value)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input. Please enter an integer: ";
	}
	return value;
}

struct Node {
	string name;
	int testSeries=0;
	int ODI=0;
	int T20=0;
	Node* next;
	Node* previous;
};

class List{
private:
	Node* head=nullptr;
	Node* tail=nullptr;
	int count = 0;
public:
	void insertAtHead(string playerName, int playerTestSeries, int playerODI, int playerT20, bool writeToFile=true);
	void Remove(string playerName);
	void display();
	void update(string playerName);
	bool isEmpty();
	int getCount() const;
	void initializeFromFiles();
};

void List::insertAtHead(string playerName, int playerTestSeries, int playerODI, int playerT20, bool writeToFile) {
	Node* newNode = new Node();
	newNode->name = playerName;
	newNode->testSeries = playerTestSeries;
	newNode->ODI = playerODI;
	newNode->T20 = playerT20;
	if (head == nullptr) {
		head = newNode;
		tail = newNode;
	}
	else {
		newNode->next = head;
		head->previous = newNode;
		head = newNode;
		head->previous = nullptr;
	}
	if (newNode->next == nullptr) {
		tail = newNode;
	}
	count++;
	
	if (writeToFile) {
		string fileName = playerName + "_File.txt";
		ofstream outFile(fileName);
		if (outFile.is_open()) {
			outFile << playerName << " " << playerTestSeries << " " << playerODI << " " << playerT20;
			outFile.close();
		}
		else {
			cerr << "Unable to open file " << fileName << ".\n";
		}
		ofstream playerFile("players_list.txt", ios::app);
		if (playerFile.is_open()) {
			playerFile << playerName << "\n";
			playerFile.close();
		}
		else {
			cerr << "Error: Could not update the player file.\n";
		}
	}
	
}
void List::Remove(string playerName) {
	if (head == nullptr) {
		cout << "The list is empty.\n";
		return;
	}
	Node* current = head;
	while (current != nullptr) {
		if (current->name == playerName) {
			if (current == head) {
				head = current->next;
				if (head != nullptr) {
					head->previous = nullptr;
				}
			}
			else if (current == tail) {
				tail = current->previous;
				if (tail != nullptr) {
					tail->next = nullptr;
				}
			}
			else {
				current->previous->next = current->next;
				current->next->previous = current->previous;
			}

			delete current;
			count--;
			
			string fileName = playerName + "_File.txt";
			if (remove(fileName.c_str()) == 0) {
				cout << "Player's file '" << fileName << "' deleted successfully.\n";
			}
			else {
				cout << "Unable to delete player's file '" << fileName << "'.\n";
				return;
			}
			cout << "Player " << playerName << " removed successfully from the list.\n";
			
			ifstream inFile("players_list.txt");
			ofstream outFile("temp_players_list.txt");

			if (!inFile.is_open() || !outFile.is_open()) {
				cout << "Error opening file.\n";
				return;
			}

			string line;
			bool found = false;

			while (getline(inFile, line)) {
				if (line != playerName) {
					outFile << line << "\n";
				}
				else {
					found = true; 
				}
			}

			inFile.close();
			outFile.close();
			if (found) {
				remove("players_list.txt");
				rename("temp_players_list.txt", "players_list.txt");
			}
			else {
				cout << "Player " << playerName << " not found in players_list.txt.\n";
			}
			return;
		}
		current = current->next;
	}
	cout << "Player " << playerName << " not found in the list.\n";
}
void List::update(string playerName) {
	if (isEmpty()) {
		cout << "The list is empty. No players to update.\n";
		return;
	}
	Node* current = head;
	while (current != nullptr) {
		if (current->name == playerName) {
			cout << "Updating information for " << playerName << ":\n";
			cout << "Enter new number of ODI: ";
			current->ODI = getValidInt();
			cout << "Enter new number of Test Series: ";
			current->testSeries = getValidInt();
			cout << "Enter new number of T20: ";
			current->T20 = getValidInt();
			string fileName = playerName + "_File.txt";
			ofstream outFile(fileName);
			if (outFile.is_open()) {
				outFile << "Player Name: " << current->name << "\nODI: " << current->ODI
					<< "\nTest Series: " << current->testSeries << "\nT20: " << current->T20;
				outFile.close();
				cout << "Player data updated successfully in file.\n";
			}
			else {
				cerr << "Error: Unable to update file " << fileName << ".\n";
			}
			return;
		}

		current = current->next;
	}

	cout << "Player " << playerName << " not found in the list.\n";
}
void List::display() {
	if (isEmpty()) {
		cout << "No matches recorded yet.\n";
		return;
	}

	Node* current = head;
	while (current != nullptr) {
		cout << "\n---------------------\n";
		cout << "Name: " << current->name << "\nODI: " << current->ODI << "\nTest Series: " << current->testSeries << "\nT20: " << current->T20;
		current = current->next;
	}
}
bool List::isEmpty() {
	return head == nullptr;
}
int List::getCount() const {
	return count;
}
void List::initializeFromFiles() {
	ifstream playersFile("players_list.txt");

	if (!playersFile.is_open()) {
		cout << "Could not open 'players_list.txt'. Starting with an empty list.\n";
		return;
	}

	string playerName;
	while (getline(playersFile, playerName)) {
		string fileName = playerName + "_File.txt";
		ifstream file(fileName);

		if (file.is_open()) {
			string name;
			int odi, testSeries, t20;
			file >> name >> testSeries >> odi >> t20;
			insertAtHead(name, testSeries, odi, t20, false);
			file.close();
		}
		else {
			cout << "Warning: Could not open file '" << fileName << "' for player '" << playerName << "'.\n";
		}
	}
	playersFile.close();
}


int main() {
	List players;
	players.initializeFromFiles();
	int choice, odi, t20, testseries;
	string name;
	while (true) {
		cout << "\n\nMenu:\n1. Insert New Player \n2. Delete Player \n3. Update Player \n";
		cout << "4. Display All Players \n5. Exit\n";
		cout << "Enter choice: ";
		choice = getValidInt();

		if (choice == 5) break;
		switch (choice) {	
		case 1:
			if (players.getCount() >= 20) {
				cout << "Cannot add more players. Maximum limit of players reached.\n";
				break;
			}
			cout << "Enter Name for Player: \n";
			cin.ignore();
			getline(cin, name);
			transform(name.begin(), name.end(), name.begin(),toupper);
			cout << "Enter number of ODI: \n";
			odi = getValidInt();
			cout << "Enter number of test series: \n";
			testseries = getValidInt();
			cout << "Enter number of T20: \n";
			t20 = getValidInt();
			players.insertAtHead(name, testseries, odi, t20);
			cout << "player inserted sucessfully";
			break;
		case 2:
			cout << "Enter player you want to delete: ";
			cin.ignore();
			getline(cin, name);
			transform(name.begin(), name.end(), name.begin(),toupper);
			players.Remove(name);
			break;
		case 3:
			cout << "Enter player you want to update: ";
			cin.ignore();
			getline(cin, name);
			transform(name.begin(), name.end(), name.begin(),toupper);
			players.update(name);
			break;
		case 4:
			cout << "List of players: \n";
			players.display();
			break;
		default:
			cout << "Invalid choice, please enter a number from 1 to 5.\n";
		}
	}
	return 0;
}
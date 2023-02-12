/*
Name: Dhruv C.
Date: 2022/05/28
Class: ICS4U-2A
Assignment: Final Project - Condo Manager
*/
#include <iostream>
#include <vector>
#include <exception>
#include <limits>

using namespace std;

/*
helper template function for getting input of any type from user
instead of doing 3 statements:
int temp;
cout << "Enter temp: ";
cin >> temp;

you can do it in 1 statement:
int temp = prompt<int>("Enter temp: ");
*/
template<typename T> T prompt(const string& prompt = "") {
	T buffer;
	cout << prompt;
	cin >> buffer;
	return buffer;
}

// exception class, deal with any error occuring
class error : public exception {
	// private variable err, store the error message
	char* err;
public:
	// receive the error message, and assign it to the private varibale err
	error(const char* err) {
		this->err = (char*)err;
	}
	// return user the error message, tell the user what error has occured
	char* what() {
		return this->err;
	}
};

void welcomeMessage() { //ascii art welcome message
	cout //have to escape '\' by using  '\\'
		<< "  ____   _                          _            \n"
		<< " |  _ \\ | |__   _ __  _   _ __   __( )___        \n"
		<< " | | | || '_ \\ | '__|| | | |\\ \\ / /|// __|       \n"
		<< " | |_| || | | || |   | |_| | \\ V /   \\__ \\       \n"
		<< " |____/ |_| |_||_|    \\__,_|  \\_/    |___/       \n"
		<< "   ____                   _                      \n"
		<< "  / ___| ___   _ __    __| |  ___                \n"
		<< " | |    / _ \\ | '_ \\  / _` | / _ \\               \n"
		<< " | |___| (_) || | | || (_| || (_) |              \n"
		<< "  \\____|\\___/ |_| |_| \\__,_| \\___/               \n"
		<< "  __  __                                         \n"
		<< " |  \\/  |  __ _  _ __    __ _   __ _   ___  _ __ \n"
		<< " | |\\/| | / _` || '_ \\  / _` | / _` | / _ \\| '__|\n"
		<< " | |  | || (_| || | | || (_| || (_| ||  __/| |   \n"
		<< " |_|  |_| \\__,_||_| |_| \\__,_| \\__, | \\___||_|   \n"
		<< "                               |___/             "
		<< endl;
}

void exitMessage() {
	cout //have to escape '\' by using  '\\'
		<<"  _____ _                 _     __   __               __  \n"
		<<" |_   _| |__   __ _ _ __ | | __ \\ \\ / /__  _   _   _  \\ \\ \n"
		<<"   | | | '_ \\ / _` | '_ \\| |/ /  \\ V / _ \\| | | | (_)  | |\n"
		<<"   | | | | | | (_| | | | |   <    | | (_) | |_| |  _   | |\n"
		<<"   |_| |_| |_|\\__,_|_| |_|_|\\_\\   |_|\\___/ \\__,_| (_)  | |\n"
		<<"                                                      /_/ \n"
		<< endl;
}

void menu() { //used in main infinite loop
	cout << '\n'
		<< "**** MAIN MENU ****\n"
		<< "1. Add Record\n"
		<< "2. List Record\n"
		<< "3. Modify Record\n"
		<< "4. Delete Record\n"
		<< "5. Quit" << endl;
}

class Condo {
	struct Occupant {
		string name, phone;
		int unitNumber;
	} occupant;
	bool initialized = false, markedForDeletion = false;
public:
	Condo() {}
	
	//getters
	virtual string condoType() = 0; //defined later for each condo type derived class
	bool isInitialized() { return initialized; }
	bool isMarkedForDeletion() { return markedForDeletion; }

	//occupant getters
	string getName() { return occupant.name; }
	string getPhone() { return occupant.phone; }
	int getUnitNumber() { return occupant.unitNumber; }

	void addRecord() {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Enter occupant's name: ";
		getline(cin, occupant.name);
		occupant.phone = prompt<string>("Enter occupant's phone number: ");
		occupant.unitNumber = prompt<int>("Enter occupant's unit: ");
		initialized = true;
	}
	void listRecord() {
		//we don't want to print uninitalized records
		if (!initialized || markedForDeletion)
			return;
		cout << condoType() << ":\n"
			<< "- name: " << occupant.name << '\n'
			<< "- phone: " << occupant.phone << '\n'
			<< "- unit: " << occupant.unitNumber << endl << endl;
	}
	void modifyRecord() {
		//allow user to change occupant data until they're done
		while (true) {
			cout << "1. name\n"
				<< "2. phone\n"
				<< "3. unit number\n"
				<< "4. stop modifying" << endl;
			const auto choice = prompt<int>("What do you want to change? ");
			if (choice == 4)
				break;
			switch (choice) {
				case 1:
					cout << "Enter new name: ";
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					getline(cin, occupant.name);
					break;
				case 2:
					occupant.phone = prompt<string>("Enter new phone number: ");
					break;
				case 3:
					occupant.unitNumber = prompt<int>("Enter new unit number: ");
					break;
				default:
					throw error("Invalid choice.");
			}
		}
	}
	void deleteRecord() {
		//condos marked for deletion are deleted from the vectors in main()
		markedForDeletion = true;
	}
};

//implement sub classes for every condo type
//also implement the condoType() virtual function
class TownHouse : public Condo {
public:
	string condoType() {
		return "Town House";
	}
};

class Apartment : public Condo {
public:
	string condoType() {
		return "Apartment";
	}
};

class PentHouse : public Condo {
public:
	string condoType() {
		return "Penthouse";
	}
};

int main() {
	//different vector for every condo type
	vector<TownHouse> towns;
	vector<Apartment> aparts;
	vector<PentHouse> pents;

	//helper lambda to find occupants across all condos by name
	//returns pointer to a condo or nullptr if name was not found
	auto findByName = [&](string name) {
		for (auto& town : towns)
			if (name == town.getName())
				return (Condo*)&town;
		for (auto& apart : aparts)
			if (name == apart.getName())
				return (Condo*)&apart;
		for (auto& pent : pents)
			if (name == pent.getName())
				return (Condo*)&pent;
		return (Condo*)nullptr;
	};

	welcomeMessage();
	while (true) {
		menu();
		const auto choice = prompt<int>("Your Choice: ");
		if (choice == 5)
			break;
		try {
			switch (choice) {
				case 1: { //add new condo record to its corresponding vector
					cout << "\n---- CONDO TYPE ----\n"
						<< "1. Town House\n"
						<< "2. Apartment\n"
						<< "3. Penthouse" << endl;
					const auto condoType = prompt<int>("Enter condo type (1-3): ");
					switch(condoType) {
						case 1: {
							TownHouse t;
							t.addRecord();
							towns.push_back(t);
							break;
						}
						case 2: {
							Apartment a;
							a.addRecord();
							aparts.push_back(a);
							break;
						}
						case 3: {
							PentHouse p;
							p.addRecord();
							pents.push_back(p);
							break;
						}
					}
					break;
				}
				case 2: { //list all condo records
					cout << endl;
					for (auto& town : towns)
						town.listRecord();
					for (auto& apart : aparts)
						apart.listRecord();
					for (auto& pent : pents)
						pent.listRecord();
					cout << "Total condos: "
						<< towns.size() + aparts.size() + pents.size() << endl;
					break;
				}
				case 3: { //find condo by name and modify its record
					cout << endl;
					cout << "Enter exact name of condo occupant: ";
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					string name;
					getline(cin, name);
					const auto condo = findByName(name);
					if (!condo)
						throw error("Could not find condo.");
					condo->modifyRecord();
					break;
				}
				case 4: { //find condo by name and delete its record
					cout << endl;
					cout << "Enter exact name of condo occupant: ";
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					string name;
					getline(cin, name);
					const auto condo = findByName(name);
					if (!condo)
						throw error("Could not find condo.");
					condo->deleteRecord();
					cout << "Marked for deletion." << endl;
					break;
				}
				default: {
					throw error("Invalid choice.");
				}
			}
		} catch (error& except) {
			cout << except.what() << endl;
		}
		//delete marked objects in all vectors
		for (auto it = towns.begin(); it != towns.end();) {
			if (it->isMarkedForDeletion())
				it = towns.erase(it);
			else ++it;
		}
		for (auto it = aparts.begin(); it != aparts.end();) {
			if (it->isMarkedForDeletion())
				it = aparts.erase(it);
			else ++it;
		}
		for (auto it = pents.begin(); it != pents.end();) {
			if (it->isMarkedForDeletion())
				it = pents.erase(it);
			else ++it;
		}
	}
	exitMessage();
}
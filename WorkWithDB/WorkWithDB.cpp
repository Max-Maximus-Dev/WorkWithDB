#include <iostream>
#include "SQL/sqlite3.h"
#include <string>
#include <vector>
#include <fstream>

// ------------- Les1 -------------
/**class Student {
private:
	std::string name;
	int age;
public:
	Student(std::string name, int age) {
		this->name = name;
		this->age = age;
	}
	std::string getName() { return name; }
	int getAge() { return age; }
};
class DBClass {
protected:
	std::string sumname;
	int sumage;
public:
	virtual void insertToDB() = 0;
	std::string getSumname() { return sumname; }
	int getSumage() { return sumage; }
};
class StudentDB : public DBClass {
public:
	void insertToDB() override {
		std::string request = "Insert INTO studentt (Name, Age) VALUES('" + sumname + "', " + std::to_string(sumage) + ");";
		sqlite3* db;
		if (sqlite3_open("test.db", &db) == SQLITE_OK) {
			if (sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Successfully inserted data." << std::endl;
			}
			else {
				std::cout << "Failed to insert data." << std::endl;
			}
		}
		else {
			std::cout << "Failed to open database." << std::endl;
		}
	}
};
class PersonDB : public DBClass {
private:
	std::string name, lastname, address;
	int age, year, phoneNumber;
public:
	PersonDB(std::string name = "John", std::string lastname = "Doe", std::string address = "123 Main St", int age = 30, int year = 2024, int phoneNumber = 1234567890) {
		this->name = name;
		this->lastname = lastname;
		this->address = address;
		this->age = age;
		this->year = year;
		this->phoneNumber = phoneNumber;
	}
	void insertToDB() override {
		std::string request = "Insert INTO personn (Name, Lastname, Address, Age, Year, PhoneNumber) VALUES('" + name + "', '" + lastname + "', '" + address + "', " + std::to_string(age) + ", " + std::to_string(year) + ", " + std::to_string(phoneNumber) + ");";
		std::string request_create = "CREATE TABLE IF NOT EXISTS personn ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT , "
			"Name TEXT,"
			"Lastname TEXT,"
			"Address TEXT,"
			"Age INT,"
			"Year INT,"
			"PhoneNumber INT);";
		sqlite3* db;
		if (sqlite3_open("test.db", &db) == SQLITE_OK) {
			if (sqlite3_exec(db, request_create.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
				if (sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
					std::cout << "Successfully inserted data." << std::endl;
				}
				else {
					std::cout << "Failed to insert data." << std::endl;
				}
			}
			else {
				std::cout << "Failed to create table." << std::endl;
			}
		}
		else {
			std::cout << "Failed to open database." << std::endl;
		}
		
	}
};**/


// ------------- Les2.Para1 -------------
int callback(void* info, int size, char** arg, char** col_name) {
	for (int i = 0; i < size; i++) {
		std::cout << "[" << col_name[i] << "] ";
		if (arg[i] != nullptr) {
			std::cout << arg[i] << " ";
		}
		else {
			std::cout << "NULL ";
		}
	}
	std::cout << std::endl;
	return 0;
}

class Employee {
private:
	std::string companyName, position, status;
	int yearWorked, salary, bonus;
public:
	Employee() {}
	Employee(std::string companyName, std::string position, int salary, int bonus, int yearWorked) {
		this->companyName = companyName;
		this->position = position;
		this->salary = salary;
		this->bonus = bonus;
		this->yearWorked = yearWorked;
		this->status = "default";
	}
	Employee(std::string companyName, std::string position, std::string status, int salary, int yearWorked) {
		this->companyName = companyName;
		this->position = position;
		this->salary = salary;
		this->yearWorked = yearWorked;
		this->status = status;
		this->bonus = 0;
	}
	Employee(std::string companyName, std::string position, std::string status, int salary, int bonus, int yearWorked) {
		this->companyName = companyName;
		this->position = position;
		this->salary = salary;
		this->bonus = bonus;
		this->yearWorked = yearWorked;
		this->status = status;
	}

	std::string getCompanyName() { return companyName; }
	std::string getPosition() { return position; }
	std::string getStatus() { return status; }

	void setStatus(std::string status) { this->status = status; }
	int getSalary() { return salary; }
	int getBonus() { return bonus; }
	int getYearWorked() { return yearWorked; }

	void setBonusToSalary(int bonus) {
		salary += bonus;
	}

	void insertToDB(std::fstream &logs) {
		std::string request_create = "CREATE TABLE IF NOT EXISTS EMPLOYEE ("
			"ID INTEGER PRIMARY KEY AUTOINCREMENT , "
			"CompanyName TEXT,"
			"Position TEXT,"
			"Status TEXT,"
			"Salary INT,"
			"Bonus INT,"
			"YearWorked INT);";
		sqlite3* db;
		if (sqlite3_open("test.db", &db) == SQLITE_OK) {
			if (sqlite3_exec(db, request_create.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Table created successfully." << std::endl;
				if (logs.is_open()) {
					logs << "Create table successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to create table." << std::endl;
			}
		}
		else {
			std::cout << "Failed to open database." << std::endl;
		}
	}
	void insert(std::fstream &logs) {
		std::string request = "Insert INTO EMPLOYEE (CompanyName, Position, Status, Salary, Bonus, YearWorked) VALUES('" + companyName + "', '" + position + "', '" + status + "', " + std::to_string(salary) + ", " + std::to_string(bonus) + ", " + std::to_string(yearWorked) + ");";
		sqlite3* db;
		if (sqlite3_open("test.db", &db) == SQLITE_OK) {
			if (sqlite3_exec(db, request.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Successfully inserted data." << std::endl;
				if (logs.is_open()) {
					logs << "Insert into table successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to insert data." << std::endl;
			}
		}
		else {
			std::cout << "Failed to open database." << std::endl;
		}
	}
	void updateAndTask(std::vector<Employee>&employees, std::fstream &logs) {
		sqlite3* db;
		for (auto& emp : employees) {
			emp.setBonusToSalary(emp.getBonus());
			std::cout << "Employee from " << emp.getCompanyName() << " working as " << emp.getPosition() << " with status " << emp.getStatus()
				<< " has new salary: " << emp.getSalary() << " after adding bonus: " << emp.getBonus() << std::endl;
		}
		std::string reqUpdateStatus = "UPDATE EMPLOYEE SET Status = 'middle' WHERE YearWorked >= 4;";
		if (sqlite3_open("test.db", &db) == SQLITE_OK) {
			if (sqlite3_exec(db, reqUpdateStatus.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Status updated successfully." << std::endl;
				for (auto& emp : employees) {
					if (emp.getYearWorked() >= 4) {
						emp.setStatus("middle");
						std::cout << "Employee from " << emp.getCompanyName() << " working as " << emp.getPosition() << " now has status: " << emp.getStatus() << std::endl;
					}
				}
				if (logs.is_open()) {
					logs << "Update table successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to update status." << std::endl;
			}
		}
		else {
			std::cout << "Failed to open database." << std::endl;
		}
	}
	void select(std::fstream &logs) {
		sqlite3* db;
		std::string reqSelectAll = "SELECT * FROM EMPLOYEE;";
		std::string reqSelectHighYear = "SELECT * FROM EMPLOYEE WHERE YearWorked > 5;";
		std::string reqSelectMinSalary = "SELECT * FROM EMPLOYEE WHERE Salary = (SELECT MIN(Salary) FROM EMPLOYEE) LIMIT 1;";
		std::string reqSelectMaxSalary = "SELECT * FROM EMPLOYEE WHERE Salary = (SELECT MAX(Salary) FROM EMPLOYEE) LIMIT 1;";
		std::string reqSelectMaxBonus = "SELECT * FROM EMPLOYEE WHERE Bonus = (SELECT MAX(Bonus) FROM EMPLOYEE) LIMIT 1;";
		if (sqlite3_open("test.db", &db) == SQLITE_OK) {
			if (sqlite3_exec(db, reqSelectAll.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Data selected successfully." << std::endl;
				if (logs.is_open()) {
					logs << "Select successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to select data." << std::endl;
			}
			if (sqlite3_exec(db, reqSelectHighYear.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Data selected successfully." << std::endl;
				if (logs.is_open()) {
					logs << "Select successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to select data." << std::endl;
			}
			if (sqlite3_exec(db, reqSelectMinSalary.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Data with minimum Salary selected successfully." << std::endl;
				if (logs.is_open()) {
					logs << "Select successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to select data." << std::endl;
			}
			if (sqlite3_exec(db, reqSelectMaxSalary.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Data with maximum Salary selected successfully." << std::endl;
				if (logs.is_open()) {
					logs << "Select successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to select data." << std::endl;
			}
			if (sqlite3_exec(db, reqSelectMaxBonus.c_str(), callback, nullptr, nullptr) == SQLITE_OK) {
				std::cout << "Data with maximum Bonus selected successfully." << std::endl;
				if (logs.is_open()) {
					logs << "Select successfully" << std::endl;
				}
			}
			else {
				std::cout << "Failed to select data." << std::endl;
			}
		}
		else {
			std::cout << "Failed to open database." << std::endl;
		}
	}
};
// ------------- Les2.Para1 -------------



// ------------- Les2.Para2 -------------




// ------------- Les2.Para2 -------------


int main()
{
	sqlite3* db;
	if (sqlite3_open("test.db", &db) == SQLITE_OK) {
		std::cout << "Database opened successfully." << std::endl;
		std::cout << std::endl;
	}
	else {
		std::cout << "Failed to open database." << std::endl;
	}
	std::string req = "DROP TABLE EMPLOYEE";
	sqlite3_exec(db, req.c_str(), nullptr, nullptr, nullptr);
	// ------------------------------ Les1 --------------------------------
	/**
	sqlite3* db; спосіб Створення бази Д
	sqlite3_open("test.db", &db); спосіб Відкриття бази Даних
	SQLITE_OK перевірка на успішне відкриття бази Даних
	sqlite3_close(db); спосіб Закриття бази Даних
	sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr); спосіб Виконання SQL запиту
	**/
	/*std::string request = "CREATE TABLE IF NOT EXISTS studentt ("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT , "
		"Name TEXT,"
		"Age INT);";
	int age1 = 18;
	std::string name = "Markl";*/
	/**Student student("Mark", 20);
	std::string req_student = "INSERT INTO studentt (Name, Age) VALUES ('" + student.getName() + "', " + std::to_string(student.getAge()) + ");";
	if (sqlite3_exec(db, req_student.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
		std::cout << "Successfully inserted data." << std::endl;
	}
	else {
		std::cout << "Failed to insert data." << std::endl;
	}**/
	/*DBClass* dbClass = new StudentDB();
	dbClass->insertToDB();*/
	/*DBClass* sumperson = new PersonDB();
	sumperson->insertToDB();*/
	/**DBClass* person1 = new PersonDB("John", "Smith", "456 Oak St", 28, 2023, 9876543210);
	DBClass* person2 = new PersonDB("Alice", "Johnson", "789 Pine St", 35, 2022, 5551234567);
	DBClass* person3 = new PersonDB("Michael", "Brown", "321 Maple St", 42, 2021, 4449876543);
	DBClass* person4 = new PersonDB("Emily", "Davis", "654 Cedar St", 30, 2024, 3334567890);
	DBClass* person5 = new PersonDB("David", "Wilson", "987 Birch St", 25, 2023, 2221234567);
	std::vector<DBClass*> persons = { person1, person2, person3, person4, person5 };
	for (auto person : persons) {
		person->insertToDB();
	}**/
	/**
	std::string request_delete = "DROP TABLE personn;";
	std::string request_delete1 = "DROP TABLE PERSON;";
	std::string request_delete2 = "DROP TABLE PERSONNeff;";
	if (sqlite3_open("test.db", &db) == SQLITE_OK) {
		std::cout << "Database opened successfully." << std::endl;
	}
	else {
		std::cout << "Failed to open database." << std::endl;
	}
	if (sqlite3_exec(db, request_delete.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
		std::cout << "Table deleted successfully." << std::endl;
	}
	if (sqlite3_exec(db, request_delete1.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
		std::cout << "Table deleted successfully." << std::endl;
	}
	if (sqlite3_exec(db, request_delete2.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
		std::cout << "Table deleted successfully." << std::endl;
	}
	else {
		std::cout << "Failed to delete table." << std::endl;
	}**/
	/*std::string request = "CREATE TABLE PERSONN ("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME TEXT NOT NULL, "
		"AGE INTEGER NOT NULL);";*/
	/*if (sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
		std::cout << "Table created successfully." << std::endl;
	}
	else {
		std::cout << "Failed to create table." << std::endl;
	}*/
	// ------------------------------ Les1 --------------------------------
	





	// -----------------------------------------------Test-----------------------------------------------
	
	//std::string reqCreate = "CREATE TABLE IF NOT EXISTS TEST ("
	//	"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
	//	"NAME TEXT NOT NULL, "
	//	"AGE INTEGER NOT NULL);";
	//int age2 = 22;
	//std::string name2 = "Alice";
	//std::string reqInsert = "INSERT INTO TEST (NAME, AGE) VALUES ('" + name2 + "', " + std::to_string(age2) + ");";
	//std::string reqUpdate = "UPDATE TEST SET NAME = 'bob', AGE = " + std::to_string(age2) + " WHERE ID = 1;";
	//std::string reqDelete = "DELETE FROM TEST WHERE ID = 1;";
	//std::string reqSelect = "SELECT * FROM TEST;";
	//char* msgError = nullptr;
	//if (sqlite3_exec(db, reqSelect.c_str(), callback, nullptr, &msgError) == SQLITE_OK) {
	//	std::cout << "It work!" << std::endl;
	//}
	//else {
	//	std::cerr << "Error - " << msgError << std::endl;
	//}
	
	//-----------------------------------------------Test-----------------------------------------------






	// --------------------------------------- Les2.Para1 -------------------------------------
	std::fstream logs("logs.txt");
	if (logs.is_open()) {
		int choice = -1;
		while (choice != 0) {
			int ask_create;
			std::cout << "---------------------------------------------------------" << std::endl;
			std::cout << "------------------Create Table Emplyee-------------------" << std::endl;
			std::cout << "---------------Insert into Table Emplyee-----------------" << std::endl;
			std::cout << "-------------------Set Bonus to salary-------------------" << std::endl;
			std::cout << "---------------Update into Table Emplyee-----------------" << std::endl;
			std::cout << "---------------Select into Table Emplyee-----------------" << std::endl;
			std::cout << "---------------------------------------------------------" << std::endl;
			if (choice == 0) {
				break;
			}
			else if (choice = 1) {

			}
		}

	}
	

	// --------------------------------------- Les2.Para1 -------------------------------------
	sqlite3_close(db);
}

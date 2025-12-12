#include <iostream>
#include "SQL/sqlite3.h"
#include <string>
#include <vector>
#include <fstream>

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
void menu(std::vector<Employee>& employees, std::fstream& logs, Employee& emplynow) {
	int choice = -1;
	std::cout << "Menu:\n";
	std::cout << "1. Add Emplyee\n2. Delete Employee\n3. Update Employee\n4. Find Employee\n5. Display All Employees\n6. Exit\n";
	std::cout << "Enter your choice: ";
	std::cin >> choice;
	if (choice == 1) {
		std::cout << "Enter company name: ";
		std::string companyName, position;
		std::cin.ignore();
		std::getline(std::cin, companyName);
		std::cout << "Enter position: ";
		std::getline(std::cin, position);
		for (auto& emp : employees) {
			if (emp.getCompanyName() == companyName && emp.getPosition() == position) {
				std::cout << "Employee already exists." << std::endl;
			}
			else {
				Employee newEmp(companyName, position, 0, 0, 0);
				employees.push_back(newEmp);
			}
		}
		std::cout << "Employee added successfully." << std::endl;
	}
	else if (choice == 2) {
		std::cout << "Enter company name: ";
		std::string companyName, position;
		std::cin.ignore();
		std::getline(std::cin, companyName);
		std::cout << "Enter position: ";
		std::getline(std::cin, position);
		for (auto it = employees.begin(); it != employees.end(); it++) {
			if (it->getCompanyName() == companyName && it->getPosition() == position) {
				employees.erase(it);
				std::cout << "Employee deleted successfully." << std::endl;
				return;
			}
		}
		std::cout << "Employee not found." << std::endl;
	}
	else if (choice == 3) {
		emplynow.updateAndTask(employees, logs);
	}
	else if (choice == 4) {
		std::cout << "Enter company name to find: ";
		std::string companyName;
		std::cin.ignore();
		std::getline(std::cin, companyName);
		for (auto& emp : employees) {
			if (emp.getCompanyName() == companyName) {
				std::cout << "Employee found: " << emp.getCompanyName() << " - " << emp.getPosition() << std::endl;
				return;
			}
		}
		std::cout << "Employee not found." << std::endl;
	}
	else if (choice == 5) {
		for (auto& emp : employees) {
			std::cout << "Company: " << emp.getCompanyName() << ", Position: " << emp.getPosition()
				<< ", Status: " << emp.getStatus() << ", Salary: " << emp.getSalary()
				<< ", Bonus: " << emp.getBonus() << ", Years Worked: " << emp.getYearWorked() << std::endl;
		}
	}
	else if (choice == 6) {
		std::cout << "Exiti" << std::endl;
	}
	else {
		std::cout << "Invalid choice." << std::endl;
	}
}
void registerUser(std::fstream& logs, std::vector<Employee>& employees) {
	std::string companyName;
	std::string position;
	int salary;
	int bonus;
	int yearWorked;
	std::cout << "Enter company name: ";
	std::getline(std::cin, companyName);
	std::cout << "Enter position: ";
	std::getline(std::cin, position);
	std::cout << "Enter salary: ";
	std::cin >> salary;
	std::cout << "Enter bonus: ";
	std::cin >> bonus;
	std::cout << "Enter years worked: ";
	std::cin >> yearWorked;
	std::cin.ignore();
	for (auto& emp : employees) {
		if (emp.getCompanyName() == companyName && emp.getPosition() == position) {
			std::cout << "Employee already exists." << std::endl;
		}
		else {
			Employee newEmp(companyName, position, salary, bonus, yearWorked);
			newEmp.insertToDB(logs);
			newEmp.insert(logs);
			employees.push_back(newEmp);
			std::cout << "Registration successful." << std::endl;
			menu(employees, logs, newEmp);
		}
	}
}

void loginUser(std::vector<Employee>& employees, std::fstream &logs) {
	std::string companyName;
	std::string position;
	std::cout << "Enter company name: ";
	std::getline(std::cin, companyName);
	std::cout << "Enter position: ";
	std::getline(std::cin, position);
	for (auto& emp : employees) {
		if (emp.getCompanyName() == companyName && emp.getPosition() == position) {
			std::cout << "Login successful." << std::endl;
			emp.updateAndTask(employees, logs);
			emp.select(logs);
			menu(employees, logs, emp);
		}
	}
	std::cout << "Login failed. Employee not found." << std::endl;
}

void SerializeData(std::vector<Employee>& employees, const std::string& filename) {
	std::ofstream ofs(filename);
	if (!ofs) {
		std::cerr << "Error opening file for serialization." << std::endl;
		return;
	}
	for (auto& emp : employees) {
		size_t companyNameSize = emp.getCompanyName().size();
		ofs << companyNameSize << ' ' << emp.getCompanyName() << ' ';
		size_t positionSize = emp.getPosition().size();
		ofs << positionSize << ' ' << emp.getPosition() << ' ';
		size_t statusSize = emp.getStatus().size();
		ofs << statusSize << ' ' << emp.getStatus() << ' ';
		ofs << emp.getSalary() << ' ' << emp.getBonus() << ' ' << emp.getYearWorked() << ' ';
	}
	ofs.close();
}
void DeserializeData(std::vector<Employee>& employees, const std::string& filename) {
	std::ifstream ifs(filename);
	if (!ifs) {
		std::cerr << "Error opening file for deserialization." << std::endl;
	}
	while (true) {
		size_t companyNameSize;
		if (!(ifs >> companyNameSize)) {
			break;
		}
		ifs.get();
		std::string companyName(companyNameSize, ' ');
		ifs.read(&companyName[0], companyNameSize);
		size_t positionSize;
		ifs >> positionSize;
		ifs.get();
		std::string position(positionSize, ' ');
		ifs.read(&position[0], positionSize);
		size_t statusSize;
		ifs >> statusSize;
		ifs.get();
		std::string status(statusSize, ' ');
		ifs.read(&status[0], statusSize);
		int salary, bonus, yearWorked;
		ifs >> salary >> bonus >> yearWorked;
		Employee emp(companyName, position, status, salary, bonus, yearWorked);
		employees.push_back(emp);
	}
	ifs.close();
}
void startMenu(std::vector<Employee>& employees, std::fstream &logs) {
	int choice;
	DeserializeData(employees, "employees.dat");
	do {
		std::cout << "1. Register\n2. Login\n3. Exit\nEnter your choice: ";
		std::cin >> choice;
		std::cin.ignore();
		switch (choice) {
		case 1:
			registerUser(logs, employees);
			SerializeData(employees, "employees.dat");
			break;
		case 2:
			loginUser(employees, logs);
			SerializeData(employees, "employees.dat");
			break;
		case 3:
			std::cout << "Exit" << std::endl;
			break;
		default:
			std::cout << "Invalid choice." << std::endl;
		}
	} while (choice != 3);
}
	


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
	/*std::string req = "DROP TABLE EMPLOYEE";
	sqlite3_exec(db, req.c_str(), nullptr, nullptr, nullptr);*/

	std::fstream logs("logs.txt", std::ios::out);
	std::vector<Employee> employees;
	startMenu(employees, logs);

	sqlite3_close(db);
}

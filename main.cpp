// AUTHOR: Haiwen Zhang
// Created at: 2020/8/10

#include<iostream>
#include<fstream>

#define completed true

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::fstream;
using std::ofstream;
using std::string;
using std::ios;

class appointment {
private:
	char patient_name[20];
	char dentist_name[20];
	char patient_phone[20];
	int treatment; // 1. extraction 2. toothwash 3. general  4. revisit
	int month;
	int date;
	int hour;
	int minute;
	bool complete;
public:
	void read_data(); // Ask for entering a new appointment record
	void write_data(); // Write the input record to the system
	void search_data(); // Search for a record in the system by name of patient
	void show_data(); // Dispplay a record
	void modify_data(bool); // Modify a record, when input is true, only change completeness of appointment to completed. (used when patient is on time for an appointment)
	void delete_data(); // delete a record from the system.
};


void appointment::read_data() {
	cout << "\nEnter Patient's Name: ";
	cin >> patient_name;
	cout << "Enter Patient's Phone Number: ";
	cin >> patient_phone;
	cout << "Type of Treatment Needed: ";
	cout << "\n\t1-->Tooth Extraction";
	cout << "\n\t2-->Toothwash";
	cout << "\n\t3-->General";
	cout << "\n\t4-->Revisit";
	cout << "\n\tEnter Choice: ";
	cin >> treatment;
	if (treatment == 4) {
		cout << "Enter the Name of the Dentist: ";
		cin >> dentist_name;
	}
	else {
		cout << "Enter the Dentist Assigned: ";
		cin >> dentist_name;
	}
	cout << "Month(1-12): ";
	cin >> month;
	cout << "Date(1-31): ";
	cin >> date;
	cout << "Hour(8-18): ";
	cin >> hour;
	cout << "Minute(0-60): ";
	cin >> minute;
	complete = false; // Initialize the completeness of appointment as false
}

void appointment::write_data() {
	ofstream outfile;
	outfile.open("appointments.clinic", ios::binary | ios::app);
	read_data();
	cout << "\nSaving Record...";
	outfile.write(reinterpret_cast<char*>(this), sizeof(*this));
	outfile.close();
	cout << "\nRecord Saved Successfully!!" << endl;
}

void appointment::search_data() {
	ifstream infile;
	infile.open("appointments.clinic", ios::binary);
	if (!infile) {
		cout << "\nCould Not Open Appointment Records" << endl;
		return;
	}
	// Get the number of records
	infile.seekg(0, ios::end);
	int count = infile.tellg() / sizeof(*this);
	// Ask for the patient name of the record
	infile.seekg(0,ios::beg);
	char p_name[20];
	cout << "\nPlease Enter Patient's Name: ";
	cin >> p_name;
	// Loop through records in the system
	while (true) {
		infile.read(reinterpret_cast<char*>(this), sizeof(*this));
		if (strcmp(patient_name, p_name)==0) {
			cout<<"\n\nAppointment Found:"<< endl;
			show_data();
			break;
		}
		else { // After looped through all record, it means record not found
			count--;
			if (count == 0) {
				cout << "\nRecord Not Found" << endl;
				break;
			}
			continue;
		}

	}
}

void appointment::show_data() {
	cout << "-------------------------------" << endl;
	cout << "Patient Name: " << patient_name << endl;
	cout << "Phone Number: " << patient_phone << endl;
	cout << "Dentist: " << dentist_name << endl;
	cout << "Time: " << month << "/" << date << " " << hour << ":" << minute << endl;
	cout << "Treatment: ";
	switch (treatment) {
	case 1:
		cout << "extraction" << endl;
		break;
	case 2:
		cout << "toothwash" << endl;
		break;
	case 3:
		cout << "general" << endl;
		break;
	case 4:
		cout << "revisit" << endl;
		break;
	default:
		cout << "Please enter number 1-4" << endl;
	}
	if (complete) {
		cout << "(COMPLETE)" << endl;
	}
	else {
		cout << "(NOT COMPLETE)" << endl;
	}
	cout << "-------------------------------" << endl;
}

void appointment::modify_data(bool completeness) {

	if (completeness == true) { // Only change copleteness, don't touch anything else
		ifstream infile;
		infile.open("appointments.clinic", ios::binary);
		if (!infile) {
			cout << "\nCould Not Open Appointment Records" << endl;
			return;
		}
		// Get the number of records in system
		infile.seekg(0, ios::end);
		int count = infile.tellg() / sizeof(*this); 
		const int c = count;
		// Ask for patient name of the record need to be marked as complete
		infile.seekg(0, ios::beg);
		char p_name[20];
		cout << "\nPlease Enter Patient's Name: ";
		cin >> p_name;

		while (true) {
			infile.read(reinterpret_cast<char*>(this), sizeof(*this));
			if (strcmp(patient_name, p_name) == 0) {
				cout << "\n\nAppointment Found:" << endl;
				int pos = infile.tellg();
				cout << "Marking the following record as complete.." << endl;
				show_data();

				fstream tmpfile;
				tmpfile.open("tmpfile.clinic", ios::out | ios::binary);
				infile.seekg(0, ios::beg);
				for (int i = 0; i < c; i++) { // write everything to tempfile, when reached record needs to be modified, change its completeness then write
					infile.read(reinterpret_cast<char*>(this), sizeof(*this));
					if (infile.tellg() == pos) {
						this->complete = true;
					}
					tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
				}
				infile.close();
				tmpfile.close();
				remove("appointments.clinic");
				rename("tmpfile.clinic", "appointments.clinic");
				cout << "Record Marked as Complete Successfully!!" << endl;
				break;
			}
			else {
				count--;
				if (count == 0) {
					cout << "\nRecord Not Found" << endl;
					break;
				}
				continue;
			}

		}
	}
	else { // When there's information besides completeness need to be changed 
		int choice;
		int treat;
		char dent_name[20];
		cout << "\nWhat needs to be modified?" << endl;
		cout << "\t1-->Treatment" << endl;
		cout << "\t2-->Dentist" << endl;
		cout << "Enter choice: ";
		cin >> choice;
		if (choice == 1) {
			cout << "Type of Treatment Needed: ";
			cout << "\n\t1-->Tooth Extraction";
			cout << "\n\t2-->Toothwash";
			cout << "\n\t3-->General";
			cout << "\n\t4-->Revisit";
			cout << "\n\tEnter Choice: ";
			cin >> treat;
		}
		else {
			cout << "Enter new dentist name: ";
			cin >> dent_name;
		}

		ifstream infile;
		infile.open("appointments.clinic", ios::binary);
		if (!infile) {
			cout << "\nCould Not Open Appointment Records" << endl;
			return;
		}
		// Get the number of records in system
		infile.seekg(0, ios::end);
		int count = infile.tellg() / sizeof(*this);
		const int c = count;
		infile.seekg(0, ios::beg);
		// Ask for patient name of the record need to be marked as complete
		char p_name[20];
		cout << "\nPlease Enter Patient's Name: ";
		cin >> p_name;
		while (true) {
			infile.read(reinterpret_cast<char*>(this), sizeof(*this));
			if (strcmp(patient_name, p_name) == 0) {
				cout << "\n\nAppointment Found:" << endl;
				int pos = infile.tellg();
				cout << "Modifying Following Record..." << endl;
				show_data();

				fstream tmpfile;
				tmpfile.open("tmpfile.clinic", ios::out | ios::binary);
				infile.seekg(0, ios::beg);
				for (int i = 0; i < c; i++) {
					infile.read(reinterpret_cast<char*>(this), sizeof(*this));
					if (infile.tellg() == pos) {
						// write everything to tempfile, when reached record needs to be modified, change whatever needs to be changed then write
						if (choice == 1) { 
							this->treatment = treat;
						}
						else {
							strcpy_s(this->dentist_name,dent_name);
						}
					}
					tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
				}
				infile.close();
				tmpfile.close();
				remove("appointments.clinic");
				rename("tmpfile.clinic", "appointments.clinic");
				cout << "Record Modified Successfully!!" << endl;
				break;
			}
			else {
				count--;
				if (count == 0) {
					cout << "\nRecord Not Found" << endl;
					break;
				}
				continue;
			}

		}
	}

}

void appointment::delete_data() {
	ifstream infile;
	infile.open("appointments.clinic", ios::binary);
	if (!infile) {
		cout << "\nCould Not Open Appointment Records" << endl;
		return;
	}
	infile.seekg(0, ios::end);
	int count = infile.tellg() / sizeof(*this);
	const int c = count;
	infile.seekg(0, ios::beg);
	char p_name[20];
	cout << "\nPlease Enter Patient's Name: ";
	cin >> p_name;
	while (true) {
		infile.read(reinterpret_cast<char*>(this), sizeof(*this));
		if (strcmp(patient_name, p_name) == 0) {
			cout << "\n\nAppointment Found:" << endl;
			int pos = infile.tellg();
			cout << "Deleting the following record.." << endl;
			show_data();

			fstream tmpfile;
			tmpfile.open("tmpfile.clinic", ios::out | ios::binary);
			infile.seekg(0, ios::beg);
			for (int i = 0; i < c; i++) {
				infile.read(reinterpret_cast<char*>(this), sizeof(*this));
				if (infile.tellg() == pos) {
					continue;
				}
				tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
			}
			infile.close();
			tmpfile.close();
			remove("appointments.clinic");
			rename("tmpfile.clinic", "appointments.clinic");
			cout << "Record Delete Successfully!!" << endl;
			break;
		}
		else {
			count--;
			if (count == 0) {
				cout << "\nRecord Not Found, might already been deleted" << endl;
				break;
			}
			continue;
		}

	}
}

int main() {
	// Display a wekcome message
	ifstream welcome;
	welcome.open("welcome.txt");
	welcome.seekg(0, ios::end);
	size_t size = welcome.tellg();
	string buffer(size, ' ');
	welcome.seekg(0);
	welcome.read(&buffer[0], size);
	system("Color E4");
	cout << buffer << endl;

	appointment app;
	int choice;
	while (true) {
		cout << "\nSelect one option below ";
		cout << "\n\t1-->Make an appointment";
		cout << "\n\t2-->Find an appointment";
		cout << "\n\t3-->Patient Here for an Appointment";
		cout << "\n\t4-->Modify an Appointment";
		cout << "\n\t5-->Cancel an Appointment";
		cout << "\n\t6-->Quit";
		cout << "\nEnter your choice: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			app.write_data();
			break;
		case 2:
			app.search_data();
			break;
		case 3:
			app.modify_data(completed);
			break;
		case 4:
			app.modify_data(false);
			break;
		case 5:
			app.delete_data();
			break;
		case 6:
			exit(0);
			break;
		default:
			cout << "\nEnter Number from 1-5" << endl;
			exit(0);
		}
	}
	return 0;
}
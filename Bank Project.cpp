#include <iostream>
#include<vector>
#include<iomanip>
#include<fstream>
#include<string>
#include<cctype>
using namespace std;
const string ClientsFileName = "ClientsFile.txt";
struct stClientData {
	string AccountNumber;
	string PinCode;
	string Name;
	string PhoneNumber;
	double AccountBalance;
	bool MarkForDelete = false;
};
enum enMainMenueOptions { eShowAllClients = 1, eAddNewClients = 2, eDeleteClient = 3, eUpdateClientData = 4, eFindClient = 5, eExit = 6 };
bool FindClientByAccountNumber(vector<stClientData>&, string, stClientData&);
stClientData ConvertLinetoRecord(string, string);
void ShowMainMenueScreen();
vector<stClientData> LoadClientsDataFromFile(string, string);
void PrintClientRecord(stClientData& Client) {
	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(15) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(15) << Client.PhoneNumber;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}
void PrintAllClientsDataFile(vector<stClientData>& vClients) {
	cout << "\n\t\t\t\t\tClients List (" << vClients.size() << ") Client(s).";
	cout << "\n__________________________________________________________";
	cout << "______________________________________________________________\n\n";
	cout << "| " << left << setw(20) << "Account Number";
	cout << "| " << left << setw(15) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n__________________________________________________________";
	cout << "______________________________________________________________\n\n";
	if (vClients.size() == 0)
		cout << "\tNo Clients Available In The System!\n";

	else {
		for (stClientData& Client : vClients) {
			PrintClientRecord(Client);
			cout << endl;
		}
	}
	cout << "__________________________________________________________";
	cout << "______________________________________________________________\n";
}
void AddClientDataToFile(string FileName, string ClientDataLine) {
	fstream ClientsFile;
	ClientsFile.open(FileName, ios::out | ios::app);
	if (ClientsFile.is_open()) {
		ClientsFile << ClientDataLine << endl;

		ClientsFile.close();
	}
}
bool clientExistsByAccountNumber(string AccountNumberToCheck, string FileName) {
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string Line;
		stClientData Client;
		while (getline(MyFile, Line)) {
			Client = ConvertLinetoRecord(Line, "#//#");
			if (Client.AccountNumber == AccountNumberToCheck) {
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}
stClientData ReadNewClient() {
	stClientData Client;

	cout << "Enter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);
	cout << endl;
	while (clientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {
		cout << "\nClient With Account Number [" << Client.AccountNumber << "] Already Exists, Enter Another Account Number? ";
		getline(cin, Client.AccountNumber);
		cout << endl;
	}
	cout << "Enter PinCode : ";
	getline(cin, Client.PinCode);
	cout << endl;

	cout << "Enter Name : ";
	getline(cin, Client.Name);
	cout << endl;

	cout << "Enter Phone Number : ";
	getline(cin, Client.PhoneNumber);
	cout << endl;

	cout << "Enter Account Balance : ";
	cin >> Client.AccountBalance;
	cout << endl;

	return Client;
}
string ConvertRecordToLine(stClientData Client, string Delim = "#//#") {
	string ClientRecord = "";
	ClientRecord += Client.AccountNumber + Delim;
	ClientRecord += Client.PinCode + Delim;
	ClientRecord += Client.Name + Delim;
	ClientRecord += Client.PhoneNumber + Delim;
	ClientRecord += to_string(Client.AccountBalance);
	return ClientRecord;
}
void AddNewClinet() {
	stClientData Client = ReadNewClient();
	AddClientDataToFile(ClientsFileName, ConvertRecordToLine(Client, "#//#"));
}
void AddNewClients() {
	char AddMore = 'Y';
	do {
		cout << "Adding New Clinet : \n\n";
		AddNewClinet();
		cout << "\nClient Added Successfully, Do You Want To Add More Clients? Y/N? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}
vector<string>SplitString(string Txt, string Delim) {
	short Pos = 0;
	string sWord;
	vector<string>vTxt;
	while ((Pos = Txt.find(Delim)) != Txt.npos) {
		sWord = Txt.substr(0, Pos);
		if (sWord != "")
			vTxt.push_back(sWord);

		Txt.erase(0, Pos + Delim.length());
	}
	if (Txt != "")
		vTxt.push_back(Txt);

	return vTxt;
}
stClientData ConvertLinetoRecord(string Line, string Delim = "#//#")
{
	stClientData Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Delim);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.PhoneNumber = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);
	return Client;
}
vector<stClientData> LoadClientsDataFromFile(string FileName, string Delim = "#//#") {
	vector <stClientData> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string Line;
		stClientData Client;
		while (getline(MyFile, Line)) {
			Client = ConvertLinetoRecord(Line, Delim);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}
void PrintClientCard(stClientData& Client) {
	cout << "\n\nThe Following Are The Client Details : \n\n";
	cout << "--------------------------------------------------\n";
	cout << "Account Number  : " << Client.AccountNumber << endl;
	cout << "Pin Code        : " << Client.PinCode << endl;
	cout << "Name            : " << Client.Name << endl;
	cout << "Phone           : " << Client.PhoneNumber << endl;
	cout << "Account Balance : " << Client.AccountBalance << endl;
	cout << "--------------------------------------------------\n";
}
bool MarkClientToDeleteByAccountNumber(vector<stClientData>& vClients, string AccountNumberToCheck) {
	for (stClientData& Client : vClients) {
		if (Client.AccountNumber == AccountNumberToCheck) {
			Client.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
vector <stClientData>SaveCleintsDataToFile(string FileName, vector<stClientData>& vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open()) {
		for (stClientData& Client : vClients) {
			if (Client.MarkForDelete == false) {
				DataLine = ConvertRecordToLine(Client, "#//#");
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}
bool FindClientByAccountNumber(vector<stClientData>& vClients, string AccountNumberToCheck, stClientData& Client) {
	for (stClientData& C : vClients) {
		if (C.AccountNumber == AccountNumberToCheck) {
			Client = C;
			return true;
		}
	}
	return false;
}
bool DeleteClientFromFileByAccountNumber(vector<stClientData>& vClients, string AccountNumberToCheck) {
	stClientData Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(vClients, AccountNumberToCheck, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre You Sure You Want To Delete This Client? Y/N ? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y') {
			MarkClientToDeleteByAccountNumber(vClients, AccountNumberToCheck);
			SaveCleintsDataToFile(ClientsFileName, vClients);
			vClients = LoadClientsDataFromFile(ClientsFileName, "#//#");
			cout << "\n\nClient Deleted Successfully.";
			return true;

		}
	}
	else {
		cout << "\nClient With Account Number (" << AccountNumberToCheck << ") Is Not Found!";
		return false;
	}
}
stClientData ReadUpdatedClient() {
	stClientData Client;

	cout << "Enter PinCode : ";
	getline(cin >> ws, Client.PinCode);
	cout << endl;

	cout << "Enter Name : ";
	getline(cin, Client.Name);
	cout << endl;

	cout << "Enter Phone Number : ";
	getline(cin, Client.PhoneNumber);
	cout << endl;

	cout << "Enter Account Balance : ";
	cin >> Client.AccountBalance;
	cout << endl;

	return Client;
}
stClientData ChangeClientRecord(string AccountNumber) {
	stClientData Client;
	Client = ReadUpdatedClient();
	Client.AccountNumber = AccountNumber;
	return Client;
}
bool UpdateClientDataInFile(vector<stClientData>& vClients, string AccountNumberToCheck) {
	stClientData Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(vClients, AccountNumberToCheck, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre You Sure You Want To Update This Client? Y/N ? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y') {
			for (stClientData& C : vClients) {
				if (C.AccountNumber == AccountNumberToCheck) {
					C = ChangeClientRecord(AccountNumberToCheck);
					break;
				}
			}
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Updated Successfully.";
			return true;

		}
	}
	else {
		cout << "\nClient With Account Number (" << AccountNumberToCheck << ") Is Not Found!";
		return false;
	}
}
string ReadClientAccountNumber()
{
	string Text;

	cout << "\nPlease Enter Account Number? " << endl;
	getline(cin >> ws, Text);

	return Text;
}
void GoBackToMainMenue() {
	cout << "\nPress Any key To Go Back To Main Menue...";
	system("pause>nul");
	ShowMainMenueScreen();
}
void FindAndPrintClient(vector <stClientData>& vClients, string AccountNumberToCheck) {
	stClientData Client;
	if (FindClientByAccountNumber(vClients, AccountNumberToCheck, Client))
		PrintClientCard(Client);

	else
		cout << "\nClient With Account Number [" << AccountNumberToCheck << "] Is Not Found!\n";

}
short ReadMainMenueOption() {
	short Choice = 1;
	cout << "Choose What Do You Want To Do? [1 To 6]? ";
	cin >> Choice;
	return Choice;
}
void ShowAllClientsScreen() {
	vector <stClientData>vClients = LoadClientsDataFromFile(ClientsFileName, "#//#");
	PrintAllClientsDataFile(vClients);
}
void ShowEndScreen() {
	cout << "\n----------------------------------------\n";
	cout << "\t    Program Ends :-)\n";
	cout << "----------------------------------------\n";
}
void ShowAddNewClientsScreen() {
	cout << "\n----------------------------------------\n";
	cout << "\t   Add New Clients Screen\n";
	cout << "----------------------------------------\n";
	AddNewClients();
}
void ShowDeleteClientScreen() {
	cout << "\n----------------------------------------\n";
	cout << "\t    Delete Client Screen\n";
	cout << "----------------------------------------\n";
	vector <stClientData>vClients = LoadClientsDataFromFile(ClientsFileName, "#//#");
	string AccountNumberToCheck = ReadClientAccountNumber();
	DeleteClientFromFileByAccountNumber(vClients, AccountNumberToCheck);
}
void ShowUpdateClientScreen() {
	cout << "\n--------------------------------------------------\n";
	cout << "\t   Update Client Information Screen\n";
	cout << "--------------------------------------------------\n";
	vector <stClientData>vClients = LoadClientsDataFromFile(ClientsFileName, "#//#");
	string AccountNumberToCheck = ReadClientAccountNumber();
	UpdateClientDataInFile(vClients, AccountNumberToCheck);

}
void ShowFindClientScreen() {
	cout << "\n----------------------------------------\n";
	cout << "\t    Find Client Screen\n";
	cout << "----------------------------------------\n";
	vector <stClientData>vClients = LoadClientsDataFromFile(ClientsFileName, "#//#");
	string AccountNumberToCheck = ReadClientAccountNumber();
	FindAndPrintClient(vClients, AccountNumberToCheck);

}
void PerformMainMenueOptions(enMainMenueOptions MainMenueOption) {
	switch (MainMenueOption) {
	case enMainMenueOptions::eShowAllClients:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eAddNewClients:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eUpdateClientData:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}
void ShowMainMenueScreen() {
	system("cls");
	cout << "=============================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "=============================================\n";
	cout << "\t[1] Show Clients List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Information.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";
	cout << "============================================\n";
	PerformMainMenueOptions((enMainMenueOptions)ReadMainMenueOption());
}
int main()
{
	ShowMainMenueScreen();
	return 0;
}
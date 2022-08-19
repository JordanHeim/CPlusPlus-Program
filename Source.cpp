#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

	using namespace std;

	/*
	Description:
		To call this function, simply pass the function name in Python that you wish to call.
	Example:
		callProcedure("printsomething");
	Output:
		Python will print on the screen: Hello from python!
	Return:
		None
	*/
	void CallProcedure(string pName)
	{
		char* procname = new char[pName.length() + 1];
		std::strcpy(procname, pName.c_str());

		Py_Initialize();
		PyObject* my_module = PyImport_ImportModule("PythonCornerGrocer");
		PyErr_Print();
		PyObject* my_function = PyObject_GetAttrString(my_module, procname);
		PyObject* my_result = PyObject_CallObject(my_function, NULL);
		Py_Finalize();

		delete[] procname;
	}

	/*
	Description:
		To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
	Example:
		int x = callIntFunc("PrintMe","Test");
	Output:
		Python will print on the screen:
			You sent me: Test
	Return:
		100 is returned to the C++
	*/
	int callIntFunc(string proc, string param)
	{
		char* procname = new char[proc.length() + 1];
		std::strcpy(procname, proc.c_str());

		char* paramval = new char[param.length() + 1];
		std::strcpy(paramval, param.c_str());


		PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
		// Initialize the Python Interpreter
		Py_Initialize();
		// Build the name object
		pName = PyUnicode_FromString((char*)"PythonCornerGrocer");
		// Load the module object
		pModule = PyImport_Import(pName);
		// pDict is a borrowed reference 
		pDict = PyModule_GetDict(pModule);
		// pFunc is also a borrowed reference 
		pFunc = PyDict_GetItemString(pDict, procname);
		if (PyCallable_Check(pFunc))
		{
			pValue = Py_BuildValue("(z)", paramval);
			PyErr_Print();
			presult = PyObject_CallObject(pFunc, pValue);
			PyErr_Print();
		}
		else
		{
			PyErr_Print();
		}
		//printf("Result is %d\n", _PyLong_AsInt(presult));
		Py_DECREF(pValue);
		// Clean up
		Py_DECREF(pModule);
		Py_DECREF(pName);
		// Finish the Python Interpreter
		Py_Finalize();

		// clean 
		delete[] procname;
		delete[] paramval;


		return _PyLong_AsInt(presult);
	}

	/*
	Description:
		To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
	Example:
		int x = callIntFunc("doublevalue",5);
	Return:
		25 is returned to the C++
	*/
	int callIntFunc(string proc, int param)
	{
		char* procname = new char[proc.length() + 1];
		std::strcpy(procname, proc.c_str());

		PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
		// Initialize the Python Interpreter
		Py_Initialize();
		// Build the name object
		pName = PyUnicode_FromString((char*)"PythonCornerGrocer");
		// Load the module object
		pModule = PyImport_Import(pName);
		// pDict is a borrowed reference 
		pDict = PyModule_GetDict(pModule);
		// pFunc is also a borrowed reference 
		pFunc = PyDict_GetItemString(pDict, procname);
		if (PyCallable_Check(pFunc))
		{
			pValue = Py_BuildValue("(i)", param);
			PyErr_Print();
			presult = PyObject_CallObject(pFunc, pValue);
			PyErr_Print();
		}
		else
		{
			PyErr_Print();
		}
		//printf("Result is %d\n", _PyLong_AsInt(presult));
		Py_DECREF(pValue);
		// Clean up
		Py_DECREF(pModule);
		Py_DECREF(pName);
		// Finish the Python Interpreter
		Py_Finalize();

		// clean 
		delete[] procname;

		return _PyLong_AsInt(presult);
	}


	int main()
	{
		char histogramChar = '*';    //charcacter for histogram output
		string individualItemName;   //variable to get item Name from file into a vector
		int individualItemAmt;       //variable to get item amount from file into a vector
		vector<int> itemAmt;         //vector to store item amount for histogram
		vector<string> itemName;     //vector to store item name for histogram
		ifstream inFS;               //input file
		string userItemName;         //variable to get item name from user for specific item amount look up
		int userInput = 0;           //varaible for user input


		while (userInput != 4) { //loop to get user input based on menu
			cout << "1: Produce a list of all items purchased and how many of each item was purchased" << endl;
			cout << "2: Produce a number for a specific item" << endl;
			cout << "3: Produce a histrogram listing all items purchased and how many of each item was purchased" << endl;
			cout << "4: Exit" << endl;
			cout << "Enter your selction as a number 1, 2, 3, or 4." << endl;
			cin >> userInput;
			cout << endl;

			while (cin.fail()) { //catch to see if there was an error in receiving an int into userInput
				cin.clear();
				cin.ignore(256, '\n');
			}

			if (userInput == 1) { //print the list of items in file
				CallProcedure("itemListPrint");
			}
			else if (userInput == 2) { //find a specific item and the amount of that item
				cout << "Enter the item name (use plural name): " << endl;
				cin >> userItemName;
				userItemName[0] = toupper(userItemName[0]);
				int x = callIntFunc("findSpecificItemAmt", userItemName);
				cout << endl;
				cout << userItemName << ": " << x << endl << endl;
			}
			else if (userInput == 3) { // create a histogram of all items pruchased through a file
				CallProcedure("createHistogram");
				inFS.open("frequency.dat");
				if (!inFS.is_open()) { //check to make sure file was opened
					cout << "error";
					return 0;
				}

				while (!inFS.eof()) { //loop to get item name and amount into vector until the end of the file
					inFS >> individualItemName;
					inFS >> individualItemAmt;

					if (!inFS.fail()) {
						itemName.push_back(individualItemName); //add item name to vector 
						itemAmt.push_back(individualItemAmt);   //add item amount to vector
					}
				}
				inFS.close(); //close the file

				for (int i = 0; i < itemName.size(); i++) { //print a histogram from the vectors containing the item name and amount
					cout.width(12);
					cout << left << itemName.at(i);
					cout << " ";
					cout << left << string(itemAmt.at(i), histogramChar) << endl;
				}
				itemName.clear(); //clear itemName vector 
				itemAmt.clear();  //clear itemAmt vector
			}
			else if (userInput == 4) { //if 4 is selected, exit the loop
				cout << "Exiting" << endl;
			}
			else { //display an invalid entry message if none of the 4 previous options were selected
				cout << "invalid Entry" << endl;
				cout << endl;
				cin.clear();
			}

		}
		return 0; //return 0 to end main
	}

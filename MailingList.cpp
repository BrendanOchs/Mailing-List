//********************************************************************************
//*                                                                              *
//* PROGRAM FILE NAME: MailingList.cpp                                              *
//*                                                                              *
//* PROGRAM AUTHOR:_______________Brendan Ochs___________________                *
//*                                                                              *
//*                                                                              *
//*                          DATE: NOVEMBER 18TH, 2016                           *
//*                                                                              *
//********************************************************************************

//*****************************PROGRAM DESCRIPTION********************************
//*                                                                              *
//*  Purpose: The purpose of this program is to maintain a mialing list for      *
//*           customers. This program will add any number of customers to the    *
//*           mailing list. It will also delete and change entries if they       *
//*           exist. It will also print a single entry or all entries.           *
//*           The program will throw errors for adding duplicate files,          *
//*           trying to delete a non-existent entry, trying to change a non-     *
//*           existent entry, and trying to print a non-existent entry.          *
//*                                                                              *
//*  USER DEFINED                                                                *
//*  MODULES     : Header     -  Print the output preamble.                      *
//*                read       -  Read in an entry.                               *
//*                search     -  Search for a duplicate entry.                   *
//*                add        -  Add a new entry.                                *
//*                erase      -  Delete a record.                                *
//*                change     -  Change one record of a specified entry.         *
//*                printAll   -  Prints all entries.                             *
//*                printSingle-  Prints a single specified entry.                *
//*                Footer     -  Prints the output salutation.                   *
//*                                                                              *
//********************************************************************************


#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
using namespace std;


//***********************************************************************
    // Instantiate a structure to hold customer information
struct CustomerType
{
    char firstName[13];
    char lastName[13];
    char street[21];
    char city[13];
    char state[3];
    char zipCode[6];
};
//***********************************************************************
void Header( ofstream &);
void printAll(ofstream &, CustomerType [], int);
void printSingle(ofstream &, CustomerType [], int);
void read(ifstream &, CustomerType &, string);
int search(CustomerType &, CustomerType [], int, int &);
void add(CustomerType [], CustomerType, int &);
void erase(CustomerType [], int &, int);
void change(ifstream &, CustomerType [], int);
void Footer( ofstream &);

int main()
{
        // Define an input file
    ifstream input("data6.txt",ios::in);
        // Define an output file
    ofstream output("mailinglist.txt",ios::out);
        // Create an array to hold multiple customers information
    CustomerType Customers[50];
        // Create a variable to terminate the program
    bool go=true;
        // Number of customers
    int customerNum=0;
        // Hold transaction type
    string transaction;
        // Create a temporary customer
    CustomerType temp;
        // Number for a specific Customer
    int position=0;
    
    
        // Print out header
    Header(output);
        // Process each entry one at a time
    while (go)
    {
        input >> transaction;
        
        
        
            // Add a record
        if (transaction == "A")
        {
            read(input,temp,transaction);
            
            if (search(temp, Customers, customerNum, position)==1)
            {
                output << "ERROR! Attempt to add a duplicate record. Attempt Failed!" << endl;
                output << "************************************************************"
                       << "**********" << endl;
            }
            else if (search(temp, Customers, customerNum, position)==0)
            {
                add(Customers, temp, customerNum);
                output << "New customer " << setw(5) << Customers[customerNum-1].firstName
                       << Customers[customerNum-1].lastName
                       << " was successfully added to the database!" << endl;
                output << "************************************************************"
                       << "**********" << endl;
            }
        }
        
        
        
        
        
            // Delete a customers information
        else if(transaction == "D")
        {
                // Read input
            read(input, temp, transaction);
                // Search for duplicate
            if (search(temp, Customers, customerNum, position)==1)
            {
                    // Erase customer
                erase(Customers, customerNum, position);
                output << "Customer " << Customers[position].firstName
                       << Customers[position].lastName << " was successfully "
                << "removed from the database" << endl;
                output << "************************************************************"
                       << "**********" << endl;
                
            }
            else if (search(temp, Customers, customerNum, position)==0)
            {
                output << "ERROR! Attempt to delete a non-existent record."
                       << " Attempt Failed!" << endl;
                output << "************************************************************"
                       << "**********" << endl;
            }
        }
        
        
        
        
        
            // Change an existing customers information
        else if (transaction == "C")
        {
                // Read input
            read(input,temp, transaction);
                // Search for duplicate
            if (search(temp, Customers, customerNum, position)==1)
            {
                    // Change information
                change(input, Customers, position);
                output << "Customer " << Customers[position].firstName
                       << Customers[position].lastName << " successfully updated!" << endl;
                output << "************************************************************"
                       << "**********" << endl;
            }
            else if (search(temp, Customers, customerNum, position)==0)
            {
                output << "ERROR! Attempt to change non-existent record."
                       << " Attempt failed!" << endl;
                output << "************************************************************"
                       << "**********" << endl;
            }
        }
        
        
        
        
        
            // Print customer/customers information
        else if (transaction == "P")
        {
            string printType;
            input >> printType;
                // Print All
            if (printType=="E")
            {
                output << setw(38) << "MAILING LIST" << endl;
                output << "Customer" << setw(6) << "Last" << setw(13) << "First" << setw(49)
                       << "Zip" << endl;
                output << "Number" << setw(8) << "Name" << setw(12) << "Name" << setw(17)
                       << "Address" << setw(17) << "City" << setw(10) << "State" << setw(7)
                       << "Code" << endl;
                output << "============================================================"
                       << "====================" << endl;
                    // Print all customers from mailing list
                printAll(output,Customers,customerNum);
                output << "************************************************************"
                       << "**********" << endl;
            }
            else if (printType=="S")
            {
                    // Read input
                read(input, temp, printType);
                    // Search for duplicate
                if (search(temp, Customers, customerNum, position)==1)
                {
                        //Print a single customer
                    printSingle(output, Customers, position);
                }
                    // Search for duplicate
                else if (search(temp, Customers, customerNum, position)==0)
                {
                    output << "ERROR! Attempt to print a non-existent record."
                           << "Attempt failed" << endl;
                    output << "************************************************************"
                           << "**********" << endl;
                }
            }
        }

        
        
        
        // Quit program
        else if (transaction == "Q")
        {
            output << endl << "END OF PROGRAM OUTPUT" << endl;
            go=false;
        }
        
    }
    Footer(output);
    return 0;
}
//***********************************FUNCTIONS********************************************

void read(ifstream & input, CustomerType & temp, string transaction)
{
        // Receives- input file, CustomerType temp
        // Task    - read in one input from input file
        // Returns - input file, CustomerType temp
    if (transaction=="A")
    {
        input >> ws;
        input.getline(temp.firstName,13);
        input >> ws;
        input.getline(temp.lastName,13);
        input >> ws;
        input.getline(temp.street,21);
        input >> ws;
        input.getline(temp.city,13);
        input >> ws;
        input.getline(temp.state,3);
        input >> ws;
        input.getline(temp.zipCode,6);
    }
    else if (transaction=="D")
    {
        input >> ws;
        input.getline(temp.firstName,13);
        input >> ws;
        input.getline(temp.lastName,13);
    }
    else if (transaction=="C" || transaction=="S")
    {
        input >> ws;
        input.getline(temp.lastName,13);
    }
}

//****************************************************************************************

int search(CustomerType & temp, CustomerType Customers[], int customerNum, int & position)
{
        // Receives- temp, Customers array, customerNum, position
        // Task    - searches for a duplicate file record
        // Returns - temp, Customers array, position
    int result=0;
    for (int i=0; i<customerNum; i++)
    {
        if (strcmp(temp.lastName,Customers[i].lastName)==0)
        {
            result=1;
            position=i;
            break;
        }
        else
        {
            result=0;
        }
    }
    return result;
}

//****************************************************************************************

void add(CustomerType Customers[], CustomerType temp, int & customerNum)
{
        // Receives- Customer array, temp, customerNum
        // Task    - add new customer to mailing list
        // Returns - Customers array, temp, customerNum
    Customers[customerNum++]=temp;
}

//****************************************************************************************

void erase(CustomerType Customers[], int & customerNum, int position)
{
        // Receives- Customers array, customerNum, position
        // Task    - remove a customer from the mailing list
        // returns - Customers array, customerNum
    CustomerType empty;
    Customers[position]=empty;
    for (int i=position; i< customerNum; i++)
    {
        Customers[i]= Customers[i+1];
        if(i==customerNum-1)
        {
            Customers[i]=empty;
        }
    }
    customerNum--;
}
//***************************************************************************************

void printAll(ofstream & output, CustomerType Customers[], int customerNum)
{
        // Receives- output file, Customers array
        // Task    - print out all customer mailing records
        // Returns - output file, Customers array
    for (int i=0; i<customerNum; i++)
    {
        output << setw(3) << i+1 << "       " << Customers[i].lastName << setw(2)
               << Customers[i].firstName << setw(1)
               << Customers[i].street << setw(1)
               << Customers[i].city << setw(1)
               << Customers[i].state << setw(10)
               << Customers[i].zipCode << endl;
        
    }
}

//***************************************************************************************

void printSingle(ofstream & output, CustomerType Customers[], int num)
{
        // Receives- output file, Customers array, num
        // Task    - print out a single customer record
        // Returns - output, Customers array
    output << "Customer Number   " << num +1 << endl;
    output << setw(7) << "Name:   " << Customers[num].firstName << " "
           << Customers[num].lastName << setw(15) << "Address:  "
           << Customers[num].street << endl;
    output << "City:   " << Customers[num].city << setw(12)
           << "State:  " << Customers[num].state << setw(14)
           << "Zip Code:" << setw(8) << Customers[num].zipCode << endl;
    output << "************************************************************"
           << "**********" << endl;
}

//***************************************************************************************


void change(ifstream & input, CustomerType Customers[], int position)
{
        // Receives- input file, Customers array, position
        // Task    - change a particular field of a customer mailing record
        // Returns - inout file, Customers array
    int field=0;
    input >> field;
    
    if (field==1)
    {
        input >> ws;
        input.getline(Customers[position].firstName,13);
    }
    else if (field==2)
    {
        input >> ws;
        input.getline(Customers[position].lastName,13);
    }
    else if (field==3)
    {
        input >> ws;
        input.getline(Customers[position].street,21);
    }
    else if (field==4)
    {
        input >> ws;
        input.getline(Customers[position].city,13);
    }
    else if (field==5)
    {
        input >> ws;
        input.getline(Customers[position].state,3);
    }
    else if (field==6)
    {
        input >> ws;
        input.getline(Customers[position].zipCode,6);
    }
    
}


//********************************** HEADER & FOOTER ****************************************
//*******************************************************************************************

void Header( ofstream &outFile)
{
    // Receives- output file
    // Task    - prints the output preamble
    // Returns - nothing
    outFile << setw(30) << "Brendan Ochs";
    outFile << setw(17) << "         ";
    outFile << setw(15) << "          " << endl;
    outFile << setw(30) << "           ";
    outFile << setw(20) << "             " << endl;
    outFile << setw(35) << "-----------------------------------";
    outFile << setw(35) << "-----------------------------------" << endl << endl;
    return;
}

//***************************************FOOTER*******************************************
//****************************************************************************************

void Footer(ofstream &outFile)
{
    // Receives- the output file
    // Task    - prints the output salutation
    // Returns - nothing
    outFile << endl;
    outFile << setw(35) << "---------------------------------" << endl;
    outFile << setw(35) << "|------END OF PROGRAM OUTPUT-------|" << endl;
    outFile << setw(35) << "---------------------------------" << endl;
    
    return;
}





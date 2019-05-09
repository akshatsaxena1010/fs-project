#include<fstream>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iomanip>
#include<iostream>

#define safarifile "saf11.txt"
#define delfile "safdel.txt"
#define recsize 50
#define max 11

using namespace std;

fstream file;
char buffer[60];
char keys[50][11];

class safari
{
	public:
		char bookId[6],name[20],contact[11],type[5],num[3];
		void initial();
		void read();
		int calcCost(int x);
		void pack();
		int getHash(char key[]);
		void display();
		void unpack();
		int search(char skey[]);
		void dispRec(int addr);
		void delRec(int addr);
		void modify(int addr);
		void reset();
};

void safari::initial()
{
	int i;
	char dummy[recsize],  next;
	char check[6];
	file.open(safarifile, ios::in);
	if(!file)
	{
		cout<<"\nUnable to Open the File";
		getch();
		exit(1);
	}
	for(i=0; i<max; i++)
		strcpy(keys[i],"NULL");
	for(i=0; i<max; i++)
	{	
		file.seekg(i*(recsize),ios::beg);
		int f=file.tellg();
		file.getline(check, recsize, '|');
		if(strcmp(check,"###############################################") != 0)
			strcpy(keys[i],check);		
		file.getline(dummy, recsize, '\n');
	}
	file.close();
}

void safari::read()
{	
	char choice;
	int flag = 0,x;
	cin.ignore();
	cout<<"Enter a booking id"<<endl;
	gets(bookId);
	while(search(bookId) >= 0)
	{
		cout<<"\nBooking ID already exists"<<endl;
		cout<<"Enter another booking id: ";
		gets(bookId);
	}
	cout<<"Enter the name"<<endl;
	gets(name);
	cout<<"Enter the Contact Number"<<endl;
	gets(contact);
	cout<<"Enter the type of Safari(Jeep, Boat or Bus)"<<endl;
	while(flag == 0)
	{
		gets(type);
		if((strcmpi(type,"jeep") == 0) || (strcmpi(type,"boat") == 0) || (strcmpi(type,"bus") == 0))
			flag = 1;
		else
			cout<<"Enter one of the choices: Jeep, Boat or Bus"<<endl;
	}
	cout<<"Enter the Number of Passengers: "<<endl;
	flag = 0;
	while(flag == 0)
	{
		gets(num);
		sscanf(num, "%d", &x); 
		if(strcmpi(type,"jeep") == 0)
			if(x<1 || x>6)
				cout<<"The number of Passengers in a Jeep must be between 1 and 6. Try again.."<<endl;
			else
				flag = 1;
		if(strcmpi(type,"bus") == 0)
			if(x<7 || x>20)
				cout<<"The number of Passengers in a Bus must be between 7 and 20. Try again.."<<endl;
			else
				flag = 1;
		if(strcmpi(type,"boat") == 0)
			if(x<1 || x>10)
				cout<<"The number of Passengers in a Boat must be between 1 and 10. Try again.."<<endl;
			else
				flag = 1;
	}
	
	system("cls");
	cout<<"The Details of the Booking are:"<<endl;
	cout<<"Booking ID: "<<bookId<<endl;
	cout<<"Name: "<<name<<endl;
	cout<<"Contact Number: "<<contact<<endl;
	cout<<"Ride Type: "<<type<<endl;
	cout<<"Number of Passengers: "<<x<<endl;
	int cost = calcCost(x);
	cout<<"Total Cost: "<<cost<<endl;
	cout<<endl<<"Do you want to confirm the booking? (Y/N)"<<endl;
	cin>>choice;
	if(choice == 'Y'|| choice == 'y')
	{
		cout<<"Booking Confirmed!"<<endl;
		cout<<"Press any key to continue"<<endl;
		getch();
		pack();
	}
	else
	{
		cout<<"Booking Cancelled!"<<endl;
		cout<<"Press any key to continue"<<endl;
		getch();
		return;
	}
}

int safari::calcCost(int x)
{
	if(strcmpi(type,"jeep") == 0)
		return 500*x;
	if(strcmpi(type,"bus") == 0)
		return 300*x;
	if(strcmpi(type,"boat") == 0)
		return 1000*x;
}

void safari::pack()
{
	int addr;
	addr = getHash(bookId);
	while(strcmp(keys[addr], "NULL") != 0)
	{
		addr++;
		if(addr == max)
		{
			addr = 0;
		}
		if(addr == getHash(bookId))
		{
			cout<<"\nOverflow of Data!";
			return;
		}
	}
	strcpy(keys[addr],bookId);
	strcpy(buffer, bookId);
	strcat(buffer,"|");
	strcat(buffer, name);
	strcat(buffer,"|");
	strcat(buffer, contact);
	strcat(buffer,"|");
	strcat(buffer, type);
	strcat(buffer,"|");
	strcat(buffer, num);
	strcat(buffer,"|");
	file.open(safarifile);
	if(!file)
	{
		cout<<"\nUnable to Open the File";
		getch();
		exit(1);
	}
	file.seekg(addr*recsize, ios::beg);
	file.fill('*');
	file<<setiosflags(ios::left)<<setw(recsize-2)<<buffer<<endl;
	file.close();
}

int safari::getHash(char key[])
{
	int i=0,sum=0;
	while(key[i]!='\0')
	{
		sum=sum+key[i]-48;
		i++;
	}
	return sum % max;
}

void safari::display()
{
	int i, x, cost;
	char dummy[recsize],check[recsize];
	cout<<setw(10)<<"Booking ID"<<setw(20)<<"Name"<<setw(20)<<"Contact No."<<setw(10)<<"Type"<<setw(25)<<"Number of Passengers"<<setw(10)<<"Cost"<<endl;
	file.open(safarifile, ios::in);
	if(!file)
	{
		cout<<"\nUnable to Open the File";
		getch();
		exit(1);
	}	
	for(i=0; i<max; i++)
	{
		file.seekg(i*(recsize),ios::beg);
		int f=file.tellg();
		file.getline(check, recsize, '|');
		if(strcmp(check,"###############################################") != 0)
		{
			strcpy(bookId,check);
			file.getline(name, 20, '|');
			file.getline(contact, 11, '|');
			file.getline(type, 5, '|');
			file.getline(num, 3, '|');
			sscanf(num, "%d", &x); 
			cost = calcCost(x);
			cout<<setw(10)<<bookId<<setw(20)<<name<<setw(20)<<contact<<setw(10)<<type<<setw(25)<<num<<setw(10)<<cost<<endl;
		}
	}
	cout<<"Press any key to continue"<<endl;
	getch();
	file.close();
}

int safari::search(char skey[6])
{
	int addr, i=0, flag=0;
	addr = getHash(skey);
	i = addr;
	do
	{
		if(strcmp(keys[i],skey) == 0)
		{
			flag = 1;
			return i;
		}
		i++;
		if(i == max)
			i=0;
		if(keys[i] == "NULL")
			break;
	}while(i != addr);
	return -1;
}

void safari::dispRec(int addr)
{
	int x;
	file.open(safarifile);
	if(!file)
	{
		cout<<"\nUnable to Open the File";
		getch();
		exit(1);
	}
	file.seekg(addr*(recsize),ios::beg);
	int f=file.tellg();
	file.getline(bookId, 6, '|');
	file.getline(name, 20, '|');
	file.getline(contact, 11, '|');
	file.getline(type, 5, '|');
	file.getline(num, 3, '|');
	cout<<"Booking ID:"<<bookId<<endl;
	cout<<"Name: "<<name<<endl;
	cout<<"Contact Number: "<<contact<<endl;
	cout<<"Ride Type: "<<type<<endl;
	sscanf(num, "%d", &x); 
	cout<<"Number of Passengers: "<<x<<endl;
	int cost = calcCost(x);
	cout<<"Total Cost: "<<cost<<endl;
	file.close();
	cout<<"Press any key to continue....";
	getch();
}

void safari::delRec(int addr)
{
	int j;
	file.open(safarifile);
	if(!file)
	{
		cout<<"\nUnable to Open the File";
		getch();
		exit(1);
	}
	file.seekp(addr*(recsize),ios::beg);
	for(j=0;j<recsize-3;j++)
		file<<"#";
	file<<"|";
	file.close();
	strcpy(keys[addr], "NULL");
}

void safari::reset()
{
	int i,j;
	for(i=0; i<max; i++)
		strcpy(keys[i],"NULL");
	
	file.open(safarifile);
	if(!file)
	{
		cout<<"\nUnable to open the file";
		getch();
		exit(1);
	}
		
	for(i=0;i<max;i++)
	{
		file.seekp(i*(recsize),ios::beg);
		for(j=0;j<recsize-3;j++)
			file<<"#";
		file<<"|";
		file<<"\n";
	}
	cout<<"Empty file created!";
	cout<<"Press any key to continue";
	file.close();
	getch();
}

int main()
{
	safari s;
	s.initial();
	int ch, addr;
	char ex, skey[6];
	for(;;)
	{
		system("cls");
		cout<<"1. Enter a new Booking.\n2. Display all the Bookings.\n3. Search for a Booking. (Using Hash Function)\n4. Modify an existing Booking.\n5. Delete a Booking.\n6. Exit the Program.\n7. Reset. \n";
		cout<<"Enter your Choice: ";
		cin>>ch;
		
		switch(ch)
		{
			case 1:	s.read();
					break;
				
			case 2: s.display();
					break;
					
			case 3: cout<<"\nEnter the Booking ID ";
					cin>>skey;
					addr = s.search(skey);
					if(addr == -1)
					{
						cout<<"\nBooking not found!!"<<endl;
						cout<<"Press any key to continue"<<endl;
						getch();
					}
					else
					{
						cout<<"\nBooking Found!!\n";
						s.dispRec(addr);
					}
					break;
					
			case 4: cout<<"\nEnter the Booking ID to modify: ";
					cin>>skey;
					addr = s.search(skey);
					if(addr == -1)
					{
						cout<<"\nBooking not found!!"<<endl;
						cout<<"Press any key to continue"<<endl;
						getch();
					}
					else
					{
						s.dispRec(addr);
						s.delRec(addr);
						s.read();
						cout<<"\nBooking Modified!"<<endl;
						cout<<"Press any key to continue"<<endl;
						getch();
					}
					break;
					
			case 5: cout<<"\nEnter the Booking ID to delete: ";
					cin>>skey;
					addr = s.search(skey);
					if(addr == -1)
					{
						cout<<"\nBooking not found!!"<<endl;
						cout<<"Press any key to continue"<<endl;
						getch();
					}
					else
					{
						cout<<endl<<"Deleting the following record:"<<endl;
						s.dispRec(addr);
						s.delRec(addr);
						cout<<"\nBooking Deleted!"<<endl;
						cout<<"Press any key to continue"<<endl;
						getch();						
					}
					break;
				
			case 6:	exit(0);
					break;
			
			case 7:	s.reset();
					break;
		}
	}	
	return 0;
}




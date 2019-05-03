#include<fstream>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iomanip>
#include<iostream>

#define safarifile "saf11.txt"
#define recsize 40
#define max 11

using namespace std;

fstream file;
char buffer[60];
char keys[50][11];

class safari
{
	public:
		char bookId[6],name[20];
		void initial();
		void reset();
		void read();
		void pack();
		int getHash(char key[]);
		void display();
		void unpack();
		void search();
		//void retrieve(int addr, char k[]);		
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
	{	
	//	cout<<i<<endl;
		
		//cout<<check<<endl;
		file.seekg(i*(recsize),ios::beg);
		int f=file.tellg();
	//	cout<<"\nf="<<f;
		file.getline(check, recsize, '|');
	//	cout<<check<<endl;
		if(strcmp(check,"#####################################") == 0)
			strcpy(keys[i], "NULL");
		else 
			strcpy(keys[i],check);		
		file.getline(dummy, recsize, '\n');
	}
	file.close();
	
//	for(i=0; i<max; i++)
//		cout<<keys[i]<<endl;
}

void safari::reset()
{
	int i,j;
	for(i=0; i<max; i++)
		strcpy(keys[i],"null");
	
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
		file<<"";
		file<<"\n";
	}
	cout<<"Empty file created!";
	cout<<"Press any key to continue";
	file.close();
	getch();
}

void safari::read()
{	
	char choice;
	cin.ignore();
	cout<<"Enter your booking id"<<endl;
	gets(bookId);
	cout<<"Enter your name"<<endl;
	gets(name);
	system("cls");
	cout<<"The Details of the Booking are:"<<endl;
	cout<<"Booking ID: "<<bookId<<endl;
	cout<<"Name: "<<name<<endl;
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

void safari::pack()
{
	int addr;
	addr = getHash(bookId);
	strcpy(keys[addr],bookId);
	strcpy(buffer, bookId);
	strcat(buffer,"|");
	strcat(buffer, name);
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
	cout<<sum;
	return sum % max;
}

void safari::display()
{
	int i;
	char dummy[recsize];
	cout<<setw(6)<<"bookId"<<setw(20)<<"name"<<endl;
	file.open(safarifile, ios::in);
	if(!file)
	{
		cout<<"\nUnable to Open the File";
		getch();
		exit(1);
	}	
	for(i=0; i<max; i++)
	{
		file.getline(dummy, recsize, '|');
		if(strcmp(dummy,"#####################################") != 0)
		{
			file.getline(bookId, 6, '|');
			file.getline(name, 20, '|');
			cout<<setw(6)<<bookId<<setw(20)<<name<<endl;
			file.getline(dummy, recsize, '|');
		}
		else
			file.getline(dummy, recsize, '|');
	}
	cout<<"Press any key to continue"<<endl;
	getch();
	file.close();
}

void safari::unpack()
{
	char dummy[recsize];
	
	file.getline(dummy, recsize, '\n');
}

void safari::search()
{
	char skey[6];
	int addr, i=0, flag=0;
	cout<<"\nEnter the Booking ID ";
	cin>>skey;
	addr = getHash(skey);
	i = addr;
	do
	{
		if(keys[addr] == skey)
		{
			cout<<"\nBooking Found!!\n";
			unpack();
			cout<<"Booking ID:"<<bookId<<endl;
			cout<<"Name: "<<name<<endl;
			flag = 1;
			cout<<"Press any key to continue....";
			getch();
		}
		i++;
		if(i == recsize)
			i=0;
	}while(i != addr);
	
	if(flag == 0)
	{
		cout<<"\nBooking not Found!!\n";
		cout<<"Press any key to continue....";
		getch();
	}
}

int main()
{
	safari s;
	s.initial();
	int ch;
	char ex;
	for(;;)
	{
		system("cls");
		cout<<"1. Read\n2. Display\n3. Search\n4. Modify\n5. Delete\n6. Exit\n7. Reset\n";
		cout<<"Enter your Choice: ";
		cin>>ch;
		
		switch(ch)
		{
			case 1:	s.read();
					break;
				
			case 2: s.display();
					break;
					
			case 3: s.search();
					break;
				
			case 6: cout<<"\nAre you sure you want to Exit? (Y/N)\n";
					cin>>ex;
					if(ex == 'Y' || ex == 'y')
						exit(0);
					break;
			
			case 7:	s.reset();
					break;
		}
	}	
	return 0;
}




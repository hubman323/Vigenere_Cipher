/*
Author: Rohan Nair
Date:  4/9/20
Description:  This is an encryption program that
uses a vigenere cipher to encrypt or decrypt a text file.
It asks whether the user wants to encrypt or decrypt, takes
in a text file to read from, a key to use in the cipher, and
a text file to write the encrypted or decrypted text
*/


#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

void encryption();              //forward declarations for each function
void decryption();
char* encrypt(char*, char*);
char* decrypt(char*, char*);
void cleanBuffer(char*);
const int NUM = 5000;           // constant number for the size of the buffer


int main(){


	int choice;
	cout << "Welcome to Assignment 4 - Encryption!" << endl;     //welcome menu
	cout << "Pick 1 for encryption or 2 for decryption\n";       //picking 1 will encrypt a file and 2 will decrypt a file
	cin >> choice;

	switch(choice){               //switch statement to determine which void function to jump to
	case 1:
		encryption();             // encryption function
		break;
	case 2:
		decryption();             // decryption function
		break;
	default:
		break;
	}





    return 0;
}



void decryption(){

	int readNum;        //number of successful bytes read
	int writeNum;       // number of successful bytes written
	FILE* rFile;        // file pointer for reading
	FILE* endFile;      // file pointer for writing
	char textFile[20];  //char array to hold reading text file
	char writeFile[20]; // char array to hold writing text file
	char theKey[128];   // char array to hold key
	char buffer[NUM];   // buffer for data

	cleanBuffer(buffer);   //cleans the buffer


	cout << "Enter a file to decrypt: " << endl;
	cin.ignore(10, '\n');     //ignores the new line character
	cin.getline(textFile, 20);   //gets user input for reading text file


	rFile = fopen(textFile, "rb");     //opens the text file for reading

	cout << "Please enter your key: " << endl;
	cin.getline(theKey, 128);   //gets user input for key

	cout << "Enter a file to write into: " << endl;
	cin.getline(writeFile, 20);   //gets user input for writing text file

	endFile = fopen(writeFile, "wb");   //opens the text file for writing

	do{
		readNum = fread(buffer, sizeof(char), NUM, rFile);   //reads in data from file and loads it into buffer
		char* text = new char[readNum];                   //creates a dynamic char array with size of bytes read in
		text = decrypt(theKey, buffer);                    // decrypts the text file
		writeNum = fwrite(text, sizeof(char), readNum, endFile);  //writes out data to file opened for writing
		cout << readNum << endl;   //shows the number of bytes read
		cout << writeNum << endl;  //shows the number of bytes written
		cleanBuffer(buffer);    //cleans the buffer for the next batch of data
		//cleanBuffer(text);
		delete [] text;      // deletes the dynamic char array

	}while(readNum == NUM);   //will keep going until the number of bytes read in does not equal 5000


	fclose(rFile);   //close the read file pointer
	fclose(endFile);  //close the write file pointer


}

char* encrypt(char* key, char* file) {


	int x = strlen(key);       //to be used as the length of the key
	int y = strlen(file);      //to be used as the length of the file
	char a;                    //used to fill up the 2D char array
	char* newBuffer = new char[y];     //creates a new buffer to hold the encrypted text

	//make the viginere table

	char** z = new char*[118];   //create 2D char array with 118 rows for 9 - 127 of ASCII table

	for(int i = 0; i < 118; i++){
		z[i] = new char[x];        //make columns same length as key
	}

	for(int j = 0; j < x; j++){           //nested for loop to fill 2d char array by column
		for(int i = 0; i < 118; i++){
			a = key[j] + i;               //fills out each spot in the array according to the position of the key
										  // added to the row number
			if(i >= (127 - key[j]))       // if i hits 127 - the key position then subtract 118 from a to get the next char
				a -= 118;                 // in the table back to the 9th position of the ASCII table

			z[i][j] = a;                 // spot of the 2D char array equals z

		}
	}


	for(int i = 0, j = 0; i < NUM && file[i] != '\0'; i++, j++){
		if(j >= x)                          //if j is equal to the length of the key reset j to zero
			j = 0;


		char b = z[file[i] - 9][j];     //encrypts the char by subtracting 9 from the character at the file position
										//to be used as the row and the position of the key as the column

		newBuffer[i] = b;              //puts encrypted char into new buffer
	}





	for(int i = 0; i < 118; i++){
		delete [] z[i];            //deletes each row of the 2D char array
	}



	delete [] z;                  //deletes the whole 2D char array



	return newBuffer;             // returns the encrypted buffer

	delete [] newBuffer;


}

void encryption(){



				int numReads;   //for the number of bytes read into the buffer
				int numWrites;  //for the number of bytes written out
				int count = 0;  //counts how many times it enters the do-while loop
				FILE* file_;    //file pointer to read in the file
				FILE* wFile;    //file pointer to write out the file
				char readArray[20];  //char array to hold the name of the text file to be read
				char writeArray[20]; //char array to hold the name of the text file to be written
				char myKey[128];    //char array to hold the key
				char buffer[NUM];   //buffer to store the data



				cleanBuffer(buffer);   //cleans the buffer


				cout << "Enter a file to encrypt: \n";
				cin.ignore(100, '\n');    //ignores the new line character
				cin.getline(readArray, 20);   //for user input to enter in the read text file

				file_ = fopen(readArray, "rb");   //opens the file for reading

				cout << "Please enter your key: \n";
				cin.getline(myKey, 128);      //for user input to enter in the key

				cout << "Enter a file to write into: \n";
				cin.getline(writeArray, 20);   //for user input to enter in the write text file

				wFile = fopen(writeArray, "wb");  //opens the file for writing


				do{
					count++;  //increments the count of how many times it enters
					numReads = fread(buffer, sizeof(char), NUM, file_);  //reads data into the buffer
					char* test = new char[numReads];  //makes a dynamic char array the size of the number of bytes read
					test = encrypt(myKey, buffer);  //encrypts the text in the buffer
					numWrites = fwrite(test, sizeof(char), numReads, wFile);  //writes out the encrypted text to the file
					cout << numReads << endl;  //number of bytes it reads
					cout << numWrites << endl; //number of bytes it writes

					cleanBuffer(buffer);  //cleans the buffer
					delete [] test;  //deletes the dynamic char array
				}while(numReads == NUM); //will keep going until numReads is not equal to 5000


				cout << count << endl;  //number of times it enters the loop


				fclose(file_);  //closes the read file pointer
				fclose(wFile);  //closes the write file pointer

}

void cleanBuffer(char* buffer){

	for(int i = 0; i < NUM; i++){
		buffer[i] = '\0';          //fills the whole buffer with the null character
	}
}

char* decrypt(char* key, char* text) {

	int a = strlen(key);   //used as the length of the key
	int b = strlen(text);  //used as the length of the file

	char* decrypText = new char[b];  //makes a dynamic char array the size of the file

	for(int i = 0, j = 0; i < b; i++, j++){
		if(j >= a)                         //if j is equal to the size of the key reset j to 0
			j = 0;

		decrypText[i] = (((text[i] - key[j]) + 118) % 118) + 9;  //formula to decrypt the text
	}



	return decrypText;    //returns the decrypted text in a dynamic char array

	delete [] decrypText;

}











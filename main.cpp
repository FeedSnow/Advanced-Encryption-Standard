#include <iostream>
#include <string>
#include "rijndael.h"

#define DELIMITER "||"

using namespace std;

string ReadText()
{
	it = 0;
	string message = "";
	system("cls");
	cout << "Input message to encode (delimiter " << DELIMITER << "):" << endl;
	cout << "(Delimiter works only when put on the end of a row)" << endl << endl;

	bool cont = true;
	while (cont)
	{
		string tmp;
		getline(cin, tmp);
		if (tmp.size() > 1 && (tmp[tmp.size() - 1] == '|' && tmp[tmp.size() - 2] == '|'))
		{
			cont = false;
			tmp.erase(tmp.size() - 2, 2);
		}

		if (message.size() > 0)
			message += '\n';
		message += tmp;
	}

	system("cls");
	cout << endl;
	cout << "Message:" << endl;
	cout << message << endl;
	return message;
}

int main()
{
	Key key = InitKey();
	string message = ReadText();

	Encryption(key, message);
	cout << endl << endl;
	Decryption(key, message);

	cout << endl << endl;
	system("pause");

	return 0;
}
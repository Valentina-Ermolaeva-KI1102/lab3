#include <fstream>
#include <iostream>

using namespace std;

int pow(int a, int b)
{
	int rez = 1;
	for(int i=1; i<=b; i++)
		rez = rez * a;
	return rez;
}

int main(int argv, char **argc)
{
	setlocale(LC_ALL, "rus");
	cout<<"Введите:\n1-сокрытия информации\n2-извлечение информации\n";
	int menu;
	do 
	{
		cin>>menu;
	} while (menu != 1 && menu != 2);
	if(menu == 1)
	{
		ifstream text("text.txt", ios_base::in); // файл с текстом
		fstream wav("out.mp3",ios_base::binary | ios_base::out | ios_base::in); // файл со звуком
		unsigned char buf, Mask[5] = {3, 12, 48, 192, 252};

		// смещение от заголовка
		wav.seekg(84, ios_base::beg);

		char bufTxt, bufWr;
		int len = 0;
		menu = wav.tellg();
		while(true)
		{
			text.get(bufTxt);
			if(text.eof())
				break;
			len++;
			for(int i=0; i<4; i++)
			{
				buf = bufTxt & Mask[i];
				for(int j=0; j < i; j++)
					buf>>=2;
				//
				wav.read(&bufWr, 1);
				menu = wav.tellg();
				bufWr &= Mask[4];
				bufWr |= buf;
				wav.seekg(-1, ios::cur);
				menu = wav.tellg();
				wav.write(&bufWr, 1);
				menu = wav.tellg();
				//
			}
		}
		unsigned char lengTxt[4];
		int bufLen, mask_count = 255;
		for(int i=0; i<4; i++)
		{
			bufLen = len & mask_count;
			mask_count *= 256;
			for(int j=0; j<i; j++)
				bufLen>>=8;
			lengTxt[i] = bufLen;
		}
		wav.seekg(64, ios_base::beg);
		for (int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				buf = lengTxt[i] & Mask[j];
				for(int k=0; k < j; k++)
					buf>>=2;
				//
				wav.read(&bufWr, 1);
				bufWr &= Mask[4];
				bufWr |= buf;
				wav.seekg(-1, ios::cur);
				wav.write(&bufWr, 1);
			}
		}
		text.close();
		wav.close();
		return 0;
	}
	else
	{
		ofstream text("text_out.txt", ios_base::out); //файл с текстом.
		ifstream wav("out.mp3",ios_base::binary | ios_base::in); //файл со звууком

		unsigned char buf, Mask[5] = {3, 12, 48, 192, 252}, charTxt[4];
		char bufTxt, bufWav;
		int lenght = 0, bufLen, len = 0;
		
		wav.seekg(64, ios_base::beg);
		for(int i=0; i<4; i++)
		{
			charTxt[i] = 0;
			for(int j=0; j<4; j++)
			{
				wav.read(&bufTxt, 1);
				buf =  bufTxt & Mask[0];
				for(int k=0; k < j; k++)
					buf <<= 2;
				charTxt[i] |= buf;
			}
		}
		for(int i=0; i<4; i++)
		{
			bufLen = charTxt[i];
			for(int j=0; j<i; j++)
				bufLen<<=8;
			lenght |= bufLen;
		}

		// from begin for write
		wav.seekg(84, ios_base::beg);
		while(len < lenght)
		{
			bufTxt = 0;
			for(int j=0; j<4; j++)
			{
				wav.read(&bufWav, 1);
				buf = bufWav & Mask[0];
				for(int k=0; k < j; k++)
					buf <<= 2;
				bufTxt |=buf;
			}
			len++;
			text<<bufTxt;
		}
		text.close();
		wav.close();
		return 0;
	}
}

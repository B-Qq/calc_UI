// calc_UI.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

int IntStrToBcd(const std::string _str)
{
	int _i = 0;
	int _len = 0;
	int _num = 0;
	int _val = 0;

	_len = _str.size();
	if (_len <= 0)
	{
		return 0;
	}
	_i = 0;
	if (_str[0] == '-')
	{
		_i = 1;
	}
	while (_i < _len)
	{
		if ((_str[_i] >= 'A') && (_str[_i] <= 'F'))
		{
			_val = (int)(_str[_i] - 'A' + 10);
		}
		else
		{
			if ((_str[_i] >= 'a') && (_str[_i] <= 'f'))
			{
				_val = (int)(_str[_i] - 'a' + 10);
			}
			else
			{
				_val = (int)(_str[_i] - '0');
			}
		}

		_num = (_num << 4) + _val;
		_i++;
	}
	if (_str[0] == '-')
	{
		_num *= -1;
	}

	return _num;
};

//ʮ�������ַ�����ʮ��������
std::string HexStrToHex(const std::string _str, const unsigned int _size)
{
	std::string _vstr;
	_vstr = _str;
	std::string _rstr;
	unsigned char _ch;
	while (_vstr.size() < _size * 2)
	{
		_vstr = _vstr + '0';
	}
	for (unsigned int _i = 0; _i < _size; _i++)
	{
		_ch = IntStrToBcd(_vstr.substr(_i * 2, 2));
		_rstr.append(1, _ch);
	}

	return _rstr;
};

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream infile;
	ifstream infile_temp;
	ofstream outfile_u;
	ofstream outfile_u_end;
	ofstream outfile_i;
	ofstream outfile_i_end;
	ofstream outfile_temp;
	ifstream infile_u;
	ifstream infile_i;

	ofstream outfile_i_end_value;
	ofstream outfile_u_end_value;

	ofstream outfile_u_excel;
	ofstream outfile_i_excel;

	ifstream infile_begin;
	ofstream outfile_begin;

	system("md only_value");
	system("md key_value");
	system("md excel");
	infile_begin.open("file.txt");
	outfile_begin.open("file_temp.txt");

	outfile_u.open("u.txt");
	outfile_i.open("i.txt");
	ofstream outfile_other;
	outfile_other.open("other.txt");
	outfile_temp.open("temp.txt");
	int index = 0;
	string _u_b;
	cout << "��ʼ��������..............." << endl;
	while (!infile_begin.eof())
	{
		getline(infile_begin,_u_b);
		if (_u_b[0] != '[')
		{
			//cout << _u_b << endl;
			continue;
		}
		else
		{
			_u_b.erase(0,1);
			_u_b.erase(_u_b.end() -2  ,_u_b.end());
			outfile_begin << _u_b;
		}
	}

	outfile_begin.close();
	infile_begin.close();
	infile.open("file_temp.txt");   //���ļ����������ļ��������� 
	assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 

	string _str_temp;
	while (!infile.eof())
	{
		index = 0;
		char _str_t[401] = {};
		infile.read(_str_t, 400);
		_str_temp = _str_t;
		if (!_str_temp.empty())
		{
			while ((index = _str_temp.find(' ', index)) != string::npos)
			{
				_str_temp.erase(index, 1);
			}
		}
		outfile_temp << _str_temp;
	}
	outfile_temp.close();
	infile.close();

	cout << "��ʼ��������..............." << endl;
	cout << "�������ݴ���i.txt" << endl;
	cout << "��ѹ���ݴ���u.txt" << endl;
	infile_temp.open("temp.txt");   //���ļ����������ļ��������� 

	char _str[9] = {};
	char temp[20] = {};

	infile_temp.seekg(0, ios::end);  
	int length = infile_temp.tellg(); 
	infile_temp.seekg(0, ios::beg);

	while (!infile_temp.eof())
	{
		infile_temp.read(_str, 8);
		//cout <<hex<< "_str:" << _str << endl;
		//outfile_u << _str << endl;
		if (_str[6] == '0' && (_str[7] == '0' || _str[7] == '1'))
		{
			outfile_u << _str << endl;
		}
		else if (_str[6] == '1' && (_str[7] == '0' || _str[7] == '1'))
		{
			outfile_i << _str << endl;
		}
		else
		{
			infile_temp.seekg(-6, ios::cur);
			//outfile_other << _str << endl;
		}
	}

	infile_temp.close();             //�ر��ļ������� 
	outfile_u.close();
	outfile_i.close();

	double sum_u = 0;
	double sum_i = 0;
	cout << "��ѹ���ݼ���......." << endl;
	infile_u.open("u.txt");   //���ļ����������ļ��������� 
	outfile_u_end.open("./key_value/u_end.txt");   //���ļ����������ļ��������� 
	outfile_u_end_value.open("./only_value/u_end_value.txt");
	outfile_u_excel.open("./excel/u.csv");
	char _u[9] = {};
	string ss = "";
	unsigned char *tt = {};
	char _tempbuf[30] = {};
	//��ѹ����
	while (!infile_u.eof())
	{
		sum_u = 0;
		infile_u.getline(_u, 9);
		if (strlen(_u) < 8)
		{
			continue;
		}
		ss = HexStrToHex(_u, 4);
		if (ss[0] & 0xc0)
		{
			ss[0] = ~(ss[0] & 0x3f);
			ss[1] = ~ss[1];
			ss[2] = ~(ss[2] - 1);
			ss[0] = (ss[0] & 0x3f);
			tt = (unsigned char *)ss.c_str();
			//16����ת16�����ַ�
			for (int _i = 0; _i < 3; _i++)
			{
				sprintf(&_tempbuf[_i * 2], "%02x", tt[_i]);
			}
			//cout << "tempbuf::" << _tempbuf << endl;
			for (int i = 6 - 1; i >= 0; i--)//��ʮ�����Ƹ�λ��ʼ��ÿλ��ת����ʮ����
			{
				//cout << "_tempbuf:::" << _tempbuf[i] << endl;
				if (_tempbuf[i] >= '0'&&_tempbuf[i] <= '9')//�����ַ���ת��
				{
					sum_u += (_tempbuf[i] - 48)*pow(16, 6 - i - 1);
				}
				else if (_tempbuf[i] >= 'A'&&_tempbuf[i] <= 'F')//��ĸ�ַ���ת��
				{
					sum_u += (_tempbuf[i] - 55)*pow(16, 6 - i - 1);
				}
				else if (_tempbuf[i] >= 'a'&&_tempbuf[i] <= 'f')
				{
					sum_u += (_tempbuf[i] - 87)*pow(16, 6 - i - 1);
				}
			}//for_end

			sum_u = sum_u * 0.00678;
			outfile_u_end << _u << "  -" << sum_u << endl;
			outfile_u_end_value << "-" << sum_u << endl;
			outfile_u_excel << "-" << sum_u << "," << endl;
		}//if_end
		else
		{
			for (int i = 6 - 1; i >= 0; i--)//��ʮ�����Ƹ�λ��ʼ��ÿλ��ת����ʮ����
			{
				if (_u[i] >= '0'&&_u[i] <= '9')//�����ַ���ת��
				{
					sum_u += (_u[i] - 48)*pow(16, 6 - i - 1);
				}
				else if (_u[i] >= 'A'&&_u[i] <= 'F')//��ĸ�ַ���ת��
				{
					sum_u += (_u[i] - 55)*pow(16, 6 - i - 1);
				}
			}//for_end

			sum_u = sum_u * 0.00678;
			outfile_u_end << _u << "  " << sum_u << endl;
			outfile_u_end_value << sum_u << endl;
			outfile_u_excel << sum_u << "," << endl;
		}//else_end
	} //while_end
	infile_u.close();
	outfile_u_end.close();
	outfile_u_end_value.close();
	outfile_u_excel.close();


	ss = "";
	tt = {};
	cout << "�������ݼ���......." << endl;
	infile_i.open("i.txt");   //���ļ����������ļ��������� 
	outfile_i_end.open("./key_value/i_end.txt");   //���ļ����������ļ��������� 
	outfile_i_end_value.open("./only_value/i_end_value.txt");   
	outfile_i_excel.open("./excel/i.csv");
	char _i[9] = {};

	//��������
	while (!infile_i.eof())
	{
		sum_i = 0;
		infile_i.getline(_i, 9);
		ss = HexStrToHex(_i, 4);

		if (strlen(_i) < 8)
		{
			continue;
		}
		/*for (int i = 0; i <= 3; i++)
		{
			printf("%02x ",ss[i]);
		}
		printf("\n--------\n");*/
		//printf("ss[0]::%02x\n",ss[0]);
		if (ss[0] & 0xc0)
		{
			ss[0] = ~(ss[0] & 0x3f);
			ss[1] = ~ss[1];
			ss[2] = ~(ss[2] - 1);
			ss[0] = (ss[0] & 0x3f);
			tt = (unsigned char *)ss.c_str();
			//16����ת16�����ַ�
			for (int _i = 0; _i < 3; _i++)
			{
				sprintf(&_tempbuf[_i * 2], "%02x", tt[_i]);
			}
			//cout << "tempbuf::" << _tempbuf << endl;
			for (int i = 6 - 1; i >= 0; i--)//��ʮ�����Ƹ�λ��ʼ��ÿλ��ת����ʮ����
			{
				//cout << "_tempbuf:::" << _tempbuf[i] << endl;
				if (_tempbuf[i] >= '0'&&_tempbuf[i] <= '9')//�����ַ���ת��
				{
					sum_i += (_tempbuf[i] - 48)*pow(16, 6 - i - 1);
				}
				else if (_tempbuf[i] >= 'A'&&_tempbuf[i] <= 'F')//��ĸ�ַ���ת��
				{
					sum_i += (_tempbuf[i] - 55)*pow(16, 6 - i - 1);
				}
				else if (_tempbuf[i] >= 'a'&&_tempbuf[i] <= 'f')
				{
					sum_i += (_tempbuf[i] - 87)*pow(16, 6 - i - 1);
				}
			}//for_end

			sum_i = sum_i * 0.0000356;
			outfile_i_end << _i << "  -" << sum_i << endl;
			outfile_i_end_value << "-" << sum_i << endl;
			outfile_i_excel << "-" << sum_i << "," << endl;
		}//if_end
		else
		{
			for (int i = 6 - 1; i >= 0; i--)//��ʮ�����Ƹ�λ��ʼ��ÿλ��ת����ʮ����
			{
				if (_i[i] >= '0'&&_i[i] <= '9')//�����ַ���ת��
				{
					sum_i += (_i[i] - 48)*pow(16, 6 - i - 1);
				}
				else if (_i[i] >= 'A'&&_i[i] <= 'F')//��ĸ�ַ���ת��
				{
					sum_i += (_i[i] - 55)*pow(16, 6 - i - 1);
				}
			}//for_end
			sum_i = sum_i * 0.0000356;
			outfile_i_end << _i << "  " << sum_i << endl;
			outfile_i_end_value << sum_i << endl;
			outfile_i_excel << sum_i << "," << endl;
		}//else_end

	}
	infile_i.close();
	outfile_i_end.close();
	outfile_i_end_value.close();
	outfile_i_excel.close();
	cout << "��������������ݴ���i_end.txt" << endl;
	cout << "��ѹ����������ݴ���u_end.txt" << endl;
	remove("./temp.txt");
	remove("./i.txt");
	remove("./u.txt");
	remove("./file_temp.txt");
	remove("./other.txt");
	system("pause");

	return 0;
}

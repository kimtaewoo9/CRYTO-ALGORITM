#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


unsigned char sub_table[16][16] = { {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76},
									{0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0},
									{0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15},
									{0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75},
									{0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84},
									{0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf},
									{0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8},
									{0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2},
									{0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73},
									{0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb},
									{0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79},
									{0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08},
									{0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a},
									{0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e},
									{0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf},
									{0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16} };

unsigned char Invsub_table[16][16] = { {0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb},
									{0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb},
									{0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e},
									{0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25},
									{0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92},
									{0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84},
									{0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06},
									{0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b},
									{0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73},
									{0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e},
									{0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b},
									{0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4},
									{0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f},
									{0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef},
									{0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61},
									{0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d} };

unsigned char Constant[4][4] = { {0x02,0x03,0x01,0x01},
									{0x01,0x02,0x03,0x01},
									{0x01,0x01,0x02,0x03},
									{0x03,0x01,0x01,0x02} };

unsigned char InvConstant[4][4] = { {0x0e,0x0b,0x0d,0x09},
									{0x09,0x0e,0x0b,0x0d},
									{0x0d,0x09,0x0e,0x0b},
									{0x0b,0x0d,0x09,0x0e} };

unsigned char RCon[4][10] = { {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36},
									{0,0,0,0,0,0,0,0,0,0},
									{0,0,0,0,0,0,0,0,0,0},
									{0,0,0,0,0,0,0,0,0,0} };
unsigned char Round_Key[4][60];

unsigned char last_num[7][4][4]={ {{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}} };

void Insert_String(char str[]);
void Insert_Hex();
void Insert_Pre_Round(int sel);
void KeyExpansion(int sel);
void SubBytes(int round);
void ShiftRows(int round);
void MixColumns(int round);
void AddRoundKey(int count, int round);
void InvMixColumns(int round);
void InvShiftRows(int round);
void InvSubBytes(int round);
unsigned char GF_mul(unsigned char a, unsigned char b);
void Hex_to_String(char change[], int len);
void String_to_Hex(char str[], int len);
void Print_Hex();


int main(){
	char str[100]={0};
	char change[100]={0};
	int count=0;
	int round=0;
	int num=0, sel=0, etc;
	int len=0;
	int backup = 0;
	int i = 0;

	printf("Input PlainText : ");
	gets(str);
	fflush(stdin);
	len = strlen(str);
	String_to_Hex(str, len);
	round = len/16+1;
	printf("\n-------------------------------------------------------------------------------\n");
	
	printf("Round (1. 10ȸ  2. 12ȸ  3. 14ȸ) : ");
	scanf("%d", &sel);

	switch (sel) {
	case 1: etc=4; num=9;
		printf("16Byte Key : ");
		break;
	case 2: etc=6; num=11;
		printf("24Byte Key : ");
		break;
	case 3: etc=8; num=13;
		printf("32Byte Key : ");
		break;
	}
	Insert_Pre_Round(sel);
	KeyExpansion(sel);
	printf("\n-------------------------------------------------------------------------------\n");
	
	for (i=0; i<round;i++) {
		count=0;
		AddRoundKey(0, i);
		while (count++ < num) {
			printf("-----------------------------Encryption Round %d------------------------------\n", count);
			SubBytes(i);
			ShiftRows(i);
			MixColumns(i);
			AddRoundKey(count, i);
		}
		printf("-----------------------------Encryption Round %d-----------------------------\n", count);
		SubBytes(i);
		ShiftRows(i);
		AddRoundKey(count, i);
		if (i != round - 1)
			printf("---------------------------16Byte Encryption Finsh---------------------------\n");
	}
	
	printf("-------------------------------Encryption Finsh--------------------------------");
	Hex_to_String(change, len);
	printf("\nCipher Text : %s", change);
	printf("-------------------------------------------------------------------------------\n");
	system("pause");
	
	backup=count;
	for (i=0; i<round; i++) {
		AddRoundKey(count, i);
		while (count-- > 1) {
			printf("------------------------------Decryption Round %d-----------------------------\n", num-count+1);
			InvShiftRows(i);
			InvSubBytes(i);
			AddRoundKey(count, i);
			InvMixColumns(i);
		}
		printf("------------------------------Decryption Round %d-----------------------------\n", num-count+1);
		InvShiftRows(i);
		InvSubBytes(i);
		AddRoundKey(0, i);
		count = backup;
		if (i != round-1)
			printf("---------------------------16Byte Decryption Finsh---------------------------\n");
	}
	
	printf("-------------------------------Decryption Finsh--------------------------------\n");
	Hex_to_String(change, len);
	printf("PlainText : %s", change);
	printf("-------------------------------------------------------------------------------\n");
	system("pause");

}
void Insert_String(char string[]) {
	int i, j;

	printf("Input PlainText : ");
	gets(string);

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			last_num[0][j][i] = string[i*4 + j];
		}
	}
}

void Insert_Hex() {
	int i, j, k;
	unsigned char num[2];
	char word[17][3];

	printf("Input Hex Plain Text : ");
	for (i=0; i<16; i++) {
		scanf("%s", word[i]);
	}
	
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			for (k=0; k<2; k++) {
				if (word[i*4 + j][k] >= '0' && word[i*4 + j][k] <= '9') {
					num[k] = word[i*4 + j][k] - '0';
				}
				else if (word[i*4 + j][k] >= 'a' && word[i*4 + j][k] <= 'f') {
					num[k] = (word[i*4 + j][k] - 'a') + 10; 
				}
			}
			last_num[0][j][i] += num[0] * 16;
			last_num[0][j][i] += num[1];
		}
	}
}
void Insert_Pre_Round(int sel) {
	char key[33][3];
	unsigned char num[2];
	int i, j, k;
	int size;
	int round=0;
	int round_cnt;
	int count=0;

	switch (sel) {
	case 1: size=16;
		round_cnt=1;
		break;
	case 2: size=24;
		round_cnt=2;
		break;
	case 3: size=32;
		round_cnt=2;
		break;
	}
	for (i=0; i < size; i++) {
		scanf("%s", key[i]);
	}
	for (round=0; round < round_cnt; round++) {
		for (i=0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (count == size) { break; }
				for (k = 0; k < 2; k++) {
					if (key[round*16 + i*4 + j][k] >= '0' && key[round*16 + i*4 + j][k] <= '9') {
						num[k] = key[i*4 + j][k] - '0';
					}
					else if (key[round*16 + i*4 + j][k] >= 'a' && key[round*16 + i*4 + j][k] <= 'f') {
						num[k] = (key[i*4 + j][k] - 'a') + 10;
					}
				}
				Round_Key[j][round*4 + i] += num[0] * 16;
				Round_Key[j][round*4 + i] += num[1];
				count++;
			}
		}
	}
}
void KeyExpansion(int sel) {
	int count=0;
	int i;
	int size=0;
	int mod=0;
	int start=0;
	int temp[4];

	switch (sel) {
	case 1: size=44; mod=4; start=4; count=0;
		break;
	case 2: size=52; mod=6; start=6; count=1;
		break;
	case 3: size=60; mod=8; start=8; count=1;
		break;
	}

	while (start < size) {
		if ((sel == 3) && (start%4 == 0) && (start%mod != 0)) {
			for (i=0; i<4; i++) {
				Round_Key[i][start] = sub_table[Round_Key[i][start-1] / 16][Round_Key[i][start-1] % 16] ^ Round_Key[i][start-mod];
			}
		}
		else if (start % mod == 0) {
			temp[3]=Round_Key[0][start-1];
			temp[0]=Round_Key[1][start-1];
			temp[1]=Round_Key[2][start-1];
			temp[2]=Round_Key[3][start-1];
			for (i=0; i<4; i++) {
				temp[i]=sub_table[temp[i] / 16][temp[i] % 16];
				Round_Key[i][start]=Round_Key[i][start - mod] ^ (temp[i] ^ RCon[i][count]);
			}
			count++;
		}
		else{
			for (i=0; i<4; i++) {
				Round_Key[i][start]=Round_Key[i][start-1]^Round_Key[i][start-mod];
			}
		}
		start++;
	}
	printf("\n-------------------------------------------------------------------------------\n");
	printf("KeyExpansion");

}
void SubBytes(int round) {
	int i, j;
	printf("SubBytes\t <");
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			last_num[round][i][j] = sub_table[last_num[round][i][j]/16][last_num[round][i][j]%16];
		}
	}
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf("%02x ", last_num[round][j][i]);
		}
	}
	printf(">\n");
}
void InvSubBytes(int round) {
	int i, j;
	
	printf("InvSubBytes\t <");
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			last_num[round][i][j]=Invsub_table[last_num[round][i][j] / 16][last_num[round][i][j]%16];
		}                
	}					 
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf("%02x ", last_num[round][j][i]);
		}
	}
	printf(">\n");
}
void ShiftRows(int round) {
	int i, j, k;
	unsigned char temp;
	//ShiftRows
	printf("ShiftRows\t <");
	for (i=0; i<4; i++) {
		for (k=0; k<i; k++) {
			for (j=0; j<3; j++) {
				temp = last_num[round][i][j];
				last_num[round][i][j] = last_num[round][i][j+1];
				last_num[round][i][j+1] = temp;
			}
		}
	}
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf("%02x ", last_num[round][j][i]);
		}
	}
	printf(">\n");
}
void InvShiftRows(int round) {
	int i, j, k;
	unsigned char temp;
	
	printf("InvShiftRows\t <");
	for (i=0; i<4; i++) {
		for (k=0; k<i; k++) {
			for (j=3; j>0; j--) {  
				temp = last_num[round][i][j];
				last_num[round][i][j] = last_num[round][i][j-1];
				last_num[round][i][j-1] = temp;
			}
		}
	}
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf("%02x ", last_num[round][j][i]);
		}
	}
	printf(">\n");
}
void MixColumns(int round) {
	int i, j, k;
	unsigned char temp = 0;
	unsigned char box[4][4];
	//MixColumns
	printf("MixColumns\t <");
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			for (k =0; k<4; k++) {
				temp^=GF_mul(last_num[round][k][i], Constant[j][k]);
			}
			box[j][i]=temp;
			temp=0;
		}
	}

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			last_num[round][j][i] = box[j][i];
			printf("%02x ", last_num[round][j][i]);
		}
	}
	printf(">\n");
}
void InvMixColumns(int round) {
	int i, j, k;
	unsigned char temp = 0;
	unsigned char box[4][4];
	//InvMixColumns
	printf("InvMixColumns\t <");
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			for (k=0; k<4; k++) {
				temp^=GF_mul(last_num[round][k][i], InvConstant[j][k]);
			}
			box[j][i] = temp;
			temp = 0;
		}
	}

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			last_num[round][j][i] = box[j][i];
			printf("%02x ", last_num[round][j][i]);
		}
	}
	printf(">\n");
}
void AddRoundKey(int count, int round) {
	int i, j;
	printf("AddRoundKey\t <");
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) { 
			last_num[round][i][j]^=Round_Key[i][count*4+j];
		}
	}
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf("%02x ", last_num[round][j][i]);
		}
	}
	printf(">\n");
}
unsigned char GF_mul(unsigned char a, unsigned char b) {
	unsigned char result=0;
	unsigned char temp=0;
	int i=0;

	for (i=0; i<8; i++) {
		if ((b & 0x01) == 0x01)
			temp ^= (a & 0xff);
		if (a >= 0x80) {
			a <<= 1;
			a ^= 0x1b;
		}
		else{
			a <<= 1;
		}
		if (b == 1) {
			result = temp;
			break;
		}
		b >>= 1;
	}

	return result;
}
void Hex_to_String(char change[], int len) {
	int i, j, k;
	for (k=0; k<7; k++) {
		for (i=0; i<4; i++) {
			for (j=0; j<4; j++) {
				if (k*16+i*4+j >= len) {
					change[k*16 + i*4 + j] = '\n';
					break;
				}
				else {
					change[k*16 + i*4 + j] = last_num[k][j][i];
				}
			}
		}
	}
	change[99] = '\n';
}
void String_to_Hex(char str[], int len) {
	int i;

	for (i=0; i<len; i++) {
		last_num[i / 16][i % 4][(i % 16) / 4] = str[i];
	}
	for (; i<100; i++) {
		last_num[i/16][i%4][(i%16)/4] = 0;
	}
}
void Print_Hex() {
	int i, j;
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf("%02x ", last_num[j][i]);
		}
	}
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "SHA256.h"

const uint32_t HashStart[8] = {0x6a09e667, 0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
//32bits word사용
const uint32_t K_const[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
							0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
							0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
							0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
							0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
							0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
							0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
							0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
							0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
							0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
							0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
							0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
							0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
							0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
							0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
							0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

enum TmpH
{
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h
};

static uint32_t sigma1(uint32_t x)
{
    uint32_t RotateRight17, RotateRight19, ShiftRight10;

    RotateRight17 = (x >> 17) | (x << 15);
    RotateRight19 = (x >> 19) | (x << 13);
    ShiftRight10 = x >> 10;

    return RotateRight17 ^ RotateRight19 ^ ShiftRight10;
}
//워드 단위 계산이므로 자료형을 uint32_t로 선언해준다.
static uint32_t sigma0(uint32_t x)
{
    uint32_t RotateRight7, RotateRight18, ShiftRight3;

    RotateRight7 = (x >> 7) | (x << 25);
    RotateRight18 = (x >> 18) | (x << 14);
    ShiftRight3 = (x >> 3);

    return RotateRight7 ^ RotateRight18 ^ ShiftRight3;
}
//워드 단위 계산이므로 자료형을 uint32_t로 선언해준다.
//x가 1이면 y 선택 x가 0이면 z선택..
//x&y x,y둘다 1일때만 값이 1이됨 x가 1이면 y값이 선택됨 x가 0이면 무조건 0
//-x&z x가 0이고 z가 1일때만 1이됨 x가 0이면 z값이 선택됨 x가 1이면 무조건 0
//(x&y) ^ ((-x)&z) XOR 연산 XOR연산은 두비트 다를때 1이됨

static uint32_t choice(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ ((~x) & z);
}

//워드 단위 계산이므로 자료형을 uint32_t로 선언해준다.
static uint32_t BigSigma1(uint32_t x)
{
    uint32_t RotateRight6, RotateRight11, RotateRight25;

    RotateRight6 = (x >> 6) | (x << 26);
    RotateRight11 = (x >> 11) | (x << 21);
    RotateRight25 = (x >> 25) | (x << 7);

    return RotateRight6 ^ RotateRight11 ^ RotateRight25;
}

static uint32_t BigSigma0(uint32_t x)
{
    uint32_t RotateRight2, RotateRight13, RotateRight22;

    RotateRight2 = (x >> 2) | (x << 30);
    RotateRight13 = (x >> 13) | (x << 19);
    RotateRight22 = (x >> 22) | (x << 10);

    return RotateRight2 ^ RotateRight13 ^ RotateRight22;
}

// 0과 1중 개수가 더 많은것 선택..
static uint32_t major(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}
// x,y,z 중에서 2개이상의 값이 해당 비트에서 1일때 해당비트를 1로 유지함
// 그외의 경우 0으로 만든다.

static uint32_t int_ratio_ceil(uint64_t Numerator, uint64_t Denominator)
{
    if ((Numerator % Denominator) != 0)
    {
        return (Numerator / Denominator) + 1;
        // 나머지 0아니면 올림
    }
    else
    {
        return Numerator / Denominator;
        // 나머지 0이면 그대로 return
    }
} // 해당 비율의 값을 정수형으로 반환함.

static uint32_t bigendian_32(uint8_t* x){

    return (uint32_t)x[3] | ((uint32_t)x[2] << 8) | ((uint32_t)x[1] << 16) | ((uint32_t)x[0] << 24);
}

static void store_bigndian_32(uint8_t* x, uint32_t u){
    //32비트 u값을 x배열에 넣어준다.
    //little endian : 78 56 34 12
    //big endian : 12 34 56 78
    x[3] = (uint8_t)u;
    u >>= 8;
    x[2] = (uint8_t)u;
    u >>= 8;
    x[1] = (uint8_t)u;
    u >>= 8;
    x[0] = (uint8_t)u;
}

uint8_t* SHA256(uint8_t *Data, uint64_t DataSizeByte)
{
    uint64_t DataSizeBits; // 입력 데이터의 크기를 비트 단위로 저장하는 변수
    uint32_t NumOfBlock;   // 데이터를 512비트(64바이트) 블록으로 나눈 개수 저장하는 변수
    uint8_t **DataBlock;   // 입력 데이터를 블록 단위로 저장할 이중 포인터 변수
    uint32_t state[8];
    uint32_t W[64]; // 32비트(4바이트) 워드배열
    // H -> Block hash : TmpH -> temporary hash in compression loop(임시로 블록의 상태를 저장할 임시변수)
    // Temp1 and Ttemp2 are auxiliar variable to calculate TmpH[]
    uint32_t H[8], TmpH[8], Temp1, Temp2;

    // Hashed data(최종 해시값 저장)
    uint8_t *Digest;

    // 입력 데이터의 크기를 비트단위로 저장
    DataSizeBits = DataSizeByte * 8;
    // 입력 데이터를 64바이트 블록으로 나눈 개수를 계산
    // int_ratio_ceil함수를 이용해서 블록 개수를 정수로 올림한다, 입력 데이터의 크기를 64로 나눈 몫을 구하고
    // 나머지가 있으면 블록 개수를 하나 늘림.
    NumOfBlock = int_ratio_ceil(DataSizeByte, 64);
    //if(NumOfBlock % 64 >= 56)
    //DataSize를 64로 나누고 올림해줌.

    // 512bit씩 블록 나눠주기
    
    DataBlock = (uint8_t**)calloc(NumOfBlock, sizeof(uint8_t*));
    for (uint32_t cnt_i = 0; cnt_i < NumOfBlock; cnt_i++)
    {
        DataBlock[cnt_i] = (uint8_t*)calloc(64, sizeof(uint8_t));
    }
    // NumOfBlock개수의 64바이트 블록을 가리키는 포인터 배열을 동적으로 할당한다.
    // DataBlock 배열에 입력 데이터를 블록으로 나누고 사전 처리하기

    // preprocessing data into blocks..
    // 입력 데이터를 64바이트 블록으로 나누는 과정 중 마지막 블록 처리해야함
    // 마지막 블록에는 입력 데이터의 나머지부분 + 패딩
    for (uint32_t Block = 0; Block < NumOfBlock; Block++)
    {
        if (Block == NumOfBlock - 1)
        {
            // 마지막 블록인 경우
            // copy the last bytes from Data
            // 입력 데이터의 나머지 부분을 Data블록에 넣어줌
            // 만약 남은 DataSizeByte가 56보다 작으면 블록추가 x
            for (uint32_t ByteOnBlock = 0; ByteOnBlock < (DataSizeByte % 64); ByteOnBlock++)
            {
                DataBlock[Block][ByteOnBlock] = Data[(Block * 64) + ByteOnBlock];
                // Data -> 입력값 인덱스는 (Block*64)+ByteOnBlock로 계산한다.
                // ByteOnBlock은 블록내의 바이트 인덱스

                // Append bit 1 at the end of data
                DataBlock[Block][DataSizeByte % 64] = 0x80;
                //총 데이터 사이즈에서 64를 나눈 나머지는 완전히 채워지지 않는 블록이므로 패딩을 진행함.
                //0x80은 1바이트 데이터로서 이진수로 10000000입니다.
                //128 in decimal.

                // Pad with 0 (512-64) = 448
                for (uint32_t ByteOnBlock = (DataSizeByte % 64) + 1; ByteOnBlock < 56; ByteOnBlock++)
                {
                    DataBlock[Block][ByteOnBlock] = 0x0;
                }
                // 나머지를 0x0으로 채우는데 448비트(56바이트)까지만 0으로 채운다.
                // 그러면 마지막 블록에는 입력 데이터의 나머지 부분과 패딩이 추가되어 있음.

                // Append 64bits of data size vlaue
                // 마지막 블록의 마지막 8바이트에는 입력 데이터의 비트 크기를 저장함
                // 64비트를 8비트씩 짤라서 마지막에 넣어준다.
                //Append 64bits of data size value in bits as big endian..
                //big endian 방법으로 바이너리 코드를 추가함..
                //Big Endian이므로 가장 상위비트가 먼저 저장되어야함.
                DataBlock[Block][56] = (DataSizeBits >> 56) & 0xFF;
                DataBlock[Block][57] = (DataSizeBits >> 48) & 0xFF;
                DataBlock[Block][58] = (DataSizeBits >> 40) & 0xFF;
                DataBlock[Block][59] = (DataSizeBits >> 32) & 0xFF;
                DataBlock[Block][60] = (DataSizeBits >> 24) & 0xFF;
                DataBlock[Block][61] = (DataSizeBits >> 16) & 0xFF;
                DataBlock[Block][62] = (DataSizeBits >> 8) & 0xFF;
                DataBlock[Block][63] = (DataSizeBits)& 0xFF;
                // DataSizeBits: 입력 데이터의 크기를 비트 단위로 저장한 변수
                //변수를 8바이트 단위로 쪼개어 각각의 하위 8비트를 DataBlock배열에 저장한다.
                //하위 8비트를 1로 설정한 8바이트 데이터를 나타낸다. 하위 8비트를 추출할때 사용함.
                //8바이트 중 하위 8비트만 1로 설정한것을 나타냅니다.
                //이진수로 표기하면 00000000 00000000 00000000 00000000 00000000 00000000 00000000 11111111 가 된다.
                //이를 16진수로 표기하면 0xFF가 된다.
                //빅 엔디언은 데이터를 메모리에 저장할때 가장 높은 바이트 부터 메모리에 저장하는 방식을 말한다.
                //즉 데이터의 가장 상위비트가 메모리 주소의 낮은 번지에 저장됩니다.
            }
        }
        else
        {
            // copy bytes from Data to block
            for (uint32_t ByteOnBlock = 0; ByteOnBlock < 64; ByteOnBlock++)
            {
                DataBlock[Block][ByteOnBlock] = Data[(Block * 64) + ByteOnBlock];
            }
        }
    }

    H[0] = HashStart[0];
    H[1] = HashStart[1];
    H[2] = HashStart[2];
    H[3] = HashStart[3];
    H[4] = HashStart[4];
    H[5] = HashStart[5];
    H[6] = HashStart[6];
    H[7] = HashStart[7];
    //입력 데이터 블록(512 bits)을 16개의 32비트 워드로 나누고, 이후 추가적으로 48개의 워드를 생성한다.
    for(uint32_t Block =0;Block < NumOfBlock; Block++){
        //512bits 블록을 16개의 32비트단위 워드로 나누기
        //[w0 to w15]는 그대로 넣어준다.
        for(uint32_t ByteOnBlock =0;ByteOnBlock < 64 ; ByteOnBlock +=4){
            //워드는 32비트 변수 uint32_t로 선언 되어 있음.
            //Big Endian 방식 사용해야되므로 낮은주소에 상위비트를 넣어주어야함
            W[ByteOnBlock / 4] = (((uint32_t)DataBlock[Block][ByteOnBlock]) << 24)   |
                                 //블록의 0번째 바이트를 32비트로 변환하고 24비트만큼 왼쪽으로 shift합니다.
                                 //블록의 0번째 바이트는 W[0]의 가장 상위 8비트에 위치합니다. Big Endian..
                                (((uint32_t)DataBlock[Block][ByteOnBlock+1]) << 16) |
                                 //블록의 1번째 바이트를 32비트로 변환하고 왼쪽으로 16비트 밀어줍니다.
                                 //블록의 1번째 바이트는 다음 8비트에 위치하게 됩니다.
                                (((uint32_t)DataBlock[Block][ByteOnBlock+2]) << 8) |
                                ((uint32_t)DataBlock[Block][ByteOnBlock+3]);
                                //Big Endian
                                //DataBlock[Block][0] : 블록의 0번째 바이트 (가장 상위 바이트)
                                //DataBlock[Block][1] : 블록의 1번째 바이트 (두번째로 상위 바이트)
                                //DataBlock[Block][2] : 블록의 2번째 바이트 (세번째로 상위 바이트)
                                //DataBlock[Block][3] : 블록의 3번째 바이트 (가장 하위 바이트)
                                //shift를 이용해서 각 바이트를 원하는 위치로 이동 시킨후 or 연산자를 사용하여 32비트 워드에 조합합니다.

        }
        //48개의 32bit word w16 to w63 넣어주기
        //앞에서 만든 16개의 워드를 이용하여 64개의 워드로 확장하기
        for(uint32_t cnt_i=16;cnt_i<64;cnt_i++){
            W[cnt_i] = sigma1(W[cnt_i-2] + W[cnt_i-7] + sigma0(W[cnt_i-15]+ W[cnt_i-16]));
        }
        //TemporaryHash
        TmpH[a] = H[0];
        TmpH[b] = H[1];
        TmpH[c] = H[2];
        TmpH[d] = H[3];
        TmpH[e] = H[4];
        TmpH[f] = H[5];
        TmpH[g] = H[6];
        TmpH[h] = H[7];
        //계산 중에 임시로 현재 블록의 상태를 저장해야 하는데, 그래서 TmpH라는 임시 변수에 H의 값을 복사합니다.
        //SHA256은 해시 값을 계산하는 데에 블록 당 64개의 32비트 워드를 사용합니다.
        //H -> 상태 변수
        for(uint32_t cnt_i=0;cnt_i<64;cnt_i++){
            Temp1 = BigSigma1(TmpH[e]) + choice(TmpH[e], TmpH[f], TmpH[g])+ K_const[cnt_i] + W[cnt_i] + TmpH[h];
            Temp2 = BigSigma0(TmpH[a]) + major(TmpH[a], TmpH[b], TmpH[c]);

            TmpH[h] = TmpH[g];
            TmpH[g] = TmpH[f];
            TmpH[f] = TmpH[e];
            TmpH[e] = TmpH[d] + Temp1;
            TmpH[d] = TmpH[c];
            TmpH[c] = TmpH[b];
            TmpH[b] = TmpH[a];
            TmpH[a] = Temp1 + Temp2;
        }
        //해시값을 현재 블록으로 업데이트해준다.
        H[0] += TmpH[a];
        H[1] += TmpH[b];
        H[2] += TmpH[c];
        H[3] += TmpH[d];
        H[4] += TmpH[e];
        H[5] += TmpH[f];
        H[6] += TmpH[g];
        H[7] += TmpH[h];
    }
    //메모리 초기화
    for(uint32_t cnt_i=0;cnt_i<NumOfBlock;cnt_i++){
        free(DataBlock[cnt_i]);
    }
    free(DataBlock);

    Digest = (uint8_t*)calloc(32,sizeof(uint8_t)); //최종 해시값 저장할 변수

    //SHA256 알고리즘은 블록당 64개의 32비트 워드로 구성된 상태 변수를 사용하여 해시 값을 계산하며 , 최종
    //해시값은 32바이트(256비트) 크기로 구성된다. 
    //최종 해시값을 16진수 문자열로 표현하기위해 32바이트의 해시값을 문자열로 변환한다.
    for(uint32_t cnt_i=0;cnt_i<32;cnt_i+=4){
        //H는 WORD단위 이므로 32비트이고 32비트를 8비트씩 짤라서 Digest배열에 넣어준다.
        Digest[cnt_i] = (uint8_t)((H[cnt_i/4] >> 24) & 0x000000FF);
        //0x000000FF를 이진수로 표현하면 상위 24비트는 0으로 하위 8비트가 모두 1로 채워져있음
        Digest[cnt_i+1] = (uint8_t)((H[cnt_i/4] >> 16) & 0x000000FF);
        Digest[cnt_i+2] = (uint8_t)((H[cnt_i/4] >> 8) & 0x000000FF);
        Digest[cnt_i+3] = (uint8_t)((H[cnt_i/4]) & 0x000000FF);
        //결과값을 8비트로 변환하여 uint8_t 데이터 타입으로 형변환 한다. 이로써 8비트의 정수 값으로 나타남.
        //해시값의 각 바이트를 8비트로 표현하기 위해서 형변환해줌
    }
    //32바이트(256 bits)의 16진수 문자열로 표현된 최종 해시값을 얻을 수 있음.
    return Digest; //최종 해시값 반환
}

#define MAX_INPUT_SIZE 1024
int main(void)
{
    char input[] = "abc";
    uint64_t DataSize;
    uint8_t* Data;
    int cnt_i;
    DataSize = (uint64_t)strlen(input);
    
    printf("Data size : %llu\n", DataSize);
    printf("input : %s", input);
    printf("\n");
    Data = SHA256((uint8_t*)input, DataSize);
    printf("\n");
    printf("Hash:");
    //32바이트(256비트) 해시값 출력
    for(cnt_i=0;cnt_i<32;cnt_i++){
        printf("%02x", Data[cnt_i]);
    }
    printf("\n\n");
    free(Data);

    return 0;
}
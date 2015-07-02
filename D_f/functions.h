#ifndef FUNCTIONS
#define FUNCTIONS

#include <iostream>
#include <fstream>

using namespace std;

int value_file(ifstream &inputFile);
void entry_file(ifstream &inputFile, bool** data_in, int amount);
void enter_key(ifstream &inputFile, bool* initial_key);
void clear(bool** data_in, bool** encrypted_data_R, bool** encrypted_data_L, bool** key, int amount);

void first_iteration_key(bool* initial_key);
void second_iteration_key(bool* initial_key, bool** key);
void third_iteration_key(bool** key);

void pre_first_iteration(bool** data_in, int amount);
void formation_R0_L0(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in);
void first_iteration(bool* encrypted_data_R, bool* encrypted_data_L1);
void second_iteration(bool* encrypted_data_R, bool* key);
void third_iteration(bool* encrypted_data_R);
void fourth_iteration(bool* encrypted_data_R);
void fifth_iteration(bool* encrypted_data_R, bool* encrypted_data_L, bool* encrypted_data_R1);

void output_file(bool* data_in, ofstream &inputFile);
void composition(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in);
void final_iteration(bool* data_in);
void fifth_iteration_reverse(bool* encrypted_data_R, bool* encrypted_data_L, bool* encrypted_data_L1);
void formation_R15_L15(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in);

int value_file(ifstream &f)
{
    char ch;
    int i = 0;
    if (f)
    {
        while (f.get(ch))
            i++;
    }
    f.clear();
    f.seekg(0, ios::beg);
    return i;
}

void entry_file(ifstream &g, bool** data_in, int amount)
{
    int i, j = 0;
    int l = 0;
    char symbol;
    char** time_enter = new char*[amount];

    for (i = 0; i < amount; i++)
        time_enter[i] = new char[8];
    i = 0;

    if (g)
    {
        while (g.get(symbol))
        {
            if (i == amount)
                break;
            cout << symbol;
            time_enter[i][j] = symbol;
            j++;
            if (j == 8)
            {
                j = 0;
                i++;
            }
        }
    }

    for (i = 0; i < amount; i++)
    {
        for (j = 0; j < 8; j++)
        {
            symbol = time_enter[i][j];
            data_in[i][j + l] = ((symbol & 0x80) ? 1 : 0);
            l++;
            data_in[i][j + l] = ((symbol & 0x40) ? 1 : 0);
            l++;
            data_in[i][j + l] = ((symbol & 0x20) ? 1 : 0);
            l++;
            data_in[i][j + l] = ((symbol & 0x10) ? 1 : 0);
            l++;
            data_in[i][j + l] = ((symbol & 0x08) ? 1 : 0);
            l++;
            data_in[i][j + l] = ((symbol & 0x04) ? 1 : 0);
            l++;
            data_in[i][j + l] = ((symbol & 0x02) ? 1 : 0);
            l++;
            data_in[i][j + l] = ((symbol & 0x01) ? 1 : 0);
        }
        l = 0;
    }
    cout << endl;
}

void first_iteration_key(bool* initial_key)
{
    int G_block[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };
    bool time_key[64];
    for (int i = 0; i < 64; i++)
        time_key[i] = initial_key[i];
    for (int i = 0; i < 56; i++)
        initial_key[i] = time_key[G_block[i] - 1];
}

void second_iteration_key(bool* initial_key, bool** key)
{
    for (int i = 0; i < 16; i++)
    {
        if ((i == 0) || (i == 1) || (i == 8) || (i == 15))
        {
            for (int j = 0; j < 55; j++)
                key[i][j] = initial_key[j + 1];

            key[i][55] = initial_key[0];
        }
        else
        {
            for (int j = 0; j < 54; j++)
                key[i][j] = initial_key[j + 2];

            key[i][54] = initial_key[0];
            key[i][55] = initial_key[1];
        }
        initial_key = key[i];
    }
}

void third_iteration_key(bool** key)
{
    int X_block[48] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };
    bool time_key[48];

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 48; j++)
            time_key[j] = key[i][j];

        for (int j = 0; j < 48; j++)
            key[i][j] = time_key[X_block[j] - 1];
    }
}

void pre_first_iteration(bool** data_in, int amount)
{
    int IP_block[64] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

    bool time_data[64];
    for (int i = 0; i < amount; i++)
    {
        for (int j = 0; j < 64; j++)
            time_data[j] = data_in[i][j];
        for (int j = 0; j < 64; j++)
            data_in[i][j] = time_data[IP_block[j] - 1];
    }
}

void formation_R0_L0(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in)
{
    for (int i = 0; i < 32; i++)
        encrypted_data_L[i] = data_in[i];
    for (int i = 32; i < 64; i++)
        encrypted_data_R[i - 32] = data_in[i];
    for (int i = 32; i < 48; i++)
        encrypted_data_R[i] = 0;
    for (int i = 32; i < 48; i++)
        encrypted_data_L[i] = 0;
}

void first_iteration(bool* encrypted_data_R, bool* encrypted_data_L1)
{
    for (int i = 0; i < 32; i++)
        encrypted_data_L1[i] = encrypted_data_R[i];

    int E_block[48] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };
    bool v[48];
    for (int i = 0; i < 48; i++)
        v[i] = encrypted_data_R[i];

    for (int i = 0; i < 48; i++)
        encrypted_data_R[i] = v[E_block[i] - 1];
}

void second_iteration(bool* encrypted_data_R, bool* key)
{
    for (int i = 0; i < 48; i++)
        encrypted_data_R[i] = ((encrypted_data_R[i] + key[i]) % 2);
}

void third_iteration(bool* encrypted_data_R)
{
    int v1, v2, v3;
    bool a[2];
    bool b[4];

    int S_block[8][4][16] = {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,

        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 12, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    };

    for (int i = 0; i < 8; i++)
    {
        a[0] = encrypted_data_R[i * 6 + 0];
        a[1] = encrypted_data_R[i * 6 + 5];
        b[0] = encrypted_data_R[i * 6 + 1];
        b[1] = encrypted_data_R[i * 6 + 2];
        b[2] = encrypted_data_R[i * 6 + 3];
        b[3] = encrypted_data_R[i * 6 + 4];

        v1 = (a[0] * 2) + a[1];
        v2 = (b[0] * 8) + (b[1] * 4) + (b[2] * 2) + b[3];

        v3 = S_block[i][v1][v2];

        encrypted_data_R[i * 4 + 0] = ((v3 & 0x08) ? 1 : 0);
        encrypted_data_R[i * 4 + 1] = ((v3 & 0x04) ? 1 : 0);
        encrypted_data_R[i * 4 + 2] = ((v3 & 0x02) ? 1 : 0);
        encrypted_data_R[i * 4 + 3] = ((v3 & 0x01) ? 1 : 0);
    }
}

void fourth_iteration(bool* encrypted_data_R)
{
    int P_block[32] = { 16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25 };
    bool time_data[32];
    for (int i = 0; i < 32; i++)
        time_data[i] = encrypted_data_R[i];

    for (int i = 0; i < 32; i++)
        encrypted_data_R[i] = time_data[P_block[i] - 1];
}

void fifth_iteration(bool* encrypted_data_R, bool* encrypted_data_L, bool* encrypted_data_R1)
{
    for (int i = 0; i < 32; i++)
        encrypted_data_R1[i] = ((encrypted_data_R[i] + encrypted_data_L[i]) % 2);
}

void output_file(bool* data_in, ofstream &o)
{
    int v2[8];
    int l = 0;
    for (int i = 0; i < 8; i++)
    {
        v2[i] = (data_in[i + l] * 128);
        l++;
        v2[i] = v2[i] + (data_in[i + l] * 64);
        l++;
        v2[i] = v2[i] + (data_in[i + l] * 32);
        l++;
        v2[i] = v2[i] + (data_in[i + l] * 16);
        l++;
        v2[i] = v2[i] + (data_in[i + l] * 8);
        l++;
        v2[i] = v2[i] + (data_in[i + l] * 4);
        l++;
        v2[i] = v2[i] + (data_in[i + l] * 2);
        l++;
        v2[i] = v2[i] + data_in[i + l];
    }

    for (int i = 0; i < 8; i++)
    {
        char ch = (char)v2[i];
        cout << ch;
        o << ch;
    }
}

void enter_key(ifstream &k, bool* initial_key)
{
    char symbol;
    char time_key[8];
    int i = 0, l = 0;

    if (k)
    {
        while (k.get(symbol))
        {
            time_key[i]= symbol;
            i++;
        }
    }
    k.close();
    for (int j = 0; j < 8; j++)
    {
        symbol = time_key[j];
        initial_key[j + l] = ((symbol & 0x80) ? 1 : 0);
        l++;
        initial_key[j + l] = ((symbol & 0x40) ? 1 : 0);
        l++;
        initial_key[j + l] = ((symbol & 0x20) ? 1 : 0);
        l++;
        initial_key[j + l] = ((symbol & 0x10) ? 1 : 0);
        l++;
        initial_key[j + l] = ((symbol & 0x08) ? 1 : 0);
        l++;
        initial_key[j + l] = ((symbol & 0x04) ? 1 : 0);
        l++;
        initial_key[j + l] = ((symbol & 0x02) ? 1 : 0);
        l++;
        initial_key[j + l] = ((symbol & 0x01) ? 1 : 0);
    }
}

void final_iteration(bool* data_in)
{
    int IP_block_reverse[64] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };
    bool time_data[64];
    for (int i = 0; i < 64; i++)
        time_data[i] = data_in[i];
    for (int i = 0; i < 64; i++)
        data_in[i] = time_data[IP_block_reverse[i] - 1];
}

void composition(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in)
{
    for (int i = 0; i < 32; i++)
        data_in[i] = encrypted_data_L[i];
    for (int i = 32; i < 64; i++)
        data_in[i] = encrypted_data_R[i - 32];
}

void fifth_iteration_reverse(bool* encrypted_data_R, bool* encrypted_data_L, bool* encrypted_data_L1)
{
    for (int i = 0; i < 32; i++)
        encrypted_data_L1[i] = ((encrypted_data_R[i] + encrypted_data_L[i]) % 2);
}

void formation_R15_L15(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in)
{
    for (int i = 0; i < 32; i++)
        encrypted_data_L[i] = data_in[i];
    for (int i = 32; i < 64; i++)
        encrypted_data_R[i - 32] = data_in[i];
    for (int i = 32; i < 48; i++)
        encrypted_data_R[i] = 0;
    for (int i = 32; i < 48; i++)
        encrypted_data_L[i] = 0;
}

void clear(bool** data_in, bool** encrypted_data_R, bool** encrypted_data_L, bool** key, int amount)
{
    for (int i = 0; i < amount; i++)
        delete[] data_in[i];
    delete[] data_in;

    for (int i = 0; i < 16; i++)
        delete[] encrypted_data_R[i];
    delete[] encrypted_data_R;

    for (int i = 0; i < 16; i++)
        delete[] encrypted_data_L[i];
    delete[] encrypted_data_L;

    for (int i = 0; i < 16; i++)
        delete[] key[i];
    delete[] key;
}
#endif // FUNCTIONS


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Change Hex to it's value (0-15)
int hexValue(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F')
        return 10 + (c - 'A');
    return -1;
}

// Change 32 hex (128 bit) to 16 bytes
bool hex128ToBytes(const string &input, unsigned char state[16])
{
    string s = input;

    // If user put "0x" it will remove
    if (s.size() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
    {
        s = s.substr(2);
    }

    // The input need to be exactly 32 hex
    if (s.size() != 32)
        return false;

    // Modify every 2 hex into 1 byte
    for (int i = 0; i < 16; i++)
    {
        int hi = hexValue(s[i * 2]);
        int lo = hexValue(s[i * 2 + 1]);
        if (hi < 0 || lo < 0)
            return false;
        state[i] = (unsigned char)((hi << 4) | lo);
    }
    return true;
}

// Print the state as 0x... (32 hex)
void printStateHex(ostream &out, const string &label, unsigned char state[16])
{
    out << label << "0x";
    const char *hex = "0123456789ABCDEF";
    for (int i = 0; i < 16; i++)
    {
        out << hex[(state[i] >> 4) & 0x0F];
        out << hex[state[i] & 0x0F];
    }
    out << "\n\n";
}

// Print as 4x4 matrix view
void printStateMatrix(ostream &out, unsigned char state[16])
{
    out << "State Matrix (4x4 View):\n";
    const char *hex = "0123456789ABCDEF";

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            unsigned char v = state[row + 4 * col];
            out << hex[(v >> 4) & 0x0F] << hex[v & 0x0F] << " ";
        }
        out << "\n";
    }
    out << "\n";
}

// AES xtime (multiply by 2 in GF(2^8))
unsigned char xtime(unsigned char a)
{
    if (a & 0x80)
        return (unsigned char)((a << 1) ^ 0x1B);
    return (unsigned char)(a << 1);
}

// Multiply in GF(2^8)
unsigned char gfMul(unsigned char a, unsigned char b)
{
    unsigned char res = 0;
    while (b)
    {
        if (b & 1)
            res ^= a;
        a = xtime(a);
        b >>= 1;
    }
    return res;
}

// MixColumns for ONE column (4 bytes)
void mixOneColumn(unsigned char c[4])
{
    unsigned char a0 = c[0], a1 = c[1], a2 = c[2], a3 = c[3];

    c[0] = (unsigned char)(gfMul(a0, 0x02) ^ gfMul(a1, 0x03) ^ a2 ^ a3);
    c[1] = (unsigned char)(a0 ^ gfMul(a1, 0x02) ^ gfMul(a2, 0x03) ^ a3);
    c[2] = (unsigned char)(a0 ^ a1 ^ gfMul(a2, 0x02) ^ gfMul(a3, 0x03));
    c[3] = (unsigned char)(gfMul(a0, 0x03) ^ a1 ^ a2 ^ gfMul(a3, 0x02));
}

// MixColumns for ALL 4 columns
void mixColumns(unsigned char state[16])
{
    for (int col = 0; col < 4; col++)
    {
        int base = col * 4;

        unsigned char c[4] = {
            state[base + 0],
            state[base + 1],
            state[base + 2],
            state[base + 3]};

        mixOneColumn(c);

        state[base + 0] = c[0];
        state[base + 1] = c[1];
        state[base + 2] = c[2];
        state[base + 3] = c[3];
    }
}

// Read hex input from file
bool readHexFromFile(const string &filename, string &hexString)
{
    ifstream fin(filename);
    if (!fin)
        return false;
    fin >> hexString;
    return !hexString.empty();
}

int main()
{
    string inputHex;
    unsigned char state[16];

    cout << "Choose input method for Q11 AES MixColumns:\n";
    cout << "1) Keyboard\n";
    cout << "2) Read from file\n";
    cout << "Enter choice: ";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        cout << "Enter 128-bit hex (32 hex chars, optional 0x): ";
        cin >> inputHex;
    }
    else if (choice == 2)
    {
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        if (!readHexFromFile(filename, inputHex))
        {
            cout << "Error: Cannot read file.\n";
            return 1;
        }
    }
    else
    {
        cout << "Error: Invalid choice.\n";
        return 1;
    }

    if (!hex128ToBytes(inputHex, state))
    {
        cout << "Error: Input must be exactly 32 hex characters (optional 0x).\n";
        cout << "Example: 0x112233445566778899AABBCCDDEEFF00\n";
        return 1;
    }

    ofstream fout("MixColumns_OUTPUT.txt");
    if (!fout)
    {
        cout << "Error: Cannot open output file.\n";
        return 1;
    }

    // BEFORE
    printStateHex(cout, "Before MixColumns: ", state);
    printStateMatrix(cout, state);

    printStateHex(fout, "Before MixColumns: ", state);
    printStateMatrix(fout, state);

    // PROCESS
    mixColumns(state);

    // AFTER
    printStateHex(cout, "After  MixColumns: ", state);
    printStateMatrix(cout, state);

    printStateHex(fout, "After  MixColumns: ", state);
    printStateMatrix(fout, state);

    cout << "[Success] Output saved to MixColumns_OUTPUT.txt\n";
    return 0;
}
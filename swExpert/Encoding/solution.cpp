
bool strcompare(const char* s1, const char* s2)
{
	const char* a = s1; const char* b = s2;

	while (*a == *b && (*a != ' ' && *b != ' '))
	{
		if (*a != *b)
			return false;
		a++; b++;
	}

	return *a == *b;
}

void strcopy(char* dest, const char* src)
{
	const char* a = src;

	while (*a != ' ')
	{
		*dest = *a;
		dest++; a++;
	}

	*dest = ' '; dest++;
}


int min(int a, int b) { return a < b ? a : b; }

int find(char dict[1024][8], int k, char* s)
{

	for (int i = 0; i < k; i++)
	{
		if (strcompare(dict[i], s))
			return i;
	}

	return -1;
}

int arrange(char* paper, int papern, char dict[1024][8], short idx_list[32768])
{
	char word[8];
	int wIdx = 0;
	int d = 0;

	int k = 0;

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 8; j++)
			dict[i][j] = ' ';


	for (int i = 0; i < papern; i++)
	{
		word[wIdx++] = paper[i];

		if (paper[i] == ' ')
		{
			int idx = find(dict, d, word);

			if (idx == -1)
			{
				strcopy(dict[d], word);
				idx = d++;
			}

			idx_list[k++] = idx;

			wIdx = 0;
		}

	}

	return k;
}

int encode_dic(char* src, char dict[1024][8], int bitSum)
{
	int bitMask[8] = {
		0b10000000,
		0b01000000,
		0b00100000,
		0b00010000,
		0b00001000,
		0b00000100,
		0b00000010,
		0b00000001
	};
	
	unsigned short mask, temp;
	int nbit;

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 8; j++)
		{
			unsigned short bins;

			if (dict[i][j] == ' ') { bins = 0b00011010; }
			else { bins = dict[i][j] - 'a'; }

			bins <<= 3;

			nbit = 0;

			while (nbit < 5)
			{
				mask = 0;
				int remain = 8 - (bitSum % 8);

				int st = nbit;
				int ed = min(remain, 5);

				for (int k = st; k < ed; k++)
					mask |= bitMask[k];

				temp = 0;
				temp = (mask & bins);

				temp <<= nbit;
				temp >>= (bitSum % 8);

				src[bitSum >> 3] |= temp;

				nbit += (ed - st);
				bitSum += (ed - st);
			}

			if (dict[i][j] == ' ') { break; }

		}

	return bitSum;
}

int encode_idx(short idx_list[32768], char* src, int bitSum, int k)
{
	int bitMask[8] = {
	0b1000'0000'0000'0000,
	0b0100'0000'0000'0000,
	0b0010'0000'0000'0000,
	0b0001'0000'0000'0000,
	0b0000'1000'0000'0000,
	0b0000'0100'0000'0000,
	0b0000'0010'0000'0000,
	0b0000'0001'0000'0000 
    };

	int nbit;
	unsigned short bins;

	for (int i = 0; i < k; i++)
	{
		nbit = 0;

		while (nbit < 10)
		{
			bins = idx_list[i];
			bins <<= 6;
			bins <<= nbit;

			unsigned short mask = 0;
			int cnt = min(10 - nbit, 8 - (bitSum % 8));
			for (int k = 0; k < cnt; k++)
				mask |= bitMask[k];
			
			unsigned short temp = (mask & bins);
			temp >>= (16 - cnt);
			int remain = 8 - (bitSum % 8);
			temp <<= (remain - cnt);

			src[bitSum >> 3] |= temp;
			nbit += cnt;
			bitSum += cnt;
		}
		
	}

	return bitSum;
}

int encode(char* src, char* paper, int papern)
{

	int N = 65536;

	for (int i = 0; i < N; i++)
		src[i] = 0;

	char dict[1024][8];

	short idx_list[32768]; 

	int k = arrange(paper, papern, dict, idx_list);

	int bitSum = encode_dic(src, dict, 0);

	bitSum = encode_idx(idx_list, src, bitSum, k);

	return (bitSum >> 3) + 1;
}


int decode_dict(char* src, char dict[1024][8], int bitSum)
{

	for (int i = 0; i < 1024; i++)
		for (int j = 0; j < 8; j++)
			dict[i][j] = ' ';

	int bitMask[8] = {
		0b10000000,
		0b01000000,
		0b00100000,
		0b00010000,
		0b00001000,
		0b00000100,
		0b00000010,
		0b00000001
	};

	int d = 0;
	unsigned short c;
	char word[8];
	short wIdx;
	int nbit, temp;

	for (int i = 0; i < 1024; i++)
	{
		wIdx = 0;
		for (int w = 0; w < 8; w++) { word[w] = ' '; }

		for (int j = 0; j < 8; j++)
		{
			c = 0;
			nbit = 0;

			while (nbit < 5)
			{
				int st = bitSum % 8;
				int ed = min(st + (5 - nbit), 8);
				short mask = 0;

				for (int k = st; k < ed; k++)
					mask |= bitMask[k];

				temp = 0;
				temp = (src[bitSum >> 3] & mask);
				temp <<= st;
				temp >>= nbit;

				c |= temp;

				nbit += (ed - st);
				bitSum += (ed - st);
			}

			c >>= 3;

			if (c == 0b00011010)
			{
				word[wIdx] = ' ';
				break;
			}

			word[wIdx++] = (c + 'a');
		}

		strcopy(dict[d++], word);
	}

	return bitSum;
}

int decode_idx(char dict[1024][8], char* src, short idx_list[32768], int s, int bitSum)
{

	int d = 0;

	int bitMask[8] = {
	0b10000000,
	0b01000000,
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b00000001
	};

	short bins, temp;
	int nbit;


	for (int i = (bitSum >> 3); i < s; i++)
	{
		bins = 0;
		nbit = 0;

		while (nbit < 10)
		{
			short mask = 0;
			temp = 0;

			int st = bitSum % 8;
			int ed = min(8, st + 10 - nbit);

			for (int k = st; k < ed; k++)
				mask |= bitMask[k];

			temp = (mask & src[bitSum >> 3]);
			temp >>= (8 - ed);

			nbit += (ed - st);
			bitSum += (ed - st);

			temp <<= (10 - nbit);
			bins |= temp;
		}

		if ((bitSum >> 3) < s)
			idx_list[d++] = bins;
	}
	
	return d;
}


void decode(char* dest, char* src, int s)
{
	char dict[1024][8];

	int bitSum = decode_dict(src, dict, 0);

	short idx_list[32768];

	int d = decode_idx(dict, src, idx_list, s, bitSum);

	int pl = 0;
	for (int i = 0; i < d; i++)
		for (int j = 0; j < 8; j++)
		{
			dest[pl++] = dict[idx_list[i]][j];
			if (dict[idx_list[i]][j] == ' ') 
				break;
		}

	return;
}
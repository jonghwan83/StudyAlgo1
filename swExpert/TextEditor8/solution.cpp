/*User Code*/

char* document[1000000];
int   size = 0;


int strlength(char* string)
{
    int ret = 0;

    while (*string != 0) {
        ret++;
        string++;
    }

    return ret;
}


void strcopy(char* dest, char* src)
{
    while (*src != 0) {
        *dest = *src;
        src++;
        dest++;
    }

    *dest = 0;
}


void append(char* string)
{
    document[size] = new char[strlength(string) + 1];
    strcopy(document[size], string);
    size++;
}


void insert(int line, char* string)
{
    for (int c = size; c > line; c--) {
        delete[] document[c];
        document[c] = new char[strlength(document[c - 1]) + 1];
        strcopy(document[c], document[c - 1]);
    }
    delete[] document[line];
    document[line] = new char[strlength(string) + 1];
    strcopy(document[line], string);
    size++;
}


void erase(int line)
{
    for (int c = line; c < size - 1; c++) {
        delete[] document[c];
        document[c] = new char[strlength(document[c + 1]) + 1];
        strcopy(document[c], document[c + 1]);
    }
    size--;
}


char* linestring(int line)
{
    return document[line];
}

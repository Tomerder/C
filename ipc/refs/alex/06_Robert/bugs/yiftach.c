


void fYiftach(char* _str)
{
	char *a = _str;
	
	while(*a)
	{
		*a += 2;
		++a;
	}
}


int main()
{
	char str = 's';
	
	fYiftach(&str);
	
	return 0;
}


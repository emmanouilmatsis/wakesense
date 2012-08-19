#ifndef BTOI_H
#define BTOI_H

	template <typename T>
T btoi(const unsigned char* bytes)
{
	T result = 0;
	unsigned short int a=0x1234;

	if (*((unsigned char*) &a)==0x12) // big endian
		for (unsigned int n = 0; n < sizeof(result); n++)
			result = (result << 8) + bytes[n];
	else // little endian
		for (int n = sizeof(result); n >= 0; n--)
			result = (result << 8) + bytes[n];

	return result;
}

#endif // BTOI_H

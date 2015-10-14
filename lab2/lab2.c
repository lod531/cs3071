#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*


SEE DOCUMENTATION AT https://github.com/lod531/cs3071/tree/master/lab2


*/
const char 	digitChars[24] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
								'a', 'b', 'c', 'd', 'e', 'f',
									'A', 'B', 'C', 'D', 'E', 'F'};
const int	numberOfOctDigits = 8;
const int	numberOfDecDigits = 10;
const int	numberOfHexDigits = 24;

int isSign(char c)
{
	return c == '-' || c == '+';
}

int isOctalIdentifier(char c)
{
	return c == 'b' || c == 'B';
}

int isHexIdentifier(char c)
{
	return c == 'h' || c == 'H';
}


//This function could be made a little faster via comparison
//with the ascii value (i.e. char octal iff char >20 and < 30 or whatever)
//kept it this way for readability.
//if boundary	== 8, isDigit == is octal digit
//				== 10, isDigit == is decimal digit
//				==24, isDigit == is hexadecimal
int isDigit(char c, int boundary)
{
	int i;
	int result = 0;
	for(i = 0; i < boundary && result == 0; i++)
	{
		result = (c == digitChars[i]);
	}
	return result;
}

int exponentiate(int base, int exponent)
{
	if(exponent == 0)
	{
		return 1;
	}
	else
	{
		int result = base;
		while(exponent > 1)
		{
			result *= base;
			exponent--;
		}
		return result;
	}
}

int numericalValueOfDigitCharacter(char c)
{
	if(c > 47 && c < 58)
	{
		return c - 48;		//octal and decimal
	}
	else if(c > 64 && c < 71)
	{
		return c - 65 + 10;//hex upper case
	}
	else if(c > 96 && c < 103)
	{
		return c - 97 + 10;//hex lower case
	}
}

//converts char * into an octal, decimal or hexadecimal integer, if valid
//validity specified by this regular expression:
//[0-7]+[bB] | [0-9a-fA-F]+[hH] | ((+|–)?[0-9]+)
int lexicalAnalyser(char * numberChars)
{
	//State 1: Starting State
	int currentChar = 0;
	int octalDigitSeen = 0;
	int decimalDigitSeen = 0;
	int hexDigitSeen = 0;
	int numberOfDigits = 0;
	int result = 0;
	int negative = 0;
	if(!isSign(numberChars[currentChar]))
	{
		while(isDigit(numberChars[currentChar], numberOfOctDigits))
		{
			//State 2: Octal digit seen
			numberOfDigits++;
			octalDigitSeen = 1;
			currentChar++;
		}
		//if below if succeeds passes, it's not an octal value, but could be a dec or hex or junk
		if(!(isOctalIdentifier(numberChars[currentChar]) && numberChars[currentChar+1] == '\0'))
		{
			while(isDigit(numberChars[currentChar], numberOfDecDigits))
			{
				//State 4: Decimal digit seen
				numberOfDigits++;
				decimalDigitSeen = 1;
				currentChar++;
			}
			if(!numberChars[currentChar] == '\0')
			{
				while(isDigit(numberChars[currentChar], numberOfHexDigits))
				{
					//State 5: Hexadecimal digit seen
					numberOfDigits++;
					hexDigitSeen = 1;
					currentChar++;
				}
				if(!isHexIdentifier(numberChars[currentChar]))
				{
					printf("Invalid number! (line 126)\n");
					//
					exit(1);
				}
				else if(octalDigitSeen || decimalDigitSeen || hexDigitSeen)
				{
					currentChar++;
					assert(numberChars[currentChar] == '\0');
					//assert there is no junk after hex ID

					//State 6: Hexadecimal identifier seen
					//if this state is reached, the number is a hexadecimal number
					//HEXADEIMAL OVERLFOW CHECK:
					//max hex number: 7FFFFFFFh (8 digits)
					assert((numberOfDigits < 9 && numericalValueOfDigitCharacter(numberChars[0]) < 8)
							|| numberOfDigits < 8);
					//if above assertion passes, no overflow.

					int i;
					for(i = 0; i < numberOfDigits; i++)
					{
						result += numericalValueOfDigitCharacter(numberChars[i]) * exponentiate(16, numberOfDigits - i - 1);
					}
					return result;
				}
			}
			else if(((octalDigitSeen || decimalDigitSeen)) && numberChars[currentChar] == '\0') // && currentChar = null
			{
				//if this state is reached, the number is a decimal number
				//Check for decimal overflow (positive)
				//positive signed int can be max of 2^31-1
				//i.e. 2147483647 (10 digits)
				assert(numberOfDigits<11);
				//far from fool-proof assert
				//to assure that there is no overflow, the most significant bit must be monitored
				//no need to carryInFlag XOR carryOutFlag of MSB as this is not signed arithmetic
				//(i.e. we will only be adding)
				//if MSB is 1 at any point, overflow.

				int i;
				int msb = 0;
				for(i = 0; i < numberOfDigits; i++)
				{
					result += numericalValueOfDigitCharacter(numberChars[i]) * exponentiate(10, numberOfDigits - i - 1);
					msb = result & 0X80000000; //0X80000000 is MSB set, rest clear
					assert(msb == 0);
				}
				return result;
			}
		}
		else if(isOctalIdentifier(numberChars[currentChar]) && octalDigitSeen
					&& numberChars[currentChar+1] == '\0')
		{
			//State 3: Octal identifier seen, end symbol seen.

			//OCTAL OVERFLOW CHECK:
			//max octal value (in octal representation): 17777777777 (11 digits)
			assert((numberOfDigits < 12 && numericalValueOfDigitCharacter(numberChars[0]) < 2)
					|| numberOfDigits < 11);
			//if that assertion passes, no overflow

			int i;
			for(i = 0; i < numberOfDigits; i++)
			{
				result += numericalValueOfDigitCharacter(numberChars[i]) * exponentiate(8, numberOfDigits - i - 1);
			}
			return result;
		}
		else
		{
			printf("invalid expression! (line 188 of code).\n");
			exit(1);
		}
	}
	else //ifSign
	{
		//State 7: Sign seen
		negative = numberChars[currentChar] == '-';
		currentChar++;
		while(isDigit(numberChars[currentChar], numberOfDecDigits))
		{
			//State 8: Decimal digit after sign seen.
			numberOfDigits++;
			decimalDigitSeen = 1;
			currentChar++;
		}
		if(decimalDigitSeen)
		{
			assert(numberChars[currentChar] == '\0');
			//asserting there is no junk after the last decimal digit
			//also asserting that there are no non-decimal digits

			//if this state is reached, the number is a decimal number
			//Check for decimal overflow (positive)
			//negative signed int can be max of 2^31
			//i.e. 2147483648 (10 digits)
			assert(numberOfDigits<11);
			//far from fool-proof assert
			//to assure that there is no overflow, the most significant bit must be monitored
			//no need to carryInFlag XOR carryOutFlag of MSB as this is not signed arithmetic
			//(i.e. we will only be adding)
			//if MSB is 0 at any point (except for when result == 0), overflow.

			int i;
			int msb = 1;
			if(!negative)
			{
				msb = 0;
				for(i = 1; i < numberOfDigits+1; i++)
				{
					result += numericalValueOfDigitCharacter(numberChars[i]) * exponentiate(10, numberOfDigits - i);
					msb = result & 0X80000000; //0X80000000 is MSB set, rest clear
					assert(msb == 0);
				}
			}
			else
			{
				for(i = 1; i < numberOfDigits+1; i++)	//i = 1 because numberChars[0] is the sign
				{
					result -= numericalValueOfDigitCharacter(numberChars[i]) * exponentiate(10, numberOfDigits - i);
					msb = result & 0X80000000; //0X80000000 is MSB set, rest clear
					assert(result == 0 || msb != 0);
				}
			}
			return result;
		}
	}
	return -1;
}

int main(int argv, char * * argc)
{
	int i;
	for(i = 1; i < argv; i++)
	{
		printf("%d\n", lexicalAnalyser(argc[i]));
	}
	return 0;
}
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

#define BASE 10000000
#define MAX_KARA 40

void print_error(int id)
{
	exit(id);
}


class LongNumber
{
private:
	int size;
	int *long_number;
	int sign;

private:
	void vector_append();
	void prunning();
	LongNumber shift(int i);
	void sum_long_to_long(const LongNumber &other);
	void sum_long_to_short(int b);
	void sub_long_to_long(const LongNumber &other);
	void sub_long_to_short(int a);
	void mult_long_to_short(int b);
	void copy(int *array, int size_array);
	int length_number();
	void div_long_number(const LongNumber &other);
	void mod_long_number(const LongNumber &other);
	LongNumber karatsuba(LongNumber a, LongNumber b);


public:
	LongNumber(const char *name);
	LongNumber();
	LongNumber(const LongNumber &other);
	LongNumber(int other);
	~LongNumber();

	int	get_short_number();
	void print_long_number() const;
	LongNumber big_pow(int n);
	LongNumber& operator =(const LongNumber &other);
	LongNumber& operator +=(const LongNumber &other);
	LongNumber& operator +=(int other);
	LongNumber& operator -=(const LongNumber &other);
	LongNumber& operator -=(int other);
	LongNumber& operator *=(const LongNumber &other);
	LongNumber& operator *=(int a);
	LongNumber& operator /=(const LongNumber &other);
	LongNumber& operator %=(const LongNumber &other);
	bool operator >(const LongNumber &other);
	bool operator >=(const LongNumber &other);
	bool operator ==(const LongNumber &other);
	bool operator !=(int a);
};

//----------------------------constructor and destructor----------------------//

LongNumber::~LongNumber()
{
	free(long_number);
};

LongNumber::LongNumber()
{
	size = 1;
	sign = 1;
	long_number = (int*)malloc(sizeof(int));
	long_number[0] = 0;
};

LongNumber::LongNumber(const char *name)
{
	size = 0;
	long_number = NULL;
	if (*name == '-')
	{
		name++;
		sign = -1;
	}
	else
		sign = 1;
	name--;
	while (*++name)
	{
		mult_long_to_short(10);
		sum_long_to_short(*name - '0');
	}
}

LongNumber::LongNumber(const LongNumber &other)
{
	size = other.size;

	sign = other.sign;
	long_number = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++)
		long_number[i] = other.long_number[i];
}

LongNumber::LongNumber(int other)
{
	size = 1;

	sign = other > 0 ? 1 : -1;
	long_number = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++)
		long_number[i] = other;
}

//----------------------------constructor and destructor END----------------------//

std::ostream& operator << ( std::ostream& stream, const LongNumber& toOutput)
{
	toOutput.print_long_number();
	return stream;
};

int	LongNumber::get_short_number()
{
	return (long_number[0]);
}

LongNumber LongNumber::shift(int n)
{
	LongNumber temp = *this;

	for (size_t i = 0; i < n; i++)
		temp.mult_long_to_short(10);
	return(temp);
}


int LongNumber::length_number()
{
	int size_base = 0;
	int base = BASE;
	int length;
	int temp;

	while (base != 1)
	{
		size_base++;
		base /= 10;
	}
	length = size_base * (size - 1);
	temp = long_number[size - 1];
	while (long_number[size - 1] / base)
	{
		base *= 10;
		length++;
	}
	return (length);
}


void LongNumber::prunning()
{
	while (size > 1 && !long_number[size - 1])
		size--;
	long_number = (int*)realloc(long_number, sizeof(int) * size);
}

void LongNumber::vector_append()
{
	size++;
	if (size == 1)
	{
		long_number = (int*)malloc(sizeof(int) * size);
		long_number[size - 1] = 0;
		return;
	}
	long_number = (int*)realloc(long_number, sizeof(int) * size);
	long_number[size - 1] = 0;
};

bool LongNumber::operator >(const LongNumber &other)
{
	if (sign > other.sign)
		return (true);
	else if (sign < other.sign)
		return (false);

	if (size > other.size)
		return (true);
	else if (size < other.size)
		return (false);

	for (int i = size - 1; i >= 0; i--)
	{
		if (long_number[i] > other.long_number[i])
			return (true);
		else if (long_number[i] < other.long_number[i])
			return (false);
	}
	return (false);
}

bool LongNumber::operator ==(const LongNumber &other)
{
	if (size != other.size || sign != other.sign)
		return (false);
	for (int i = 0; i < size; i++)
		if (long_number[i] != other.long_number[i])
			return (false);
	return (true);
}

bool LongNumber::operator !=(int a)
{
	return (size != 1 || long_number[0] != a);
}

bool LongNumber::operator >=(const LongNumber &other)
{
	return (*this > other || *this == other);
}

LongNumber& LongNumber::operator =(const LongNumber &other)
{
	size = other.size;

	sign = other.sign;
	if (long_number)
	 free(long_number);
	long_number = (int*)malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++)
		long_number[i] = other.long_number[i];
}


void	LongNumber::print_long_number() const
{
	int base;
	int temp;

	if (sign == -1 && (size != 1 || long_number[0] != 0))
		cout << "-";
	if (size == 1 && long_number[0] == 0)
	{
		cout << 0 << endl;
		return ;
	}
	for (int i = size - 1 ; i >= 0; i--)
	{
		base = BASE / 10;
		temp = long_number[i];
		if (i != size - 1)
			while (base)
			{
				cout << temp / base;
				temp %= base;
				base = base / 10;
			}
		else
		{
			while (base && !(temp / base))
				base = base / 10;
			while (base)
			{
				cout << temp / base;
				temp %= base;
				base = base / 10;
			}
		}
	}
	cout << endl;
}


//---------------------sum--------------------//

LongNumber& LongNumber::operator +=(int other)
{
	sum_long_to_short(other);
	return (*this);
};

LongNumber& LongNumber::operator +=(const LongNumber &other)
{
	sum_long_to_long(other);
	return (*this);
};

LongNumber operator +(const LongNumber& leftOperand, int rightOperand)
{
	LongNumber temp = leftOperand;

	temp += rightOperand;
	return(temp);
};

LongNumber operator +(const LongNumber& leftOperand, const LongNumber& rightOperand)
{
	LongNumber temp = leftOperand;

	temp += rightOperand;
	return(temp);
};

void LongNumber::sum_long_to_long(const LongNumber &other)
{
	int i;
	int carry;

	carry = 0;
	if (!size)
		vector_append();
	i = -1;
	if (sign == other.sign)
		while (++i < max(size, other.size) || carry)
		{
			if (i == size)
				vector_append();
			long_number[i] += (other.size > i ? other.long_number[i] : 0) + carry;
			carry = long_number[i] >= BASE;
			if (carry)
				long_number[i] -= BASE;
		}
	else
	{
		if (sign == -1)
			sign = 1;
		else
			sign = -1;
		sub_long_to_long(other);
		if (sign == -1)
			sign = 1;
		else
			sign = -1;
	}
};

void LongNumber::sum_long_to_short(int b)
{
	int i = 1;
	int carry;


	if (!size)
		vector_append();
	long_number[0] += b;
	carry = long_number[0] >= BASE;
	if (carry)
		long_number[0] -= BASE;
	while (carry)
	{
		if (i == size)
			vector_append();
		long_number[i] += carry;
		carry = long_number[i] >= BASE;
		if (carry)
			long_number[i] -= BASE;
		i++;
	}
};

//---------------------sum_end------------------//


//---------------------sub---------------------//

LongNumber& LongNumber::operator -=(const LongNumber &other)
{
	sub_long_to_long(other);
	return (*this);
}


void LongNumber::sub_long_to_long(const LongNumber &other)
{
	int carry = 0;

	if (sign == other.sign)
	{
		if (*this >= other)
		{
			for (size_t i = 0; i < size || carry; ++i)
			{
				long_number[i] -= carry + (i < other.size ? other.long_number[i] : 0);
				carry = long_number[i] < 0;
				if (carry) long_number[i] += BASE;
			}
		}
		else
		{
			sign = -1;
			for (size_t i = 0; i < other.size || carry; ++i)
			{
				other.long_number[i] -= carry + (i < size ? long_number[i] : 0);
				carry = other.long_number[i] < 0;
				if (carry) other.long_number[i] += BASE;
			}
		}
	}
	else
	{
		if (sign == -1)
			sign = 1;
		else
			sign = -1;
		sum_long_to_long(other);
		if (sign == -1)
			sign = 1;
		else
			sign = -1;
	}
	prunning();
}

LongNumber operator -(const LongNumber& leftOperand, const LongNumber& rightOperand)
{
	LongNumber temp = leftOperand;
	temp -= rightOperand;
	return(temp);
}

//--------------------sub_end-----------------//



//---------------------mult--------------------//


LongNumber operator *(const LongNumber& leftOperand, const LongNumber& rightOperand)
{
	LongNumber temp = leftOperand;

	temp *= rightOperand;
	return(temp);
}

LongNumber operator *(const LongNumber& leftOperand, int a)
{
	LongNumber temp = leftOperand;

	temp *= a;
	return(temp);
}

void LongNumber::mult_long_to_short(int b)
{
	int	carry;
	int	i;
	long long cur;

	carry = 0;
	i = 0;
	if (!size)
		vector_append();
	while (i < size || carry)
	{
		if (i == size)
			vector_append();
		cur = carry + (long long)long_number[i] * (long long)b;
		long_number[i] = (int)(cur % BASE);
		carry = (int)(cur / BASE);
		i++;
	}
}

LongNumber& LongNumber::operator *=(const LongNumber &other)
{
	LongNumber temp;
	int temp_sign;

	temp_sign = sign * other.sign;
	*this = karatsuba(*this, other);
	sign = temp_sign;
	return (*this);
}

LongNumber& LongNumber::operator *=(int a)
{
	mult_long_to_short(a);
	return (*this);
}

void	LongNumber::copy(int *array, int size_array)
{
	size = size_array;
	long_number = (int*)malloc(sizeof(int) * size);
	for (size_t i = 0; i < size; i++)
		long_number[i] = array[i];
}

LongNumber LongNumber::karatsuba(LongNumber a, LongNumber b)
{
	LongNumber product;
	int i;

	product.size = a.size + b.size;
	product.long_number = (int*)malloc(sizeof(int) * product.size);

	if (a.size < MAX_KARA)
	{
		long long cur;
		long long carry;

		memset(product.long_number, 0, sizeof(int) * product.size);
		for (int i = 0; i < a.size; ++i)
			for (int j = 0, carry = 0; j < b.size || carry; ++j)
			{
				cur = product.long_number[i+j] + a.long_number[i] * 1ll * (j < (int)b.size ? b.long_number[j] : 0) + carry;
				product.long_number[i+j] = int (cur % BASE);
				carry = int (cur / BASE);
			}
		product.prunning();
		return (product);
	}

	LongNumber a_part1;
	a_part1.copy(a.long_number, (a.size + 1) / 2);

	LongNumber a_part2;
	a_part2.copy(a.long_number + a_part1.size, a.size / 2);

	LongNumber b_part1;
	b_part1.copy(b.long_number, (b.size + 1) / 2);

	LongNumber b_part2;
	b_part2.copy(b.long_number + b_part1.size, b.size / 2);

	LongNumber sum_of_a_parts = a_part1 + a_part2;
	LongNumber sum_of_b_parts = b_part1 + b_part2;
	LongNumber product_of_sums_of_parts = karatsuba(sum_of_a_parts, sum_of_b_parts);
	LongNumber product_of_first_parts = karatsuba(a_part1, b_part1);
	LongNumber product_of_second_parts = karatsuba(a_part2, b_part2);
	LongNumber sum_of_middle_terms = product_of_sums_of_parts - product_of_first_parts - product_of_second_parts;

	memcpy(product.long_number, product_of_first_parts.long_number,
			product_of_first_parts.size * sizeof(int));
	memcpy(product.long_number + product_of_first_parts.size,
			product_of_second_parts.long_number, product_of_second_parts.size * sizeof(int));

	int carry = 0;
	carry = 0;
	for (i = 0; i < sum_of_middle_terms.size || carry; ++i)
	{
		if (i == product.size)
			product.vector_append();
		product.long_number[a_part1.size + i] += sum_of_middle_terms.long_number[i] + carry;
		carry = product.long_number[a_part1.size + i] >= BASE;
		if (carry)
			product.long_number[a_part1.size + i] -= BASE;
	}
	return (product);
}

//---------------------mult_end--------------------//


//---------------------div_mod--------------------//

LongNumber& LongNumber::operator /=(const LongNumber &other)
{
	div_long_number(other);
	return (*this);
}

LongNumber& LongNumber::operator %=(const LongNumber &other)
{
	mod_long_number(other);
	return (*this);
}

LongNumber operator %(const LongNumber& leftOperand, const LongNumber& rightOperand)
{
	LongNumber temp = leftOperand;
	temp %= rightOperand;
	return (temp);
}

LongNumber operator /(const LongNumber& leftOperand, const LongNumber& rightOperand)
{
	LongNumber temp = leftOperand;
	temp /= rightOperand;
	return (temp);
}

void	LongNumber::div_long_number(const LongNumber &other)
{
	LongNumber result;
	LongNumber dividend = *this;
	LongNumber temp;
	LongNumber divider = other;
	int j;
	int n;
	int len;

	len = length_number() - divider.length_number() + 1;
	n = dividend.length_number() - divider.length_number();
	while (dividend >= divider)
	{
		temp = divider.shift(n);
		if (temp > dividend)
		{
			if (dividend == *this)
				len--;
			result *= 10;
			temp = divider.shift(--n);
		}
		for (j = 1; dividend >= temp * (j + 1); j++);
		temp *= j;
		result *= 10;
		result.sum_long_to_short(j);
		dividend -= temp;
		n--;
	}
	len -= result.length_number();
	for (int i = 0; i < len; i++)
		result *= 10;
	*this = result;
}

void LongNumber::mod_long_number(const LongNumber &other)
{
	LongNumber result;
	LongNumber dividend = *this;
	LongNumber temp;
	LongNumber divider = other;
	int j;
	int n;
	int len;

	len = length_number() - divider.length_number() + 1;
	n = dividend.length_number() - divider.length_number();
	while (dividend >= divider)
	{
		temp = divider.shift(n);
		if (temp > dividend)
		{
			if (dividend == *this)
				len--;
			result *= 10;
			temp = divider.shift(--n);
		}
		for (j = 1; dividend >= temp * (j + 1); j++);
		temp *= j;
		result *= 10;
		result.sum_long_to_short(j);
		dividend -= temp;
		n--;
	}
	len -= result.length_number();
	for (int i = 0; i < len; i++)
		result *= 10;
	*this = dividend;
}

//---------------------div_mod END--------------------//

//---------------------pow---------------------------//
LongNumber LongNumber::big_pow(int n)
{
	LongNumber res("1");
	LongNumber temp = *this;

	while (n)
	{
		if (n & 1) {
			res *= temp;
			--n;
		}
		else {
			temp *= temp;
			n >>= 1;
		}
	}
	return res;
}




//--------------------pow END---------------------------//


//--------------------test------------------------------//

void	strrev(char *str)
{
	char *ptr_str = str;
	char temp;

	while (*str)
		str++;
	str--;
	while (str > ptr_str)
	{
		temp = *ptr_str;
		*ptr_str = *str;
		*str = temp;
		str--;
		ptr_str++;
	}
}

char*		my_itoa(LongNumber &number, int base)
{
	char *result;
	int size;
	LongNumber base_long(base);

	size = 1;
	result = (char*)malloc(sizeof(char));

	while (number != 0)
	{
		LongNumber temp = number % base_long;
		result[size - 1] = (temp.get_short_number() > 9 ? temp.get_short_number() - 10 + 'A' : temp.get_short_number() + '0');
		number /= base_long;
		size++;
		result = (char*)realloc(result, sizeof(char) * size);
	}
	result[size - 1] = 0;
	strrev(result);
	return (result);

}

LongNumber	my_atoi(const char *number, int base)
{
	LongNumber result;

	number--;
	while (*++number)
	{
		if (isdigit(*number))
			result = result * base + (*number - '0');
		else
			result = result * base + (tolower(*number) - 'a' + 10);
	}
	return (result);
}

LongNumber fuc(LongNumber number)
{
	LongNumber result("1");
	LongNumber temp("2");

	while (number >= temp)
	{
		result *= temp;
		temp += 1;
	}
	return (result);
}

LongNumber bin_kaf(LongNumber n, LongNumber k)
{
	LongNumber a = fuc(n);
	LongNumber c = fuc(k) * fuc(n - k);

	return (a / c);
}

LongNumber fib(int n)
{
	LongNumber fib1(1);
	LongNumber fib2(1);
	LongNumber fib_sum;

	for (int i = 0; i < n - 2; i++)
	{
		fib_sum = fib1 + fib2;
		fib1 = fib2;
		fib2 = fib_sum;
	}
	return(fib2);
}

//-------------------test END ----------------------------//
int main()
{
	LongNumber one("100");
	LongNumber two("4");

	cout << "n = "; one.print_long_number();
    cout << "k = "; two.print_long_number();
	cout << "fibanachi(100) = " << fib(100);
	cout << "C{k, n} = " << bin_kaf(one, two);
	cout << "1100100{2} = " << my_atoi("1100100", 2);
    cout << "100 = " << my_itoa(one, 2) << "{2}" << endl;

	return 0;
}

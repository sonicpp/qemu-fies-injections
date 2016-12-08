#include <angelio.h>
#include <angellib.h>

void secret_answer(void)
{
	aprintf("42\n");
}

int check_password(const char *passwd)
{
	return 0;
}

void ultimate_question(const char *passwd)
{
	if (check_password(passwd))
		secret_answer();
	else
		aprintf("access denied!\n");
}

int main(void)
{
	aprintf("Answer to the Ultimate Question of Life, The Universe," \
		"and Everything: ");
	ultimate_question("test");

	aexit(0);
}


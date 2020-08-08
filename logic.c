
#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1000 //max input line size

int main(int argc, char *argv[])
{
    int c, c2, c3, c4, c5, c6;
    int i = 0;
    char line[81];
    char doneLine[5];
    int charCount = 0;

    printf("\n");

    //checks if being redirected
    if (isatty(STDIN_FILENO))
    {
        while ((c = getchar()) != EOF)
        {
            if (c == '\n')
            {
                c2 = getchar();
                if (c2 == 'D')
                {
                    doneLine[0] = c2;
                    c3 = getchar();
                    if (c3 == 'O')
                    {
                        doneLine[1] = c3;
                        c4 = getchar();
                        if (c4 == 'N')
                        {
                            doneLine[2] = c4;
                            c5 = getchar();
                            if (c5 == 'E')
                            {
                                doneLine[3] = c5;
                                c6 = getchar();
                                if (c6 != '\n')
                                {
                                    doneLine[4] = c6;
                                    for (int j = 0; j < 5; j++)
                                    {
                                        line[i] = doneLine[j];
                                        i++;
                                    }
                                    charCount += 5;
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (c2 != 'D')
                {
                    line[i++] = ' ';
                    line[i++] = c2;
                    charCount += 2;
                    continue;
                }
                else
                {
                    c = ' ';
                }
            }
            else if (c == '+')
            {
                c2 = getchar();
                if (c2 == '+')
                {
                    c = '^';
                    i = i--;
                }
                else
                {
                    line[i] = c;
                    i = i + 1;
                    line[i] = c2;
                    charCount += 2;
                    putchar(c);
                    putchar(c2);
                    continue;
                }
            }

            line[i] = c;
            i++;
            charCount++;
            putchar(c);

            if (charCount == 80)
            {
                line[i] = '\0';
                printf("%s\n", line);
                charCount = 0;
                i = 0;
            }
        }
        line[i] = '\0';

        printf("%s\n", line);
    }
    else
    {
        while ((c = getchar()) != EOF)
        {
            if (c == '\n')
            {
                c2 = getchar();
                if (c2 == 'D')
                {
                    doneLine[0] = c2;
                    c3 = getchar();
                    if (c3 == 'O')
                    {
                        doneLine[1] = c3;
                        c4 = getchar();
                        if (c4 == 'N')
                        {
                            doneLine[2] = c4;
                            c5 = getchar();
                            if (c5 == 'E')
                            {
                                doneLine[3] = c5;
                                c6 = getchar();
                                if (c6 != '\n')
                                {
                                    doneLine[4] = c6;
                                    for (int j = 0; j < 5; j++)
                                    {
                                        line[i] = doneLine[j];
                                        i++;
                                    }
                                    charCount += 5;
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (c2 != 'D')
                {
                    line[i++] = ' ';
                    line[i++] = c2;
                    charCount += 2;
                    continue;
                }
                else
                {
                    c = ' ';
                }
            }
            else if (c == '+')
            {
                c2 = getchar();
                if (c2 == '+')
                {
                    c = '^';
                    i = i--;
                }
                else
                {
                    line[i] = c;
                    i = i + 1;
                    line[i] = c2;
                    charCount += 2;
                    putchar(c);
                    putchar(c2);
                    continue;
                }
            }

            line[i] = c;
            i++;
            charCount++;

            if (charCount == 80)
            {
                line[i] = '\0';
                for (int i = 0; i < 81; i++)
                {
                    putchar(line[i]);
                }
                charCount = 0;
                i = 0;
            }
        }
    }

    printf("\n");

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


static char buf1[100];
int inansi_flag = 0;



void pascal write_string(char *string)
	{
	char tbuf[50];
	char *cptr = string;
	static char *cptr1;
	char *cptr2;
	char outchar = (char)'\0';
	static int save_cursor = 0;
	static int valid = 0;
	int tcolor;
	int cursor;
	int tval;
	int row;
	int col;

	while (*cptr)
		{
		if (*cptr == '\x1b' && !inansi_flag)
			{
			inansi_flag = 1;
			cptr1 = buf1;
			}
		else if (*cptr == '\x1b' && inansi_flag)
			cptr1 = buf1;
		if (inansi_flag)
			{
			if ((cptr1 == (buf1 + 1) && *cptr == '[') || cptr1 != (buf1 + 1))
				{
	*cptr1++ = *cptr;
				if (isalpha(*cptr) || *cptr == '@')
					{
	valid = 0;  /** hmmm **/
					*cptr1 = '\0';
					switch (*cptr)
						{
						case 's': 	/* SCP */
							if (buf1[2] == 's')
								valid = 1;
							break;
						case 'u':	/* RCP */
							if (buf1[2] == 'u')
								valid = 1;
							break;
						case 'P':	/* DCH */
							valid = 1;
							break;
						case '@':	/* ICH */
							valid = 1;
							break;
						case 'D':	/* CUB */
							if (buf1[2] == 'D')
								valid = 1;
							else
								valid = 1;
							break;
                    	case 'B':	/* CUD */
							if (buf1[2] == 'B')
								valid = 1;
							else
								valid = 1;
							break;
                    	case 'C':	/* CUF */
							if (buf1[2] == 'C')
								valid = 1;
							else
								valid = 1;
							break;
                    	case 'H':	/* CUP */
							if (buf1[2] == 'H')
								valid = 1;
							else
								{
								if (buf1[2] == ';')
									{
									if (buf1[3] == 'H')
										valid = 1;
									else
										valid = 1;
									}
								else
									valid = 1;
								}
							break;
						case 'A':	/* CUU */
							if (buf1[2] == 'A')
								valid = 1;
							else
								valid = 1;
							break;
						case 'n':	/* DSR */
							valid = 1;
							break;
                    	case 'J':	/* ED */
							if (buf1[2] == 'J')
								valid = 1;
							else
								{
								switch (atoi(buf1 + 2))
									{
									case 0:
										valid = 1;
										break;
									case 1:
										valid = 1;
										break;
									case 2:
										valid = 1;
										break;
									default:
										break;
									}
                            	}
							break;
                    	case 'K':	/* EL */
							valid = 1;
							break;
                    	case 'f':	/* HVP */
							if (buf1[2] == 'f')
								valid = 1;
							else
								{
								if (buf1[2] == ';')
									{
									if (buf1[3] == 'f')
										valid = 1;
									else
										valid = 1;
									}
								else
									valid = 1;
								}
							break;
                    	case 'm':	/* SGR */
							if (buf1[2] == 'm')
								valid = 1;
							else
								{
								strcpy(tbuf,buf1 + 2,strlen(buf1) - 1);
								cptr1 = strtok(tbuf,";m");
								while (cptr1)
									{
									tcolor = atoi(cptr1);
									switch (tcolor)
										{
										case 0:
											valid = 1;
											break;
										case 1:
											valid = 1;
											break;
                                    	case 5:
											valid = 1;
											break;
                                    	case 7:
											valid = 1;
											break;
										case 8:
											valid = 1;
											break;
                                    	case 30:
											valid = 1;
											break;
                                    	case 31:
											valid = 1;
											break;
                                    	case 32:
											valid = 1;
											break;
                                    	case 33:
											valid = 1;
											break;
                                    	case 34:
											valid = 1;
											break;
                                    	case 35:
											valid = 1;
											break;
                                    	case 36:
											valid = 1;
											break;
                                    	case 37:
											valid = 1;
											break;
                                    	case 40:
											valid = 1;
											break;
                                    	case 41:
											valid = 1;
											break;
                                    	case 42:
											valid = 1;
											break;
                                    	case 43:
											valid = 1;
											break;
                                    	case 44:
											valid = 1;
											break;
                                    	case 45:
											valid = 1;
											break;
                                    	case 46:
											valid = 1;
											break;
                                    	case 47:
											valid = 1;
											break;
										default:
											printf("Got %d\n",tcolor);
											valid = 0;
											break;
										}
									if (!valid)
										break;
									cptr1 = strtok(NULL,";m");
									}
								}
							break;
						default:
							printf("Default \"%s\"\n",buf1);
							break;
                    	}
					if (!valid)
						printf("Invalid sequence \"%s\"\n",buf1 + 1);
					inansi_flag = 0;
					valid = 0;
					}
				}
			}
		++cptr;
		}
	}


main()
	{
	char buffer[81];

	while (fgets(buffer,sizeof(buffer),stdin))
		write_string(buffer);
	}

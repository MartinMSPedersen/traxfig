#include <stdio.h>

int 
main(argc, argv)
	int             argc;
	char          **argv;
{
	FILE           *fin;
	FILE           *fout;
	int             c;
	int             f = 1;
	if (argc == 1)
		printf("filtre <nomfichier> [ <nomfichier> ... ]\n");
	else {
		while (f < argc) {
			printf("%s...\n",argv[f]);
			fin = fopen(argv[f], "r");
			fout = fopen("aliceinchains", "w");
			while ((c = fgetc(fin)) != EOF)
			{	if (c != 13)
				{	fputc(c, fout);
				}
			}
			fclose(fin);
			fclose(fout);
			remove(argv[f]);
			rename("aliceinchains", argv[f]);
			f++;
		}
	}
	return 0;
}

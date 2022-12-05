#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void rebuild (int argc, char* argv[])
{
	if (argc != 3)
		errx(EXIT_FAILURE,"Usage: rebuild <grid not solved> <grid solved>\n");

	SDL_Surface* number[] = 
		{IMG_Load("ressources/1.png"),
		 IMG_Load("ressources/2.png"),
		 IMG_Load("ressources/3.png"),
		 IMG_Load("ressources/4.png"),
		 IMG_Load("ressources/5.png"),
		 IMG_Load("ressources/6.png"),
		 IMG_Load("ressources/7.png"),
		 IMG_Load("ressources/8.png"),
		 IMG_Load("ressources/9.png")};

	SDL_Surface* number_added[] =
		{IMG_Load("ressources/1_a.png"),
		 IMG_Load("ressources/2_a.png"),
		 IMG_Load("ressources/3_a.png"),
		 IMG_Load("ressources/4_a.png"),
		 IMG_Load("ressources/5_a.png"),
		 IMG_Load("ressources/6_a.png"),
		 IMG_Load("ressources/7_a.png"),
		 IMG_Load("ressources/8_a.png"),
		 IMG_Load("ressources/9_a.png")};


	SDL_Rect dest;
	dest.x = 5;
	dest.y = 5;
	dest.w = 45;
	dest.h = 45;

	SDL_Surface* grid = IMG_Load("ressources/blank-sudoku-grid.png");

	char *grid_ns = malloc(sizeof(char)*81);
	char *grid_s = malloc(sizeof(char)*81);

	FILE* ptr1;
	FILE* ptr2;

	ptr1 = fopen(argv[1], "r");
	if (ptr1 == NULL)
		errx(EXIT_FAILURE, "File GNS can't be opened \n");

	int n = 0;
	while (!feof(ptr1))
	{
		char ch = fgetc(ptr1);
		if (ch != '\n' && ch != ' ')
		{
			grid_ns[n] = ch;
			n++;
		}
	}
	fclose(ptr1);

	ptr2 = fopen(argv[2], "r");

	if (ptr2 == NULL)
		errx(EXIT_FAILURE, "File GS can't be opened \n");

	n = 0;
	while (!feof(ptr2))
	{
		char ch = fgetc(ptr2);
		if (ch != '\n' && ch != ' ')
		{
			grid_s[n] = ch;
			n++;
		}
	}
	fclose(ptr2);
	
	for (int i = 0; i < 81; i++)
	{
		int j = grid_s[i] - 49;
		if (grid_ns[i] != 46)
		{
			SDL_BlitSurface(number[j], NULL, grid, &dest);
		}
		else
		{
			SDL_BlitSurface(number_added[j], NULL, grid, &dest);
		}
		dest.x += 55;
		if (dest.x > 495)
		{
			dest.x = 5;
			dest.y += 55;
		}
	}
	IMG_SavePNG(grid, "rebuild_grid.png");
}

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main (int argc, char* argv)
{
	//if (argc != 3)
		//errx(EXIT_FAILURE, "Usage: rebuild <grid not solved> <grid solved>");

	SDL_Surface** number =
		[load_image("ressources/n1.png"),
		 load_image("ressources/n2.png"),
		 load_image("ressources/n3.png"),
		 load_image("ressources/n4.png"),
		 load_image("ressources/n5.png"),
		 load_image("ressources/n6.png"),
		 load_image("ressources/n7.png"),
		 load_image("ressources/n8.png"),
		 load_image("ressources/n9.png"),];

	SDL_Surface** number_added =
		[load_image("ressources/n1_a.png"),
		 load_image("ressources/n2_a.png"),
		 load_image("ressources/n3_a.png"),
		 load_image("ressources/n4_a.png"),
		 load_image("ressources/n5_a.png"),
		 load_image("ressources/n6_a.png"),
		 load_image("ressources/n7_a.png"),
		 load_image("ressources/n8_a.png"),
		 load_image("ressources/n9_a.png"),];


	SDL_Rect dest;
	dest.x = 5;
	dest.y = 5;
	dest.width = 45;
	dest.height = 45;

	SDL_Surface* grid = load_image("ressources/blank-sudoku-grid.png");

	char* grid_ns = malloc(sizeof(char)*118);
	char* grid_s = malloc(sizeof(char)*118);

	FILE* ptr1;
	FILE* ptr2;

	ptr1 = fopen(argv[1], "r");
	//if (ptr1 == NULL)
		//errx(EXIT_FAILURE, "File GNS can't be opened \n");

	int n = 0;
	while (!feof(ptr1))
	{
		char ch = fgets(ptr1);
		if (ch != '\n')
		{
			grid_ns[n] = ch;
			n++;
		}
	}
	fclose(ptr1);

	ptr2 = fopen(argv[2], "r");

	//if (ptr2 == NULL)
	//	errx(EXIT_FAILURE, "File GS can't be opened \n");

	n = 0;
	while (!feof(ptr2))
	{
		char ch = fgets(ptr2);
		if ((ch != '\n'))
		{
			grid_s[n] = ch;
			n++;
		}
	}
	fclose(ptr2);
	
	for (int i = 0; i < 118; i++)
	{
		if (grid_s[i] != " ")
		{
			int j = grid_s[i] - 48;
			if (grid_ns[i] != ".")
				SDL_BlitSurface(number[j], NULL, grid, dest);		
			else
				SDL_BlitSurface(number_added[j], NULL, grid, dest);		
			dest.x += 55;
			if (dest.x > 495)
			{
				dest.x = 5;
				dest.y += 55;
			}
		}

	}
	IMG_SavePNG(grid, "rebuild_grid.png"); 

}

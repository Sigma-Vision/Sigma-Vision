#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void rebuild (int argc, char* argv[])
{
	if (argc != 3)
		printf("Usage: rebuild <grid not solved> <grid solved>");

	if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{ 
        	printf("Could not initialize SDL: %s.\n", SDL_GetError());
        	exit(-1);
  	 }	
	//SDL_Surface* number[] = 
	//	{IMG_Load("ressources/n1.png"),
	//	 IMG_Load("ressources/n2.png"),
	//	 IMG_Load("ressources/n3.png"),
	//	 IMG_Load("ressources/n4.png"),
	//	 IMG_Load("ressources/n5.png"),
	//	 IMG_Load("ressources/n6.png"),
	//	 IMG_Load("ressources/n7.png"),
	//	 IMG_Load("ressources/n8.png"),
	//	 IMG_Load("ressources/n9.png")};

	//SDL_Surface* number_added[] =
	//	{IMG_Load("ressources/n1_a.png"),
	//	 IMG_Load("ressources/n2_a.png"),
	//	 IMG_Load("ressources/n3_a.png"),
	//	 IMG_Load("ressources/n4_a.png"),
	//	 IMG_Load("ressources/n5_a.png"),
	//	 IMG_Load("ressources/n6_a.png"),
	//	 IMG_Load("ressources/n7_a.png"),
	//	 IMG_Load("ressources/n8_a.png"),
	//	 IMG_Load("ressources/n9_a.png")};


	SDL_Rect dest;
	dest.x = 5;
	dest.y = 5;
	dest.w = 45;
	dest.h = 45;

	//SDL_Surface* grid = IMG_Load("ressources/blank-sudoku-grid.png");

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
		char ch = fgetc(ptr1);
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
		char ch = fgetc(ptr2);
		if ((ch != '\n'))
		{
			grid_s[n] = ch;
			n++;
		}
	}
	fclose(ptr2);
	
	for (int i = 0; i < 118; i++)
	{
		if (*(grid_s+i) != " ")
		{
			int j = grid_s[i] - 48;
			if (*(grid_ns+i) != ".")
				printf("a");
				//SDL_BlitSurface(number[j], NULL, grid, &dest);		
				
			else
				printf("b");
				//SDL_BlitSurface(number_added[j], NULL, grid, &dest);		
			dest.x += 55;
			if (dest.x > 495)
			{
				dest.x = 5;
				dest.y += 55;
				printf("\n");
			}
		}

	}
	//IMG_SavePNG(grid, "rebuild_grid.png");
}
